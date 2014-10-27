/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "platform/heap/ThreadState.h"

#include "platform/ScriptForbiddenScope.h"
#include "platform/TraceEvent.h"
#include "platform/heap/AddressSanitizer.h"
#include "platform/heap/CallbackStack.h"
#include "platform/heap/Handle.h"
#include "platform/heap/Heap.h"
#include "public/platform/Platform.h"
#include "public/platform/WebThread.h"
#include "wtf/ThreadingPrimitives.h"
#if ENABLE(GC_PROFILE_HEAP)
#include "platform/TracedValue.h"
#endif

#if OS(WIN)
#include <stddef.h>
#include <windows.h>
#include <winnt.h>
#elif defined(__GLIBC__)
extern "C" void* __libc_stack_end;  // NOLINT
#endif

#if defined(MEMORY_SANITIZER)
#include <sanitizer/msan_interface.h>
#endif

#if OS(FREEBSD)
#include <pthread_np.h>
#endif

namespace blink {

static void* getStackStart()
{
#if defined(__GLIBC__) || OS(ANDROID) || OS(FREEBSD)
    pthread_attr_t attr;
    int error;
#if OS(FREEBSD)
    pthread_attr_init(&attr);
    error = pthread_attr_get_np(pthread_self(), &attr);
#else
    error = pthread_getattr_np(pthread_self(), &attr);
#endif
    if (!error) {
        void* base;
        size_t size;
        error = pthread_attr_getstack(&attr, &base, &size);
        RELEASE_ASSERT(!error);
        pthread_attr_destroy(&attr);
        return reinterpret_cast<Address>(base) + size;
    }
#if OS(FREEBSD)
    pthread_attr_destroy(&attr);
#endif
#if defined(__GLIBC__)
    // pthread_getattr_np can fail for the main thread. In this case
    // just like NaCl we rely on the __libc_stack_end to give us
    // the start of the stack.
    // See https://code.google.com/p/nativeclient/issues/detail?id=3431.
    return __libc_stack_end;
#else
    ASSERT_NOT_REACHED();
    return 0;
#endif
#elif OS(MACOSX)
    return pthread_get_stackaddr_np(pthread_self());
#elif OS(WIN) && COMPILER(MSVC)
    // On Windows stack limits for the current thread are available in
    // the thread information block (TIB). Its fields can be accessed through
    // FS segment register on x86 and GS segment register on x86_64.
#ifdef _WIN64
    return reinterpret_cast<void*>(__readgsqword(offsetof(NT_TIB64, StackBase)));
#else
    return reinterpret_cast<void*>(__readfsdword(offsetof(NT_TIB, StackBase)));
#endif
#else
#error Unsupported getStackStart on this platform.
#endif
}

static size_t getUnderestimatedStackSize()
{
#if defined(__GLIBC__) || OS(ANDROID) || OS(FREEBSD)
    // We cannot get the stack size in these platforms because
    // pthread_getattr_np() can fail for the main thread.
    // This is OK because ThreadState::current() doesn't use the stack size
    // in these platforms.
    return 0;
#elif OS(MACOSX)
    return pthread_get_stacksize_np(pthread_self());
#elif OS(WIN) && COMPILER(MSVC)
    // On Windows stack limits for the current thread are available in
    // the thread information block (TIB). Its fields can be accessed through
    // FS segment register on x86 and GS segment register on x86_64.
#ifdef _WIN64
    return __readgsqword(offsetof(NT_TIB64, StackBase)) - __readgsqword(offsetof(NT_TIB64, StackLimit));
#else
    return __readfsdword(offsetof(NT_TIB, StackBase)) - __readfsdword(offsetof(NT_TIB, StackLimit));
#endif
#else
    return 0;
#endif
}

WTF::ThreadSpecific<ThreadState*>* ThreadState::s_threadSpecific = 0;
uintptr_t ThreadState::s_mainThreadStackStart = 0;
uintptr_t ThreadState::s_mainThreadUnderestimatedStackSize = 0;
uint8_t ThreadState::s_mainThreadStateStorage[sizeof(ThreadState)];
SafePointBarrier* ThreadState::s_safePointBarrier = 0;
bool ThreadState::s_inGC = false;

static Mutex& threadAttachMutex()
{
    AtomicallyInitializedStatic(Mutex&, mutex = *new Mutex);
    return mutex;
}

static double lockingTimeout()
{
    // Wait time for parking all threads is at most 100 MS.
    return 0.100;
}


typedef void (*PushAllRegistersCallback)(SafePointBarrier*, ThreadState*, intptr_t*);
extern "C" void pushAllRegisters(SafePointBarrier*, ThreadState*, PushAllRegistersCallback);

class SafePointBarrier {
public:
    SafePointBarrier() : m_canResume(1), m_unparkedThreadCount(0) { }
    ~SafePointBarrier() { }

    // Request other attached threads that are not at safe points to park themselves on safepoints.
    bool parkOthers()
    {
        ASSERT(ThreadState::current()->isAtSafePoint());

        // Lock threadAttachMutex() to prevent threads from attaching.
        threadAttachMutex().lock();

        ThreadState::AttachedThreadStateSet& threads = ThreadState::attachedThreads();

        MutexLocker locker(m_mutex);
        atomicAdd(&m_unparkedThreadCount, threads.size());
        releaseStore(&m_canResume, 0);

        ThreadState* current = ThreadState::current();
        for (ThreadState::AttachedThreadStateSet::iterator it = threads.begin(), end = threads.end(); it != end; ++it) {
            if (*it == current)
                continue;

            const Vector<ThreadState::Interruptor*>& interruptors = (*it)->interruptors();
            for (size_t i = 0; i < interruptors.size(); i++)
                interruptors[i]->requestInterrupt();
        }

        while (acquireLoad(&m_unparkedThreadCount) > 0) {
            double expirationTime = currentTime() + lockingTimeout();
            if (!m_parked.timedWait(m_mutex, expirationTime)) {
                // One of the other threads did not return to a safepoint within the maximum
                // time we allow for threads to be parked. Abandon the GC and resume the
                // currently parked threads.
                resumeOthers(true);
                return false;
            }
        }
        return true;
    }

    void resumeOthers(bool barrierLocked = false)
    {
        ThreadState::AttachedThreadStateSet& threads = ThreadState::attachedThreads();
        atomicSubtract(&m_unparkedThreadCount, threads.size());
        releaseStore(&m_canResume, 1);

        // FIXME: Resumed threads will all contend for m_mutex just to unlock it
        // later which is a waste of resources.
        if (UNLIKELY(barrierLocked)) {
            m_resume.broadcast();
        } else {
            // FIXME: Resumed threads will all contend for
            // m_mutex just to unlock it later which is a waste of
            // resources.
            MutexLocker locker(m_mutex);
            m_resume.broadcast();
        }

        ThreadState* current = ThreadState::current();
        for (ThreadState::AttachedThreadStateSet::iterator it = threads.begin(), end = threads.end(); it != end; ++it) {
            if (*it == current)
                continue;

            const Vector<ThreadState::Interruptor*>& interruptors = (*it)->interruptors();
            for (size_t i = 0; i < interruptors.size(); i++)
                interruptors[i]->clearInterrupt();
        }

        threadAttachMutex().unlock();
        ASSERT(ThreadState::current()->isAtSafePoint());
    }

    void checkAndPark(ThreadState* state, SafePointAwareMutexLocker* locker = 0)
    {
        ASSERT(!state->isSweepInProgress());
        if (!acquireLoad(&m_canResume)) {
            // If we are leaving the safepoint from a SafePointAwareMutexLocker
            // call out to release the lock before going to sleep. This enables the
            // lock to be acquired in the sweep phase, e.g. during weak processing
            // or finalization. The SafePointAwareLocker will reenter the safepoint
            // and reacquire the lock after leaving this safepoint.
            if (locker)
                locker->reset();
            pushAllRegisters(this, state, parkAfterPushRegisters);
        }
    }

    void enterSafePoint(ThreadState* state)
    {
        ASSERT(!state->isSweepInProgress());
        pushAllRegisters(this, state, enterSafePointAfterPushRegisters);
    }

    void leaveSafePoint(ThreadState* state, SafePointAwareMutexLocker* locker = 0)
    {
        if (atomicIncrement(&m_unparkedThreadCount) > 0)
            checkAndPark(state, locker);
    }

private:
    void doPark(ThreadState* state, intptr_t* stackEnd)
    {
        state->recordStackEnd(stackEnd);
        MutexLocker locker(m_mutex);
        if (!atomicDecrement(&m_unparkedThreadCount))
            m_parked.signal();
        while (!acquireLoad(&m_canResume))
            m_resume.wait(m_mutex);
        atomicIncrement(&m_unparkedThreadCount);
    }

    static void parkAfterPushRegisters(SafePointBarrier* barrier, ThreadState* state, intptr_t* stackEnd)
    {
        barrier->doPark(state, stackEnd);
    }

    void doEnterSafePoint(ThreadState* state, intptr_t* stackEnd)
    {
        state->recordStackEnd(stackEnd);
        state->copyStackUntilSafePointScope();
        // m_unparkedThreadCount tracks amount of unparked threads. It is
        // positive if and only if we have requested other threads to park
        // at safe-points in preparation for GC. The last thread to park
        // itself will make the counter hit zero and should notify GC thread
        // that it is safe to proceed.
        // If no other thread is waiting for other threads to park then
        // this counter can be negative: if N threads are at safe-points
        // the counter will be -N.
        if (!atomicDecrement(&m_unparkedThreadCount)) {
            MutexLocker locker(m_mutex);
            m_parked.signal(); // Safe point reached.
        }
    }

    static void enterSafePointAfterPushRegisters(SafePointBarrier* barrier, ThreadState* state, intptr_t* stackEnd)
    {
        barrier->doEnterSafePoint(state, stackEnd);
    }

    volatile int m_canResume;
    volatile int m_unparkedThreadCount;
    Mutex m_mutex;
    ThreadCondition m_parked;
    ThreadCondition m_resume;
};

// Statically unfold the heap initialization loop so the compiler statically
// knows the heap index when using HeapIndexTrait.
template<int num> struct InitializeHeaps {
    static const int index = num - 1;
    static void init(BaseHeap** heaps, ThreadState* state)
    {
        InitializeHeaps<index>::init(heaps, state);
        heaps[index] = new typename HeapIndexTrait<index>::HeapType(state, index);
    }
};
template<> struct InitializeHeaps<0> {
    static void init(BaseHeap** heaps, ThreadState* state) { }
};

ThreadState::ThreadState()
    : m_thread(currentThread())
    , m_persistents(adoptPtr(new PersistentAnchor()))
    , m_startOfStack(reinterpret_cast<intptr_t*>(getStackStart()))
    , m_endOfStack(reinterpret_cast<intptr_t*>(getStackStart()))
    , m_safePointScopeMarker(0)
    , m_atSafePoint(false)
    , m_interruptors()
    , m_gcRequested(false)
    , m_forcePreciseGCForTesting(false)
    , m_sweepRequested(0)
    , m_sweepInProgress(false)
    , m_noAllocationCount(0)
    , m_inGC(false)
    , m_isTerminating(false)
    , m_shouldFlushHeapDoesNotContainCache(false)
    , m_lowCollectionRate(false)
    , m_numberOfSweeperTasks(0)
    , m_traceDOMWrappers(0)
#if defined(ADDRESS_SANITIZER)
    , m_asanFakeStack(__asan_get_current_fake_stack())
#endif
{
    ASSERT(!**s_threadSpecific);
    **s_threadSpecific = this;

    if (isMainThread()) {
        s_mainThreadStackStart = reinterpret_cast<uintptr_t>(m_startOfStack) - sizeof(void*);
        s_mainThreadUnderestimatedStackSize = getUnderestimatedStackSize() - sizeof(void*);
    }

    InitializeHeaps<NumberOfHeaps>::init(m_heaps, this);

    m_weakCallbackStack = new CallbackStack();

    if (blink::Platform::current())
        m_sweeperThread = adoptPtr(blink::Platform::current()->createThread("Blink GC Sweeper Thread"));
}

ThreadState::~ThreadState()
{
    checkThread();
    delete m_weakCallbackStack;
    m_weakCallbackStack = 0;
    for (int i = 0; i < NumberOfHeaps; i++)
        delete m_heaps[i];
    deleteAllValues(m_interruptors);
    **s_threadSpecific = 0;
    s_mainThreadStackStart = 0;
    s_mainThreadUnderestimatedStackSize = 0;
}

void ThreadState::init()
{
    s_threadSpecific = new WTF::ThreadSpecific<ThreadState*>();
    s_safePointBarrier = new SafePointBarrier;
}

void ThreadState::shutdown()
{
    delete s_safePointBarrier;
    s_safePointBarrier = 0;

    // Thread-local storage shouldn't be disposed, so we don't call ~ThreadSpecific().
}

void ThreadState::attachMainThread()
{
    RELEASE_ASSERT(!Heap::s_shutdownCalled);
    MutexLocker locker(threadAttachMutex());
    ThreadState* state = new(s_mainThreadStateStorage) ThreadState();
    attachedThreads().add(state);
}

void ThreadState::detachMainThread()
{
    // Enter a safe point before trying to acquire threadAttachMutex
    // to avoid dead lock if another thread is preparing for GC, has acquired
    // threadAttachMutex and waiting for other threads to pause or reach a
    // safepoint.
    ThreadState* state = mainThreadState();

    {
        SafePointAwareMutexLocker locker(threadAttachMutex(), NoHeapPointersOnStack);

        // First add the main thread's heap pages to the orphaned pool.
        state->cleanupPages();

        // Second detach thread.
        ASSERT(attachedThreads().contains(state));
        attachedThreads().remove(state);
        state->~ThreadState();
    }
    shutdownHeapIfNecessary();
}

void ThreadState::shutdownHeapIfNecessary()
{
    // We don't need to enter a safe point before acquiring threadAttachMutex
    // because this thread is already detached.

    MutexLocker locker(threadAttachMutex());
    // We start shutting down the heap if there is no running thread
    // and Heap::shutdown() is already called.
    if (!attachedThreads().size() && Heap::s_shutdownCalled)
        Heap::doShutdown();
}

void ThreadState::attach()
{
    RELEASE_ASSERT(!Heap::s_shutdownCalled);
    MutexLocker locker(threadAttachMutex());
    ThreadState* state = new ThreadState();
    attachedThreads().add(state);
}

void ThreadState::cleanupPages()
{
    for (int i = 0; i < NumberOfHeaps; ++i)
        m_heaps[i]->cleanupPages();
}

void ThreadState::cleanup()
{
    for (size_t i = 0; i < m_cleanupTasks.size(); i++)
        m_cleanupTasks[i]->preCleanup();

    {
        // Grab the threadAttachMutex to ensure only one thread can shutdown at
        // a time and that no other thread can do a global GC. It also allows
        // safe iteration of the attachedThreads set which happens as part of
        // thread local GC asserts. We enter a safepoint while waiting for the
        // lock to avoid a dead-lock where another thread has already requested
        // GC.
        SafePointAwareMutexLocker locker(threadAttachMutex(), NoHeapPointersOnStack);

        // From here on ignore all conservatively discovered
        // pointers into the heap owned by this thread.
        m_isTerminating = true;

        // Set the terminate flag on all heap pages of this thread. This is used to
        // ensure we don't trace pages on other threads that are not part of the
        // thread local GC.
        setupHeapsForTermination();

        // Do thread local GC's as long as the count of thread local Persistents
        // changes and is above zero.
        PersistentAnchor* anchor = static_cast<PersistentAnchor*>(m_persistents.get());
        int oldCount = -1;
        int currentCount = anchor->numberOfPersistents();
        ASSERT(currentCount >= 0);
        while (currentCount != oldCount) {
            Heap::collectGarbageForTerminatingThread(this);
            oldCount = currentCount;
            currentCount = anchor->numberOfPersistents();
        }
        // We should not have any persistents left when getting to this point,
        // if we have it is probably a bug so adding a debug ASSERT to catch this.
        ASSERT(!currentCount);
        // All of pre-finalizers should be consumed.
        ASSERT(m_preFinalizers.isEmpty());

        // Add pages to the orphaned page pool to ensure any global GCs from this point
        // on will not trace objects on this thread's heaps.
        cleanupPages();

        ASSERT(attachedThreads().contains(this));
        attachedThreads().remove(this);
    }

    for (size_t i = 0; i < m_cleanupTasks.size(); i++)
        m_cleanupTasks[i]->postCleanup();
    m_cleanupTasks.clear();
}


void ThreadState::detach()
{
    ThreadState* state = current();
    state->cleanup();
    delete state;
    shutdownHeapIfNecessary();
}

void ThreadState::visitPersistentRoots(Visitor* visitor)
{
    TRACE_EVENT0("blink_gc", "ThreadState::visitPersistentRoots");
    {
        // All threads are at safepoints so this is not strictly necessary.
        // However we acquire the mutex to make mutation and traversal of this
        // list symmetrical.
        MutexLocker locker(globalRootsMutex());
        globalRoots()->trace(visitor);
    }

    AttachedThreadStateSet& threads = attachedThreads();
    for (AttachedThreadStateSet::iterator it = threads.begin(), end = threads.end(); it != end; ++it)
        (*it)->visitPersistents(visitor);
}

void ThreadState::visitStackRoots(Visitor* visitor)
{
    TRACE_EVENT0("blink_gc", "ThreadState::visitStackRoots");
    AttachedThreadStateSet& threads = attachedThreads();
    for (AttachedThreadStateSet::iterator it = threads.begin(), end = threads.end(); it != end; ++it)
        (*it)->visitStack(visitor);
}

NO_SANITIZE_ADDRESS
void ThreadState::visitAsanFakeStackForPointer(Visitor* visitor, Address ptr)
{
#if defined(ADDRESS_SANITIZER)
    Address* start = reinterpret_cast<Address*>(m_startOfStack);
    Address* end = reinterpret_cast<Address*>(m_endOfStack);
    Address* fakeFrameStart = 0;
    Address* fakeFrameEnd = 0;
    Address* maybeFakeFrame = reinterpret_cast<Address*>(ptr);
    Address* realFrameForFakeFrame =
        reinterpret_cast<Address*>(
            __asan_addr_is_in_fake_stack(
                m_asanFakeStack, maybeFakeFrame,
                reinterpret_cast<void**>(&fakeFrameStart),
                reinterpret_cast<void**>(&fakeFrameEnd)));
    if (realFrameForFakeFrame) {
        // This is a fake frame from the asan fake stack.
        if (realFrameForFakeFrame > end && start > realFrameForFakeFrame) {
            // The real stack address for the asan fake frame is
            // within the stack range that we need to scan so we need
            // to visit the values in the fake frame.
            for (Address* p = fakeFrameStart; p < fakeFrameEnd; p++)
                Heap::checkAndMarkPointer(visitor, *p);
        }
    }
#endif
}

NO_SANITIZE_ADDRESS
void ThreadState::visitStack(Visitor* visitor)
{
    if (m_stackState == NoHeapPointersOnStack)
        return;

    Address* start = reinterpret_cast<Address*>(m_startOfStack);
    // If there is a safepoint scope marker we should stop the stack
    // scanning there to not touch active parts of the stack. Anything
    // interesting beyond that point is in the safepoint stack copy.
    // If there is no scope marker the thread is blocked and we should
    // scan all the way to the recorded end stack pointer.
    Address* end = reinterpret_cast<Address*>(m_endOfStack);
    Address* safePointScopeMarker = reinterpret_cast<Address*>(m_safePointScopeMarker);
    Address* current = safePointScopeMarker ? safePointScopeMarker : end;

    // Ensure that current is aligned by address size otherwise the loop below
    // will read past start address.
    current = reinterpret_cast<Address*>(reinterpret_cast<intptr_t>(current) & ~(sizeof(Address) - 1));

    for (; current < start; ++current) {
        Address ptr = *current;
#if defined(MEMORY_SANITIZER)
        // |ptr| may be uninitialized by design. Mark it as initialized to keep
        // MSan from complaining.
        // Note: it may be tempting to get rid of |ptr| and simply use |current|
        // here, but that would be incorrect. We intentionally use a local
        // variable because we don't want to unpoison the original stack.
        __msan_unpoison(&ptr, sizeof(ptr));
#endif
        Heap::checkAndMarkPointer(visitor, ptr);
        visitAsanFakeStackForPointer(visitor, ptr);
    }

    for (Vector<Address>::iterator it = m_safePointStackCopy.begin(); it != m_safePointStackCopy.end(); ++it) {
        Address ptr = *it;
#if defined(MEMORY_SANITIZER)
        // See the comment above.
        __msan_unpoison(&ptr, sizeof(ptr));
#endif
        Heap::checkAndMarkPointer(visitor, ptr);
        visitAsanFakeStackForPointer(visitor, ptr);
    }
}

void ThreadState::visitPersistents(Visitor* visitor)
{
    m_persistents->trace(visitor);
    if (m_traceDOMWrappers) {
        TRACE_EVENT0("blink_gc", "V8GCController::traceDOMWrappers");
        m_traceDOMWrappers(m_isolate, visitor);
    }
}

#if ENABLE(GC_PROFILE_MARKING)
const GCInfo* ThreadState::findGCInfo(Address address)
{
    BaseHeapPage* page = heapPageFromAddress(address);
    if (page) {
        return page->findGCInfo(address);
    }
    return 0;
}
#endif

#if ENABLE(GC_PROFILE_HEAP)
size_t ThreadState::SnapshotInfo::getClassTag(const GCInfo* gcinfo)
{
    HashMap<const GCInfo*, size_t>::AddResult result = classTags.add(gcinfo, classTags.size());
    if (result.isNewEntry) {
        liveCount.append(0);
        deadCount.append(0);
        liveSize.append(0);
        deadSize.append(0);
        generations.append(Vector<int, 8>());
        generations.last().fill(0, 8);
    }
    return result.storedValue->value;
}

void ThreadState::snapshot()
{
    SnapshotInfo info(this);
    RefPtr<TracedValue> json = TracedValue::create();

#define SNAPSHOT_HEAP(HeapType)                                           \
    {                                                                     \
        json->beginDictionary();                                          \
        json->setString("name", #HeapType);                               \
        m_heaps[HeapType##Heap]->snapshot(json.get(), &info);             \
        json->endDictionary();                                            \
        json->beginDictionary();                                          \
        json->setString("name", #HeapType"NonFinalized");                 \
        m_heaps[HeapType##HeapNonFinalized]->snapshot(json.get(), &info); \
        json->endDictionary();                                            \
    }
    json->beginArray("heaps");
    SNAPSHOT_HEAP(General1);
    SNAPSHOT_HEAP(General2);
    SNAPSHOT_HEAP(General3);
    SNAPSHOT_HEAP(General4);
    SNAPSHOT_HEAP(CollectionBacking);
    FOR_EACH_TYPED_HEAP(SNAPSHOT_HEAP);
    json->endArray();
#undef SNAPSHOT_HEAP

    json->setInteger("allocatedSpace", m_stats.totalAllocatedSpace());
    json->setInteger("objectSpace", m_stats.totalObjectSpace());
    json->setInteger("pageCount", info.pageCount);
    json->setInteger("freeSize", info.freeSize);

    Vector<String> classNameVector(info.classTags.size());
    for (HashMap<const GCInfo*, size_t>::iterator it = info.classTags.begin(); it != info.classTags.end(); ++it)
        classNameVector[it->value] = it->key->m_className;

    size_t liveSize = 0;
    size_t deadSize = 0;
    json->beginArray("classes");
    for (size_t i = 0; i < classNameVector.size(); ++i) {
        json->beginDictionary();
        json->setString("name", classNameVector[i]);
        json->setInteger("liveCount", info.liveCount[i]);
        json->setInteger("deadCount", info.deadCount[i]);
        json->setInteger("liveSize", info.liveSize[i]);
        json->setInteger("deadSize", info.deadSize[i]);
        liveSize += info.liveSize[i];
        deadSize += info.deadSize[i];

        json->beginArray("generations");
        for (size_t j = 0; j < heapObjectGenerations; ++j)
            json->pushInteger(info.generations[i][j]);
        json->endArray();
        json->endDictionary();
    }
    json->endArray();
    json->setInteger("liveSize", liveSize);
    json->setInteger("deadSize", deadSize);

    TRACE_EVENT_OBJECT_SNAPSHOT_WITH_ID("blink_gc", "ThreadState", this, json.release());
}
#endif

void ThreadState::pushWeakObjectPointerCallback(void* object, WeakPointerCallback callback)
{
    CallbackStack::Item* slot = m_weakCallbackStack->allocateEntry();
    *slot = CallbackStack::Item(object, callback);
}

bool ThreadState::popAndInvokeWeakPointerCallback(Visitor* visitor)
{
    // For weak processing we should never reach orphaned pages since orphaned
    // pages are not traced and thus objects on those pages are never be
    // registered as objects on orphaned pages. We cannot assert this here since
    // we might have an off-heap collection. We assert it in
    // Heap::pushWeakObjectPointerCallback.
    if (CallbackStack::Item* item = m_weakCallbackStack->pop()) {
        item->call(visitor);
        return true;
    }
    return false;
}

PersistentNode* ThreadState::globalRoots()
{
    AtomicallyInitializedStatic(PersistentNode*, anchor = new PersistentAnchor);
    return anchor;
}

Mutex& ThreadState::globalRootsMutex()
{
    AtomicallyInitializedStatic(Mutex&, mutex = *new Mutex);
    return mutex;
}

// Trigger garbage collection on a 50% increase in size, but not for
// less than 512kbytes.
bool ThreadState::increasedEnoughToGC(size_t newSize, size_t oldSize)
{
    if (newSize < 1 << 19)
        return false;
    size_t limit = oldSize + (oldSize >> 1);
    return newSize > limit;
}

// FIXME: The heuristics are local for a thread at this
// point. Consider using heuristics that take memory for all threads
// into account.
bool ThreadState::shouldGC()
{
    // Do not GC during sweeping. We allow allocation during
    // finalization, but those allocations are not allowed
    // to lead to nested garbage collections.
    return !m_sweepInProgress && increasedEnoughToGC(m_stats.totalObjectSpace(), m_statsAfterLastGC.totalObjectSpace());
}

// Trigger conservative garbage collection on a 100% increase in size,
// but not for less than 4Mbytes. If the system currently has a low
// collection rate, then require a 300% increase in size.
bool ThreadState::increasedEnoughToForceConservativeGC(size_t newSize, size_t oldSize)
{
    if (newSize < 1 << 22)
        return false;
    size_t limit = (m_lowCollectionRate ? 4 : 2) * oldSize;
    return newSize > limit;
}

// FIXME: The heuristics are local for a thread at this
// point. Consider using heuristics that take memory for all threads
// into account.
bool ThreadState::shouldForceConservativeGC()
{
    // Do not GC during sweeping. We allow allocation during
    // finalization, but those allocations are not allowed
    // to lead to nested garbage collections.
    return !m_sweepInProgress && increasedEnoughToForceConservativeGC(m_stats.totalObjectSpace(), m_statsAfterLastGC.totalObjectSpace());
}

bool ThreadState::sweepRequested()
{
    ASSERT(isAnyThreadInGC() || checkThread());
    return m_sweepRequested;
}

void ThreadState::setSweepRequested()
{
    // Sweep requested is set from the thread that initiates garbage
    // collection which could be different from the thread for this
    // thread state. Therefore the setting of m_sweepRequested needs a
    // barrier.
    atomicTestAndSetToOne(&m_sweepRequested);
}

void ThreadState::clearSweepRequested()
{
    checkThread();
    m_sweepRequested = 0;
}

bool ThreadState::gcRequested()
{
    checkThread();
    return m_gcRequested;
}

void ThreadState::setGCRequested()
{
    checkThread();
    m_gcRequested = true;
}

void ThreadState::clearGCRequested()
{
    checkThread();
    m_gcRequested = false;
}

void ThreadState::performPendingGC(StackState stackState)
{
    if (stackState == NoHeapPointersOnStack) {
        if (forcePreciseGCForTesting()) {
            setForcePreciseGCForTesting(false);
            Heap::collectAllGarbage();
        } else if (gcRequested()) {
            Heap::collectGarbage(NoHeapPointersOnStack);
        }
    }
}

void ThreadState::setForcePreciseGCForTesting(bool value)
{
    checkThread();
    m_forcePreciseGCForTesting = value;
}

bool ThreadState::forcePreciseGCForTesting()
{
    checkThread();
    return m_forcePreciseGCForTesting;
}

void ThreadState::makeConsistentForSweeping()
{
    for (int i = 0; i < NumberOfHeaps; i++)
        m_heaps[i]->makeConsistentForSweeping();
}

#if ENABLE(ASSERT)
bool ThreadState::isConsistentForSweeping()
{
    for (int i = 0; i < NumberOfHeaps; i++) {
        if (!m_heaps[i]->isConsistentForSweeping())
            return false;
    }
    return true;
}
#endif

void ThreadState::prepareRegionTree()
{
    // Add the regions allocated by this thread to the region search tree.
    for (size_t i = 0; i < m_allocatedRegionsSinceLastGC.size(); ++i)
        Heap::addPageMemoryRegion(m_allocatedRegionsSinceLastGC[i]);
    m_allocatedRegionsSinceLastGC.clear();
}

void ThreadState::flushHeapDoesNotContainCacheIfNeeded()
{
    if (m_shouldFlushHeapDoesNotContainCache) {
        Heap::flushHeapDoesNotContainCache();
        m_shouldFlushHeapDoesNotContainCache = false;
    }
}

void ThreadState::prepareForGC()
{
    for (int i = 0; i < NumberOfHeaps; i++) {
        BaseHeap* heap = m_heaps[i];
        heap->makeConsistentForSweeping();
        // If a new GC is requested before this thread got around to sweep, ie. due to the
        // thread doing a long running operation, we clear the mark bits and mark any of
        // the dead objects as dead. The latter is used to ensure the next GC marking does
        // not trace already dead objects. If we trace a dead object we could end up tracing
        // into garbage or the middle of another object via the newly conservatively found
        // object.
        if (sweepRequested())
            heap->clearLiveAndMarkDead();
    }
    prepareRegionTree();
    setSweepRequested();
    flushHeapDoesNotContainCacheIfNeeded();
}

void ThreadState::setupHeapsForTermination()
{
    for (int i = 0; i < NumberOfHeaps; i++)
        m_heaps[i]->prepareHeapForTermination();
}

BaseHeapPage* ThreadState::heapPageFromAddress(Address address)
{
    for (int i = 0; i < NumberOfHeaps; i++) {
        if (BaseHeapPage* page = m_heaps[i]->heapPageFromAddress(address))
            return page;
    }
    return 0;
}

void ThreadState::getStats(HeapStats& stats)
{
    stats = m_stats;
}

void ThreadState::getStatsForTesting(HeapStats& stats)
{
    ASSERT(isConsistentForSweeping());
    for (int i = 0; i < NumberOfHeaps; i++)
        m_heaps[i]->getStatsForTesting(stats);
}

bool ThreadState::stopThreads()
{
    return s_safePointBarrier->parkOthers();
}

void ThreadState::resumeThreads()
{
    s_safePointBarrier->resumeOthers();
}

void ThreadState::safePoint(StackState stackState)
{
    checkThread();
    performPendingGC(stackState);
    ASSERT(!m_atSafePoint);
    m_stackState = stackState;
    m_atSafePoint = true;
    s_safePointBarrier->checkAndPark(this);
    m_atSafePoint = false;
    m_stackState = HeapPointersOnStack;
    performPendingSweep();
}

#ifdef ADDRESS_SANITIZER
// When we are running under AddressSanitizer with detect_stack_use_after_return=1
// then stack marker obtained from SafePointScope will point into a fake stack.
// Detect this case by checking if it falls in between current stack frame
// and stack start and use an arbitrary high enough value for it.
// Don't adjust stack marker in any other case to match behavior of code running
// without AddressSanitizer.
NO_SANITIZE_ADDRESS static void* adjustScopeMarkerForAdressSanitizer(void* scopeMarker)
{
    Address start = reinterpret_cast<Address>(getStackStart());
    Address end = reinterpret_cast<Address>(&start);
    RELEASE_ASSERT(end < start);

    if (end <= scopeMarker && scopeMarker < start)
        return scopeMarker;

    // 256 is as good an approximation as any else.
    const size_t bytesToCopy = sizeof(Address) * 256;
    if (static_cast<size_t>(start - end) < bytesToCopy)
        return start;

    return end + bytesToCopy;
}
#endif

void ThreadState::enterSafePoint(StackState stackState, void* scopeMarker)
{
#ifdef ADDRESS_SANITIZER
    if (stackState == HeapPointersOnStack)
        scopeMarker = adjustScopeMarkerForAdressSanitizer(scopeMarker);
#endif
    ASSERT(stackState == NoHeapPointersOnStack || scopeMarker);
    performPendingGC(stackState);
    checkThread();
    ASSERT(!m_atSafePoint);
    m_atSafePoint = true;
    m_stackState = stackState;
    m_safePointScopeMarker = scopeMarker;
    s_safePointBarrier->enterSafePoint(this);
}

void ThreadState::leaveSafePoint(SafePointAwareMutexLocker* locker)
{
    checkThread();
    ASSERT(m_atSafePoint);
    s_safePointBarrier->leaveSafePoint(this, locker);
    m_atSafePoint = false;
    m_stackState = HeapPointersOnStack;
    clearSafePointScopeMarker();
    performPendingSweep();
}

void ThreadState::copyStackUntilSafePointScope()
{
    if (!m_safePointScopeMarker || m_stackState == NoHeapPointersOnStack)
        return;

    Address* to = reinterpret_cast<Address*>(m_safePointScopeMarker);
    Address* from = reinterpret_cast<Address*>(m_endOfStack);
    RELEASE_ASSERT(from < to);
    RELEASE_ASSERT(to <= reinterpret_cast<Address*>(m_startOfStack));
    size_t slotCount = static_cast<size_t>(to - from);
    // Catch potential performance issues.
#if defined(LEAK_SANITIZER) || defined(ADDRESS_SANITIZER)
    // ASan/LSan use more space on the stack and we therefore
    // increase the allowed stack copying for those builds.
    ASSERT(slotCount < 2048);
#else
    ASSERT(slotCount < 1024);
#endif

    ASSERT(!m_safePointStackCopy.size());
    m_safePointStackCopy.resize(slotCount);
    for (size_t i = 0; i < slotCount; ++i) {
        m_safePointStackCopy[i] = from[i];
    }
}

void ThreadState::registerSweepingTask()
{
    MutexLocker locker(m_sweepMutex);
    ++m_numberOfSweeperTasks;
}

void ThreadState::unregisterSweepingTask()
{
    MutexLocker locker(m_sweepMutex);
    ASSERT(m_numberOfSweeperTasks > 0);
    if (!--m_numberOfSweeperTasks)
        m_sweepThreadCondition.signal();
}

void ThreadState::waitUntilSweepersDone()
{
    TRACE_EVENT0("blink_gc", "ThreadState::waitUntilSweepersDone");
    MutexLocker locker(m_sweepMutex);
    while (m_numberOfSweeperTasks > 0)
        m_sweepThreadCondition.wait(m_sweepMutex);
}


class SweepNonFinalizedHeapTask final : public WebThread::Task {
public:
    SweepNonFinalizedHeapTask(ThreadState* state, BaseHeap* heap, HeapStats* stats)
        : m_threadState(state)
        , m_heap(heap)
        , m_stats(stats)
    {
        m_threadState->registerSweepingTask();
    }

    virtual ~SweepNonFinalizedHeapTask()
    {
        m_threadState->unregisterSweepingTask();
    }

    virtual void run()
    {
        TRACE_EVENT0("blink_gc", "ThreadState::sweepNonFinalizedHeaps");
        m_heap->sweep(m_stats);
    }

private:
    ThreadState* m_threadState;
    BaseHeap* m_heap;
    HeapStats* m_stats;
};

void ThreadState::performPendingSweep()
{
    if (!sweepRequested())
        return;

#if ENABLE(GC_PROFILE_HEAP)
    // We snapshot the heap prior to sweeping to get numbers for both resources
    // that have been allocated since the last GC and for resources that are
    // going to be freed.
    bool gcTracingEnabled;
    TRACE_EVENT_CATEGORY_GROUP_ENABLED("blink_gc", &gcTracingEnabled);
    if (gcTracingEnabled && m_stats.totalObjectSpace() > 0)
        snapshot();
#endif

    TRACE_EVENT0("blink_gc", "ThreadState::performPendingSweep");

    double timeStamp = WTF::currentTimeMS();
    const char* samplingState = TRACE_EVENT_GET_SAMPLING_STATE();
    if (isMainThread()) {
        ScriptForbiddenScope::enter();
        TRACE_EVENT_SET_SAMPLING_STATE("blink", "BlinkGCSweeping");
    }

    size_t objectSpaceBeforeSweep = m_stats.totalObjectSpace();
    {
        NoSweepScope scope(this);

        // Disallow allocation during weak processing.
        enterNoAllocationScope();
        {
            TRACE_EVENT0("blink_gc", "ThreadState::threadLocalWeakProcessing");
            // Perform thread-specific weak processing.
            while (popAndInvokeWeakPointerCallback(Heap::s_markingVisitor)) { }
        }
        {
            TRACE_EVENT0("blink_gc", "ThreadState::invokePreFinalizers");
            invokePreFinalizers(*Heap::s_markingVisitor);
        }
        leaveNoAllocationScope();

        // Perform sweeping and finalization.
        performPendingSweepInParallel();
    }

    clearGCRequested();
    clearSweepRequested();
    // If we collected less than 50% of objects, record that the
    // collection rate is low which we use to determine when to
    // perform the next GC.
    setLowCollectionRate(m_stats.totalObjectSpace() > (objectSpaceBeforeSweep / 2));

    if (blink::Platform::current()) {
        blink::Platform::current()->histogramCustomCounts("BlinkGC.PerformPendingSweep", WTF::currentTimeMS() - timeStamp, 0, 10 * 1000, 50);
    }

    if (isMainThread()) {
        TRACE_EVENT_SET_NONCONST_SAMPLING_STATE(samplingState);
        ScriptForbiddenScope::exit();
    }
}

void ThreadState::performPendingSweepInParallel()
{
    // Sweeping will recalculate the stats
    m_stats.clear();

    // Sweep the non-finalized heap pages on multiple threads.
    // Attempt to load-balance by having the sweeper thread sweep as
    // close to half of the pages as possible.
    int nonFinalizedPages = 0;
    for (int i = 0; i < NumberOfNonFinalizedHeaps; i++)
        nonFinalizedPages += m_heaps[FirstNonFinalizedHeap + i]->normalPageCount();

    int finalizedPages = 0;
    for (int i = 0; i < NumberOfFinalizedHeaps; i++)
        finalizedPages += m_heaps[FirstFinalizedHeap + i]->normalPageCount();

    int pagesToSweepInParallel = nonFinalizedPages < finalizedPages ? nonFinalizedPages : ((nonFinalizedPages + finalizedPages) / 2);

    // Start the sweeper thread for the non finalized heaps. No
    // finalizers need to run and therefore the pages can be
    // swept on other threads.
    static const int minNumberOfPagesForParallelSweep = 10;
    HeapStats heapStatsVector[NumberOfNonFinalizedHeaps];
    OwnPtr<BaseHeap> splitOffHeaps[NumberOfNonFinalizedHeaps];
    for (int i = 0; i < NumberOfNonFinalizedHeaps && pagesToSweepInParallel > 0; i++) {
        BaseHeap* heap = m_heaps[FirstNonFinalizedHeap + i];
        int pageCount = heap->normalPageCount();
        // Only use the sweeper thread if it exists and there are
        // pages to sweep.
        if (m_sweeperThread && pageCount > minNumberOfPagesForParallelSweep) {
            // Create a new thread heap instance to make sure that the
            // state modified while sweeping is separate for the
            // sweeper thread and the owner thread.
            int pagesToSplitOff = std::min(pageCount, pagesToSweepInParallel);
            pagesToSweepInParallel -= pagesToSplitOff;
            splitOffHeaps[i] = heap->split(pagesToSplitOff);
            HeapStats* stats = &heapStatsVector[i];
            m_sweeperThread->postTask(new SweepNonFinalizedHeapTask(this, splitOffHeaps[i].get(), stats));
        }
    }

    {
        // Sweep the remainder of the non-finalized pages (or all of them
        // if there is no sweeper thread).
        TRACE_EVENT0("blink_gc", "ThreadState::sweepNonFinalizedHeaps");
        for (int i = 0; i < NumberOfNonFinalizedHeaps; i++) {
            HeapStats stats;
            m_heaps[FirstNonFinalizedHeap + i]->sweep(&stats);
            m_stats.add(&stats);
        }
    }

    {
        // Sweep the finalized pages.
        TRACE_EVENT0("blink_gc", "ThreadState::sweepFinalizedHeaps");
        for (int i = 0; i < NumberOfFinalizedHeaps; i++) {
            HeapStats stats;
            m_heaps[FirstFinalizedHeap + i]->sweep(&stats);
            m_stats.add(&stats);
        }
    }

    // Wait for the sweeper threads and update the heap stats with the
    // stats for the heap portions swept by those threads.
    waitUntilSweepersDone();
    for (int i = 0; i < NumberOfNonFinalizedHeaps; i++) {
        m_stats.add(&heapStatsVector[i]);
        if (splitOffHeaps[i])
            m_heaps[FirstNonFinalizedHeap + i]->merge(splitOffHeaps[i].release());
    }

    for (int i = 0; i < NumberOfHeaps; i++)
        m_heaps[i]->postSweepProcessing();

    getStats(m_statsAfterLastGC);
}

void ThreadState::addInterruptor(Interruptor* interruptor)
{
    SafePointScope scope(HeapPointersOnStack, SafePointScope::AllowNesting);

    {
        MutexLocker locker(threadAttachMutex());
        m_interruptors.append(interruptor);
    }
}

void ThreadState::removeInterruptor(Interruptor* interruptor)
{
    SafePointScope scope(HeapPointersOnStack, SafePointScope::AllowNesting);

    {
        MutexLocker locker(threadAttachMutex());
        size_t index = m_interruptors.find(interruptor);
        RELEASE_ASSERT(index >= 0);
        m_interruptors.remove(index);
    }
}

void ThreadState::Interruptor::onInterrupted()
{
    ThreadState* state = ThreadState::current();
    ASSERT(state);
    ASSERT(!state->isAtSafePoint());
    state->safePoint(HeapPointersOnStack);
}

ThreadState::AttachedThreadStateSet& ThreadState::attachedThreads()
{
    DEFINE_STATIC_LOCAL(AttachedThreadStateSet, threads, ());
    return threads;
}

void ThreadState::unregisterPreFinalizerInternal(void* target)
{
    if (isSweepInProgress())
        return;
    auto it = m_preFinalizers.find(target);
    ASSERT(it != m_preFinalizers.end());
    m_preFinalizers.remove(it);
}

void ThreadState::invokePreFinalizers(Visitor& visitor)
{
    Vector<void*> deadObjects;
    for (auto& entry : m_preFinalizers) {
        if (entry.value(entry.key, visitor))
            deadObjects.append(entry.key);
    }
    // FIXME: removeAll is inefficient.  It can shrink repeatedly.
    m_preFinalizers.removeAll(deadObjects);
}

#if ENABLE(GC_PROFILE_MARKING)
const GCInfo* ThreadState::findGCInfoFromAllThreads(Address address)
{
    bool needLockForIteration = !isAnyThreadInGC();
    if (needLockForIteration)
        threadAttachMutex().lock();

    ThreadState::AttachedThreadStateSet& threads = attachedThreads();
    for (ThreadState::AttachedThreadStateSet::iterator it = threads.begin(), end = threads.end(); it != end; ++it) {
        if (const GCInfo* gcInfo = (*it)->findGCInfo(address)) {
            if (needLockForIteration)
                threadAttachMutex().unlock();
            return gcInfo;
        }
    }
    if (needLockForIteration)
        threadAttachMutex().unlock();
    return 0;
}
#endif

}

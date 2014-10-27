// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#include "config.h"
#include "V8TestInterfaceConstructor3.h"

#include "bindings/core/v8/ExceptionState.h"
#include "bindings/core/v8/V8DOMConfiguration.h"
#include "bindings/core/v8/V8HiddenValue.h"
#include "bindings/core/v8/V8ObjectConstructor.h"
#include "core/dom/ContextFeatures.h"
#include "core/dom/Document.h"
#include "core/frame/LocalDOMWindow.h"
#include "platform/RuntimeEnabledFeatures.h"
#include "platform/TraceEvent.h"
#include "wtf/GetPtr.h"
#include "wtf/RefPtr.h"

namespace blink {

const WrapperTypeInfo V8TestInterfaceConstructor3::wrapperTypeInfo = { gin::kEmbedderBlink, V8TestInterfaceConstructor3::domTemplate, V8TestInterfaceConstructor3::refObject, V8TestInterfaceConstructor3::derefObject, V8TestInterfaceConstructor3::trace, 0, 0, 0, V8TestInterfaceConstructor3::installConditionallyEnabledMethods, V8TestInterfaceConstructor3::installConditionallyEnabledProperties, 0, WrapperTypeInfo::WrapperTypeObjectPrototype, WrapperTypeInfo::ObjectClassId, WrapperTypeInfo::Independent, WrapperTypeInfo::RefCountedObject };

// This static member must be declared by DEFINE_WRAPPERTYPEINFO in TestInterfaceConstructor3.h.
// For details, see the comment of DEFINE_WRAPPERTYPEINFO in
// bindings/core/v8/ScriptWrappable.h.
const WrapperTypeInfo& TestInterfaceConstructor3::s_wrapperTypeInfo = V8TestInterfaceConstructor3::wrapperTypeInfo;

namespace TestInterfaceConstructor3V8Internal {

static void constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        V8ThrowException::throwException(createMinimumArityTypeErrorForConstructor("TestInterfaceConstructor3", 1, info.Length(), info.GetIsolate()), info.GetIsolate());
        return;
    }
    V8StringResource<> stringArg;
    {
        TOSTRING_VOID_INTERNAL(stringArg, info[0]);
    }
    RefPtr<TestInterfaceConstructor3> impl = TestInterfaceConstructor3::create(stringArg);
    v8::Handle<v8::Object> wrapper = info.Holder();
    impl->associateWithWrapper(&V8TestInterfaceConstructor3::wrapperTypeInfo, wrapper, info.GetIsolate());
    v8SetReturnValue(info, wrapper);
}

} // namespace TestInterfaceConstructor3V8Internal

void V8TestInterfaceConstructor3::constructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SCOPED_SAMPLING_STATE("blink", "DOMConstructor");
    if (!info.IsConstructCall()) {
        V8ThrowException::throwTypeError(ExceptionMessages::constructorNotCallableAsFunction("TestInterfaceConstructor3"), info.GetIsolate());
        return;
    }

    if (ConstructorMode::current(info.GetIsolate()) == ConstructorMode::WrapExistingObject) {
        v8SetReturnValue(info, info.Holder());
        return;
    }

    TestInterfaceConstructor3V8Internal::constructor(info);
}

static void installV8TestInterfaceConstructor3Template(v8::Handle<v8::FunctionTemplate> functionTemplate, v8::Isolate* isolate)
{
    functionTemplate->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = V8DOMConfiguration::installDOMClassTemplate(functionTemplate, "TestInterfaceConstructor3", v8::Local<v8::FunctionTemplate>(), V8TestInterfaceConstructor3::internalFieldCount,
        0, 0,
        0, 0,
        0, 0,
        isolate);
    functionTemplate->SetCallHandler(V8TestInterfaceConstructor3::constructorCallback);
    functionTemplate->SetLength(1);
    v8::Local<v8::ObjectTemplate> instanceTemplate = functionTemplate->InstanceTemplate();
    ALLOW_UNUSED_LOCAL(instanceTemplate);
    v8::Local<v8::ObjectTemplate> prototypeTemplate = functionTemplate->PrototypeTemplate();
    ALLOW_UNUSED_LOCAL(prototypeTemplate);

    // Custom toString template
    functionTemplate->Set(v8AtomicString(isolate, "toString"), V8PerIsolateData::from(isolate)->toStringTemplate());
}

v8::Handle<v8::FunctionTemplate> V8TestInterfaceConstructor3::domTemplate(v8::Isolate* isolate)
{
    return V8DOMConfiguration::domClassTemplate(isolate, const_cast<WrapperTypeInfo*>(&wrapperTypeInfo), installV8TestInterfaceConstructor3Template);
}

bool V8TestInterfaceConstructor3::hasInstance(v8::Handle<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->hasInstance(&wrapperTypeInfo, v8Value);
}

v8::Handle<v8::Object> V8TestInterfaceConstructor3::findInstanceInPrototypeChain(v8::Handle<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->findInstanceInPrototypeChain(&wrapperTypeInfo, v8Value);
}

TestInterfaceConstructor3* V8TestInterfaceConstructor3::toImplWithTypeCheck(v8::Isolate* isolate, v8::Handle<v8::Value> value)
{
    return hasInstance(value, isolate) ? blink::toScriptWrappableBase(v8::Handle<v8::Object>::Cast(value))->toImpl<TestInterfaceConstructor3>() : 0;
}

void V8TestInterfaceConstructor3::refObject(ScriptWrappableBase* scriptWrappableBase)
{
    scriptWrappableBase->toImpl<TestInterfaceConstructor3>()->ref();
}

void V8TestInterfaceConstructor3::derefObject(ScriptWrappableBase* scriptWrappableBase)
{
    scriptWrappableBase->toImpl<TestInterfaceConstructor3>()->deref();
}

template<>
v8::Handle<v8::Value> toV8NoInline(TestInterfaceConstructor3* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    return toV8(impl, creationContext, isolate);
}

} // namespace blink

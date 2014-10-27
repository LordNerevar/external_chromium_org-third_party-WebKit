# This file is generated by gyp; do not edit.

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := STATIC_LIBRARIES
LOCAL_MODULE := third_party_WebKit_Source_core_webcore_dom_gyp
LOCAL_MODULE_SUFFIX := .a
LOCAL_MODULE_TARGET_ARCH := $(TARGET_$(GYP_VAR_PREFIX)ARCH)
LOCAL_SDK_VERSION := 21
gyp_intermediate_dir := $(call local-intermediates-dir,,$(GYP_VAR_PREFIX))
gyp_shared_intermediate_dir := $(call intermediates-dir-for,GYP,shared,,,$(GYP_VAR_PREFIX))

# Make sure our deps are built first.
GYP_TARGET_DEPENDENCIES := \
	$(call intermediates-dir-for,GYP,third_party_WebKit_Source_core_webcore_prerequisites_gyp,,,$(GYP_VAR_PREFIX))/webcore_prerequisites.stamp \
	$(call intermediates-dir-for,STATIC_LIBRARIES,skia_skia_library_gyp,,,$(GYP_VAR_PREFIX))/skia_skia_library_gyp.a

GYP_GENERATED_OUTPUTS :=

# Make sure our deps and generated files are built first.
LOCAL_ADDITIONAL_DEPENDENCIES := $(GYP_TARGET_DEPENDENCIES) $(GYP_GENERATED_OUTPUTS)

LOCAL_GENERATED_SOURCES :=

GYP_COPIED_SOURCE_ORIGIN_DIRS :=

LOCAL_SRC_FILES := \
	third_party/WebKit/Source/core/dom/ActiveDOMObject.cpp \
	third_party/WebKit/Source/core/dom/AddConsoleMessageTask.cpp \
	third_party/WebKit/Source/core/dom/Attr.cpp \
	third_party/WebKit/Source/core/dom/CDATASection.cpp \
	third_party/WebKit/Source/core/dom/CSSSelectorWatch.cpp \
	third_party/WebKit/Source/core/dom/CharacterData.cpp \
	third_party/WebKit/Source/core/dom/ChildFrameDisconnector.cpp \
	third_party/WebKit/Source/core/dom/ChildListMutationScope.cpp \
	third_party/WebKit/Source/core/dom/ChildNodeList.cpp \
	third_party/WebKit/Source/core/dom/ClassCollection.cpp \
	third_party/WebKit/Source/core/dom/ClientRect.cpp \
	third_party/WebKit/Source/core/dom/ClientRectList.cpp \
	third_party/WebKit/Source/core/dom/Comment.cpp \
	third_party/WebKit/Source/core/dom/ContainerNode.cpp \
	third_party/WebKit/Source/core/dom/ContextFeatures.cpp \
	third_party/WebKit/Source/core/dom/ContextLifecycleNotifier.cpp \
	third_party/WebKit/Source/core/dom/ContextLifecycleObserver.cpp \
	third_party/WebKit/Source/core/dom/DOMArrayBuffer.cpp \
	third_party/WebKit/Source/core/dom/DOMArrayBufferDeallocationObserver.cpp \
	third_party/WebKit/Source/core/dom/DOMDataView.cpp \
	third_party/WebKit/Source/core/dom/DOMError.cpp \
	third_party/WebKit/Source/core/dom/DOMException.cpp \
	third_party/WebKit/Source/core/dom/DOMImplementation.cpp \
	third_party/WebKit/Source/core/dom/DOMMatrix.cpp \
	third_party/WebKit/Source/core/dom/DOMMatrixReadOnly.cpp \
	third_party/WebKit/Source/core/dom/DOMPoint.cpp \
	third_party/WebKit/Source/core/dom/DOMPointReadOnly.cpp \
	third_party/WebKit/Source/core/dom/DOMRect.cpp \
	third_party/WebKit/Source/core/dom/DOMRectReadOnly.cpp \
	third_party/WebKit/Source/core/dom/DOMSettableTokenList.cpp \
	third_party/WebKit/Source/core/dom/DOMStringList.cpp \
	third_party/WebKit/Source/core/dom/DOMStringMap.cpp \
	third_party/WebKit/Source/core/dom/DOMTokenList.cpp \
	third_party/WebKit/Source/core/dom/DOMTypedArray.cpp \
	third_party/WebKit/Source/core/dom/DOMURL.cpp \
	third_party/WebKit/Source/core/dom/DOMURLUtils.cpp \
	third_party/WebKit/Source/core/dom/DOMURLUtilsReadOnly.cpp \
	third_party/WebKit/Source/core/dom/DatasetDOMStringMap.cpp \
	third_party/WebKit/Source/core/dom/DecodedDataDocumentParser.cpp \
	third_party/WebKit/Source/core/dom/Document.cpp \
	third_party/WebKit/Source/core/dom/DocumentEncodingData.cpp \
	third_party/WebKit/Source/core/dom/DocumentFragment.cpp \
	third_party/WebKit/Source/core/dom/DocumentFullscreen.cpp \
	third_party/WebKit/Source/core/dom/DocumentInit.cpp \
	third_party/WebKit/Source/core/dom/DocumentLifecycle.cpp \
	third_party/WebKit/Source/core/dom/DocumentLifecycleNotifier.cpp \
	third_party/WebKit/Source/core/dom/DocumentLifecycleObserver.cpp \
	third_party/WebKit/Source/core/dom/DocumentMarker.cpp \
	third_party/WebKit/Source/core/dom/DocumentMarkerController.cpp \
	third_party/WebKit/Source/core/dom/DocumentOrderedList.cpp \
	third_party/WebKit/Source/core/dom/DocumentOrderedMap.cpp \
	third_party/WebKit/Source/core/dom/DocumentParser.cpp \
	third_party/WebKit/Source/core/dom/DocumentStyleSheetCollection.cpp \
	third_party/WebKit/Source/core/dom/DocumentStyleSheetCollector.cpp \
	third_party/WebKit/Source/core/dom/DocumentType.cpp \
	third_party/WebKit/Source/core/dom/Element.cpp \
	third_party/WebKit/Source/core/dom/ElementData.cpp \
	third_party/WebKit/Source/core/dom/ElementDataCache.cpp \
	third_party/WebKit/Source/core/dom/ElementFullscreen.cpp \
	third_party/WebKit/Source/core/dom/ElementRareData.cpp \
	third_party/WebKit/Source/core/dom/EmptyNodeList.cpp \
	third_party/WebKit/Source/core/dom/ExecutionContext.cpp \
	third_party/WebKit/Source/core/dom/Fullscreen.cpp \
	third_party/WebKit/Source/core/dom/IconURL.cpp \
	third_party/WebKit/Source/core/dom/IdTargetObserver.cpp \
	third_party/WebKit/Source/core/dom/IdTargetObserverRegistry.cpp \
	third_party/WebKit/Source/core/dom/IncrementLoadEventDelayCount.cpp \
	third_party/WebKit/Source/core/dom/LiveNodeList.cpp \
	third_party/WebKit/Source/core/dom/LiveNodeListBase.cpp \
	third_party/WebKit/Source/core/dom/MainThreadTaskRunner.cpp \
	third_party/WebKit/Source/core/dom/MessageChannel.cpp \
	third_party/WebKit/Source/core/dom/MessagePort.cpp \
	third_party/WebKit/Source/core/dom/Microtask.cpp \
	third_party/WebKit/Source/core/dom/MutationObserver.cpp \
	third_party/WebKit/Source/core/dom/MutationObserverInterestGroup.cpp \
	third_party/WebKit/Source/core/dom/MutationObserverRegistration.cpp \
	third_party/WebKit/Source/core/dom/MutationRecord.cpp \
	third_party/WebKit/Source/core/dom/NameNodeList.cpp \
	third_party/WebKit/Source/core/dom/NamedNodeMap.cpp \
	third_party/WebKit/Source/core/dom/Node.cpp \
	third_party/WebKit/Source/core/dom/NodeChildRemovalTracker.cpp \
	third_party/WebKit/Source/core/dom/NodeFilter.cpp \
	third_party/WebKit/Source/core/dom/NodeIterator.cpp \
	third_party/WebKit/Source/core/dom/NodeIteratorBase.cpp \
	third_party/WebKit/Source/core/dom/NodeListsNodeData.cpp \
	third_party/WebKit/Source/core/dom/NodeRareData.cpp \
	third_party/WebKit/Source/core/dom/NodeRenderingTraversal.cpp \
	third_party/WebKit/Source/core/dom/NodeTraversal.cpp \
	third_party/WebKit/Source/core/dom/PendingScript.cpp \
	third_party/WebKit/Source/core/dom/Position.cpp \
	third_party/WebKit/Source/core/dom/PositionIterator.cpp \
	third_party/WebKit/Source/core/dom/PresentationAttributeStyle.cpp \
	third_party/WebKit/Source/core/dom/ProcessingInstruction.cpp \
	third_party/WebKit/Source/core/dom/PseudoElement.cpp \
	third_party/WebKit/Source/core/dom/QualifiedName.cpp \
	third_party/WebKit/Source/core/dom/Range.cpp \
	third_party/WebKit/Source/core/dom/RenderTreeBuilder.cpp \
	third_party/WebKit/Source/core/dom/SandboxFlags.cpp \
	third_party/WebKit/Source/core/dom/ScriptLoader.cpp \
	third_party/WebKit/Source/core/dom/ScriptRunner.cpp \
	third_party/WebKit/Source/core/dom/ScriptableDocumentParser.cpp \
	third_party/WebKit/Source/core/dom/ScriptedAnimationController.cpp \
	third_party/WebKit/Source/core/dom/SecurityContext.cpp \
	third_party/WebKit/Source/core/dom/SelectorQuery.cpp \
	third_party/WebKit/Source/core/dom/ShadowTreeStyleSheetCollection.cpp \
	third_party/WebKit/Source/core/dom/SpaceSplitString.cpp \
	third_party/WebKit/Source/core/dom/StringCallback.cpp \
	third_party/WebKit/Source/core/dom/StyleChangeReason.cpp \
	third_party/WebKit/Source/core/dom/StyleElement.cpp \
	third_party/WebKit/Source/core/dom/StyleEngine.cpp \
	third_party/WebKit/Source/core/dom/StyleSheetCandidate.cpp \
	third_party/WebKit/Source/core/dom/StyleSheetCollection.cpp \
	third_party/WebKit/Source/core/dom/TagCollection.cpp \
	third_party/WebKit/Source/core/dom/Text.cpp \
	third_party/WebKit/Source/core/dom/TextLinkColors.cpp \
	third_party/WebKit/Source/core/dom/Touch.cpp \
	third_party/WebKit/Source/core/dom/TouchList.cpp \
	third_party/WebKit/Source/core/dom/TransformSourceLibxslt.cpp \
	third_party/WebKit/Source/core/dom/TreeScope.cpp \
	third_party/WebKit/Source/core/dom/TreeScopeAdopter.cpp \
	third_party/WebKit/Source/core/dom/TreeScopeStyleSheetCollection.cpp \
	third_party/WebKit/Source/core/dom/TreeWalker.cpp \
	third_party/WebKit/Source/core/dom/UserActionElementSet.cpp \
	third_party/WebKit/Source/core/dom/ViewportDescription.cpp \
	third_party/WebKit/Source/core/dom/VisitedLinkState.cpp \
	third_party/WebKit/Source/core/dom/WeakNodeMap.cpp \
	third_party/WebKit/Source/core/dom/XMLDocument.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElement.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementAsyncImportMicrotaskQueue.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementCallbackInvocation.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementCallbackQueue.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementDefinition.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementException.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementMicrotaskDispatcher.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementMicrotaskImportStep.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementMicrotaskQueueBase.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementMicrotaskResolutionStep.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementMicrotaskRunQueue.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementObserver.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementProcessingStack.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementRegistrationContext.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementRegistry.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementScheduler.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementSyncMicrotaskQueue.cpp \
	third_party/WebKit/Source/core/dom/custom/CustomElementUpgradeCandidateMap.cpp \
	third_party/WebKit/Source/core/dom/shadow/ComposedTreeWalker.cpp \
	third_party/WebKit/Source/core/dom/shadow/ContentDistribution.cpp \
	third_party/WebKit/Source/core/dom/shadow/ElementShadow.cpp \
	third_party/WebKit/Source/core/dom/shadow/InsertionPoint.cpp \
	third_party/WebKit/Source/core/dom/shadow/SelectRuleFeatureSet.cpp \
	third_party/WebKit/Source/core/dom/shadow/ShadowRoot.cpp \
	third_party/WebKit/Source/core/events/AnimationPlayerEvent.cpp \
	third_party/WebKit/Source/core/events/ApplicationCacheErrorEvent.cpp \
	third_party/WebKit/Source/core/events/BeforeTextInsertedEvent.cpp \
	third_party/WebKit/Source/core/events/BeforeUnloadEvent.cpp \
	third_party/WebKit/Source/core/events/ClipboardEvent.cpp \
	third_party/WebKit/Source/core/events/CompositionEvent.cpp \
	third_party/WebKit/Source/core/events/CustomEvent.cpp \
	third_party/WebKit/Source/core/events/DOMWindowEventQueue.cpp \
	third_party/WebKit/Source/core/events/ErrorEvent.cpp \
	third_party/WebKit/Source/core/events/Event.cpp \
	third_party/WebKit/Source/core/events/EventDispatchMediator.cpp \
	third_party/WebKit/Source/core/events/EventDispatcher.cpp \
	third_party/WebKit/Source/core/events/EventListenerMap.cpp \
	third_party/WebKit/Source/core/events/EventPath.cpp \
	third_party/WebKit/Source/core/events/EventTarget.cpp \
	third_party/WebKit/Source/core/events/FocusEvent.cpp \
	third_party/WebKit/Source/core/events/GenericEventQueue.cpp \
	third_party/WebKit/Source/core/events/GestureEvent.cpp \
	third_party/WebKit/Source/core/events/KeyboardEvent.cpp \
	third_party/WebKit/Source/core/events/MessageEvent.cpp \
	third_party/WebKit/Source/core/events/MouseEvent.cpp \
	third_party/WebKit/Source/core/events/MouseRelatedEvent.cpp \
	third_party/WebKit/Source/core/events/MutationEvent.cpp \
	third_party/WebKit/Source/core/events/NavigatorEvents.cpp \
	third_party/WebKit/Source/core/events/NodeEventContext.cpp \
	third_party/WebKit/Source/core/events/OverflowEvent.cpp \
	third_party/WebKit/Source/core/events/PageTransitionEvent.cpp \
	third_party/WebKit/Source/core/events/PopStateEvent.cpp \
	third_party/WebKit/Source/core/events/ProgressEvent.cpp \
	third_party/WebKit/Source/core/events/RelatedEvent.cpp \
	third_party/WebKit/Source/core/events/ResourceProgressEvent.cpp \
	third_party/WebKit/Source/core/events/ScopedEventQueue.cpp \
	third_party/WebKit/Source/core/events/TextEvent.cpp \
	third_party/WebKit/Source/core/events/TouchEvent.cpp \
	third_party/WebKit/Source/core/events/TouchEventContext.cpp \
	third_party/WebKit/Source/core/events/TransitionEvent.cpp \
	third_party/WebKit/Source/core/events/TreeScopeEventContext.cpp \
	third_party/WebKit/Source/core/events/UIEvent.cpp \
	third_party/WebKit/Source/core/events/UIEventWithKeyState.cpp \
	third_party/WebKit/Source/core/events/WebKitAnimationEvent.cpp \
	third_party/WebKit/Source/core/events/WheelEvent.cpp \
	third_party/WebKit/Source/core/events/WindowEventContext.cpp


# Flags passed to both C and C++ files.
MY_CFLAGS_Debug := \
	-fstack-protector \
	--param=ssp-buffer-size=4 \
	 \
	-fno-strict-aliasing \
	-Wall \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-fvisibility=hidden \
	-pipe \
	-fPIC \
	-Wno-unused-local-typedefs \
	-ffunction-sections \
	-funwind-tables \
	-g \
	-fstack-protector \
	-fno-short-enums \
	-finline-limit=64 \
	-Wa,--noexecstack \
	-U_FORTIFY_SOURCE \
	-Wno-extra \
	-Wno-ignored-qualifiers \
	-Wno-type-limits \
	-Wno-unused-but-set-variable \
	-Os \
	-g \
	-gdwarf-4 \
	-fdata-sections \
	-ffunction-sections \
	-fomit-frame-pointer \
	-funwind-tables

MY_DEFS_Debug := \
	'-DV8_DEPRECATION_WARNINGS' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DNO_TCMALLOC' \
	'-DDISABLE_NACL' \
	'-DCHROMIUM_BUILD' \
	'-DUSE_LIBJPEG_TURBO=1' \
	'-DENABLE_WEBRTC=1' \
	'-DUSE_PROPRIETARY_CODECS' \
	'-DENABLE_BROWSER_CDMS' \
	'-DENABLE_CONFIGURATION_POLICY' \
	'-DDISCARDABLE_MEMORY_ALWAYS_SUPPORTED_NATIVELY' \
	'-DSYSTEM_NATIVELY_SIGNALS_MEMORY_PRESSURE' \
	'-DENABLE_EGLIMAGE=1' \
	'-DCLD_VERSION=1' \
	'-DENABLE_PRINTING=1' \
	'-DENABLE_MANAGED_USERS=1' \
	'-DVIDEO_HOLE=1' \
	'-DENABLE_LOAD_COMPLETION_HACKS=1' \
	'-DBLINK_IMPLEMENTATION=1' \
	'-DINSIDE_BLINK' \
	'-DENABLE_OPENTYPE_VERTICAL=1' \
	'-DWTF_USE_CONCATENATED_IMPULSE_RESPONSES=1' \
	'-DWTF_USE_WEBAUDIO_OPENMAX_DL_FFT=1' \
	'-DENABLE_WEB_AUDIO=1' \
	'-DU_USING_ICU_NAMESPACE=0' \
	'-DU_ENABLE_DYLOAD=0' \
	'-DU_STATIC_IMPLEMENTATION' \
	'-DSK_ENABLE_INST_COUNT=0' \
	'-DSK_SUPPORT_GPU=1' \
	'-DGR_GL_CUSTOM_SETUP_HEADER="GrGLConfig_chrome.h"' \
	'-DSK_ENABLE_LEGACY_API_ALIASING=1' \
	'-DSK_ATTR_DEPRECATED=SK_NOTHING_ARG1' \
	'-DGR_GL_IGNORE_ES3_MSAA=0' \
	'-DSK_WILL_NEVER_DRAW_PERSPECTIVE_TEXT' \
	'-DSK_FM_NEW_MATCH_FAMILY_STYLE_CHARACTER=1' \
	'-DSK_SUPPORT_LEGACY_TEXTRENDERMODE' \
	'-DSK_LEGACY_NO_DISTANCE_FIELD_PATHS' \
	'-DSK_BUILD_FOR_ANDROID' \
	'-DSK_USE_POSIX_THREADS' \
	'-DCHROME_PNG_WRITE_SUPPORT' \
	'-DPNG_USER_CONFIG' \
	'-DCHROME_PNG_READ_PACK_SUPPORT' \
	'-DLIBXML_STATIC' \
	'-DLIBXSLT_STATIC' \
	'-DUSE_OPENSSL=1' \
	'-DUSE_OPENSSL_CERTS=1' \
	'-D__STDC_CONSTANT_MACROS' \
	'-D__STDC_FORMAT_MACROS' \
	'-DANDROID' \
	'-D__GNU_SOURCE=1' \
	'-DUSE_STLPORT=1' \
	'-D_STLP_USE_PTR_SPECIALIZATIONS=1' \
	'-DCHROME_BUILD_ID=""' \
	'-DDYNAMIC_ANNOTATIONS_ENABLED=1' \
	'-DWTF_USE_DYNAMIC_ANNOTATIONS=1' \
	'-D_DEBUG'


# Include paths placed before CFLAGS/CPPFLAGS
LOCAL_C_INCLUDES_Debug := \
	$(gyp_shared_intermediate_dir) \
	$(LOCAL_PATH)/third_party/WebKit/Source \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/skia/config \
	$(LOCAL_PATH)/third_party/khronos \
	$(LOCAL_PATH)/gpu \
	$(gyp_shared_intermediate_dir)/blink \
	$(LOCAL_PATH)/third_party/openmax_dl \
	$(LOCAL_PATH)/third_party/angle/include \
	$(LOCAL_PATH)/third_party/icu/source/i18n \
	$(LOCAL_PATH)/third_party/icu/source/common \
	$(LOCAL_PATH)/third_party/WebKit \
	$(LOCAL_PATH)/third_party/skia/src/core \
	$(LOCAL_PATH)/third_party/skia/include/core \
	$(LOCAL_PATH)/third_party/skia/include/effects \
	$(LOCAL_PATH)/third_party/skia/include/pdf \
	$(LOCAL_PATH)/third_party/skia/include/gpu \
	$(LOCAL_PATH)/third_party/skia/include/lazy \
	$(LOCAL_PATH)/third_party/skia/include/pathops \
	$(LOCAL_PATH)/third_party/skia/include/pipe \
	$(LOCAL_PATH)/third_party/skia/include/ports \
	$(LOCAL_PATH)/third_party/skia/include/utils \
	$(LOCAL_PATH)/skia/ext \
	$(LOCAL_PATH)/third_party/iccjpeg \
	$(LOCAL_PATH)/third_party/libpng \
	$(LOCAL_PATH)/third_party/libwebp \
	$(LOCAL_PATH)/third_party/libxml/linux/include \
	$(LOCAL_PATH)/third_party/libxml/src/include \
	$(LOCAL_PATH)/third_party/libxslt \
	$(LOCAL_PATH)/third_party/npapi \
	$(LOCAL_PATH)/third_party/npapi/bindings \
	$(LOCAL_PATH)/third_party/ots/include \
	$(LOCAL_PATH)/third_party/qcms/src \
	$(LOCAL_PATH)/third_party/sqlite \
	$(LOCAL_PATH)/third_party/zlib \
	$(LOCAL_PATH)/v8/include


# Flags passed to only C++ (and not C) files.
LOCAL_CPPFLAGS_Debug := \
	-fno-exceptions \
	-fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden \
	-Wsign-compare \
	-Wno-c++0x-compat \
	-Wno-uninitialized \
	-std=gnu++11 \
	-Wno-narrowing \
	-Wno-literal-suffix \
	-Wno-non-virtual-dtor \
	-Wno-sign-promo


# Flags passed to both C and C++ files.
MY_CFLAGS_Release := \
	-fstack-protector \
	--param=ssp-buffer-size=4 \
	 \
	-fno-strict-aliasing \
	-Wall \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-fvisibility=hidden \
	-pipe \
	-fPIC \
	-Wno-unused-local-typedefs \
	-ffunction-sections \
	-funwind-tables \
	-g \
	-fstack-protector \
	-fno-short-enums \
	-finline-limit=64 \
	-Wa,--noexecstack \
	-U_FORTIFY_SOURCE \
	-Wno-extra \
	-Wno-ignored-qualifiers \
	-Wno-type-limits \
	-Wno-unused-but-set-variable \
	-Os \
	-fno-ident \
	-fdata-sections \
	-ffunction-sections \
	-fomit-frame-pointer \
	-funwind-tables

MY_DEFS_Release := \
	'-DV8_DEPRECATION_WARNINGS' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DNO_TCMALLOC' \
	'-DDISABLE_NACL' \
	'-DCHROMIUM_BUILD' \
	'-DUSE_LIBJPEG_TURBO=1' \
	'-DENABLE_WEBRTC=1' \
	'-DUSE_PROPRIETARY_CODECS' \
	'-DENABLE_BROWSER_CDMS' \
	'-DENABLE_CONFIGURATION_POLICY' \
	'-DDISCARDABLE_MEMORY_ALWAYS_SUPPORTED_NATIVELY' \
	'-DSYSTEM_NATIVELY_SIGNALS_MEMORY_PRESSURE' \
	'-DENABLE_EGLIMAGE=1' \
	'-DCLD_VERSION=1' \
	'-DENABLE_PRINTING=1' \
	'-DENABLE_MANAGED_USERS=1' \
	'-DVIDEO_HOLE=1' \
	'-DENABLE_LOAD_COMPLETION_HACKS=1' \
	'-DBLINK_IMPLEMENTATION=1' \
	'-DINSIDE_BLINK' \
	'-DENABLE_OPENTYPE_VERTICAL=1' \
	'-DWTF_USE_CONCATENATED_IMPULSE_RESPONSES=1' \
	'-DWTF_USE_WEBAUDIO_OPENMAX_DL_FFT=1' \
	'-DENABLE_WEB_AUDIO=1' \
	'-DU_USING_ICU_NAMESPACE=0' \
	'-DU_ENABLE_DYLOAD=0' \
	'-DU_STATIC_IMPLEMENTATION' \
	'-DSK_ENABLE_INST_COUNT=0' \
	'-DSK_SUPPORT_GPU=1' \
	'-DGR_GL_CUSTOM_SETUP_HEADER="GrGLConfig_chrome.h"' \
	'-DSK_ENABLE_LEGACY_API_ALIASING=1' \
	'-DSK_ATTR_DEPRECATED=SK_NOTHING_ARG1' \
	'-DGR_GL_IGNORE_ES3_MSAA=0' \
	'-DSK_WILL_NEVER_DRAW_PERSPECTIVE_TEXT' \
	'-DSK_FM_NEW_MATCH_FAMILY_STYLE_CHARACTER=1' \
	'-DSK_SUPPORT_LEGACY_TEXTRENDERMODE' \
	'-DSK_LEGACY_NO_DISTANCE_FIELD_PATHS' \
	'-DSK_BUILD_FOR_ANDROID' \
	'-DSK_USE_POSIX_THREADS' \
	'-DCHROME_PNG_WRITE_SUPPORT' \
	'-DPNG_USER_CONFIG' \
	'-DCHROME_PNG_READ_PACK_SUPPORT' \
	'-DLIBXML_STATIC' \
	'-DLIBXSLT_STATIC' \
	'-DUSE_OPENSSL=1' \
	'-DUSE_OPENSSL_CERTS=1' \
	'-D__STDC_CONSTANT_MACROS' \
	'-D__STDC_FORMAT_MACROS' \
	'-DANDROID' \
	'-D__GNU_SOURCE=1' \
	'-DUSE_STLPORT=1' \
	'-D_STLP_USE_PTR_SPECIALIZATIONS=1' \
	'-DCHROME_BUILD_ID=""' \
	'-DNDEBUG' \
	'-DNVALGRIND' \
	'-DDYNAMIC_ANNOTATIONS_ENABLED=0' \
	'-D_FORTIFY_SOURCE=2'


# Include paths placed before CFLAGS/CPPFLAGS
LOCAL_C_INCLUDES_Release := \
	$(gyp_shared_intermediate_dir) \
	$(LOCAL_PATH)/third_party/WebKit/Source \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/skia/config \
	$(LOCAL_PATH)/third_party/khronos \
	$(LOCAL_PATH)/gpu \
	$(gyp_shared_intermediate_dir)/blink \
	$(LOCAL_PATH)/third_party/openmax_dl \
	$(LOCAL_PATH)/third_party/angle/include \
	$(LOCAL_PATH)/third_party/icu/source/i18n \
	$(LOCAL_PATH)/third_party/icu/source/common \
	$(LOCAL_PATH)/third_party/WebKit \
	$(LOCAL_PATH)/third_party/skia/src/core \
	$(LOCAL_PATH)/third_party/skia/include/core \
	$(LOCAL_PATH)/third_party/skia/include/effects \
	$(LOCAL_PATH)/third_party/skia/include/pdf \
	$(LOCAL_PATH)/third_party/skia/include/gpu \
	$(LOCAL_PATH)/third_party/skia/include/lazy \
	$(LOCAL_PATH)/third_party/skia/include/pathops \
	$(LOCAL_PATH)/third_party/skia/include/pipe \
	$(LOCAL_PATH)/third_party/skia/include/ports \
	$(LOCAL_PATH)/third_party/skia/include/utils \
	$(LOCAL_PATH)/skia/ext \
	$(LOCAL_PATH)/third_party/iccjpeg \
	$(LOCAL_PATH)/third_party/libpng \
	$(LOCAL_PATH)/third_party/libwebp \
	$(LOCAL_PATH)/third_party/libxml/linux/include \
	$(LOCAL_PATH)/third_party/libxml/src/include \
	$(LOCAL_PATH)/third_party/libxslt \
	$(LOCAL_PATH)/third_party/npapi \
	$(LOCAL_PATH)/third_party/npapi/bindings \
	$(LOCAL_PATH)/third_party/ots/include \
	$(LOCAL_PATH)/third_party/qcms/src \
	$(LOCAL_PATH)/third_party/sqlite \
	$(LOCAL_PATH)/third_party/zlib \
	$(LOCAL_PATH)/v8/include


# Flags passed to only C++ (and not C) files.
LOCAL_CPPFLAGS_Release := \
	-fno-exceptions \
	-fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden \
	-Wsign-compare \
	-Wno-c++0x-compat \
	-Wno-uninitialized \
	-std=gnu++11 \
	-Wno-narrowing \
	-Wno-literal-suffix \
	-Wno-non-virtual-dtor \
	-Wno-sign-promo


LOCAL_CFLAGS := $(MY_CFLAGS_$(GYP_CONFIGURATION)) $(MY_DEFS_$(GYP_CONFIGURATION))
LOCAL_C_INCLUDES := $(GYP_COPIED_SOURCE_ORIGIN_DIRS) $(LOCAL_C_INCLUDES_$(GYP_CONFIGURATION))
LOCAL_CPPFLAGS := $(LOCAL_CPPFLAGS_$(GYP_CONFIGURATION))
LOCAL_ASFLAGS := $(LOCAL_CFLAGS)
### Rules for final target.
### Set directly by aosp_build_settings.
LOCAL_CLANG := false
LOCAL_NDK_STL_VARIANT := stlport_static

# Add target alias to "gyp_all_modules" target.
.PHONY: gyp_all_modules
gyp_all_modules: third_party_WebKit_Source_core_webcore_dom_gyp

# Alias gyp target name.
.PHONY: webcore_dom
webcore_dom: third_party_WebKit_Source_core_webcore_dom_gyp

include $(BUILD_STATIC_LIBRARY)

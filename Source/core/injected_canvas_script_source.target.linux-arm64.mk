# This file is generated by gyp; do not edit.

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := GYP
LOCAL_MODULE := third_party_WebKit_Source_core_injected_canvas_script_source_gyp
LOCAL_MODULE_STEM := injected_canvas_script_source
LOCAL_MODULE_SUFFIX := .stamp
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_TARGET_ARCH := $(TARGET_$(GYP_VAR_PREFIX)ARCH)
gyp_intermediate_dir := $(call local-intermediates-dir,,$(GYP_VAR_PREFIX))
gyp_shared_intermediate_dir := $(call intermediates-dir-for,GYP,shared,,,$(GYP_VAR_PREFIX))

# Make sure our deps are built first.
GYP_TARGET_DEPENDENCIES :=

### Rules for action "ConvertFileToHeaderWithCharacterArray":
$(gyp_shared_intermediate_dir)/blink/core/InjectedScriptCanvasModuleSource.h: gyp_local_path := $(LOCAL_PATH)
$(gyp_shared_intermediate_dir)/blink/core/InjectedScriptCanvasModuleSource.h: gyp_var_prefix := $(GYP_VAR_PREFIX)
$(gyp_shared_intermediate_dir)/blink/core/InjectedScriptCanvasModuleSource.h: gyp_intermediate_dir := $(abspath $(gyp_intermediate_dir))
$(gyp_shared_intermediate_dir)/blink/core/InjectedScriptCanvasModuleSource.h: gyp_shared_intermediate_dir := $(abspath $(gyp_shared_intermediate_dir))
$(gyp_shared_intermediate_dir)/blink/core/InjectedScriptCanvasModuleSource.h: export PATH := $(subst $(ANDROID_BUILD_PATHS),,$(PATH))
$(gyp_shared_intermediate_dir)/blink/core/InjectedScriptCanvasModuleSource.h: $(LOCAL_PATH)/third_party/WebKit/Source/build/scripts/xxd.py $(LOCAL_PATH)/third_party/WebKit/Source/core/inspector/InjectedScriptCanvasModuleSource.js $(GYP_TARGET_DEPENDENCIES)
	@echo "Gyp action: Generating $(gyp_shared_intermediate_dir)/blink/core/InjectedScriptCanvasModuleSource.h from inspector/InjectedScriptCanvasModuleSource.js ($@)"
	$(hide)cd $(gyp_local_path)/third_party/WebKit/Source/core; mkdir -p $(gyp_shared_intermediate_dir)/blink/core; python ../build/scripts/xxd.py InjectedScriptCanvasModuleSource_js inspector/InjectedScriptCanvasModuleSource.js "$(gyp_shared_intermediate_dir)/blink/core/InjectedScriptCanvasModuleSource.h"



GYP_GENERATED_OUTPUTS := \
	$(gyp_shared_intermediate_dir)/blink/core/InjectedScriptCanvasModuleSource.h

# Make sure our deps and generated files are built first.
LOCAL_ADDITIONAL_DEPENDENCIES := $(GYP_TARGET_DEPENDENCIES) $(GYP_GENERATED_OUTPUTS)

### Rules for final target.
# Add target alias to "gyp_all_modules" target.
.PHONY: gyp_all_modules
gyp_all_modules: third_party_WebKit_Source_core_injected_canvas_script_source_gyp

# Alias gyp target name.
.PHONY: injected_canvas_script_source
injected_canvas_script_source: third_party_WebKit_Source_core_injected_canvas_script_source_gyp

LOCAL_MODULE_PATH := $(PRODUCT_OUT)/gyp_stamp
LOCAL_UNINSTALLABLE_MODULE := true
LOCAL_2ND_ARCH_VAR_PREFIX := $(GYP_VAR_PREFIX)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(LOCAL_ADDITIONAL_DEPENDENCIES)
	$(hide) echo "Gyp timestamp: $@"
	$(hide) mkdir -p $(dir $@)
	$(hide) touch $@

LOCAL_2ND_ARCH_VAR_PREFIX :=
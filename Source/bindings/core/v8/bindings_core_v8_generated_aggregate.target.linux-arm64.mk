# This file is generated by gyp; do not edit.

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := GYP
LOCAL_MODULE := third_party_WebKit_Source_bindings_core_v8_bindings_core_v8_generated_aggregate_gyp
LOCAL_MODULE_STEM := bindings_core_v8_generated_aggregate
LOCAL_MODULE_SUFFIX := .stamp
LOCAL_MODULE_TARGET_ARCH := $(TARGET_$(GYP_VAR_PREFIX)ARCH)
LOCAL_SDK_VERSION := 21
gyp_intermediate_dir := $(call local-intermediates-dir,,$(GYP_VAR_PREFIX))
gyp_shared_intermediate_dir := $(call intermediates-dir-for,GYP,shared,,,$(GYP_VAR_PREFIX))

# Make sure our deps are built first.
GYP_TARGET_DEPENDENCIES :=

### Rules for action "generate_aggregate_bindings_core_v8":
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp: gyp_local_path := $(LOCAL_PATH)
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp: gyp_var_prefix := $(GYP_VAR_PREFIX)
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp: gyp_intermediate_dir := $(abspath $(gyp_intermediate_dir))
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp: gyp_shared_intermediate_dir := $(abspath $(gyp_shared_intermediate_dir))
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp: export PATH := $(subst $(ANDROID_BUILD_PATHS),,$(PATH))
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp: $(LOCAL_PATH)/third_party/WebKit/Source/bindings/scripts/aggregate_generated_bindings.py $(LOCAL_PATH)/third_party/WebKit/Source/bindings/core/v8/core_idl_files_list.tmp $(GYP_TARGET_DEPENDENCIES)
	@echo "Gyp action: Generating aggregate generated core V8 bindings files ($@)"
	$(hide)cd $(gyp_local_path)/third_party/WebKit/Source/bindings/core/v8; mkdir -p $(gyp_shared_intermediate_dir)/blink/bindings/core/v8; python ../../scripts/aggregate_generated_bindings.py core core_idl_files_list.tmp -- "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings02.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings03.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings04.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings05.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings06.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings07.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings08.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings09.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings10.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings11.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings12.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings13.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings14.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings15.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings16.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings17.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings18.cpp" "$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings19.cpp"

$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings02.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings03.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings04.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings05.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings06.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings07.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings08.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings09.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings10.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings11.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings12.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings13.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings14.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings15.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings16.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings17.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings18.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;
$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings19.cpp: $(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp ;


GYP_GENERATED_OUTPUTS := \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings01.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings02.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings03.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings04.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings05.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings06.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings07.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings08.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings09.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings10.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings11.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings12.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings13.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings14.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings15.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings16.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings17.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings18.cpp \
	$(gyp_shared_intermediate_dir)/blink/bindings/core/v8/V8GeneratedCoreBindings19.cpp

# Make sure our deps and generated files are built first.
LOCAL_ADDITIONAL_DEPENDENCIES := $(GYP_TARGET_DEPENDENCIES) $(GYP_GENERATED_OUTPUTS)

### Rules for final target.
### Set directly by aosp_build_settings.
LOCAL_CLANG := false
LOCAL_NDK_STL_VARIANT := stlport_static

# Add target alias to "gyp_all_modules" target.
.PHONY: gyp_all_modules
gyp_all_modules: third_party_WebKit_Source_bindings_core_v8_bindings_core_v8_generated_aggregate_gyp

# Alias gyp target name.
.PHONY: bindings_core_v8_generated_aggregate
bindings_core_v8_generated_aggregate: third_party_WebKit_Source_bindings_core_v8_bindings_core_v8_generated_aggregate_gyp

LOCAL_MODULE_PATH := $(PRODUCT_OUT)/gyp_stamp
LOCAL_UNINSTALLABLE_MODULE := true
LOCAL_2ND_ARCH_VAR_PREFIX := $(GYP_VAR_PREFIX)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(LOCAL_ADDITIONAL_DEPENDENCIES)
	$(hide) echo "Gyp timestamp: $@"
	$(hide) mkdir -p $(dir $@)
	$(hide) touch $@

LOCAL_2ND_ARCH_VAR_PREFIX :=

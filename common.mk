#Common headers
common_includes := hardware/qcom/display-caf/libgralloc
<<<<<<< HEAD
common_includes += hardware/qcom/display-caf/libgenlock
=======
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
common_includes += hardware/qcom/display-caf/liboverlay
common_includes += hardware/qcom/display-caf/libcopybit
common_includes += hardware/qcom/display-caf/libqdutils
common_includes += hardware/qcom/display-caf/libhwcomposer
common_includes += hardware/qcom/display-caf/libexternal
common_includes += hardware/qcom/display-caf/libqservice
<<<<<<< HEAD

ifeq ($(TARGET_USES_POST_PROCESSING),true)
    common_flags     += -DUSES_POST_PROCESSING
    common_includes += $(TARGET_OUT_HEADERS)/pp/inc
endif
=======
common_includes += hardware/qcom/display-caf/libvirtual
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c

common_header_export_path := qcom/display

#Common libraries external to display-caf HAL
common_libs := liblog libutils libcutils libhardware

#Common C flags
common_flags := -DDEBUG_CALC_FPS -Wno-missing-field-initializers
<<<<<<< HEAD
#common_flags += -Werror
=======
common_flags += -Werror
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c

ifeq ($(ARCH_ARM_HAVE_NEON),true)
    common_flags += -D__ARM_HAVE_NEON
endif

<<<<<<< HEAD
ifeq ($(TARGET_BOARD_PLATFORM), msm8974)
    common_flags += -DVENUS_COLOR_FORMAT
endif

common_deps  :=
kernel_includes :=

# Executed only on QCOM BSPs
ifeq ($(TARGET_USES_QCOM_BSP),true)
# This flag is used to compile out any features that depend on framework changes
    common_flags += -DQCOM_BSP
endif

ifeq ($(call is-vendor-board-platform,QCOM),true)
common_deps += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr
kernel_includes += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
=======
ifneq ($(filter msm8974 msm8x74 msm8226 msm8x26,$(TARGET_BOARD_PLATFORM)),)
    # TODO: This define makes us pick a few inline functions
    # from the kernel header media/msm_media_info.h. However,
    # the bionic clean_headers utility scrubs them out.
    # Figure out a way to import those macros correctly
    # common_flags += -DVENUS_COLOR_FORMAT
    common_flags += -DMDSS_TARGET
endif

# Executed only on QCOM BSPs
ifeq ($(TARGET_USES_QCOM_BSP),true)
# This flag is used to compile out any features that depend on framework changes
    common_flags += -DQCOM_BSP
endif

ifeq ($(call is-vendor-board-platform,QCOM),true)
common_deps += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr
kernel_includes += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
endif

ifeq ($(TARGET_DISPLAY_USE_RETIRE_FENCE),true)
    common_flags += -DUSE_RETIRE_FENCE
endif

ifneq ($(TARGET_DISPLAY_INSECURE_MM_HEAP),true)
    common_flags += -DSECURE_MM_HEAP
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
endif


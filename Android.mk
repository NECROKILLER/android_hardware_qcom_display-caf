ifeq ($(TARGET_QCOM_DISPLAY_VARIANT),caf)
<<<<<<< HEAD
display-hals := libgralloc libgenlock libcopybit
display-hals += libhwcomposer liboverlay libqdutils libexternal libqservice
=======
display-hals := libgralloc libgenlock libcopybit libhwcomposer liboverlay
display-hals += libqdutils libexternal libvirtual libqservice
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
ifneq ($(TARGET_PROVIDES_LIBLIGHT),true)
display-hals += liblight
endif

ifeq ($(call is-vendor-board-platform,QCOM),true)
    include $(call all-named-subdir-makefiles,$(display-hals))
else
<<<<<<< HEAD
ifneq ($(filter msm8960,$(TARGET_BOARD_PLATFORM)),)
=======
ifneq ($(filter msm8226 msm8x26 msm8960 msm8974 msm8x74,$(TARGET_BOARD_PLATFORM)),)
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    #This is for mako since it doesn't have the QCOM make functions
    include $(call all-named-subdir-makefiles,$(display-hals))
endif
endif
endif

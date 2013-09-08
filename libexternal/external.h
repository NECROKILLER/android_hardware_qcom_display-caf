/*
 * Copyright (C) 2010 The Android Open Source Project
 * Copyright (C) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Not a Contribution, Apache license notifications and license are
 * retained for attribution purposes only.

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HWC_EXTERNAL_DISPLAY_H
#define HWC_EXTERNAL_DISPLAY_H

#include <linux/fb.h>

struct hwc_context_t;
struct msm_hdmi_mode_timing_info;

namespace qhwc {

//Type of scanning of EDID(Video Capability Data Block)
enum external_scansupport_type {
    EXT_SCAN_NOT_SUPPORTED      = 0,
    EXT_SCAN_ALWAYS_OVERSCANED  = 1,
    EXT_SCAN_ALWAYS_UNDERSCANED = 2,
    EXT_SCAN_BOTH_SUPPORTED     = 3
};

class ExternalDisplay
{
public:
    ExternalDisplay(hwc_context_t* ctx);
    ~ExternalDisplay();
<<<<<<< HEAD
    int getModeCount() const;
    void getEDIDModes(int *out) const;
    bool isCEUnderscanSupported() { return mUnderscanSupported; }
    void setExternalDisplay(bool connected, int extFbNum = 0);
    bool isExternalConnected() { return mConnected;};
    void  setExtDpyNum(int extDpyNum) { mExtDpyNum = extDpyNum;};
    bool post();
=======
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    void setHPD(uint32_t startEnd);
    void setActionSafeDimension(int w, int h);
<<<<<<< HEAD
    void processUEventOnline(const char *str);
    void processUEventOffline(const char *str);

private:
=======
    bool isCEUnderscanSupported() { return mUnderscanSupported; }
    int configure();
    int teardown();
    bool isConnected() {
        return  mHwcContext->dpyAttr[HWC_DISPLAY_EXTERNAL].connected;
    }

private:
    int getModeCount() const;
    void getEDIDModes(int *out) const;
    void setEDIDMode(int resMode);
    void setSPDInfo(const char* node, const char* property);
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    void readCEUnderscanInfo();
    bool readResolution();
    int  parseResolution(char* edidStr, int* edidModes);
    void setResolution(int ID);
<<<<<<< HEAD
    bool openFrameBuffer(int fbNum);
    bool closeFrameBuffer();
    bool writeHPDOption(int userOption) const;
    bool isValidMode(int ID);
    void handleUEvent(char* str, int len);
=======
    bool openFrameBuffer();
    bool closeFrameBuffer();
    bool writeHPDOption(int userOption) const;
    bool isValidMode(int ID);
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    int  getModeOrder(int mode);
    int  getUserMode();
    int  getBestMode();
    bool isInterlacedMode(int mode);
    void resetInfo();
<<<<<<< HEAD
    void setDpyHdmiAttr();
    void setDpyWfdAttr();
=======
    void setAttributes();
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    void getAttrForMode(int& width, int& height, int& fps);
    void updateExtDispDevFbIndex();
    int  configureHDMIDisplay();
    int  configureWFDDisplay();
    int  teardownHDMIDisplay();
    int  teardownWFDDisplay();
    int  getExtFbNum(int &fbNum);

    int mFd;
    int mFbNum;
    int mCurrentMode;
<<<<<<< HEAD
    int mConnected;
    int mConnectedFbNum;
    int mResolutionMode;
=======
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    char mEDIDs[128];
    int mEDIDModes[64];
    int mModeCount;
    bool mUnderscanSupported;
    hwc_context_t *mHwcContext;
    fb_var_screeninfo mVInfo;
<<<<<<< HEAD
    int mHdmiFbNum;
    int mWfdFbNum;
    int mExtDpyNum;
=======
    // Holds all the HDMI modes and timing info supported by driver
    msm_hdmi_mode_timing_info* supported_video_mode_lut;
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
};

}; //qhwc
// ---------------------------------------------------------------------------
#endif //HWC_EXTERNAL_DISPLAY_H

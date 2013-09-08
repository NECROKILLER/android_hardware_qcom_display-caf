/*
 * Copyright (C) 2010 The Android Open Source Project
 * Copyright (C) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Not a Contribution, Apache license notifications and license are retained
 * for attribution purposes only.
 *
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
<<<<<<< HEAD
#define ATRACE_TAG ATRACE_TAG_GRAPHICS
=======
#define ATRACE_TAG (ATRACE_TAG_GRAPHICS | ATRACE_TAG_HAL)
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
#include <fcntl.h>
#include <errno.h>

#include <cutils/log.h>
#include <cutils/atomic.h>
#include <EGL/egl.h>
#include <utils/Trace.h>
<<<<<<< HEAD

=======
#include <sys/ioctl.h>
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
#include <overlay.h>
#include <overlayRotator.h>
#include <mdp_version.h>
#include "hwc_utils.h"
<<<<<<< HEAD
#include "hwc_video.h"
=======
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
#include "hwc_fbupdate.h"
#include "hwc_mdpcomp.h"
#include "external.h"
#include "hwc_copybit.h"
<<<<<<< HEAD

using namespace qhwc;
#define VSYNC_DEBUG 0
=======
#include "profiler.h"

using namespace qhwc;
using namespace overlay;

#define VSYNC_DEBUG 0
#define BLANK_DEBUG 1
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c

static int hwc_device_open(const struct hw_module_t* module,
                           const char* name,
                           struct hw_device_t** device);

static struct hw_module_methods_t hwc_module_methods = {
    open: hwc_device_open
};

hwc_module_t HAL_MODULE_INFO_SYM = {
    common: {
        tag: HARDWARE_MODULE_TAG,
        version_major: 2,
        version_minor: 0,
        id: HWC_HARDWARE_MODULE_ID,
        name: "Qualcomm Hardware Composer Module",
        author: "CodeAurora Forum",
        methods: &hwc_module_methods,
        dso: 0,
        reserved: {0},
    }
};

/* In case of proprietary WFD session, we are fooling SF by piggybacking on
 * HDMI display ID for virtual. This helper is needed to differentiate their
 * paths in HAL.
 * TODO: Not needed once we have WFD client working on top of Google API's */

static int getHWCDpy(hwc_context_t *ctx, int dpy) {
    if(dpy == HWC_DISPLAY_EXTERNAL && ctx->mVirtualonExtActive)
        return HWC_DISPLAY_VIRTUAL;
    return dpy;
}

/*
 * Save callback functions registered to HWC
 */
static void hwc_registerProcs(struct hwc_composer_device_1* dev,
                              hwc_procs_t const* procs)
{
    ALOGI("%s", __FUNCTION__);
    hwc_context_t* ctx = (hwc_context_t*)(dev);
    if(!ctx) {
        ALOGE("%s: Invalid context", __FUNCTION__);
        return;
    }
    ctx->proc = procs;

    // Now that we have the functions needed, kick off
    // the uevent & vsync threads
    init_uevent_thread(ctx);
    init_vsync_thread(ctx);
}

//Helper
static void reset(hwc_context_t *ctx, int numDisplays,
                  hwc_display_contents_1_t** displays) {
    memset(ctx->listStats, 0, sizeof(ctx->listStats));
<<<<<<< HEAD
    for(int i = 0; i < MAX_DISPLAYS; i++) {
=======
    for(int i = 0; i < HWC_NUM_DISPLAY_TYPES; i++) {
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
        hwc_display_contents_1_t *list = displays[i];
        // XXX:SurfaceFlinger no longer guarantees that this
        // value is reset on every prepare. However, for the layer
        // cache we need to reset it.
        // We can probably rethink that later on
        if (LIKELY(list && list->numHwLayers > 1)) {
            for(uint32_t j = 0; j < list->numHwLayers; j++) {
                if(list->hwLayers[j].compositionType != HWC_FRAMEBUFFER_TARGET)
                    list->hwLayers[j].compositionType = HWC_FRAMEBUFFER;
            }
        }

        if(ctx->mFBUpdate[i])
            ctx->mFBUpdate[i]->reset();
<<<<<<< HEAD

        if(ctx->mCopyBit[i])
            ctx->mCopyBit[i]->reset();
    }
    VideoOverlay::reset();
}

//clear prev layer prop flags and realloc for current frame
static void reset_layer_prop(hwc_context_t* ctx, int dpy) {
    int layer_count = ctx->listStats[dpy].numAppLayers;

    if(ctx->layerProp[dpy]) {
       delete[] ctx->layerProp[dpy];
       ctx->layerProp[dpy] = NULL;
    }

    if(layer_count) {
       ctx->layerProp[dpy] = new LayerProp[layer_count];
=======
        if(ctx->mCopyBit[i])
            ctx->mCopyBit[i]->reset();
        if(ctx->mLayerRotMap[i])
            ctx->mLayerRotMap[i]->reset();
    }

}

//clear prev layer prop flags and realloc for current frame
static void reset_layer_prop(hwc_context_t* ctx, int dpy, int numAppLayers) {
    if(ctx->layerProp[dpy]) {
       delete[] ctx->layerProp[dpy];
       ctx->layerProp[dpy] = NULL;
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    }
    ctx->layerProp[dpy] = new LayerProp[numAppLayers];
}


static int hwc_prepare_primary(hwc_composer_device_1 *dev,
        hwc_display_contents_1_t *list) {
    hwc_context_t* ctx = (hwc_context_t*)(dev);
    const int dpy = HWC_DISPLAY_PRIMARY;
    if(UNLIKELY(!ctx->mBasePipeSetup) && 
            qdutils::MDPVersion::getInstance().getMDPVersion() >= qdutils::MDP_V4_2)
        setupBasePipe(ctx);
    if (LIKELY(list && list->numHwLayers > 1) &&
<<<<<<< HEAD
        ctx->dpyAttr[dpy].isActive) {

=======
            ctx->dpyAttr[dpy].isActive) {
        reset_layer_prop(ctx, dpy, list->numHwLayers - 1);
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
        uint32_t last = list->numHwLayers - 1;
        hwc_layer_1_t *fbLayer = &list->hwLayers[last];
        if(fbLayer->handle) {
            setListStats(ctx, list, dpy);
<<<<<<< HEAD
            reset_layer_prop(ctx, dpy);
            int ret = ctx->mMDPComp->prepare(ctx, list);
            if(!ret) {
                // IF MDPcomp fails use this route
                VideoOverlay::prepare(ctx, list, dpy);
                ctx->mFBUpdate[dpy]->prepare(ctx, list);
            }
            ctx->mLayerCache[dpy]->updateLayerCache(list);
            if (ctx->mMDP.version < qdutils::MDP_V4_0) {
                if(!ret && ctx->mCopyBit[dpy])
                  ctx->mCopyBit[dpy]->prepare(ctx, list, dpy);

=======
            if(ctx->mMDPComp[dpy]->prepare(ctx, list) < 0) {
                const int fbZ = 0;
                ctx->mFBUpdate[dpy]->prepare(ctx, list, fbZ);

                // Use Copybit, when MDP comp fails
                // (only for 8960 which has  dedicated 2D core)
                if( (ctx->mMDP.version == qdutils::MDP_V4_1) &&
                                             ctx->mCopyBit[dpy])
                    ctx->mCopyBit[dpy]->prepare(ctx, list, dpy);
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
            }
        }
    }
    return 0;
}

static int hwc_prepare_external(hwc_composer_device_1 *dev,
        hwc_display_contents_1_t *list, int dpy) {
    hwc_context_t* ctx = (hwc_context_t*)(dev);
    const int dpy = HWC_DISPLAY_EXTERNAL;

    if (LIKELY(list && list->numHwLayers > 1) &&
<<<<<<< HEAD
        ctx->dpyAttr[dpy].isActive &&
        ctx->dpyAttr[dpy].connected) {
        uint32_t last = list->numHwLayers - 1;
        if(!ctx->dpyAttr[dpy].isPause) {
            hwc_layer_1_t *fbLayer = &list->hwLayers[last];
            if(fbLayer->handle) {
                setListStats(ctx, list, dpy);
                reset_layer_prop(ctx, dpy);
                VideoOverlay::prepare(ctx, list, dpy);
                ctx->mFBUpdate[dpy]->prepare(ctx, list);
                ctx->mLayerCache[dpy]->updateLayerCache(list);
                if(ctx->mCopyBit[dpy])
                    ctx->mCopyBit[dpy]->prepare(ctx, list, dpy);
                ctx->mExtDispConfiguring = false;
            }
        } else {
            // External Display is in Pause state.
=======
            ctx->dpyAttr[dpy].isActive &&
            ctx->dpyAttr[dpy].connected) {
        reset_layer_prop(ctx, dpy, list->numHwLayers - 1);
        uint32_t last = list->numHwLayers - 1;
        hwc_layer_1_t *fbLayer = &list->hwLayers[last];
        if(!ctx->dpyAttr[dpy].isPause) {
            if(fbLayer->handle) {
                ctx->dpyAttr[dpy].isConfiguring = false;
                setListStats(ctx, list, dpy);
                if(ctx->mMDPComp[dpy]->prepare(ctx, list) < 0) {
                    const int fbZ = 0;
                    ctx->mFBUpdate[dpy]->prepare(ctx, list, fbZ);
                    // Use Copybit, when MDP comp fails
                    // (only for 8960 which has  dedicated 2D core)
                    if((ctx->mMDP.version == qdutils::MDP_V4_1) &&
                                              ctx->mCopyBit[dpy] &&
                            !ctx->listStats[dpy].isDisplayAnimating)
                        ctx->mCopyBit[dpy]->prepare(ctx, list, dpy);
                }

                if(ctx->listStats[dpy].isDisplayAnimating) {
                    // Mark all app layers as HWC_OVERLAY for external during
                    // animation, so that SF doesnt draw it on FB
                    for(int i = 0 ;i < ctx->listStats[dpy].numAppLayers; i++) {
                        hwc_layer_1_t *layer = &list->hwLayers[i];
                        layer->compositionType = HWC_OVERLAY;
                    }
                }
            }
        } else {
            // External Display is in Pause state.
            // ToDo:
            // Mark all application layers as OVERLAY so that
            // GPU will not compose. This is done for power
            // optimization
        }
    }
    return 0;
}

static int hwc_prepare_virtual(hwc_composer_device_1 *dev,
        hwc_display_contents_1_t *list) {

    hwc_context_t* ctx = (hwc_context_t*)(dev);
    const int dpy = HWC_DISPLAY_VIRTUAL;

    if (LIKELY(list && list->numHwLayers > 1) &&
            ctx->dpyAttr[dpy].isActive &&
            ctx->dpyAttr[dpy].connected) {
        reset_layer_prop(ctx, dpy, list->numHwLayers - 1);
        uint32_t last = list->numHwLayers - 1;
        hwc_layer_1_t *fbLayer = &list->hwLayers[last];
        if(!ctx->dpyAttr[dpy].isPause) {
            if(fbLayer->handle) {
                ctx->dpyAttr[dpy].isConfiguring = false;
                setListStats(ctx, list, dpy);
                if(ctx->mMDPComp[dpy]->prepare(ctx, list) < 0) {
                    const int fbZ = 0;
                    ctx->mFBUpdate[dpy]->prepare(ctx, list, fbZ);
                }

                if(ctx->listStats[dpy].isDisplayAnimating) {
                    // Mark all app layers as HWC_OVERLAY for virtual during
                    // animation, so that SF doesnt draw it on FB
                    for(int i = 0 ;i < ctx->listStats[dpy].numAppLayers; i++) {
                        hwc_layer_1_t *layer = &list->hwLayers[i];
                        layer->compositionType = HWC_OVERLAY;
                    }
                }
            }
        } else {
            // Virtual Display is in Pause state.
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
            // ToDo:
            // Mark all application layers as OVERLAY so that
            // GPU will not compose. This is done for power
            // optimization
        }
    }
    return 0;
}


static int hwc_prepare(hwc_composer_device_1 *dev, size_t numDisplays,
                       hwc_display_contents_1_t** displays)
{
    int ret = 0;
    hwc_context_t* ctx = (hwc_context_t*)(dev);
<<<<<<< HEAD
    Locker::Autolock _l(ctx->mBlankLock);
    reset(ctx, numDisplays, displays);

    ctx->mOverlay->configBegin();
=======
    //Will be unlocked at the end of set
    ctx->mDrawLock.lock();
    reset(ctx, numDisplays, displays);

    ctx->mOverlay->configBegin();
    ctx->mRotMgr->configBegin();
    ctx->mNeedsRotator = false;
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c

    for (int32_t i = numDisplays; i >= 0; i--) {
        hwc_display_contents_1_t *list = displays[i];
        int dpy = getHWCDpy(ctx, i);
        switch(dpy) {
            case HWC_DISPLAY_PRIMARY:
                ret = hwc_prepare_primary(dev, list);
                break;
            case HWC_DISPLAY_EXTERNAL:
            case HWC_DISPLAY_VIRTUAL:
                ret = hwc_prepare_external(dev, list, i);
                break;
            case HWC_DISPLAY_VIRTUAL:
                ret = hwc_prepare_virtual(dev, list);
                break;
            default:
                ret = -EINVAL;
        }
    }

    ctx->mOverlay->configDone();
<<<<<<< HEAD
=======
    ctx->mRotMgr->configDone();

>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    return ret;
}

static int hwc_eventControl(struct hwc_composer_device_1* dev, int dpy,
                             int event, int enable)
{
    int ret = 0;
    hwc_context_t* ctx = (hwc_context_t*)(dev);

    if(!ctx->dpyAttr[dpy].isActive) {
        ALOGE("Display is blanked - Cannot %s vsync",
              enable ? "enable" : "disable");
        return -EINVAL;
    }

    switch(event) {
        case HWC_EVENT_VSYNC:
            if (ctx->vstate.enable == enable)
                break;
            ret = hwc_vsync_control(ctx, dpy, enable);
<<<<<<< HEAD
            if(ret == 0) {
                ctx->vstate.enable = !!enable;
                pthread_cond_signal(&ctx->vstate.cond);
            }
            ALOGD_IF (VSYNC_DEBUG, "VSYNC state changed to %s",
                      (enable)?"ENABLED":"DISABLED");
=======
            if(ret == 0)
                ctx->vstate.enable = !!enable;
            ALOGD_IF (VSYNC_DEBUG, "VSYNC state changed to %s",
                      (enable)?"ENABLED":"DISABLED");
            break;
#ifdef QCOM_BSP
        case  HWC_EVENT_ORIENTATION:
            if(dpy == HWC_DISPLAY_PRIMARY) {
                Locker::Autolock _l(ctx->mDrawLock);
                // store the primary display orientation
                // will be used in hwc_video::configure to disable
                // rotation animation on external display
                ctx->deviceOrientation = enable;
            }
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
            break;
#endif
        default:
            ret = -EINVAL;
    }
    return ret;
}

static int hwc_blank(struct hwc_composer_device_1* dev, int dpy, int blank)
{
    ATRACE_CALL();
    hwc_context_t* ctx = (hwc_context_t*)(dev);
<<<<<<< HEAD
    private_module_t* m = reinterpret_cast<private_module_t*>(
        ctx->mFbDev->common.module);
    Locker::Autolock _l(ctx->mBlankLock);
    int ret = 0;
    ALOGD("%s: %s display: %d", __FUNCTION__,
          blank==1 ? "Blanking":"Unblanking", dpy);
    switch(dpy) {
        case HWC_DISPLAY_PRIMARY:
            if(blank) {
                ctx->mOverlay->configBegin();
                ctx->mOverlay->configDone();
                ret = ioctl(m->framebuffer->fd, FBIOBLANK, FB_BLANK_POWERDOWN);
            } else {
                ret = ioctl(m->framebuffer->fd, FBIOBLANK, FB_BLANK_UNBLANK);
            }
            break;
        case HWC_DISPLAY_EXTERNAL:
        case HWC_DISPLAY_VIRTUAL:
            if(blank) {
                // External/Virtual Display post commits the changes to display
                // Call this on blank, so that any pipe unsets gets committed
                if (!ctx->mExtDisplay->post()) {
                    ret = -1;
                    ALOGE("%s:ctx->mExtDisplay->post fail!! ", __FUNCTION__);
                }
            } else {
=======

    Locker::Autolock _l(ctx->mDrawLock);
    int ret = 0, value = 0;
    ALOGD_IF(BLANK_DEBUG, "%s: %s display: %d", __FUNCTION__,
          blank==1 ? "Blanking":"Unblanking", dpy);
    if(blank) {
        // free up all the overlay pipes in use
        // when we get a blank for either display
        // makes sure that all pipes are freed
        ctx->mOverlay->configBegin();
        ctx->mOverlay->configDone();
        ctx->mRotMgr->clear();
    }
    switch(dpy) {
    case HWC_DISPLAY_PRIMARY:
        if(blank) {
            int dpy = HWC_DISPLAY_PRIMARY;
            if(!Overlay::displayCommit(ctx->dpyAttr[dpy].fd)){
                ALOGE("%s: display commit fail for primary!", __FUNCTION__);
                ret = -1;
            }
        }
        value = blank ? FB_BLANK_POWERDOWN : FB_BLANK_UNBLANK;
        if(ioctl(ctx->dpyAttr[dpy].fd, FBIOBLANK, value) < 0 ) {
            ALOGE("%s: Failed to handle blank event(%d) for Primary!!",
                  __FUNCTION__, blank );
            return -1;
        }

        if(!blank) {
            // Enable HPD here, as during bootup unblank is called
            // when SF is completely initialized
            ctx->mExtDisplay->setHPD(1);
        }

        /* Since SF is not aware of VIRTUAL DISPLAY being handle by HWC,
         * it wont send blank / unblank events for it. We piggyback on
         * PRIMARY DISPLAY events to release mdp pips and
         * activate/deactive VIRTUAL DISPLAY */

        if(ctx->dpyAttr[HWC_DISPLAY_VIRTUAL].connected) {
            if(blank) {
                int dpy = HWC_DISPLAY_VIRTUAL;
                if(!Overlay::displayCommit(ctx->dpyAttr[dpy].fd,1)) {
                    ALOGE("%s: display commit fail for virtual!", __FUNCTION__);
                    ret = -1;
                }
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
            }
            ctx->dpyAttr[HWC_DISPLAY_VIRTUAL].isActive = !blank;
        }
        break;
    case HWC_DISPLAY_EXTERNAL:
        if(blank) {
            if(!Overlay::displayCommit(ctx->dpyAttr[dpy].fd,1)) {
                ALOGE("%s: display commit fail for external!", __FUNCTION__);
                ret = -1;
            }
        }
        break;
    default:
        return -EINVAL;
    }
<<<<<<< HEAD
    // Enable HPD here, as during bootup unblank is called
    // when SF is completely initialized
    ctx->mExtDisplay->setHPD(1);
    if(ret == 0){
        ctx->dpyAttr[dpy].isActive = !blank;
    } else {
        ALOGE("%s: Failed in %s display: %d error:%s", __FUNCTION__,
              blank==1 ? "blanking":"unblanking", dpy, strerror(errno));
        return ret;
    }

    ALOGD("%s: Done %s display: %d", __FUNCTION__,
          blank==1 ? "blanking":"unblanking", dpy);
    return 0;
=======

    ctx->dpyAttr[dpy].isActive = !blank;

    ALOGD_IF(BLANK_DEBUG, "%s: Done %s display: %d", __FUNCTION__,
          blank ? "blanking":"unblanking", dpy);
    return ret;
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
}

static int hwc_query(struct hwc_composer_device_1* dev,
                     int param, int* value)
{
    hwc_context_t* ctx = (hwc_context_t*)(dev);
    int supported = HWC_DISPLAY_PRIMARY_BIT;

    switch (param) {
    case HWC_BACKGROUND_LAYER_SUPPORTED:
        // Not supported for now
        value[0] = 0;
        break;
    case HWC_DISPLAY_TYPES_SUPPORTED:
        if(ctx->mMDP.hasOverlay)
            supported |= HWC_DISPLAY_EXTERNAL_BIT;
        value[0] = supported;
        break;
    default:
        return -EINVAL;
    }
    return 0;

}


static int hwc_set_primary(hwc_context_t *ctx, hwc_display_contents_1_t* list) {
    ATRACE_CALL();
    int ret = 0;
    const int dpy = HWC_DISPLAY_PRIMARY;
<<<<<<< HEAD

=======
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    if (LIKELY(list) && ctx->dpyAttr[dpy].isActive) {
        uint32_t last = list->numHwLayers - 1;
        hwc_layer_1_t *fbLayer = &list->hwLayers[last];
        int fd = -1; //FenceFD from the Copybit(valid in async mode)
        bool copybitDone = false;
        if(ctx->mCopyBit[dpy])
            copybitDone = ctx->mCopyBit[dpy]->draw(ctx, list, dpy, &fd);
        if(list->numHwLayers > 1)
            hwc_sync(ctx, list, dpy, fd);
<<<<<<< HEAD
        if (!VideoOverlay::draw(ctx, list, dpy)) {
            ALOGE("%s: VideoOverlay::draw fail!", __FUNCTION__);
            ret = -1;
        }
        if (!ctx->mMDPComp->draw(ctx, list)) {
            ALOGE("%s: MDPComp::draw fail!", __FUNCTION__);
=======


        if (!ctx->mMDPComp[dpy]->draw(ctx, list)) {
            ALOGE("%s: MDPComp draw failed", __FUNCTION__);
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
            ret = -1;
        }

        //TODO We dont check for SKIP flag on this layer because we need PAN
        //always. Last layer is always FB
<<<<<<< HEAD
        private_handle_t *hnd = NULL;
        if(copybitDone) {
            hnd = ctx->mCopyBit[dpy]->getCurrentRenderBuffer();
        } else {
            hnd = (private_handle_t *)fbLayer->handle;
        }
        if(fbLayer->compositionType == HWC_FRAMEBUFFER_TARGET && hnd) {
            if(!(fbLayer->flags & HWC_SKIP_LAYER) &&
                (list->numHwLayers > 1)) {
                if (!ctx->mFBUpdate[dpy]->draw(ctx, hnd)) {
                    ALOGE("%s: FBUpdate::draw fail!", __FUNCTION__);
                    ret = -1;
                }
            }
        }
        if (ctx->mFbDev->post(ctx->mFbDev, fbLayer->handle)) {
            ALOGE("%s: ctx->mFbDev->post fail!", __FUNCTION__);
=======
        private_handle_t *hnd = (private_handle_t *)fbLayer->handle;
        if(copybitDone) {
            hnd = ctx->mCopyBit[dpy]->getCurrentRenderBuffer();
        }

        if(hnd) {
            if (!ctx->mFBUpdate[dpy]->draw(ctx, hnd)) {
                ALOGE("%s: FBUpdate draw failed", __FUNCTION__);
                ret = -1;
            }
        }

        if(!Overlay::displayCommit(ctx->dpyAttr[dpy].fd)) {
            ALOGE("%s: display commit fail for %d dpy!", __FUNCTION__, dpy);
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
            ret = -1;
        }
    }

    closeAcquireFds(list);
    return ret;
}

static int hwc_set_external(hwc_context_t *ctx,
<<<<<<< HEAD
                            hwc_display_contents_1_t* list, int dpy)
=======
                            hwc_display_contents_1_t* list)
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
{
    ATRACE_CALL();
    int ret = 0;

<<<<<<< HEAD
    if (LIKELY(list) && ctx->dpyAttr[dpy].isActive &&
        !ctx->dpyAttr[dpy].isPause &&
        ctx->dpyAttr[dpy].connected) {
=======
    const int dpy = HWC_DISPLAY_EXTERNAL;


    if (LIKELY(list) && ctx->dpyAttr[dpy].isActive &&
        ctx->dpyAttr[dpy].connected &&
        !ctx->dpyAttr[dpy].isPause) {
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
        uint32_t last = list->numHwLayers - 1;
        hwc_layer_1_t *fbLayer = &list->hwLayers[last];
        int fd = -1; //FenceFD from the Copybit(valid in async mode)
        bool copybitDone = false;
        if(ctx->mCopyBit[dpy])
            copybitDone = ctx->mCopyBit[dpy]->draw(ctx, list, dpy, &fd);
<<<<<<< HEAD

        if(list->numHwLayers > 1)
            hwc_sync(ctx, list, dpy, fd);

        if (!VideoOverlay::draw(ctx, list, dpy)) {
            ALOGE("%s: VideoOverlay::draw fail!", __FUNCTION__);
            ret = -1;
        }

        private_handle_t *hnd = NULL;
        if(copybitDone) {
            hnd = ctx->mCopyBit[dpy]->getCurrentRenderBuffer();
        } else {
            hnd = (private_handle_t *)fbLayer->handle;
        }

        if(fbLayer->compositionType == HWC_FRAMEBUFFER_TARGET &&
                !(fbLayer->flags & HWC_SKIP_LAYER) && hnd &&
                (list->numHwLayers > 1)) {
=======

        if(list->numHwLayers > 1)
            hwc_sync(ctx, list, dpy, fd);


        if (!ctx->mMDPComp[dpy]->draw(ctx, list)) {
            ALOGE("%s: MDPComp draw failed", __FUNCTION__);
            ret = -1;
        }

        int extOnlyLayerIndex =
                ctx->listStats[dpy].extOnlyLayerIndex;

        private_handle_t *hnd = (private_handle_t *)fbLayer->handle;
        if(extOnlyLayerIndex!= -1) {
            hwc_layer_1_t *extLayer = &list->hwLayers[extOnlyLayerIndex];
            hnd = (private_handle_t *)extLayer->handle;
        } else if(copybitDone) {
            hnd = ctx->mCopyBit[dpy]->getCurrentRenderBuffer();
        }

        if(hnd && !isYuvBuffer(hnd)) {
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
            if (!ctx->mFBUpdate[dpy]->draw(ctx, hnd)) {
                ALOGE("%s: FBUpdate::draw fail!", __FUNCTION__);
                ret = -1;
            }
        }
<<<<<<< HEAD
        if (!ctx->mExtDisplay->post()) {
            ALOGE("%s: ctx->mExtDisplay->post fail!", __FUNCTION__);
=======

        if(!Overlay::displayCommit(ctx->dpyAttr[dpy].fd)) {
            ALOGE("%s: display commit fail for %d dpy!", __FUNCTION__, dpy);
            ret = -1;
        }
    }

    closeAcquireFds(list);
    return ret;
}

static int hwc_set_virtual(hwc_context_t *ctx,
                            hwc_display_contents_1_t* list)
{
    ATRACE_CALL();
    int ret = 0;
    const int dpy = HWC_DISPLAY_VIRTUAL;

    if (LIKELY(list) && ctx->dpyAttr[dpy].isActive &&
                           ctx->dpyAttr[dpy].connected) {
        if(!ctx->dpyAttr[dpy].isPause) {
            uint32_t last = list->numHwLayers - 1;
            hwc_layer_1_t *fbLayer = &list->hwLayers[last];
            int fd = -1; //FenceFD from the Copybit(valid in async mode)
            bool copybitDone = false;
            if(ctx->mCopyBit[dpy])
                copybitDone = ctx->mCopyBit[dpy]->draw(ctx, list, dpy, &fd);

            if(list->numHwLayers > 1)
                hwc_sync(ctx, list, dpy, fd);


            if (!ctx->mMDPComp[dpy]->draw(ctx, list)) {
                ALOGE("%s: MDPComp draw failed", __FUNCTION__);
                ret = -1;
            }

            int extOnlyLayerIndex =
                    ctx->listStats[dpy].extOnlyLayerIndex;

            private_handle_t *hnd = (private_handle_t *)fbLayer->handle;
            if(extOnlyLayerIndex!= -1) {
                hwc_layer_1_t *extLayer = &list->hwLayers[extOnlyLayerIndex];
                hnd = (private_handle_t *)extLayer->handle;
            } else if(copybitDone) {
                hnd = ctx->mCopyBit[dpy]->getCurrentRenderBuffer();
            }

            if(hnd && !isYuvBuffer(hnd)) {
                if (!ctx->mFBUpdate[dpy]->draw(ctx, hnd)) {
                    ALOGE("%s: FBUpdate::draw fail!", __FUNCTION__);
                    ret = -1;
                }
            }
        }

        if(!Overlay::displayCommit(ctx->dpyAttr[dpy].fd)) {
            ALOGE("%s: display commit fail for %d dpy!", __FUNCTION__, dpy);
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
            ret = -1;
        }
    }

    closeAcquireFds(list);
<<<<<<< HEAD
=======

    if (list && !ctx->mVirtualonExtActive && (list->retireFenceFd < 0) ) {
        // SF assumes HWC waits for the acquire fence and returns a new fence
        // that signals when we're done. Since we don't wait, and also don't
        // touch the buffer, we can just handle the acquire fence back to SF
        // as the retire fence.
        list->retireFenceFd = list->outbufAcquireFenceFd;
    }

>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    return ret;
}


static int hwc_set(hwc_composer_device_1 *dev,
                   size_t numDisplays,
                   hwc_display_contents_1_t** displays)
{
    int ret = 0;
    hwc_context_t* ctx = (hwc_context_t*)(dev);
<<<<<<< HEAD
    Locker::Autolock _l(ctx->mBlankLock);
=======
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    for (uint32_t i = 0; i <= numDisplays; i++) {
        hwc_display_contents_1_t* list = displays[i];
        int dpy = getHWCDpy(ctx, i);
        switch(dpy) {
            case HWC_DISPLAY_PRIMARY:
                ret = hwc_set_primary(ctx, list);
                break;
            case HWC_DISPLAY_EXTERNAL:
            case HWC_DISPLAY_VIRTUAL:
            /* ToDo: We are using hwc_set_external path for both External and
                     Virtual displays on HWC1.1. Eventually, we will have
                     separate functions when we move to HWC1.2
            */
                ret = hwc_set_external(ctx, list, i);
                break;
            case HWC_DISPLAY_VIRTUAL:
                ret = hwc_set_virtual(ctx, list);
                break;
            default:
                ret = -EINVAL;
        }
    }
    // This is only indicative of how many times SurfaceFlinger posts
    // frames to the display.
    CALC_FPS();
    MDPComp::resetIdleFallBack();
    //Was locked at the beginning of prepare
    ctx->mDrawLock.unlock();
    return ret;
}

int hwc_getDisplayConfigs(struct hwc_composer_device_1* dev, int disp,
        uint32_t* configs, size_t* numConfigs) {
    int ret = 0;
    hwc_context_t* ctx = (hwc_context_t*)(dev);
    disp = getHWCDpy(ctx, disp);
    //in 1.1 there is no way to choose a config, report as config id # 0
    //This config is passed to getDisplayAttributes. Ignore for now.
    switch(disp) {
        case HWC_DISPLAY_PRIMARY:
            if(*numConfigs > 0) {
                configs[0] = 0;
                *numConfigs = 1;
            }
            ret = 0; //NO_ERROR
            break;
        case HWC_DISPLAY_EXTERNAL:
        case HWC_DISPLAY_VIRTUAL:
            ret = -1; //Not connected
            if(ctx->dpyAttr[disp].connected) {
                ret = 0; //NO_ERROR
                if(*numConfigs > 0) {
                    configs[0] = 0;
                    *numConfigs = 1;
                }
            }
            break;
    }
    return ret;
}

int hwc_getDisplayAttributes(struct hwc_composer_device_1* dev, int disp,
        uint32_t config, const uint32_t* attributes, int32_t* values) {

    hwc_context_t* ctx = (hwc_context_t*)(dev);
    disp = getHWCDpy(ctx, disp);
    //If hotpluggable displays(i.e, HDMI, WFD) are inactive return error
    if( (disp != HWC_DISPLAY_PRIMARY) && !ctx->dpyAttr[disp].connected) {
        return -1;
    }

    //From HWComposer
    static const uint32_t DISPLAY_ATTRIBUTES[] = {
        HWC_DISPLAY_VSYNC_PERIOD,
        HWC_DISPLAY_WIDTH,
        HWC_DISPLAY_HEIGHT,
        HWC_DISPLAY_DPI_X,
        HWC_DISPLAY_DPI_Y,
        HWC_DISPLAY_NO_ATTRIBUTE,
    };

    const int NUM_DISPLAY_ATTRIBUTES = (sizeof(DISPLAY_ATTRIBUTES) /
            sizeof(DISPLAY_ATTRIBUTES)[0]);

    for (size_t i = 0; i < NUM_DISPLAY_ATTRIBUTES - 1; i++) {
        switch (attributes[i]) {
        case HWC_DISPLAY_VSYNC_PERIOD:
            values[i] = ctx->dpyAttr[disp].vsync_period;
            break;
        case HWC_DISPLAY_WIDTH:
            values[i] = ctx->dpyAttr[disp].xres;
            ALOGD("%s disp = %d, width = %d",__FUNCTION__, disp,
                    ctx->dpyAttr[disp].xres);
            break;
        case HWC_DISPLAY_HEIGHT:
            values[i] = ctx->dpyAttr[disp].yres;
            ALOGD("%s disp = %d, height = %d",__FUNCTION__, disp,
                    ctx->dpyAttr[disp].yres);
            break;
        case HWC_DISPLAY_DPI_X:
            values[i] = (int32_t) (ctx->dpyAttr[disp].xdpi*1000.0);
            break;
        case HWC_DISPLAY_DPI_Y:
            values[i] = (int32_t) (ctx->dpyAttr[disp].ydpi*1000.0);
            break;
        default:
            ALOGE("Unknown display attribute %d",
                    attributes[i]);
            return -EINVAL;
        }
    }
    return 0;
}

void hwc_dump(struct hwc_composer_device_1* dev, char *buff, int buff_len)
{
    hwc_context_t* ctx = (hwc_context_t*)(dev);
<<<<<<< HEAD
=======
    Locker::Autolock _l(ctx->mDrawLock);
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    android::String8 aBuf("");
    dumpsys_log(aBuf, "Qualcomm HWC state:\n");
    dumpsys_log(aBuf, "  MDPVersion=%d\n", ctx->mMDP.version);
    dumpsys_log(aBuf, "  DisplayPanel=%c\n", ctx->mMDP.panel);
<<<<<<< HEAD
    ctx->mMDPComp->dump(aBuf);
    char ovDump[2048] = {'\0'};
    ctx->mOverlay->getDump(ovDump, 2048);
    dumpsys_log(aBuf, ovDump);
=======
    for(int dpy = 0; dpy < HWC_NUM_DISPLAY_TYPES; dpy++) {
        if(ctx->mMDPComp[dpy])
            ctx->mMDPComp[dpy]->dump(aBuf);
    }
    char ovDump[2048] = {'\0'};
    ctx->mOverlay->getDump(ovDump, 2048);
    dumpsys_log(aBuf, ovDump);
    ovDump[0] = '\0';
    ctx->mRotMgr->getDump(ovDump, 2048);
    dumpsys_log(aBuf, ovDump);
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    strlcpy(buff, aBuf.string(), buff_len);
}

static int hwc_device_close(struct hw_device_t *dev)
{
    if(!dev) {
        ALOGE("%s: NULL device pointer", __FUNCTION__);
        return -1;
    }
    closeContext((hwc_context_t*)dev);
    free(dev);

    return 0;
}

static int hwc_device_open(const struct hw_module_t* module, const char* name,
                           struct hw_device_t** device)
{
    int status = -EINVAL;

    if (!strcmp(name, HWC_HARDWARE_COMPOSER)) {
        struct hwc_context_t *dev;
        dev = (hwc_context_t*)malloc(sizeof(*dev));
        memset(dev, 0, sizeof(*dev));

        //Initialize hwc context
        initContext(dev);

        //Setup HWC methods
        dev->device.common.tag          = HARDWARE_DEVICE_TAG;
        dev->device.common.version      = HWC_DEVICE_API_VERSION_1_2;
        dev->device.common.module       = const_cast<hw_module_t*>(module);
        dev->device.common.close        = hwc_device_close;
        dev->device.prepare             = hwc_prepare;
        dev->device.set                 = hwc_set;
        dev->device.eventControl        = hwc_eventControl;
        dev->device.blank               = hwc_blank;
        dev->device.query               = hwc_query;
        dev->device.registerProcs       = hwc_registerProcs;
        dev->device.dump                = hwc_dump;
        dev->device.getDisplayConfigs   = hwc_getDisplayConfigs;
        dev->device.getDisplayAttributes = hwc_getDisplayAttributes;
        *device = &dev->device.common;
        status = 0;
    }
    return status;
}

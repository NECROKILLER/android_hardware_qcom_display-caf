/*
* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*    * Redistributions of source code must retain the above copyright
*      notice, this list of conditions and the following disclaimer.
*    * Redistributions in binary form must reproduce the above
*      copyright notice, this list of conditions and the following
*      disclaimer in the documentation and/or other materials provided
*      with the distribution.
*    * Neither the name of The Linux Foundation nor the names of its
*      contributors may be used to endorse or promote products derived
*      from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
* ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "overlayGenPipe.h"
#include "overlay.h"
#include "mdp_version.h"

namespace overlay {

<<<<<<< HEAD
GenericPipe::GenericPipe(int dpy) : mFbNum(dpy), mRot(0), mRotUsed(false),
        mRotDownscaleOpt(false), pipeState(CLOSED) {
=======
GenericPipe::GenericPipe(int dpy) : mDpy(dpy), mRotDownscaleOpt(false),
    pipeState(CLOSED) {
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    init();
}

GenericPipe::~GenericPipe() {
    close();
}

bool GenericPipe::init()
{
    ALOGE_IF(DEBUG_OVERLAY, "GenericPipe init");
<<<<<<< HEAD
    mRotUsed = false;
    mRotDownscaleOpt = false;
    if(mFbNum)
        mFbNum = Overlay::getInstance()->getExtFbNum();

    ALOGD_IF(DEBUG_OVERLAY,"%s: mFbNum:%d",__FUNCTION__, mFbNum);

    if(!mCtrlData.ctrl.init(mFbNum)) {
=======
    mRotDownscaleOpt = false;

    int fbNum = Overlay::getFbForDpy(mDpy);
    if( fbNum < 0 ) {
        ALOGE("%s: Invalid FB for the display: %d",__FUNCTION__, mDpy);
        return false;
    }

    ALOGD_IF(DEBUG_OVERLAY,"%s: mFbNum:%d",__FUNCTION__, fbNum);


    if(!mCtrlData.ctrl.init(fbNum)) {
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
        ALOGE("GenericPipe failed to init ctrl");
        return false;
    }

<<<<<<< HEAD
    if(!mCtrlData.data.init(mFbNum)) {
=======
    if(!mCtrlData.data.init(fbNum)) {
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
        ALOGE("GenericPipe failed to init data");
        return false;
    }

<<<<<<< HEAD
    //get a new rotator object, take ownership
    mRot = Rotator::getRotator();

=======
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    return true;
}

bool GenericPipe::close() {
    bool ret = true;

    if(!mCtrlData.ctrl.close()) {
        ALOGE("GenericPipe failed to close ctrl");
        ret = false;
    }
    if (!mCtrlData.data.close()) {
        ALOGE("GenericPipe failed to close data");
        ret = false;
    }

<<<<<<< HEAD
    delete mRot;
    mRot = 0;

=======
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    setClosed();
    return ret;
}

<<<<<<< HEAD
bool GenericPipe::setSource(
        const utils::PipeArgs& args)
{
    utils::PipeArgs newargs(args);
    //Interlace video handling.
    if(newargs.whf.format & INTERLACE_MASK) {
        setMdpFlags(newargs.mdpFlags, utils::OV_MDP_DEINTERLACE);
    }
    utils::Whf whf(newargs.whf);
    //Extract HAL format from lower bytes. Deinterlace if interlaced.
    whf.format = utils::getColorFormat(whf.format);
    //Get MDP equivalent of HAL format.
    whf.format = utils::getMdpFormat(whf.format);
    newargs.whf = whf;

    //Cache if user wants 0-rotation
    mRotUsed = newargs.rotFlags & utils::ROT_0_ENABLED;
    mRotDownscaleOpt = newargs.rotFlags & utils::ROT_DOWNSCALE_ENABLED;

    mRot->setSource(newargs.whf);
    mRot->setFlags(newargs.mdpFlags);
    return mCtrlData.ctrl.setSource(newargs);
}

bool GenericPipe::setCrop(
        const overlay::utils::Dim& d) {
    return mCtrlData.ctrl.setCrop(d);
}

bool GenericPipe::setTransform(
        const utils::eTransform& orient)
{
    //Rotation could be enabled by user for zero-rot or the layer could have
    //some transform. Mark rotation enabled in either case.
    mRotUsed |= (orient != utils::OVERLAY_TRANSFORM_0);
    mRot->setTransform(orient);

    return mCtrlData.ctrl.setTransform(orient);
}

bool GenericPipe::setPosition(const utils::Dim& d)
{
    return mCtrlData.ctrl.setPosition(d);
}

void GenericPipe::setRotatorUsed(const bool& rotUsed) {
    mRot->setRotatorUsed(rotUsed);
    mCtrlData.ctrl.setRotatorUsed(rotUsed);
=======
void GenericPipe::setSource(const utils::PipeArgs& args) {
    mRotDownscaleOpt = args.rotFlags & utils::ROT_DOWNSCALE_ENABLED;
    mCtrlData.ctrl.setSource(args);
}

void GenericPipe::setCrop(const overlay::utils::Dim& d) {
    mCtrlData.ctrl.setCrop(d);
}

void GenericPipe::setTransform(const utils::eTransform& orient) {
    mCtrlData.ctrl.setTransform(orient);
}

void GenericPipe::setPosition(const utils::Dim& d) {
    mCtrlData.ctrl.setPosition(d);
}

bool GenericPipe::setVisualParams(const MetaData_t &metadata)
{
        return mCtrlData.ctrl.setVisualParams(metadata);
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
}

bool GenericPipe::commit() {
    bool ret = false;
    int downscale_factor = utils::ROT_DS_NONE;

    if(mRotDownscaleOpt) {
<<<<<<< HEAD
        /* Can go ahead with calculation of downscale_factor since
         * we consider area when calculating it */
        downscale_factor = mCtrlData.ctrl.getDownscalefactor();
        if(downscale_factor)
            mRotUsed = true;
    }

    setRotatorUsed(mRotUsed);
    mCtrlData.ctrl.doTransform();

    mCtrlData.ctrl.doDownscale(downscale_factor);
    mRot->setDownscale(downscale_factor);

    if(mRotUsed) {
        //If wanting to use rotator, start it.
        if(!mRot->commit()) {
            ALOGE("GenPipe Rotator commit failed");
            //If rot commit fails, flush rotator session, memory, fd and create
            //a hollow rotator object
            delete mRot;
            mRot = Rotator::getRotator();
            pipeState = CLOSED;
            return false;
        }
        /* Set the mdp src format to the output format of the rotator.
         * The output format of the rotator might be different depending on
         * whether fastyuv mode is enabled in the rotator.
         */
        mCtrlData.ctrl.updateSrcformat(mRot->getDstFormat());
    }

    ret = mCtrlData.ctrl.commit();

    //If mdp commit fails, flush rotator session, memory, fd and create a hollow
    //rotator object
    if(ret == false) {
        delete mRot;
        mRot = Rotator::getRotator();
    }

=======
        ovutils::Dim src(mCtrlData.ctrl.getCrop());
        ovutils::Dim dst(mCtrlData.ctrl.getPosition());
        downscale_factor = ovutils::getDownscaleFactor(
                src.w, src.h, dst.w, dst.h);
    }

    mCtrlData.ctrl.setDownscale(downscale_factor);
    ret = mCtrlData.ctrl.commit();

>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    pipeState = ret ? OPEN : CLOSED;
    return ret;
}

bool GenericPipe::queueBuffer(int fd, uint32_t offset) {
    //TODO Move pipe-id transfer to CtrlData class. Make ctrl and data private.
    OVASSERT(isOpen(), "State is closed, cannot queueBuffer");
    int pipeId = mCtrlData.ctrl.getPipeId();
    OVASSERT(-1 != pipeId, "Ctrl ID should not be -1");
    // set pipe id from ctrl to data
    mCtrlData.data.setPipeId(pipeId);

<<<<<<< HEAD
    int finalFd = fd;
    uint32_t finalOffset = offset;
    //If rotator is to be used, queue to it, so it can ROTATE.
    if(mRotUsed) {
        if(!mRot->queueBuffer(fd, offset)) {
            ALOGE("GenPipe Rotator play failed");
            return false;
        }
        //Configure MDP's source buffer as the current output buffer of rotator
        if(mRot->getDstMemId() != -1) {
            finalFd = mRot->getDstMemId();
            finalOffset = mRot->getDstOffset();
        } else {
            //Could be -1 for NullRotator, if queue above succeeds.
            //Need an actual rotator. Modify overlay State Traits.
            //Not fatal, keep queuing to MDP without rotation.
            ALOGE("Null rotator in use, where an actual is required");
        }
    }
    return mCtrlData.data.queueBuffer(finalFd, finalOffset);
=======
    return mCtrlData.data.queueBuffer(fd, offset);
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
}

int GenericPipe::getCtrlFd() const {
    return mCtrlData.ctrl.getFd();
}

<<<<<<< HEAD
utils::ScreenInfo GenericPipe::getScreenInfo() const
{
    return mCtrlData.ctrl.getScreenInfo();
}

=======
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
utils::Dim GenericPipe::getCrop() const
{
    return mCtrlData.ctrl.getCrop();
}

void GenericPipe::dump() const
{
    ALOGE("== Dump Generic pipe start ==");
    ALOGE("pipe state = %d", (int)pipeState);
<<<<<<< HEAD
    OVASSERT(mRot, "GenericPipe should have a valid Rot");
    mCtrlData.ctrl.dump();
    mCtrlData.data.dump();
    mRot->dump();
=======
    mCtrlData.ctrl.dump();
    mCtrlData.data.dump();

>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
    ALOGE("== Dump Generic pipe end ==");
}

void GenericPipe::getDump(char *buf, size_t len) {
    mCtrlData.ctrl.getDump(buf, len);
    mCtrlData.data.getDump(buf, len);
<<<<<<< HEAD
    if(mRotUsed && mRot)
        mRot->getDump(buf, len);
=======
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c
}

bool GenericPipe::isClosed() const  {
    return (pipeState == CLOSED);
}

bool GenericPipe::isOpen() const  {
    return (pipeState == OPEN);
}

bool GenericPipe::setClosed() {
    pipeState = CLOSED;
    return true;
}

<<<<<<< HEAD
=======
void GenericPipe::forceSet() {
    mCtrlData.ctrl.forceSet();
}
>>>>>>> 4d81b555d1fb44132f03cfd8208c0216e5a6755c

} //namespace overlay

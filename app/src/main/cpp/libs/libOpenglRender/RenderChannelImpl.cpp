// Copyright (C) 2016 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "RenderChannelImpl.h"

#define EMUGL_DEBUG_LEVEL 0

#include "RenderThread.h"
#include "android/base/synchronization/Lock.h"
#include "emugl/common/debug.h"

#include <algorithm>
#include <utility>
#include <assert.h>
#include <string.h>

namespace emugl {

    using AutoLock = android::base::AutoLock;

    static constexpr size_t kGuestToHostQueueCapacity = 1024U;
    static constexpr size_t kHostToGuestQueueCapacity = 16U;

    RenderChannelImpl::RenderChannelImpl(IOStream *loadStream)
            : mFromGuest(kGuestToHostQueueCapacity, mLock),
              mToGuest(kHostToGuestQueueCapacity, mLock) {
        updateStateLocked();
        mRenderThread.reset(new RenderThread(this, loadStream));
        mRenderThread->start();
    }

    void RenderChannelImpl::setEventCallback(EventCallback &&callback) {
        mEventCallback = std::move(callback);
        notifyStateChangeLocked();
    }

    void RenderChannelImpl::setWantedEvents(State state) {
        D("state=%d", (int) state);
        AutoLock lock(mLock);
        mWantedEvents |= state;
        notifyStateChangeLocked();
    }

    RenderChannel::State RenderChannelImpl::state() const {
        AutoLock lock(mLock);
        return mState;
    }

    void RenderChannelImpl::stop() {
        D("enter");
        AutoLock lock(mLock);
        mFromGuest.closeLocked();
        mToGuest.closeLocked();
        mEventCallback = [](State state) {};
    }

    void RenderChannelImpl::stopFromHost() {
        D("enter");

        AutoLock lock(mLock);
        mFromGuest.closeLocked();
        mToGuest.closeLocked();
        mState |= State::Stopped;
        notifyStateChangeLocked();
        mEventCallback = [](State state) {};
    }

    bool RenderChannelImpl::isStopped() const {
        AutoLock lock(mLock);
        return (mState & State::Stopped) != 0;
    }

    RenderThread *RenderChannelImpl::renderThread() const {
        return mRenderThread.get();
    }

    void RenderChannelImpl::pausePreSnapshot() {
        AutoLock lock(mLock);
        mFromGuest.setSnapshotModeLocked(true);
        mToGuest.setSnapshotModeLocked(true);
    }

    void RenderChannelImpl::resume() {
        AutoLock lock(mLock);
        mFromGuest.setSnapshotModeLocked(false);
        mToGuest.setSnapshotModeLocked(false);
    }

    RenderChannelImpl::~RenderChannelImpl() {
        // Make sure the render thread is stopped before the channel is gone.
        mRenderThread->wait();
    }

    void RenderChannelImpl::updateStateLocked() {
        State state = RenderChannel::State::Empty;

        if (mToGuest.canPopLocked()) {
            state |= State::CanRead;
        }
        if (mFromGuest.canPushLocked()) {
            state |= State::CanWrite;
        }
        if (mToGuest.isClosedLocked()) {
            state |= State::Stopped;
        }
        mState = state;
    }

    void RenderChannelImpl::notifyStateChangeLocked() {
        // Always report stop events, event if not explicitly asked for.
        State available = mState & (mWantedEvents | State::Stopped);
        if (available != 0) {
            D("callback with %d", (int) available);
            mWantedEvents &= ~mState;
            mEventCallback(available);
        }
    }

}  // namespace emugl

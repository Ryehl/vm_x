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
#pragma once

#include "android/base/containers/BufferQueue.h"
#include "RenderChannel.h"
#include "RendererImpl.h"

namespace emugl {

class RenderThread;

using android::base::BufferQueue;

// Implementation of the RenderChannel interface that connects a guest
// client thread (really an AndroidPipe implementation) to a host
// RenderThread instance.
class RenderChannelImpl final : public RenderChannel {
public:
    explicit RenderChannelImpl(IOStream* loadStream = nullptr);
    ~RenderChannelImpl();

    /////////////////////////////////////////////////////////////////
    // RenderChannel overriden methods. These are called from the guest
    // client thread.

    // Set the event |callback| to be notified when the host changes the
    // state of the channel, according to the event mask provided by
    // setWantedEvents(). Call this function right after creating the
    // instance.
    virtual void setEventCallback(EventCallback&& callback) override final;

    // Set the mask of events the guest wants to be notified of from the
    // host thread.
    virtual void setWantedEvents(State state) override final;

    // Return the current channel state relative to the guest.
    virtual State state() const override final;

    // Close the channel from the guest.
    virtual void stop() override final;

    // Close the channel from the host.
    void stopFromHost();

    // Check if either guest or host stopped the channel.
    bool isStopped() const;

    // Return the underlying render thread object.
    RenderThread* renderThread() const;

    // Pause normal operations and enter the snapshot mode. In snapshot mode
    // RenderChannel is supposed to allow everyone to write data into the
    // channel, but it should not return any data back. This way we can make
    // sure all data at the snapshot time is here and is saved, and we won't
    // miss some important rendering call.
    void pausePreSnapshot();

    // Resume the normal operation after saving or loading a snapshot.
    void resume();

private:
    void updateStateLocked();
    void notifyStateChangeLocked();

    EventCallback mEventCallback;
    std::unique_ptr<RenderThread> mRenderThread;

    // A single lock to protect the state and the two buffer queues at the
    // same time. NOTE: This needs to appear before the BufferQueue instances.
    mutable android::base::Lock mLock;
    State mState = State::Empty;
    State mWantedEvents = State::Empty;
    BufferQueue<RenderChannel::Buffer> mFromGuest;
    BufferQueue<RenderChannel::Buffer> mToGuest;
};

}  // namespace emugl

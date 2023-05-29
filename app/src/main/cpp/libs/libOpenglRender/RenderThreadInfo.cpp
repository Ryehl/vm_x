/*
* Copyright (C) 2011 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "RenderThreadInfo.h"

#include "android/base/containers/Lookup.h"
#include "android/base/files/StreamSerializing.h"
#include "android/base/memory/LazyInstance.h"
#include "android/base/synchronization/Lock.h"

#include "emugl/common/lazy_instance.h"
#include "emugl/common/thread_store.h"
#include "FrameBuffer.h"

#include <unordered_map>

namespace {

    class ThreadInfoStore : public ::emugl::ThreadStore {
    public:
        ThreadInfoStore() : ::emugl::ThreadStore(NULL) {}
    };

}  // namespace

static ::emugl::LazyInstance<ThreadInfoStore> s_tls = LAZY_INSTANCE_INIT;

RenderThreadInfo::RenderThreadInfo() {
    s_tls->set(this);
}

RenderThreadInfo::~RenderThreadInfo() {
    s_tls->set(NULL);
}

RenderThreadInfo* RenderThreadInfo::get() {
    return static_cast<RenderThreadInfo*>(s_tls->get());
}

// Each RenderThreadInfo may have 1 SyncThread associated with it.
// These SyncThreads must be handled carefully for snapshots:
// 1. The guest might ask for a SyncThread handle that doesn't
// exist on the host anymore.
// 2. The host might allocate a new SyncThread that has the same
// uint64_t handle as a stale SyncThread on the guest.
//
// This problem has some convenient properties:
//
// 1. Since RenderThread and SyncThread 1:1, and rcTriggerWait +
// rcCreateSyncKHR are in 1:1, and rcTriggerWait is the only API call
// that has a SyncThread handle as argument, we only have one
// inconsistent call to rcTriggerWait per sync thread,
// then the rest will update when rcClientWaitSyncKHR is called next
// (and reads the new sync thread handle).
// 2. Only ~1 such rcTriggerWait is pending total, I believe this
// is because there is usually not much eglSwapBuffers activity
// going on at any time.
//
// So we maintain a StalePtrRegistry<SyncThread>, sSyncThreadRegistry,
// with getPtr removing entries from stale database.
static android::base::LazyInstance<StalePtrRegistry<SyncThread> >
        sSyncThreadRegistry = LAZY_INSTANCE_INIT;

// createSyncThread() tracks new sync threads in the registry.
void RenderThreadInfo::createSyncThread() {
    syncThread.reset(new SyncThread(currContext->getEGLContext()));
    sSyncThreadRegistry->addPtr(syncThread.get());
}

// destroySyncThread() should untrack.
void RenderThreadInfo::destroySyncThread() {
    sSyncThreadRegistry->removePtr(syncThread.get());
    syncThread.reset(nullptr);
}

/* static */
StalePtrRegistry<SyncThread>* RenderThreadInfo::getSyncThreadRegistry() {
    return sSyncThreadRegistry.ptr();
}

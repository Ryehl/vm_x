// Copyright (C) 2014 The Android Open Source Project
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

#include "Compiler.h"
#include "android/base/synchronization/Lock.h"
#include "libs/libOpenglRender/FrameBuffer.h"

#include <pthread.h>
#include <assert.h>

namespace android {
    namespace base {

// A class that implements a condition variable, which can be used in
// association with a Lock to blocking-wait for specific conditions.
// Useful to implement various synchronization data structures.
        class ConditionVariable {
        public:
            // A set of functions to efficiently unlock the lock used with
            // the current condition variable and signal or broadcast it.
            //
            // The functions are needed because on some platforms (Posix) it's more
            // efficient to signal the variable before unlocking mutex, while on others
            // (Windows) it's exactly the opposite. Functions implement the best way
            // for each platform and abstract it out from the user.
            void signalAndUnlock(Lock *lock);

            void signalAndUnlock(AutoLock *lock);

            void broadcastAndUnlock(Lock *lock);

            void broadcastAndUnlock(AutoLock *lock);

            void wait(AutoLock *userLock) {
                assert(userLock->mLocked);
                wait(&userLock->mLock);
            }

            //
            // Convenience functions to get rid of the loop in condition variable usage
            // Instead of hand-writing a loop, e.g.
            //
            //      while (mRefCount < 3) {
            //          mCv.wait(&mLock);
            //      }
            //
            // use the following two wait() overloads:
            //
            //      mCv.wait(&mLock, [this]() { return mRefCount >= 3; });
            //
            // Parameters:
            // |lock| - a Lock or AutoLock pointer used with the condition variable.
            // |pred| - a functor predicate that's compatible with "bool pred()"
            //          signature and returns a condition when one should stop waiting.
            //

            template<class Predicate>
            void wait(Lock *lock, Predicate pred) {
                while (!pred()) {
                    this->wait(lock);
                }
            }

            template<class Predicate>
            void wait(AutoLock *lock, Predicate pred) {
                this->wait(&lock->mLock, pred);
            }

            // Note: on Posix systems, make it a naive wrapper around pthread_cond_t.

            ConditionVariable() {
                pthread_cond_init(&mCond, NULL);
            }

            ~ConditionVariable() {
                pthread_cond_destroy(&mCond);
            }

            void wait(Lock *userLock) {
                pthread_cond_wait(&mCond, &userLock->mLock);
            }

            bool timedWait(Lock *userLock, System::Duration waitUntilUs) {
                timespec abstime;
                abstime.tv_sec = waitUntilUs / 1000000LL;
                abstime.tv_nsec = (waitUntilUs % 1000000LL) * 1000;
                return pthread_cond_timedwait(&mCond, &userLock->mLock, &abstime) == 0;
            }

            void signal() {
                pthread_cond_signal(&mCond);
            }

            void broadcast() {
                pthread_cond_broadcast(&mCond);
            }

        private:
            pthread_cond_t mCond;

            DISALLOW_COPY_ASSIGN_AND_MOVE(ConditionVariable);
        };

        inline void ConditionVariable::signalAndUnlock(Lock *lock) {
            signal();
            lock->unlock();
        }

        inline void ConditionVariable::signalAndUnlock(AutoLock *lock) {
            signal();
            lock->unlock();
        }

        inline void ConditionVariable::broadcastAndUnlock(Lock *lock) {
            broadcast();
            lock->unlock();
        }

        inline void ConditionVariable::broadcastAndUnlock(AutoLock *lock) {
            broadcast();
            lock->unlock();
        }

    }  // namespace base
}  // namespace android

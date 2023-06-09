// Copyright 2014-2015 The Android Open Source Project
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

#include "RenderWindow.h"

#include "emugl/common/logging.h"
#include "emugl/common/message_channel.h"
#include "emugl/common/mutex.h"
#include "emugl/common/thread.h"
#include "FrameBuffer.h"

#include <stdarg.h>
#include <stdio.h>

#define DEBUG 0
#define LOG_NDEBUG 0
#define LOG_TAG "render_window"

#include "log/log_main.h"

#if DEBUG
#  define D(...) my_debug(__PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#else
#  define D(...) ((void)0)
#endif

namespace {

#if DEBUG
    void my_debug(const char* function, int line, const char* format, ...) {
        static ::emugl::Mutex mutex;
        va_list args;
        va_start(args, format);
        mutex.lock();
        fprintf(stderr, "%s:%d:", function, line);
        vfprintf(stderr, format, args);
        mutex.unlock();
        va_end(args);
    }
#endif

// List of possible commands to send to the render window thread from
// the main one.
    enum Command {
        CMD_INITIALIZE,
        CMD_SET_POST_CALLBACK,
        CMD_SETUP_SUBWINDOW,
        CMD_REMOVE_SUBWINDOW,
        CMD_SET_ROTATION,
        CMD_SET_TRANSLATION,
        CMD_REPAINT,
        CMD_HAS_GUEST_POSTED_A_FRAME,
        CMD_RESET_GUEST_POSTED_A_FRAME,
        CMD_FINALIZE,
    };

}  // namespace

// A single message sent from the main thread to the render window thread.
// |cmd| determines which fields are valid to read.
struct RenderWindowMessage {
    Command cmd;
    union {
        // CMD_INITIALIZE
        struct {
            int width;
            int height;
            bool useSubWindow;
            bool egl2egl;
        } init;

        // CMD_SET_POST_CALLBACK
        struct {
            emugl::Renderer::OnPostCallback on_post;
            void *on_post_context;
        } set_post_callback;

        // CMD_SETUP_SUBWINDOW
        struct {
            FBNativeWindowType parent;
            int wx;
            int wy;
            int ww;
            int wh;
            int fbw;
            int fbh;
            float dpr;
            float rotation;
            bool deleteExisting;
        } subwindow;

        // CMD_SET_TRANSLATION;
        struct {
            float px;
            float py;
        } trans;

        // CMD_SET_ROTATION
        float rotation;

        // result of operations.
        bool result;
    };

    // Process the current message, and updates its |result| field.
    // Returns true on success, or false on failure.
    bool process() const {
        const RenderWindowMessage &msg = *this;
        FrameBuffer *fb;
        bool result = false;
        switch (msg.cmd) {
            case CMD_INITIALIZE:
                ALOGE("RenderWindow: CMD_INITIALIZE w=%d h=%d",
                      msg.init.width, msg.init.height);
                result = FrameBuffer::initialize(msg.init.width,
                                                 msg.init.height,
                                                 msg.init.useSubWindow,
                                                 msg.init.egl2egl);
                break;

            case CMD_FINALIZE:
                GL_LOG("CMD_FINALIZE");
                D("CMD_FINALIZE\n");
                // this command may be issued even when frame buffer is not
                // yet created (e.g. if CMD_INITIALIZE failed),
                // so make sure we check if it is there before finalizing
                if (const auto fb = FrameBuffer::getFB()) {
                    fb->finalize();
                }
                result = true;
                break;

            case CMD_SET_POST_CALLBACK:
                GL_LOG("CMD_SET_POST_CALLBACK");
                D("CMD_SET_POST_CALLBACK\n");
                fb = FrameBuffer::getFB();
                fb->setPostCallback(msg.set_post_callback.on_post,
                                    msg.set_post_callback.on_post_context);
                result = true;
                break;

            case CMD_SETUP_SUBWINDOW:
                GL_LOG("CMD_SETUP_SUBWINDOW: parent=%p wx=%d wy=%d ww=%d wh=%d fbw=%d fbh=%d dpr=%f rotation=%f",
                       (void *) (intptr_t) msg.subwindow.parent,
                       msg.subwindow.wx,
                       msg.subwindow.wy,
                       msg.subwindow.ww,
                       msg.subwindow.wh,
                       msg.subwindow.fbw,
                       msg.subwindow.fbh,
                       msg.subwindow.dpr,
                       msg.subwindow.rotation);
                D("CMD_SETUP_SUBWINDOW: parent=%p wx=%d wy=%d ww=%d wh=%d fbw=%d fbh=%d dpr=%f rotation=%f\n",
                  (void *) (intptr_t) msg.subwindow.parent,
                  msg.subwindow.wx,
                  msg.subwindow.wy,
                  msg.subwindow.ww,
                  msg.subwindow.wh,
                  msg.subwindow.fbw,
                  msg.subwindow.fbh,
                  msg.subwindow.dpr,
                  msg.subwindow.rotation);
                result = FrameBuffer::getFB()->setupSubWindow(
                        msg.subwindow.parent,
                        msg.subwindow.wx,
                        msg.subwindow.wy,
                        msg.subwindow.ww,
                        msg.subwindow.wh,
                        msg.subwindow.fbw,
                        msg.subwindow.fbh,
                        msg.subwindow.dpr,
                        msg.subwindow.rotation,
                        msg.subwindow.deleteExisting);
                break;

            case CMD_REMOVE_SUBWINDOW:
                GL_LOG("CMD_REMOVE_SUBWINDOW");
                D("CMD_REMOVE_SUBWINDOW\n");
                result = FrameBuffer::getFB()->removeSubWindow();
                break;

            case CMD_SET_ROTATION:
                GL_LOG("CMD_SET_ROTATION rotation=%f", msg.rotation);
                D("CMD_SET_ROTATION rotation=%f\n", msg.rotation);
                fb = FrameBuffer::getFB();
                if (fb) {
                    fb->setDisplayRotation(msg.rotation);
                    result = true;
                }
                break;

            case CMD_SET_TRANSLATION:
                GL_LOG("CMD_SET_TRANSLATION translation=%f,%f", msg.trans.px, msg.trans.py);
                D("CMD_SET_TRANSLATION translation=%f,%f\n", msg.trans.px, msg.trans.py);
                fb = FrameBuffer::getFB();
                if (fb) {
                    fb->setDisplayTranslation(msg.trans.px, msg.trans.py);
                    result = true;
                }
                break;

            case CMD_REPAINT:
                GL_LOG("CMD_REPAINT");
                D("CMD_REPAINT\n");
                fb = FrameBuffer::getFB();
                if (fb) {
                    fb->repost();
                    result = true;
                } else {
                    GL_LOG("CMD_REPAINT: no repost, no FrameBuffer");
                }
                break;

            case CMD_HAS_GUEST_POSTED_A_FRAME:
                GL_LOG("CMD_HAS_GUEST_POSTED_A_FRAME");
                D("CMD_HAS_GUEST_POSTED_A_FRAME\n");
                fb = FrameBuffer::getFB();
                if (fb) {
                    result = fb->hasGuestPostedAFrame();
                } else {
                    GL_LOG("CMD_HAS_GUEST_POSTED_A_FRAME: no FrameBuffer");
                }
                break;

            case CMD_RESET_GUEST_POSTED_A_FRAME:
                GL_LOG("CMD_RESET_GUEST_POSTED_A_FRAME");
                D("CMD_RESET_GUEST_POSTED_A_FRAME\n");
                fb = FrameBuffer::getFB();
                if (fb) {
                    fb->resetGuestPostedAFrame();
                    result = true;
                } else {
                    GL_LOG("CMD_RESET_GUEST_POSTED_A_FRAME: no FrameBuffer");
                }
                break;


            default:;
        }
        return result;
    }
};

// Simple synchronization structure used to exchange data between the
// main and render window threads. Usage is the following:
//
// The main thread does the following in a loop:
//
//      canWriteCmd.wait()
//      updates |message| by writing a new |cmd| value and appropriate
//      parameters.
//      canReadCmd.signal()
//      canReadResult.wait()
//      reads |message.result|
//      canWriteResult.signal()
//
// The render window thread will do the following:
//
//      canReadCmd.wait()
//      reads |message.cmd| and acts upon it.
//      canWriteResult.wait()
//      writes |message.result|
//      canReadResult.signal()
//      canWriteCmd.signal()
//
class RenderWindowChannel {
public:
    RenderWindowChannel() : mIn(), mOut() {}

    ~RenderWindowChannel() {}

    // Send a message from the main thread.
    // Note that the content of |msg| is copied into the channel.
    // Returns with the command's result (true or false).
    bool sendMessageAndGetResult(const RenderWindowMessage &msg) {
        D("msg.cmd=%d\n", msg.cmd);
        mIn.send(msg);
        D("waiting for result\n");
        bool result = false;
        mOut.receive(&result);
        D("result=%s\n", result ? "success" : "failure");
        return result;
    }

    // Receive a message from the render window thread.
    // On exit, |*msg| gets a copy of the message. The caller
    // must always call sendResult() after processing the message.
    void receiveMessage(RenderWindowMessage *msg) {
        D("entering\n");
        mIn.receive(msg);
        D("message cmd=%d\n", msg->cmd);
    }

    // Send result from the render window thread to the main one.
    // Must always be called after receiveMessage().
    void sendResult(bool result) {
        D("waiting to send result (%s)\n", result ? "success" : "failure");
        mOut.send(result);
        D("result sent\n");
    }

private:
    emugl::MessageChannel<RenderWindowMessage, 16U> mIn;
    emugl::MessageChannel<bool, 16U> mOut;
};

namespace {

// This class implements the window render thread.
// Its purpose is to listen for commands from the main thread in a loop,
// process them, then return a boolean result for each one of them.
//
// The thread ends with a CMD_FINALIZE.
//
    class RenderWindowThread : public emugl::Thread {
    public:
        RenderWindowThread(RenderWindowChannel *channel) : mChannel(channel) {}

        virtual intptr_t main() {
            D("Entering render window thread thread\n");
#ifndef _WIN32
            sigset_t set;
            sigfillset(&set);
            pthread_sigmask(SIG_SETMASK, &set, NULL);
#endif
            bool running = true;
            while (running) {
                RenderWindowMessage msg = {};

                D("Waiting for message from main thread\n");
                mChannel->receiveMessage(&msg);

                bool result = msg.process();
                if (msg.cmd == CMD_FINALIZE) {
                    running = false;
                }

                D("Sending result (%s) to main thread\n", result ? "success" : "failure");
                mChannel->sendResult(result);
            }
            D("Exiting thread\n");
            return 0;
        }

    private:
        RenderWindowChannel *mChannel;
    };

}  // namespace

RenderWindow::RenderWindow(int width,
                           int height,
                           bool use_thread,
                           bool use_sub_window,
                           bool egl2egl)
        : mRepostThread([this] {
    while (auto cmd = mRepostCommands.receive()) {
        if (*cmd == RepostCommand::Sync) {
            continue;
        } else if (*cmd == RepostCommand::Repost) {
            GL_LOG("Reposting thread dequeueing a CMD_REPAINT");
            RenderWindowMessage msg = {CMD_REPAINT};
            (void) msg.process();
        }
    }
}) {
//    if (use_thread) {
//        mChannel = new RenderWindowChannel();
//        mThread = new RenderWindowThread(mChannel);
//        mThread->start();
//    } else {
    mRepostThread.start();
//    }

    RenderWindowMessage msg = {};
    msg.cmd = CMD_INITIALIZE;
    msg.init.width = width;
    msg.init.height = height;
    msg.init.useSubWindow = use_sub_window;
    msg.init.egl2egl = egl2egl;
    mValid = processMessage(msg);
}

RenderWindow::~RenderWindow() {
    D("Entering\n");
    removeSubWindow();
    mRepostCommands.stop();
    D("Sending CMD_FINALIZE\n");
    RenderWindowMessage msg = {};
    msg.cmd = CMD_FINALIZE;
    (void) processMessage(msg);

//    if (useThread()) {
//        mThread->wait(NULL);
//        delete mThread;
//        delete mChannel;
//    } else {
    mRepostThread.wait();
//    }
}

bool RenderWindow::getHardwareStrings(const char **vendor,
                                      const char **renderer,
                                      const char **version) {
    D("Entering\n");
    // TODO(digit): Move this to render window thread.
    FrameBuffer *fb = FrameBuffer::getFB();
    if (!fb) {
        D("No framebuffer!\n");
        return false;
    }
    fb->getGLStrings(vendor, renderer, version);
    D("Exiting vendor=[%s] renderer=[%s] version=[%s]\n",
      *vendor, *renderer, *version);

    return true;
}

void RenderWindow::setPostCallback(emugl::Renderer::OnPostCallback onPost, void *onPostContext) {
    D("Entering\n");
    RenderWindowMessage msg = {};
    msg.cmd = CMD_SET_POST_CALLBACK;
    msg.set_post_callback.on_post = onPost;
    msg.set_post_callback.on_post_context = onPostContext;
    (void) processMessage(msg);
    D("Exiting\n");
}

bool RenderWindow::asyncReadbackSupported() {
    D("Entering\n");
    return FrameBuffer::getFB()->asyncReadbackSupported();
}

emugl::Renderer::ReadPixelsCallback RenderWindow::getReadPixelsCallback() {
    D("Entering\n");
    return FrameBuffer::getFB()->getReadPixelsCallback();
}

bool RenderWindow::setupSubWindow(FBNativeWindowType window,
                                  int wx,
                                  int wy,
                                  int ww,
                                  int wh,
                                  int fbw,
                                  int fbh,
                                  float dpr,
                                  float zRot,
                                  bool deleteExisting) {
    D("Entering mHasSubWindow=%s\n", mHasSubWindow ? "true" : "false");

    RenderWindowMessage msg = {};
    msg.cmd = CMD_SETUP_SUBWINDOW;
    msg.subwindow.parent = window;
    msg.subwindow.wx = wx;
    msg.subwindow.wy = wy;
    msg.subwindow.ww = ww;
    msg.subwindow.wh = wh;
    msg.subwindow.fbw = fbw;
    msg.subwindow.fbh = fbh;
    msg.subwindow.dpr = dpr;
    msg.subwindow.rotation = zRot;
    msg.subwindow.deleteExisting = deleteExisting;

    mHasSubWindow = processMessage(msg);

    D("Exiting mHasSubWindow=%s\n", mHasSubWindow ? "true" : "false");
    return mHasSubWindow;
}

bool RenderWindow::removeSubWindow() {
    D("Entering mHasSubWindow=%s\n", mHasSubWindow ? "true" : "false");
    if (!mHasSubWindow) {
        return false;
    }
    mHasSubWindow = false;
    mRepostCommands.send(RepostCommand::Sync);
    mRepostCommands.waitForEmpty();

    RenderWindowMessage msg = {};
    msg.cmd = CMD_REMOVE_SUBWINDOW;
    bool result = processMessage(msg);
    D("Exiting result=%s\n", result ? "success" : "failure");
    return result;
}

void RenderWindow::setRotation(float zRot) {
    D("Entering rotation=%f\n", zRot);
    RenderWindowMessage msg = {};
    msg.cmd = CMD_SET_ROTATION;
    msg.rotation = zRot;
    (void) processMessage(msg);
    D("Exiting\n");
}


void RenderWindow::setScreenMask(int width, int height, const unsigned char *rgbaData) {
    FrameBuffer *fb = FrameBuffer::getFB();
    if (fb) {
        fb->getTextureDraw()->setScreenMask(width, height, rgbaData);
    }
}

void RenderWindow::repaint() {
    D("Entering\n");
    RenderWindowMessage msg = {};
    msg.cmd = CMD_REPAINT;
    (void) processMessage(msg);
    D("Exiting\n");
}

bool RenderWindow::hasGuestPostedAFrame() {
    D("Entering\n");
    RenderWindowMessage msg = {};
    msg.cmd = CMD_HAS_GUEST_POSTED_A_FRAME;
    bool res = processMessage(msg);
    D("Exiting\n");
    return res;
}

void RenderWindow::resetGuestPostedAFrame() {
    D("Entering\n");
    RenderWindowMessage msg = {};
    msg.cmd = CMD_RESET_GUEST_POSTED_A_FRAME;
    (void) processMessage(msg);
    D("Exiting\n");
}

bool RenderWindow::processMessage(const RenderWindowMessage &msg) {
    if (useThread()) {
        if (msg.cmd == CMD_REPAINT) {
            GL_LOG("Sending CMD_REPAINT to render window channel");
        }
        return mChannel->sendMessageAndGetResult(msg);
    } else if (msg.cmd == CMD_REPAINT) {
        GL_LOG("Sending CMD_REPAINT to reposting thread");
        mRepostCommands.send(RepostCommand::Repost);
        return true;
    } else {
        return msg.process();
    }
}

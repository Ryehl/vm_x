#pragma once

#include "android/base/synchronization/Lock.h"

#include "emu_egl/egl.h"

#include <functional>
#include <vector>

class ColorBuffer;

class FrameBuffer;

struct RenderThreadInfo;

class PostWorker {
public:
    using BindSubwinCallback = std::function<bool(void)>;

    PostWorker(BindSubwinCallback &&cb);

    ~PostWorker();

    // post: posts the next color buffer.
    // Assumes framebuffer lock is held.
    void post(ColorBuffer *cb);

    // viewport: (re)initializes viewport dimensions.
    // Assumes framebuffer lock is held.
    // This is called whenever the subwindow needs a refresh (FrameBuffer::setupSubWindow).
    void viewport(int width, int height);

    // clear: blanks out emulator display when refreshing the subwindow
    // if there is no last posted color buffer to show yet.
    void clear();

private:
    FrameBuffer *mFb;

    std::function<bool(void)> mBindSubwin;

    bool m_initialized = false;
    int m_viewportWidth = 0;
    int m_viewportHeight = 0;
    DISALLOW_COPY_AND_ASSIGN(PostWorker);
};

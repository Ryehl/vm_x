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
#ifndef __IO_STREAM_H__
#define __IO_STREAM_H__

#define LOG_NDEBUG 0

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "ErrorLog.h"

class IOStream {
public:
    IOStream(size_t bufSize) {
        m_buf = NULL;
        m_bufsize = bufSize;
        m_free = 0;
    }

    virtual void *allocBuffer(size_t minSize) = 0;

    virtual int commitBuffer(size_t size) = 0;

    virtual int recv(void *buf, size_t len) = 0;

    virtual const unsigned char *readFully(void *buf, size_t len) = 0;

    virtual const unsigned char *read(void *buf, size_t *inout_len) = 0;

    virtual int writeFully(const void *buf, size_t len) = 0;

    //android::base::Stream fun
    virtual void putByte(uint8_t value) = 0;

    virtual void putBe16(uint16_t value) = 0;

    virtual void putBe32(uint32_t value) = 0;

    virtual void putBe64(uint64_t value) = 0;

    virtual uint8_t getByte() = 0;

    virtual uint32_t getBe32() = 0;

    virtual uint64_t getBe64() = 0;

    virtual void putFloat(float value) = 0;

    virtual float getFloat() = 0;
//    void putString(StringView str);
//    void putString(const char* str);
//    void putString(const char* str, size_t strlen);
//    std::string getString();
//    void putPackedNum(uint64_t num);
//    uint64_t getPackedNum();
//    void putPackedSignedNum(int64_t num);
//    int64_t getPackedSignedNum();
    //android::base::Stream fun

    virtual ~IOStream() {
        // NOTE: m_buf is 'owned' by the child class thus we expect it to be released by it
    }

    unsigned char *alloc(size_t len) {

        if (m_buf && len > m_free) {
            if (flush() < 0) {
                ERR("Failed to flush in alloc\n");
                return NULL; // we failed to flush so something is wrong
            }
        }

        if (!m_buf || len > m_bufsize) {
            int allocLen = m_bufsize < len ? len : m_bufsize;
            m_buf = (unsigned char *) allocBuffer(allocLen);
            if (!m_buf) {
                ERR("Alloc (%u bytes) failed\n", allocLen);
                return NULL;
            }
            m_bufsize = m_free = allocLen;
        }

        unsigned char *ptr;

        ptr = m_buf + (m_bufsize - m_free);
        m_free -= len;

        return ptr;
    }

    int flush() {
        if (!m_buf || m_free == m_bufsize) return 0;

        int stat = commitBuffer(m_bufsize - m_free);
        m_buf = NULL;
        m_free = 0;
        return stat;
    }

    const unsigned char *readback(void *buf, size_t len) {
        flush();
        return readFully(buf, len);
    }

//    void readbackPixels(void* context, int width, int height, unsigned int format, unsigned int type, void* pixels);


private:
    unsigned char *m_buf;
    size_t m_bufsize;
    size_t m_free;
};

//
// When a client opens a connection to the renderer, it should
// send unsigned int value indicating the "clientFlags".
// The following are the bitmask of the clientFlags.
// currently only one bit is used which flags the server
// it should exit.
//
//#define IOSTREAM_CLIENT_EXIT_SERVER      1

#endif

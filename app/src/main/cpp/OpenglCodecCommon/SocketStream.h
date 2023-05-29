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
#ifndef __SOCKET_STREAM_H
#define __SOCKET_STREAM_H

#include "IOStream.h"
#include <stdlib.h>

class SocketStream : public IOStream {
public:
    typedef enum {
        ERR_INVALID_SOCKET = -1000
    } SocketStreamError;

    explicit SocketStream(size_t bufsize = 10000);

    virtual ~SocketStream();

    virtual int listen(const char* name) = 0;

    virtual SocketStream *accept() = 0;

    virtual int connect(unsigned short port) = 0;

    virtual void *allocBuffer(size_t minSize);

    virtual int commitBuffer(size_t size);

    virtual const unsigned char *readFully(void *buf, size_t len);

    virtual const unsigned char *read(void *buf, size_t *inout_len);

    bool valid() { return m_sock >= 0; }

    virtual int recv(void *buf, size_t len);

    virtual int writeFully(const void *buf, size_t len);

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

protected:
    int m_sock;
    size_t m_bufsize;
    unsigned char *m_buf;

    SocketStream(int sock, size_t bufSize);
};

#endif /* __SOCKET_STREAM_H */

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
#ifndef __TCP_STREAM_H
#define __TCP_STREAM_H

#include "SocketStream.h"

class TcpStream : public SocketStream {
public:
    explicit TcpStream(size_t bufsize = 10000);
    virtual int listen(const char* name);
    virtual SocketStream *accept();
    virtual int connect(unsigned short port);
    int connect(const char* hostname, unsigned short port);

    //android::base::Stream fun
    virtual void putByte(uint8_t value);

    virtual void putBe16(uint16_t value);

    virtual void putBe32(uint32_t value);

    virtual void putBe64(uint64_t value);

    virtual uint8_t getByte();

    virtual float getFloat();

    virtual uint32_t getBe32();

    virtual uint64_t getBe64();

    virtual void putFloat(float value);

//    void putString(StringView str);
//    void putString(const char* str);
//    void putString(const char* str, size_t strlen);
//    std::string getString();
//    void putPackedNum(uint64_t num);
//    uint64_t getPackedNum();
//    void putPackedSignedNum(int64_t num);
//    int64_t getPackedSignedNum();
    //android::base::Stream fun
//private:
    TcpStream(int sock, size_t bufSize);
};

#endif

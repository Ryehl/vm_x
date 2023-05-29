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
#define LOG_TAG "TcpStream"

#include "TcpStream.h"
#include "cutils/sockets.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>

static int socket_loopback_server(const char* name) {
    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path + 1, name);
    int addr_len = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(name);

    int s = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s < 0)
        return -1;

    if (bind(s, reinterpret_cast<sockaddr *>(&addr), addr_len) < 0) {
        close(s);
        return -1;
    }

    if (listen(s, 4) < 0) {
        close(s);
        return -1;
    }

    return s;
}

TcpStream::TcpStream(size_t bufSize) :
        SocketStream(bufSize) {
}

TcpStream::TcpStream(int sock, size_t bufSize) :
        SocketStream(sock, bufSize) {
}

int TcpStream::listen(const char* name) {
    m_sock = socket_loopback_server(name);
    if (!valid()) return int(ERR_INVALID_SOCKET);

    return 0;
}

SocketStream *TcpStream::accept() {
    int clientSock;

    while (true) {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        clientSock = ::accept(m_sock, (sockaddr *) &addr, &len);

        if (clientSock < 0 && errno == EINTR) {
            continue;
        }
        break;
    }

    TcpStream *clientStream = NULL;

    if (clientSock >= 0) {
        clientStream = new TcpStream(clientSock, m_bufsize);
    }
    return clientStream;
}

int TcpStream::connect(unsigned short port) {
    return connect("127.0.0.1", port);
}

int TcpStream::connect(const char *hostname, unsigned short port) {
    m_sock = socket_network_client(hostname, port, SOCK_STREAM);
    if (!valid()) return -1;
    return 0;
}

void TcpStream::putByte(uint8_t value) {
    ::send(m_sock, &value, 1, 0);
}

void TcpStream::putFloat(float value) {
    union {
        float f;
        uint8_t bytes[sizeof(float)];
    } u;
    u.f = value;
    ::send(m_sock, &u.bytes, sizeof(u.bytes), 0);
}

void TcpStream::putBe16(uint16_t value) {
    uint8_t b[2] = {(uint8_t) (value >> 8), (uint8_t) value};
    ::send(m_sock, b, 2, 0);
}

void TcpStream::putBe32(uint32_t value) {
    uint8_t b[4] = {
            (uint8_t) (value >> 24),
            (uint8_t) (value >> 16),
            (uint8_t) (value >> 8),
            (uint8_t) value};
    ::send(m_sock, b, 4, 0);
}

void TcpStream::putBe64(uint64_t value) {
    uint8_t b[8] = {
            (uint8_t) (value >> 56),
            (uint8_t) (value >> 48),
            (uint8_t) (value >> 40),
            (uint8_t) (value >> 32),
            (uint8_t) (value >> 24),
            (uint8_t) (value >> 16),
            (uint8_t) (value >> 8),
            (uint8_t) value};
    ::send(m_sock, b, 8, 0);
}

uint8_t TcpStream::getByte() {
    uint8_t value[1] = {0};
    ::recv(m_sock, &value, 1, 0);
    return value[0];
}


float TcpStream::getFloat() {
    union {
        float f;
        uint8_t bytes[sizeof(float)];
    } u;
    ::recv(m_sock, &u.bytes, sizeof(u.bytes), 0);
    return u.f;
}


uint32_t TcpStream::getBe32() {
    uint8_t b[4] = {0};
    ::recv(m_sock, &b, 4, 0);
    return ((uint32_t) b[0] << 24) |
           ((uint32_t) b[1] << 16) |
           ((uint32_t) b[2] << 8) |
           (uint32_t) b[3];
}

uint64_t TcpStream::getBe64() {
    uint8_t b[8] = {0};
    ::recv(m_sock, &b, 8, 0);
    return ((uint64_t) b[0] << 56) |
           ((uint64_t) b[1] << 48) |
           ((uint64_t) b[2] << 40) |
           ((uint64_t) b[3] << 32) |
           ((uint64_t) b[4] << 24) |
           ((uint64_t) b[5] << 16) |
           ((uint64_t) b[6] << 8) |
           (uint64_t) b[7];
}

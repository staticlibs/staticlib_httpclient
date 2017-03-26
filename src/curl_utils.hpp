/*
 * Copyright 2017, alex at staticlibs.net
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

/* 
 * File:   curl_utils.hpp
 * Author: alex
 *
 * Created on March 25, 2017, 8:29 PM
 */

#ifndef STATICLIB_HTTPCLIENT_CURL_UTILS_HPP
#define	STATICLIB_HTTPCLIENT_CURL_UTILS_HPP

#include <cstring>
#include <string>
#include <utility>

namespace staticlib {
namespace httpclient {

// http://curl-library.cool.haxx.narkive.com/2sYifbgu/issue-with-curl-multi-timeout-while-doing-non-blocking-http-posts-in-vms
inline struct timeval create_timeout_struct(long timeo) {
    struct timeval timeout;
    std::memset(std::addressof(timeout), '\0', sizeof (timeout));
    timeout.tv_sec = 10;
    if (timeo > 0) {
        long ctsecs = timeo / 1000;
        if (ctsecs < 10) {
            timeout.tv_sec = ctsecs;
        }
        timeout.tv_usec = (timeo % 1000) * 1000;
    }
    return timeout;
}

inline fd_set create_fd() {
    fd_set res;
    FD_ZERO(std::addressof(res));
    return res;
}

// http://stackoverflow.com/a/9681122/314015
inline std::pair<std::string, std::string> curl_parse_header(const char* buffer, size_t len) {
    std::string name{};
    std::string value{};
    for (size_t i = 0; i < len; i++) {
        if (':' != buffer[i]) {
            name.push_back(buffer[i]);
        } else {
            // 2 for ': ', 2 for '\r\n'
            size_t valen = len - i - 2 - 2;
            if (valen > 0) {
                value.resize(valen);
                std::memcpy(std::addressof(value.front()), buffer + i + 2, value.length());
                break;
            }
        }
    }
    return {name, value};
}

} // namespace
}

#endif	/* STATICLIB_HTTPCLIENT_CURL_UTILS_HPP */

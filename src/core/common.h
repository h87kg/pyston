// Copyright (c) 2014 Dropbox, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PYSTON_CORE_COMMON_H
#define PYSTON_CORE_COMMON_H

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <stdint.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#define _STRINGIFY(N) #N
#define STRINGIFY(N) _STRINGIFY(N)

// GCC and clang handle always_inline very differently;
// we mostly only care about it for the stdlib, so just remove the attributes
// if we're not in clang
#ifdef __clang__
#define ALWAYSINLINE __attribute__((always_inline))
#define NOINLINE __attribute__((noinline))
#else
#define ALWAYSINLINE 
#define NOINLINE
#endif

// From http://stackoverflow.com/questions/3767869/adding-message-to-assert, modified to use fprintf
#define RELEASE_ASSERT(condition, fmt, ...) \
do { \
    if (! (condition)) { \
        fprintf(stderr, __FILE__ ":" STRINGIFY(__LINE__) ": %s: Assertion `" #condition "' failed: " fmt "\n", __PRETTY_FUNCTION__, ##__VA_ARGS__); \
        abort(); \
    } \
} while (false)
#ifndef NDEBUG
#   define ASSERT RELEASE_ASSERT
#else
#   define ASSERT(condition, fmt, ...) do { } while (false)
#endif

#define UNIMPLEMENTED() RELEASE_ASSERT(0, "unimplemented")

#define OFFSET(cls, attr) ((char*)&(((cls*)0x01)->attr) - (char*)0x1)

//#ifndef NDEBUG
//#define VALGRIND
//#endif

// Allow using std::pair as keys in hashtables:
namespace std {
    template <typename T1, typename T2> struct hash<pair<T1, T2> > {
        size_t operator() (const pair<T1, T2> p) const {
            return hash<T1>()(p.first) ^ (hash<T2>()(p.second) << 1);
        }
    };
}

#endif


//
//  interop.h
//  ScribbleLabApp File System Interface
//
//  Copyright (c) 2025 ScribbleLab. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this
//     list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef FS_INTEROP_H
#define FS_INTEROP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
    #define __FS_WINDOWS__ 1
#elif defined(__APPLE__) && defined(__MACH__)
    #define __FS_APPLE__ 1
    #include <TargetConditionals.h>
#elif defined(__linux__) || defined(__unix__)
    #define __FS_LINUX__ 1
#endif

/* Swift compiler detection */
#if defined(__FS_APPLE__)
    #if TARGET_OS_OSX || TARGET_OS_IOS || TARGET_OS_TV || TARGET_OS_WATCH
        #if defined(__has_attribute) && __has_attribute(swift_name)
            #define __SWIFT__ 1
        #endif
    #endif
#elif defined(__FS_WINDOWS__)
    #if defined(__clang__) && defined(__has_attribute) && __has_attribute(swift_name)
        #define __SWIFT__ 1
    #endif
#elif defined(__FS_LINUX__)
    #if defined(__has_attribute) && __has_attribute(swift_name)
        #define __SWIFT__ 1
    #endif
#endif

#if !defined(__SWIFT__)
    #if defined(__swift__) || defined(SWIFT_COMPILER)
        #define __SWIFT__ 1
        #if !defined(__has_attribute) || !__has_attribute(swift_name)
            #error "Swift compilation requested but Swift attributes not supported"
        #endif
    #endif
#endif

#ifdef __FS_WINDOWS__
    #define __FS_EXPORT__ __declspec(dllexport)
    #define __FS_IMPORT__ __declspec(dllimport)
    #define __FS_CDECL__ __cdecl
#else
    #define __FS_EXPORT__ __attribute__((visibility("default")))
    #define __FS_IMPORT__
    #define __FS_CDECL__
#endif

#if defined(__SWIFT__)
    #define __fs_SWIFT_NAME__(name) __attribute__((swift_name(#name)))
    #define __fs_SWIFT_PRIVATE__ __attribute__((swift_private))
    #define __fs_SWIFT_SENDABLE__ __attribute__((swift_attr("@Sendable")))
    #define __fs_SWIFT_ASYNC__ __attribute__((swift_async))
    #define __fs_SWIFT_ERROR__ __attribute__((swift_error))
    #define __fs_SWIFT_BRIDGED__(type) __attribute__((swift_bridge(#type)))
    #define __fs_SWIFT_ACTOR__ __attribute__((swift_attr("@Actor")))
    #define __fs_SWIFT_FROZEN__ __attribute__((swift_attr("@frozen")))
    #define __fs_SWIFT_NONISOLATED__ __attribute__((swift_attr("@nonisolated")))
    #define __fs_SWIFT_CALLABLE__(name) __FS_EXPORT__ __fs_SWIFT_NAME__(name)

#else
    #define __fs_SWIFT_NAME__(name)
    #define __fs_SWIFT_PRIVATE__
    #define __fs_SWIFT_SENDABLE__
    #define __fs_SWIFT_ASYNC__
    #define __fs_SWIFT_ERROR__
    #define __fs_SWIFT_BRIDGED__(type)
    #define __fs_SWIFT_ACTOR__
    #define __fs_SWIFT_FROZEN__
    #define __fs_SWIFT_NONISOLATED__
    #define __fs_SWIFT_CALLABLE__(name) __FS_EXPORT__
#endif

#ifdef __FS_WINDOWS__
    #define __FS_API__ __FS_CDECL__
#else
    #define __FS_API__
#endif

#if !defined(__nullable)
    #define __nullable
    #define __nonnull
    #define __null_unspecified
#endif

typedef struct {
    void* __nullable ptr;
    size_t len;
} __fs_SWIFT_NAME__(FSBuffer) fs_buffer_t;

typedef struct {
    const char* __nullable utf8_ptr;
    size_t len;
    bool owned;
} __fs_SWIFT_NAME__(FSString) fs_string_t;

typedef struct {
    void* __nullable bytes;
    size_t len;
    bool owned;
} __fs_SWIFT_NAME__(FSData) fs_data_t;

typedef struct {
    void* __nullable items;
    size_t count;
    size_t capacity;
    size_t item_size;
    bool owned;
} __fs_SWIFT_NAME__(FSArray) fs_array_t;

typedef struct {
    void* __nullable keys;
    void* __nullable values;
    size_t count;
    size_t capacity;
    bool owned;
} __fs_SWIFT_NAME__(FSDictionary) fs_dictionary_t;

typedef enum {
    FS_ERROR_NONE = 0,
    FS_ERROR_INVALID_ARGUMENT = 1,
    FS_ERROR_OUT_OF_MEMORY = 2,
    FS_ERROR_IO = 3,
    FS_ERROR_PERMISSION = 4,
    FS_ERROR_NOT_FOUND = 5,
    FS_ERROR_ALREADY_EXISTS = 6,
    FS_ERROR_NOT_SUPPORTED = 7,
    FS_ERROR_TIMEOUT = 8,
    FS_ERROR_CANCELLED = 9,
    FS_ERROR_UNKNOWN = 10
} __fs_SWIFT_NAME__(FSError) fs_error_t;

typedef struct {
    void* __nullable value;
    fs_error_t error;
} __fs_SWIFT_NAME__(FSResult) fs_result_t;

typedef void (*fs_swift_callback)(void* __nullable context);
typedef void (*fs_swift_error_callback)(void* __nullable context, fs_error_t error);
typedef void (*fs_swift_result_callback)(void* __nullable context, fs_result_t result);

#define __fs_SWIFT_CALLABLE__(name) \
    __fs_SWIFT_NAME__(name) \
    __attribute__((visibility("default")))

#define __fs_SWIFT_PROTOCOL__(name) \
    __fs_SWIFT_NAME__(name) \
    __attribute__((objc_protocol_requires_explicit_implementation))

typedef void (*fs_swift_retain)(void* __nonnull ptr);
typedef void (*fs_swift_release)(void* __nonnull ptr);

typedef struct {
    fs_swift_retain __nonnull retain;
    fs_swift_release __nonnull release;
} __fs_SWIFT_NAME__(FSMemoryManagement) fs_memory_management_t;

typedef struct {
    void* __nullable context;
    fs_swift_callback __nullable completion;
    fs_swift_error_callback __nullable error;
    bool is_cancelled;
} __fs_SWIFT_NAME__(FSAsyncOperation) fs_async_operation_t;

typedef struct {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
    const char* __nullable build;
} __fs_SWIFT_NAME__(FSVersion) fs_version_t;

#endif
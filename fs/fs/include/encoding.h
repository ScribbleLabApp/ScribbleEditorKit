//
//  encoding.h
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

#ifndef ENCODING_H
#define ENCODING_H

#include <stdint.h>

/* Line Endings */
#define FS_EOL_LF   "\n"
#define FS_EOL_CRLF "\r\n"
#define FS_EOL_CR   "\r"

/* Common Character Encodings */
typedef enum {
    FS_ENC_UNKNOWN = 0,
    FS_ENC_UTF8,
    FS_ENC_UTF16LE,
    FS_ENC_UTF16BE,
    FS_ENC_UTF32LE,
    FS_ENC_UTF32BE,
    FS_ENC_ASCII,
    FS_ENC_ISO_8859_1,
    FS_ENC_WIN_1252,
    FS_ENC_MAC_ROMAN,
    FS_ENC_EBCDIC,
    FS_ENC_MAX
} fs_enc_t;

/* BOM (Byte Order Mark) Definitions */
#define FS_BOM_UTF8   "\xEF\xBB\xBF"
#define FS_BOM_UTF16LE "\xFF\xFE"
#define FS_BOM_UTF16BE "\xFE\xFF"
#define FS_BOM_UTF32LE "\xFF\xFE\x00\x00"
#define FS_BOM_UTF32BE "\x00\x00\xFE\xFF"
#define FS_BOM_ASCII "\x00\x00\x00"
#define FS_BOM_ISO_8859_1 "\x00\x00\x00"
#define FS_BOM_WIN_1252 "\x00\x00\x00"
#define FS_BOM_MAC_ROMAN "\x00\x00\x00"
#define FS_BOM_EBCDIC "\x00\x00\x00"

/* BOM Lengths */
#define FS_BOM_UTF8_LEN   3
#define FA_BOM_UTF16_LEN  2
#define FS_BOM_UTF32_LEN  4
#define FS_BOM_ASCII_LEN  3
#define FS_BOM_ISO_8859_1_LEN 3
#define FS_BOM_WIN_1252_LEN 3
#define FS_BOM_MAC_ROMAN_LEN 3
#define FS_BOM_EBCDIC_LEN 3

/* Platform-specific default line ending */
#ifdef _WIN32
    #define FS_EOL_DEFAULT FS_EOL_CRLF
#else
    #define FS_EOL_DEFAULT FS_EOL_LF
#endif

/* Common encoding detection flags */
#define FS_ENC_F_BOM           (1 << 0)
#define FS_ENC_F_BINARY        (1 << 1)
#define FS_ENC_F_MIXED_EOL     (1 << 2)
#define FS_ENC_F_VALID_UTF8    (1 << 3)
#define FS_ENC_F_VALID_UTF16LE (1 << 4)
#define FS_ENC_F_VALID_UTF16BE (1 << 5)
#define FS_ENC_F_VALID_UTF32LE (1 << 6)
#define FS_ENC_F_VALID_UTF32BE (1 << 7)
#define FS_ENC_F_VALID_ASCII   (1 << 8)
#define FS_ENC_F_VALID_ISO_8859_1 (1 << 9)
#define FS_ENC_F_VALID_WIN_1252   (1 << 10)
#define FS_ENC_F_VALID_MAC_ROMAN  (1 << 11)
#define FS_ENC_F_VALID_EBCDIC     (1 << 12)

#define FS_IS_UTF8(enc)      ((enc) & FS_ENC_F_VALID_UTF8)
#define FS_IS_UTF16(enc)     ((enc) & (FS_ENC_F_VALID_UTF16LE | FS_ENC_F_VALID_UTF16BE))
#define FS_IS_UTF32(enc)     ((enc) & (FS_ENC_F_VALID_UTF32LE | FS_ENC_F_VALID_UTF32BE))
#define FS_IS_ASCII(enc)      ((enc) & FS_ENC_F_VALID_ASCII)
#define FS_IS_ISO_8859_1(enc) ((enc) & FS_ENC_F_VALID_ISO_8859_1)
#define FS_IS_WIN_1252(enc)   ((enc) & FS_ENC_F_VALID_WIN_1252)
#define FS_IS_MAC_ROMAN(enc)  ((enc) & FS_ENC_F_VALID_MAC_ROMAN)
#define FS_IS_EBCDIC(enc)     ((enc) & FS_ENC_F_VALID_EBCDIC)

typedef struct {
    fs_enc_t encoding;
    const char* eol;
    uint32_t flags;
    bool has_bom;
} fs_enc_info_t;

#endif
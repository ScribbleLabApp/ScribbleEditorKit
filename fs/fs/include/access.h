//
//  _fs.h
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

#ifndef ACCESS_H
#define ACCESS_H

#include <sys/types.h>

#define FS_X_OK      (1 << 0)
#define FS_W_OK      (1 << 1)
#define FS_R_OK      (1 << 2)

#define FS_PERM_RWXU 0700
#define FS_PERM_RUSR 0400
#define FS_PERM_WUSR 0200
#define FS_PERM_XUSR 0100

#define FS_PERM_RWXG 0070
#define FS_PERM_RGRP 0040
#define FS_PERM_WGRP 0020
#define FS_PERM_XGRP 0010

#define FS_PERM_RWXO 0007
#define FS_PERM_ROTH 0004
#define FS_PERM_WOTH 0002
#define FS_PERM_XOTH 0001

#define FS_PERM_ALL  0777
#define FS_PERM_755  0755
#define FS_PERM_644  0644
#define FS_PERM_600  0600
#define FS_PERM_666  0666

typedef enum {
    FS_ACCESS_NONE = 0,
    FS_ACCESS_READ = (1 << 0),
    FS_ACCESS_WRITE = (1 << 1),
    FS_ACCESS_EXEC = (1 << 2),
    FS_ACCESS_RW = (FS_ACCESS_READ | FS_ACCESS_WRITE),
    FS_ACCESS_RX = (FS_ACCESS_READ | FS_ACCESS_EXEC),
    FS_ACCESS_WX = (FS_ACCESS_WRITE | FS_ACCESS_EXEC),
    FS_ACCESS_RWX = (FS_ACCESS_RW | FS_ACCESS_EXEC),
    FS_ACCESS_ALL = 0xFFFFFFFF
} fs_access_mode_t;

typedef enum {
    FS_ACE_USR,
    FS_ACE_GRP,
    FS_ACE_OTH
} fs_ace_type_t;

typedef struct {
    fs_ace_type_t type;
    uid_t uid;
    gid_t gid;
    fs_access_mode_t permissions;
} fs_ace_t;

typedef enum {
    FS_ERR_NONE = 0,
    FS_ERR_PERM,
    FS_ERR_ACCESS,
    FS_ERR_NOENT,
    FS_ERR_EXIST,
    FS_ERR_NOTDIR,
    FS_ERR_ISDIR,
    FS_ERR_INVAL,
    FS_ERR_NOACCESS,
    FS_ERR_NOUSER,
    FS_ERR_NOGROUP,
    FS_ERR_NOFILE,
    FS_ERR_NOMEM,
    FS_ERR_NOTSUP,
    FS_ERR_MAX
} fs_ace_error_t;

/* Basic permission checks */
#define FS_IS_READABLE(mode) ((mode) & (FS_ACCESS_READ | FS_ACCESS_RW))
#define FS_IS_WRITABLE(mode) ((mode) & (FS_ACCESS_WRITE | FS_ACCESS_RW))
#define FS_IS_EXECUTABLE(mode) ((mode) & FS_ACCESS_EXEC)

/* Generic flag check */
#define FS_IS_SET(mode, flag) ((mode) & (flag))

/* User/Group/Other checks */
#define FS_IS_SET_UID(mode) FS_IS_SET((mode), FS_ACCESS_RWXU)
#define FS_IS_SET_GID(mode) FS_IS_SET((mode), FS_ACCESS_RWXG)
#define FS_IS_SET_OTH(mode) FS_IS_SET((mode), FS_ACCESS_RWXO)

/* Individual permission checks */
#define FS_IS_SET_READ(mode) FS_IS_SET((mode), FS_ACCESS_READ)
#define FS_IS_SET_WRITE(mode) FS_IS_SET((mode), FS_ACCESS_WRITE)
#define FS_IS_SET_EXEC(mode) FS_IS_SET((mode), FS_ACCESS_EXEC)

/* Combined permission checks */
#define FS_IS_SET_RW(mode) FS_IS_SET((mode), FS_ACCESS_RW)
#define FS_IS_SET_RX(mode) FS_IS_SET((mode), FS_ACCESS_RX)
#define FS_IS_SET_WX(mode) FS_IS_SET((mode), FS_ACCESS_WX)
#define FS_IS_SET_RWX(mode) FS_IS_SET((mode), FS_ACCESS_RWX)

/* Full permission checks */
#define FS_IS_SET_ALL(mode) ((mode) == FS_ACCESS_ALL)
#define FS_IS_SET_ALL_RWX(mode) ((mode) == (FS_ACCESS_RWX | FS_ACCESS_ALL))
#define FS_IS_SET_ALL_RW(mode) ((mode) == (FS_ACCESS_RW | FS_ACCESS_ALL))
#define FS_IS_SET_ALL_RX(mode) ((mode) == (FS_ACCESS_RX | FS_ACCESS_ALL))
#define FS_IS_SET_ALL_WX(mode) ((mode) == (FS_ACCESS_WX | FS_ACCESS_ALL))

#endif
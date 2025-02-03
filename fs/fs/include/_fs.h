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

#ifndef _FS_H
#define _FS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <access.h>

typedef enum {
    FS_FILE_TYPE_UNKNOWN = 0,
    FS_FILE_TYPE_REGULAR = (1 << 0),
    FS_FILE_TYPE_DIRECTORY = (1 << 1),
    FS_FILE_TYPE_SYMLINK = (1 << 2),
    FS_FILE_TYPE_FIFO = (1 << 3),
    FS_FILE_TYPE_SOCK = (1 << 4),
    FS_FILE_TYPE_BLK = (1 << 5),
    FS_FILE_TYPE_CHR = (1 << 6),
    FS_FILE_TYPE_WHT = (1 << 7)
} fsFileType;

typedef struct {
    fsFileType type;
    off_t size;
    time_t creationTime;
    time_t modificationTime;
} fsFileAttributes;

bool fsFileExists(const char *path);
fsFileType fsGetFileType(const char *path);
fsFileAttributes fsGetFileAttributes(const char *path);
char **fsListFilesInDirectory(const char *directoryPath, int *count);

bool fsCreateFile(const char *path, const char *data);
bool fsWriteToFile(const char *path, const char *data);
bool fsAppendToFile(const char *path, const char *data);
char *fsReadFile(const char *path);
bool fsDeleteFile(const char *path);
bool fsMoveFile(const char *fromPath, const char *toPath);
bool fsCopyFile(const char *fromPath, const char *toPath);

bool fsCreateDirectory(const char *path);
bool fsDeleteDirectory(const char *path);
bool fsMoveDirectory(const char *fromPath, const char *toPath);
bool fsCopyDirectory(const char *fromPath, const char *toPath);

bool fsCreateSymbolicLink(const char *target, const char *linkPath);
bool fsReadSymbolicLink(const char *linkPath, char *buffer, size_t size);

bool fsSetFilePermissions(const char *path, mode_t mode);
bool fsGetFilePermissions(const char *path, mode_t *mode);

bool fsSetUserPermissions(const char *path, fs_access_mode_t perms);
bool fsSetGroupPermissions(const char *path, fs_access_mode_t perms);
bool fsSetOthersPermissions(const char *path, fs_access_mode_t perms);

bool fsAddFileAccessControlEntry(const char *path, fs_ace_t *ace);
bool fsRemoveFileAccessControlEntry(const char *path, fs_ace_t *ace);
fs_ace_t *fsGetFileAccessControlEntry(const char *path, size_t *count);

char *fsGetFileExtension(const char *path);
char *fsGetFileName(const char *path);
char *fsGetFileNameWithoutExtension(const char *path);
char *fsGetFileParentDirectory(const char *path);

bool fsIsReadable(const char *path);
bool fsIsWritable(const char *path);
bool fsIsExecutable(const char *path);

#endif // _FS_H

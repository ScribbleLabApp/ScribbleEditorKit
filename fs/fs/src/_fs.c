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

#include <_fs.h>
#include <access.h>

bool fsFileExists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

fsFileType fsGetFileType(const char *path) {
    struct stat buffer;
    if (stat(path, &buffer) != 0) return FS_FILE_TYPE_UNKNOWN;
    if (S_ISREG(buffer.st_mode)) return FS_FILE_TYPE_REGULAR;
    if (S_ISDIR(buffer.st_mode)) return FS_FILE_TYPE_DIRECTORY;
    if (S_ISLNK(buffer.st_mode)) return FS_FILE_TYPE_SYMLINK;
    return FS_FILE_TYPE_UNKNOWN;
}

fsFileAttributes fsGetFileAttributes(const char *path) {
    struct stat buffer;
    fsFileAttributes attrs = { 0, 0, FS_FILE_TYPE_UNKNOWN };

    if (stat(path, &buffer) == 0) {
        attrs.size = buffer.st_size;
        attrs.creationTime = buffer.st_ctime;
        attrs.modificationTime = buffer.st_mtime;
        attrs.type = fsGetFileType(path);
    }

    return attrs;
}

char **fsListFilesInDirectory(const char *directoryPath, int *count) {
    DIR *dir = opendir(directoryPath);
    if (!dir) return NULL;

    struct dirent *entry;
    char **fileList = malloc(sizeof(char *) * 256);
    int index = 0;

    while ((entry = readdir(dir) != NULL)) {
        fileList[index] = strdup(entry->d_name);
        index++;
    }

    closedir(dir);
    *count = index;
    return fileList;
}


bool fsCreateFile(const char *path, const char *data) {
    FILE *file = fopen(path, FS_ACCESS_WRITE); // "w"

    if (!file) return false;
    if (data) fputs(data, file);

    fclose(file);
    return true;
}

bool fsWriteToFile(const char *path, const char *data) {
    FILE *file = fopen(path, FS_ACCESS_WRITE); // "w"

    if (!file) return false;
    fputs(data, file);

    fclose(file);
    return true;
}

bool fsAppendToFile(const char *path, const char *data) {
    FILE *file = fopen(path, FS_ACCESS_ALL); // "a"
    
    if (!file) return false;
    fputs(data, file);

    fclose(file);
    return true;
}

char *fsReadFile(const char *path) {
    FILE *file = fopen(path, FS_ACCESS_WRITE); // "r"
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *buffer = malloc(length + 1);
    fread(buffer, 1 , length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

bool fsDeleteFile(const char *path) {
    return (remove(path) == 0);
}

bool fsMoveFile(const char *fromPath, const char *toPath) {
    return (rename(fromPath, toPath) == 0);
}

bool fsCopyFile(const char *fromPath, const char *toPath) {
    char *data = fsReadFile(fromPath);

    if (!data) return false;
    bool success = fsWriteToFile(toPath, data);

    free(data);
    return success;
}


bool fsCreateDirectory(const char *path) {
    return (mkdir(path, FS_PERM_755) == 0);
}

bool fsDeleteDirectory(const char *path) {
    return (rmdir(path) == 0);
}

bool fsMoveDirectory(const char *fromPath, const char *toPath) {}
bool fsCopyDirectory(const char *fromPath, const char *toPath) {}


bool fsCreateSymbolicLink(const char *target, const char *linkPath) {}
bool fsReadSymbolicLink(const char *linkPath, char *buffer, size_t size) {}


bool fsSetFilePermissions(const char *path, mode_t mode) {}
bool fsGetFilePermissions(const char *path, mode_t *mode) {}


bool fsSetUserPermissions(const char *path, fs_access_mode_t perms) {}
bool fsSetGroupPermissions(const char *path, fs_access_mode_t perms) {}
bool fsSetOthersPermissions(const char *path, fs_access_mode_t perms) {}


bool fsAddFileAccessControlEntry(const char *path, fs_ace_t *ace) {}
bool fsRemoveFileAccessControlEntry(const char *path, fs_ace_t *ace) {}
fs_ace_t *fsGetFileAccessControlEntry(const char *path, size_t *count) {}


char *fsGetFileExtension(const char *path) {
    char *ext = strrchr(path, '.');
    return ext ? strdup(ext + 1) : strdup("");
}

char *fsGetFileName(const char *path) {}
char *fsGetFileNameWithoutExtension(const char *path) {}

char *fsGetFileParentDirectory(const char *path) {
    char *dirPath = strdup(path);
    char *lastSlash = strrchr(dirPath, "/");
    if (lastSlash) *lastSlash = '\0';

    return dirPath;
}


bool fsIsReadable(const char *path) {
    return (access(path, FS_R_OK) == 0);
}

bool fsIsWritable(const char *path) {
    return (access(path, FS_W_OK) == 0);
}

bool fsIsExecutable(const char *path) {
    return (access(path, FS_X_OK) == 0);
}
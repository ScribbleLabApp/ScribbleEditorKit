//
//  FSManager.m
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

#import <FSManager.h>

@implementation FSManager

+ (instancetype)sharedManager {
    static FSManager *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

- (BOOL)fileExistsAtPath:(NSString *)path {
    return fsFileExists([path fileSystemRepresentation]);
}

- (NSData * _Nullable)readFileAtPath:(NSString *)path {
    char *content = fsReadFile([path fileSystemRepresentation]);
    if (!content) return nil;
    NSData *data = [NSData dataWithBytes:content length:strlen(content)];
    free(content);
    return data;
}

- (BOOL)writeData:(NSData *)data toFileAtPath:(NSString *)path {
    return fsWriteToFile([path fileSystemRepresentation], data.bytes);
}

- (BOOL)appendData:(NSData *)data toFileAtPath:(NSString *)path {
    return fsAppendToFile([path fileSystemRepresentation], data.bytes);
}

- (BOOL)deleteFileAtPath:(NSString *)path {
    return fsDeleteFile([path fileSystemRepresentation]);
}

- (BOOL)moveFileFromPath:(NSString *)fromPath toPath:(NSString *)toPath {
    return fsMoveFile([fromPath fileSystemRepresentation], [toPath fileSystemRepresentation]);
}

- (BOOL)copyFileFromPath:(NSString *)fromPath toPath:(NSString *)toPath {
    return fsCopyFile([fromPath fileSystemRepresentation], [toPath fileSystemRepresentation]);
}

- (NSArray<NSString *> * _Nullable)listFilesInDirectoryAtPath:(NSString *)path {
    int count = 0;
    char **files = fsListFilesInDirectory([path fileSystemRepresentation], &count);
    
    if (!files) return nil;
    
    NSMutableArray<NSString *> *fileList = [NSMutableArray array];
    for (int i = 0; i < count; i++) {
        [fileList addObject:[NSString stringWithUTF8String:files[i]]];
        free(files[i]);
    }
    free(files);
    return fileList;
}

- (BOOL)createDirectoryAtPath:(NSString *)path {
    return fsCreateDirectory([path fileSystemRepresentation]);
}

- (BOOL)deleteDirectoryAtPath:(NSString *)path {
    return fsDeleteDirectory([path fileSystemRepresentation]);
}

- (BOOL)moveDirectoryFromPath:(NSString *)fromPath toPath:(NSString *)toPath {
    return fsMoveDirectory([fromPath fileSystemRepresentation], [toPath fileSystemRepresentation]);
}

- (BOOL)copyDirectoryFromPath:(NSString *)fromPath toPath:(NSString *)toPath {
    return fsCopyDirectory([fromPath fileSystemRepresentation], [toPath fileSystemRepresentation]);
}

- (fsFileType)fileTypeAtPath:(NSString *)path {
    return fsGetFileType([path fileSystemRepresentation]);
}

- (nullable fsFileAttributes *)fileAttributesAtPath:(NSString *)path {
    fsFileAttributes attributes = fsGetFileAttributes([path fileSystemRepresentation]);
    return &attributes;
}

- (BOOL)setFilePermissionsAtPath:(NSString *)path mode:(mode_t)mode {
    return fsSetFilePermissions([path fileSystemRepresentation], mode);
}

- (mode_t)getFilePermissionsAtPath:(NSString *)path {
    mode_t mode;
    return fsGetFilePermissions([path fileSystemRepresentation], &mode) ? mode : 0;
}

- (BOOL)isReadableAtPath:(NSString *)path {
    return fsIsReadable([path fileSystemRepresentation]);
}

- (BOOL)isWritableAtPath:(NSString *)path {
    return fsIsWritable([path fileSystemRepresentation]);
}

- (BOOL)isExecutableAtPath:(NSString *)path {
    return fsIsExecutable([path fileSystemRepresentation]);
}

- (nullable NSString *)fileExtensionAtPath:(NSString *)path {
    char *ext = fsGetFileExtension([path fileSystemRepresentation]);
    if (!ext) return nil;
    NSString *extension = [NSString stringWithUTF8String:ext];
    free(ext);
    return extension;
}

- (nullable NSString *)fileNameAtPath:(NSString *)path {
    char *name = fsGetFileName([path fileSystemRepresentation]);
    if (!name) return nil;
    NSString *fileName = [NSString stringWithUTF8String:name];
    free(name);
    return fileName;
}

- (nullable NSString *)fileNameWithoutExtensionAtPath:(NSString *)path {
    char *name = fsGetFileNameWithoutExtension([path fileSystemRepresentation]);
    if (!name) return nil;
    NSString *fileNameWithoutExt = [NSString stringWithUTF8String:name];
    free(name);
    return fileNameWithoutExt;
}

- (nullable NSString *)parentDirectoryAtPath:(NSString *)path {
    char *parent = fsGetFileParentDirectory([path fileSystemRepresentation]);
    if (!parent) return nil;
    NSString *parentDir = [NSString stringWithUTF8String:parent];
    free(parent);
    return parentDir;
}

@end

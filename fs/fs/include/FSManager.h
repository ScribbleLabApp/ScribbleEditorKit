//
//  FSManager.h
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

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSManager : NSObject

+ (instancetype)sharedManager;

- (BOOL)fileExistsAtPath:(NSString *)path;
- (NSData * _Nullable)readFileAtPath:(NSString *)path;
- (BOOL)writeData:(NSData *)data toFileAtPath:(NSString *)path;
- (BOOL)appendData:(NSData *)data toFileAtPath:(NSString *)path;
- (BOOL)deleteFileAtPath:(NSString *)path;
- (BOOL)moveFileFromPath:(NSString *)fromPath toPath:(NSString *)toPath;
- (BOOL)copyFileFromPath:(NSString *)fromPath toPath:(NSString *)toPath;

- (NSArray<NSString *> * _Nullable)listFilesInDirectoryAtPath:(NSString *)path;
- (BOOL)createDirectoryAtPath:(NSString *)path;
- (BOOL)deleteDirectoryAtPath:(NSString *)path;
- (BOOL)moveDirectoryFromPath:(NSString *)fromPath toPath:(NSString *)toPath;
- (BOOL)copyDirectoryFromPath:(NSString *)fromPath toPath:(NSString *)toPath;

- (fsFileType)fileTypeAtPath:(NSString *)path;
- (nullable fsFileAttributes *)fileAttributesAtPath:(NSString *)path;

- (BOOL)setFilePermissionsAtPath:(NSString *)path mode:(mode_t)mode;
- (mode_t)getFilePermissionsAtPath:(NSString *)path;
- (BOOL)isReadableAtPath:(NSString *)path;
- (BOOL)isWritableAtPath:(NSString *)path;
- (BOOL)isExecutableAtPath:(NSString *)path;

- (nullable NSString *)fileExtensionAtPath:(NSString *)path;
- (nullable NSString *)fileNameAtPath:(NSString *)path;
- (nullable NSString *)fileNameWithoutExtensionAtPath:(NSString *)path;
- (nullable NSString *)parentDirectoryAtPath:(NSString *)path;

@end

NS_ASSUME_NONNULL_END
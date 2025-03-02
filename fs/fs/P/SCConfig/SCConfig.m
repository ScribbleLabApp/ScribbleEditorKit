//
//  SCConfig.m
//  ScribbleLabApp File Configurations
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

#import "SCConfig.h"

@implementation SCConfig

- (void)_saveConfigToFile:(NSString *)filePath {
    NSMutableString *xmlContent = [NSMutableString stringWithString:@"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"];
    [xmlContent appendString:@"<scconfig>\n"];

    NSDictionary *fields = @{
        @"created": self.created,
        @"modified": self.modified,
        @"author": self.author,
        @"title": self.title,
        @"theme": self.theme,
        @"provider": self.syncProvider,
        @"algorithm": self.encryptionAlgorithm,
        @"key": self.encryptionKey,
        @"canvasSize": self.canvasSize,
        @"pageOrientation": self.pageOrientation,
        @"background": self.background,
        @"margins": self.margins,
        @"lineSpacing": self.lineSpacing,
        @"resolution": self.resolution,
        @"lineCoding": self.lineCoding
    };

    for (NSString *key in fields) {
        if (fields[key]) {
            [xmlContent appendFormat:@"  <%@>%@</%@>\n", key, fields[key], key];
        }
    }

    NSDictionary *integers = @{
        @"frequency": @(self.syncFrequency),
        @"keyLength": @(self.encryptionKeyLength),
        @"autosave": @(self.autosaveInterval),
        @"revisions": @(self.revisions),
        @"dpi": @(self.dpi),
        @"bitDepth": @(self.bitDepth)
    };

    for (NSString *key in integers) {
        [xmlContent appendFormat:@"  <%@>%ld</%@>\n", key, (long)[integers[key] integerValue], key];
    }

    NSDictionary *booleans = @{
        @"enabled": @(self.syncEnabled),
        @"syncOnSave": @(self.syncOnSave),
        @"periodicSync": @(self.periodicSync),
        @"cloudSync": @(self.cloudSync),
        @"localBackup": @(self.localBackup),
        @"encryption": @(self.encryptionEnabled)
    };

    for (NSString *key in booleans) {
        [xmlContent appendFormat:@"  <%@>%@</%@>\n", key, [booleans[key] boolValue] ? @"true" : @"false", key];
    }

    [xmlContent appendString:@"</scconfig>\n"];

    NSError *error;
    [xmlContent writeToFile:filePath atomically:YES encoding:NSUTF8StringEncoding error:&error];

    if (error) {
        NSLog(@"[SCConfig] Failed to save config file: %@", error.localizedDescription);
    }
}

@end
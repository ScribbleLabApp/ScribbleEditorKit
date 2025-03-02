//
//  SCState.m
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

#import <fs/SCState.h>
#include <Foundation/Foundation.h>

@implementation SCState

- (instancetype)init {
    return [self initWithDictionary:@{}];
}

- (instancetype)initWithDictionary:(NSDictionary *)dictionary {
    self = [ super init ];
    if (self) {
        NSString *timestampString = dictionary[@"lastModified"];
        _lastModified = timestampString ? [self dateFromISOString:timestampString] : [NSDate date];

        _autosaveEnabled = [dictionary[@"autosaveEnabled"] boolValue];
        _undoHistory = [dictionary[@"undoHistory"] ?: @[] mutableCopy];
        _redoHistory = [dictionary[@"redoHistory"] ?: @[] mutableCopy];
        _mouseState = [dictionary[@"mouseState"] ?: @{} mutableCopy];
        _keyboardState = [dictionary[@"keyboardState"] ?: @{} mutableCopy];
        _selectionState = [dictionary[@"selectionState"] ?: @{} mutableCopy];
        _metadata = dictionary[@"metadata"];
    }

    return self;
}

- (NSDictionary *)toDictionary {
    return @{
        @"lastModified": [self isoStringFromDate:_lastModified],
        @"autosaveEnabled": @(self.autosaveEnabled),
        @"undoHistory": self.undoHistory,
        @"redoHistory": self.redoHistory,
        @"mouseState": self.mouseState,
        @"keyboardState": self.keyboardState,
        @"selectionState": self.selectionState,
        @"metadata": self.metadata ?: @{}
    };
}

- (NSDate *)dateFromISOString:(NSString *)isoString {
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    formatter.dateFormat = @"yyyy-MM-dd'T'HH:mm:ss.SSSZ";
    return [formatter dateFromString:isoString];
}

- (NSString *)isoStringFromDate:(NSDate *)date {
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    formatter.dateFormat = @"yyyy-MM-dd'T'HH:mm:ss.SSSZ";
    return [formatter stringFromDate:date];
}

@end

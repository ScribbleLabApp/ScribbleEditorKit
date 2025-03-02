//
//  SCStateParser.m
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
//


//  Description:
//  SCStateParser is responsible for handling the serialization and deserialization
//  of `.scstate` files, which store metadata related to document state in
//  ScribbleLabApp.
//
//  The `.scstate` files are stored in JSON format and contain information such as:
//  - Undo/redo history
//  - Last modification date
//  - Autosave preferences
//  - UI interaction states (mouse, keyboard, selection)
//  - Additional metadata
//
//  This class provides static methods to load an `.scstate` file into an SCState
//  object and save an SCState object back to disk.
//

#import "SCState.h"
#import "SCStateParser.h"
#include <Foundation/Foundation.h>

@implementation SCStateParser

+ (nullable SCState *)loadStateFromFile:(NSString *)filePath {
    if (![[NSFileManager defaultManager] fileExistsAtPath:filePath]) {
        NSLog(@"[SCStateParser]: State File does not exist at path: %@", filePath);
        return nil;
    }

    NSError *readError = nil;
    NSData *data = [NSData dataWithContentsOfFile:filePath options:0 error:&readError];

    if (!data) {
        NSLog(@"[SCStateParser]: Error reading file at path: %@\n%@", filePath,
              readError.localizedDescription);
        return nil;
    }

    NSError *jsonError = nil;
    NSDictionary *jsonDict = [NSJSONSerialization JSONObjectWithData:data
                             options:NSJSONReadingMutableContainers error:&jsonError];
    
    if (!jsonDict || ![jsonDict isKindOfClass:[NSDictionary class]]) {
        NSLog(@"[SCStateParser]: Error parsing JSON data from file at path: %@\n%@", filePath, 
              jsonError.localizedDescription);
        return nil;
    }

    return [[SCState alloc] initWithDictionary:jsonDict];
}

+ (BOOL)saveState:(SCState *)state toFile:(NSString *)filePath {
    if (!state) {
        NSLog(@"[SCStateParser]: Cannot save nil state object to file at path: %@", filePath);
        return NO;
    }

    NSDictionary *stateDict = [state toDictionary];

    NSError *jsonError = nil;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:stateDict 
                       options:NSJSONWritingPrettyPrinted error:&jsonError];

    if (!jsonData) {
        NSLog(@"[SCStateParser]: Error serializing state object to JSON data for file at path: %@\n%@", 
              filePath, jsonError.localizedDescription);
        return NO;
    }

    NSError *writeError = nil;
    BOOL success = [jsonData writeToFile:filePath options:NSDataWritingAtomic error:&writeError];

    if (!success) {
        NSLog(@"[SCStateParser]: Error writing JSON data to file at path: %@\n%@", filePath, 
              writeError.localizedDescription);
    }
    
    return success;
}

@end

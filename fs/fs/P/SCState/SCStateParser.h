//
//  SCStateParser.h
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

#import <Foundation/Foundation.h>
#include <objc/NSObject.h>
#import "SCState.h"

NS_ASSUME_NONNULL_BEGIN

/// A utility class for parsing and serializing `.scstate` files.
///
/// SCStateParser provides functionality to:
/// - Read `.scstate` files and convert them into `SCState` objects.
/// - Validate `.scstate` files to ensure they follow the correct format.
/// - Serialize `SCState` objects back into JSON files.
/// - Handle file I/O errors gracefully.
///
/// @note This class only handles `.scstate` files and does not manage `.sc` timestamp backups.
NS_AVAILABLE(15, 18)
@interface SCStateParser : NSObject

/// Loads an `.scstate` file and parses its contents into an `SCState` object.
///
/// This method reads a JSON file from disk, validates its structure, and
/// initializes an `SCState` object with the extracted data.
///
/// @param filePath The full path to the `.scstate` file.
/// @return An `SCState` object if parsing succeeds, or `nil` if the file could not be read or was invalid.
/// @note This method returns `nil` if the JSON structure is incorrect or the file is missing.
+ (nullable SCState *)loadStateFromFile:(NSString *)filePath;

/// Saves the given `SCState` object to a `.scstate` JSON file.
///
/// This method converts the `SCState` object into a JSON representation and
/// writes it to disk.
///
/// @param state The `SCState` object containing the document state.
/// @param filePath The destination file path where the `.scstate` file should be saved.
/// @return `YES` if the operation was successful, `NO` otherwise.
/// @note If saving fails, an error message is logged, but the method does not throw exceptions.
+ (BOOL)saveState:(SCState *)state toFile:(NSString *)filePath;

@end

NS_ASSUME_NONNULL_END
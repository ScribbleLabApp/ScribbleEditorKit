//
//  SCState.h
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

//  Defines the structure of a `.scstate` file, which stores document state metadata.

#import <Foundation/Foundation.h>
#include <objc/NSObject.h>

NS_ASSUME_NONNULL_BEGIN

/// Represents the document state stored in an `.scstate` file.
///
/// This class manages state persistence, ensuring that document history,
/// cursor position, selection, and other metadata are saved and restored properly.
NS_AVAILABLE(15, 18)
@interface SCState : NSObject

#pragma mark - Properties
/// The last modified timestamp of the document.
/// This value is stored in ISO 8601 format for compatibility across systems.
@property (nonatomic, strong) NSDate *lastModified;

/// Indicates whether autosave is enabled.
/// If `YES`, the document is automatically saved at regular intervals.
@property (nonatomic, assign) BOOL autosaveEnabled;

/// A list of undo operations performed in the document.
///
/// Each entry in this array is a dictionary containing:
/// - `"command"`: The action performed (e.g., `"bold"`, `"delete"`)
/// - `"stateBefore"`: The document state before the action
/// - `"stateAfter"`: The document state after the action
@property (nonatomic, strong) NSArray<NSDictionary *> *undoHistory;

/// A list of redo operations, similar to `undoHistory`.
///
/// Entries follow the same format:
/// - `"command"`: The action to redo
/// - `"stateBefore"`: The document state before redo
/// - `"stateAfter"`: The document state after redo
@property (nonatomic, strong) NSArray<NSDictionary *> *redoHistory;

/// Stores the current state of the mouse.
///
/// This dictionary may contain:
/// - `"cursorPosition"`: A dictionary with `x` and `y` coordinates
/// - `"selection"`: The text selection range (`start`, `end`)
/// - `"mouseButtonState"`: Status of mouse buttons (`left`, `right`, `middle`)
@property (nonatomic, strong) NSDictionary *mouseState;

/// Stores the current state of the keyboard.
///
/// This dictionary may include:
/// - `"activeKeys"`: An array of currently pressed keys
/// - `"modifiers"`: Boolean values for Shift, Ctrl, Alt, Command
/// - `"inputLanguage"`: The current keyboard language
@property (nonatomic, strong) NSDictionary *keyboardState;

/// Stores the text selection state within the document.
///
/// This dictionary may include:
/// - `"selectedText"`: The actual selected string
/// - `"selectionRange"`: A dictionary with `start` and `end` positions
/// - `"highlightColor"`: Color used for highlighting the selection
@property (nonatomic, strong) NSDictionary *selectionState;

/// Additional metadata related to the document.
///
/// This can include:
/// - `"documentID"`: A unique identifier for the document
/// - `"theme"`: The current UI theme (light/dark mode)
/// - `"tags"`: User-defined tags associated with the document
@property (nonatomic, strong, nullable) NSDictionary *metadata;

#pragma mark - Initializer

- (instancetype)init;

/// Initializes an `SCState` object using a dictionary representation.
///
/// This method is useful for loading `.scstate` files into memory.
///
/// @param dictionary A dictionary representation of the document state.
/// @return An initialized `SCState` object.
- (instancetype)initWithDictionary:(NSDictionary *)dictionary NS_DESIGNATED_INITIALIZER;

#pragma mark - Methods
/// Converts the current state into a dictionary for JSON serialization.
///
/// This method is useful for saving the `.scstate` file to disk.
///
/// @return A dictionary representation of the current state.
- (NSDictionary *)toDictionary;

@end

NS_ASSUME_NONNULL_END

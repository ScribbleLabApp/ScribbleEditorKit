//
//  SCConfig.h
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

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// **SCConfig** is a configuration model for managing ScribbleLab application settings.
///
/// This class encapsulates **metadata, sync settings, encryption, autosave, and document settings** 
/// for a user’s file configurations. It provides properties for tracking document properties, 
/// enabling/disabling sync, configuring encryption, and defining autosave behavior.
///
/// Configurations can be loaded from XML files, edited at runtime, and saved back to disk.
///
/// @note **Availability:** macOS 15.0+ / iOS 18.0+
NS_AVAILABLE(15, 18)
@interface SCConfig : NSObject

#pragma mark - Metadata
/// The creation date of the configuration file.
/// This is typically set when the document is first created.
@property (nonatomic, strong) NSString *created;

/// The last modified date of the configuration file.
/// Updated whenever changes are made to the document.
@property (nonatomic, strong) NSString *modified;

/// The name or identifier of the document's author.
/// This is typically set when the document is first created.
@property (nonatomic, strong) NSString *author;

#pragma mark - General
/// The title of the document.
/// This is typically displayed in the document viewer and editor UI.
@property (nonatomic, strong) NSString *title;

/// The theme of the document (e.g., "dark", "light", "custom").
/// This affects how the document is visually styled.
@property (nonatomic, strong) NSString *theme;

#pragma mark - Sync
/// Enables or disables synchronization of the document with a remote server.
@property (nonatomic, assign) BOOL syncEnabled;

/// The URL or identifier of the sync server.
/// This specifies where document changes are synchronized.
@property (nonatomic, strong) NSString *syncProvider; // syncServer

/// The frequency of automatic synchronization, measured in seconds.
/// This determines how often the document is synchronized with the server.
@property (nonatomic, assign) NSInteger syncFrequency;

/// Enables or disables automatic sync whenever the document is saved.
@property (nonatomic, assign) BOOL syncOnSave;

/// Enables or disables periodic sync, which syncs the document at regular intervals.
@property (nonatomic, assign) BOOL periodicSync;

/// Enables or disables cloud-based synchronization.
/// This determines whether the document is synced with a cloud service.
@property (nonatomic, assign) BOOL cloudSync;

/// Enables or disables automatic local backups.
/// Local backups provide a recovery option in case of sync failures.
@property (nonatomic, assign) BOOL localBackup;

#pragma mark - Encryption
/// Enables or disables encryption for the document.
/// When enabled, the document contents will be securely encrypted.
@property (nonatomic, assign) BOOL encryptionEnabled;

/// The encryption algorithm used for securing the document.
/// Example values: "AES-256", "RSA", "ChaCha20".
@property (nonatomic, strong) NSString *encryptionAlgorithm;

/// The encryption key used for securing the document.
/// Stored securely and required for decryption.
@property (nonatomic, strong) NSString *encryptionKey;

/// The length of the encryption key, measured in bits.
/// Example: 128, 256, 512.
@property (nonatomic, assign) NSInteger encryptionKeyLength;

#pragma mark - Autosave
/// The interval, in seconds, at which autosave is triggered.
/// Example: If set to `60`, the document will be saved every 60 seconds.
@property (nonatomic, assign) NSInteger autosaveInterval;

/// The number of historical revisions stored for the document.
/// Used to track changes and allow undo functionality.
@property (nonatomic, assign) NSInteger revisions;

#pragma mark - Document Settings
/// The dimensions of the document canvas, typically specified as "WidthxHeight".
/// Example: "1920x1080", "A4", "Letter".
@property (nonatomic, strong) NSString *canvasSize;

/// The orientation of the document.
/// Possible values: "portrait" or "landscape".
@property (nonatomic, strong) NSString *pageOrientation;

/// The background type or color of the document.
/// Example values: "white", "transparent", "#FF5733" (hex color).
@property (nonatomic, strong) NSString *background;

/// The document resolution in dots per inch (DPI).
/// Example: 72, 150, 300.
@property (nonatomic, assign) NSInteger dpi;

/// The document margins, typically formatted as "top,right,bottom,left".
/// Example: "10,10,10,10" for 10px margins on all sides.
@property (nonatomic, strong) NSString *margins;

/// The spacing between lines of text or elements.
/// Example values: "single", "1.5", "double".
@property (nonatomic, strong) NSString *lineSpacing;

/// The color space of the document.
/// Example values: "RGB", "CMYK", "Grayscale".
@property (nonatomic, strong) NSString *colorSpace;

/// The bit depth of the document (color precision per channel).
/// Example values: 8, 16, 32.
@property (nonatomic, assign) NSInteger bitDepth;

/// The resolution setting, typically formatted as "width x height".
/// Example: "1920x1080".
@property (nonatomic, strong) NSString *resolution;

/// The encoding or compression method for lines in the document.
/// Example values: "LF", "CRLF", "CR".
@property (nonatomic, strong) NSString *lineCoding;

#pragma mark - Methods

/// Saves the configuration to a specified file path.
///
/// This method writes the current configuration to an external file,
/// ensuring that all settings are stored persistently.
///
/// @param filePath The file path where the configuration should be saved.
/// @discussion If the file path is invalid or inaccessible, the save operation will fail.
/// @note **Requires write permissions** to the specified directory.
- (void)_saveConfigToFile:(NSString *)filePath;

@end

NS_ASSUME_NONNULL_END

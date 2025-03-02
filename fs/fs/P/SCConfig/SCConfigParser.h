//
//  SCConfigParser.h
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

/// A parser responsible for handling configuration files for ScribbleLabApp.  
/// This class supports parsing XML-based configuration files, saving configurations,  
/// and notifying delegates when configuration changes occur.
///
/// `SCConfigParser` is designed to read, parse, and manage configuration settings  
/// stored in an XML file format. It loads the configuration into an `SCConfig`  
/// object, which provides structured access to various settings such as metadata,  
/// synchronization options, encryption parameters, and document-specific settings.
///
/// This class follows the **NSXMLParserDelegate** protocol for parsing XML files  
/// efficiently and asynchronously. The parser notifies delegates when the configuration  
/// changes, allowing other components to react dynamically.
///

#include <Foundation/Foundation.h>
#import "SCConfig.h"

NS_ASSUME_NONNULL_BEGIN

/// A protocol for handling configuration changes.
NS_AVAILABLE(15, 18)
@protocol SCConfigDelegate <NSObject>
/// Called when the configuration file is modified or updated.
///
/// Use this method to respond to configuration changes, such as  
/// reloading UI elements, updating settings, or refreshing stored values
- (void)configDidChange;
@end

/// A class responsible for parsing and managing configuration files.
///
/// This parser reads an XML-based configuration file, extracts settings,  
/// and initializes an `SCConfig` object. The parser also supports  
/// saving modified configurations back to disk.
///
/// `SCConfigParser` can be used to dynamically load settings at runtime,  
/// ensuring that changes to the configuration file are reflected within  
/// the ScribbleLabApp without requiring a restart.
///
/// ```objc
/// SCConfigParser *parser = [[SCConfigParser alloc] initWithXML:@"config.xml"];
/// if ([parser parse]) {
///     SCConfig *config = parser.config;
///     NSLog(@"Configuration Loaded: %@", config.title);
/// }
/// ```
NS_CLASS_AVAILABLE(15, 18)
@interface SCConfigParser : NSObject <NSXMLParserDelegate>

#pragma mark - Properties
/// The current configuration object parsed from the file.
///
/// This object contains all the settings extracted from the XML file.  
/// Any modifications made to this object **do not** automatically persist  
/// to disk. Call `saveConfigToFile:` to store changes.
///
/// - Note: This property is marked as `atomic` for thread safety.
@property (atomic, strong) SCConfig *config;

/// The delegate responsible for responding to configuration changes.
///
/// If set, this delegate will receive `configDidChange` notifications  
/// when the configuration file is updated.
///
/// - Warning: Ensure the delegate is properly assigned and conforms to  
///   `SCConfigDelegate` to avoid runtime errors.
@property (nonatomic, weak) id<SCConfigDelegate> delegate;

/// The file path to the configuration file.
///
/// This path specifies the location of the XML file that the parser  
/// will read and modify.
///
/// - Important: Ensure the file path is valid and accessible.
@property (nonatomic, strong) NSString *configFilePath;

#pragma mark - Initializer

- (nullable instancetype)init;

/// Initializes the configuration parser with an XML file.
///
/// This method loads the specified configuration file and prepares  
/// the parser for extracting its contents.
///
/// - Parameter filePath: The full path to the XML configuration file.
/// - Returns: An instance of `SCConfigParser` ready for parsing.
- (nullable instancetype)initWithXML:(NSString *)filePath NS_DESIGNATED_INITIALIZER;

#pragma mark - Methods
/// Parses the XML configuration file.
///
/// This method reads the file specified in `configFilePath`,  
/// extracts relevant settings, and populates the `config` object.
///
/// - Returns: `YES` if parsing succeeds, `NO` if an error occurs.
///
/// ### Error Handling
/// If parsing fails, ensure that the file path is correct and  
/// that the XML format follows the expected structure.
- (BOOL)parse;

/// Saves the current configuration to the specified file.
///
/// Any modifications made to the `config` object will be written  
/// to the provided file path.
///
/// - Parameter filePath: The destination file path for saving the configuration.
/// - Note: Ensure that the application has the necessary permissions  
///   to write to the file.
- (void)saveConfigToFile:(NSString *)filePath;

@end

NS_ASSUME_NONNULL_END

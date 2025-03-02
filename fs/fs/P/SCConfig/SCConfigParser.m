//
//  SCConfigParser.m
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

#import "SCConfig.h"
#import "SCConfigParser.h"
#include <Foundation/Foundation.h>

@interface SCConfigParser ()
@property (nonatomic, strong, nullable) NSMutableString *currentElementValue;
@end

@implementation SCConfigParser

- (nullable instancetype)init {
    return [self initWithXML:@""];
}

- (nullable instancetype)initWithXML:(NSString *)filePath {
    self = [super init];
    if (self) {
        _configFilePath = [filePath copy];
        _config = [[SCConfig alloc] init];

        NSData *xmlData = [NSData dataWithContentsOfFile:filePath];
        if (!xmlData) {
            NSLog(@"[SCConfigParser] Error: Unable to read file at path: %@", filePath);
            return nil;
        }

        NSXMLParser *parser = [[NSXMLParser alloc] initWithData:xmlData];
        parser.delegate = self;
        [parser parse];
    }
    return self;
}

- (void)saveConfigToFile:(NSString *)filePath {
    [self.config _saveConfigToFile:filePath];
}

- (BOOL)parse {
    return self.config != nil;
}

- (void)configDidChange {
    if (self.configFilePath) {
        [self saveConfigToFile:self.configFilePath];
    }
}

#pragma mark - NSXMLParserDelegate Methods

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName
  namespaceURI:(nullable NSString *)namespaceURI
  qualifiedName:(nullable NSString *)qName attributes:(NSDictionary<NSString *, NSString *> *)attributeDict {
    
    self.currentElementValue = [[NSMutableString alloc] init];

    if ([elementName isEqualToString:@"scconfig"]) {
        self.config = [[SCConfig alloc] init];
    }
}

- (void)parser:(NSXMLParser *)parser foundCharacters:(nonnull NSString *)string {
    [self.currentElementValue appendString:string];
}

- (void)parser:(NSXMLParser *)parser didEndElement:(nonnull NSString *)elementName
  namespaceURI:(nullable NSString *)namespaceURI qualifiedName:(nullable NSString *)qName {
    
    if (!self.currentElementValue) {
        return;
    }
    
    NSString *trimmedValue = [self.currentElementValue stringByTrimmingCharactersInSet:
                             [NSCharacterSet whitespaceAndNewlineCharacterSet]];

    if ([self.config respondsToSelector:NSSelectorFromString(elementName)]) {
        [self.config setValue:trimmedValue forKey:elementName];
    }

    if ([self.delegate respondsToSelector:@selector(configDidChange)]) {
        [self.delegate configDidChange];
    }
}

@end

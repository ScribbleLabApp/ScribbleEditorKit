cmake_minimum_required(VERSION 3.22)
project(ScribbleEditorKit VERSION 0.0.1 LANGUAGES Swift OBJC C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} -fobjc-arc")

set(CMAKE_OSX_ARCHITECTURES "arm64;arm64e;x86_64")
set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0")
set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "18.0")

set(SDK_DIR ${CMAKE_BINARY_DIR}/SDK)

find_library(FOUNDATION_FRAMEWORK Foundation)

add_subdirectory(fs)

add_library(ScribbleEditorKit SHARED
    Sources/ScribbleEditorKit/ScribbleEditorKit.swift
)

target_link_libraries(ScribbleEditorKit PRIVATE fs)

set_target_properties(ScribbleEditorKit PROPERTIES
    FRAMEWORK TRUE
    OUTPUT_NAME "ScribbleEditorKit"
    XCODE_ATTRIBUTE_PRODUCT_NAME "ScribbleEditorKit"
)

add_custom_command(
    TARGET ScribbleEditorKit POST_BUILD
    COMMAND xcodebuild -create-xcframework
        -library ${CMAKE_BINARY_DIR}/libfs.a -headers ${CMAKE_SOURCE_DIR}/fs/include
        -output ${CMAKE_BINARY_DIR}/fs.xcframework
    COMMENT "Creating fs.xcframework"
)

target_include_directories(ScribbleEditorKit PRIVATE
    ${CMAKE_BINARY_DIR}/fs.xcframework
)

install(TARGETS ScribbleEditorKit
    DESTINATION "${CMAKE_BINARY_DIR}/Frameworks"
)
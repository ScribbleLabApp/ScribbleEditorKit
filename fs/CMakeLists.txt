cmake_minimum_required(VERSION 3.22)
project(fs VERSION 0.0.1 LANGUAGES C OBJC)

add_library(fs STATIC
#    fs/src/*.c
    fs/src/*.m
    fs/*.h
    fs/include/*.h
)

target_include_directories(fs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/fs/include
)

set_target_properties(fs PROPERTIES
    FRAMEWORK TRUE
    OUTPUT_NAME "fs"
    XCODE_ATTRIBUTE_PRODUCT_NAME "fs"
)

set(TBD_FILE "${CMAKE_BINARY_DIR}/fs.tbd")
set(XCFRAMEWORK_PATH "${CMAKE_BINARY_DIR}/fs.xcframework")

set_target_properties(fs PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION "A"
    MACOSX_FRAMEWORK_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/fs/Info.plist"
    MACOSX_FRAMEWORK_IDENTIFIER "com.scribblefoundation.fs"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "fs"
    XCODE_ATTRIBUTE_PRODUCT_NAME "fs"
)

add_custom_command(
    OUTPUT ${TBD_FILE}
    COMMAND xcrun tapi stubify ${CMAKE_BINARY_DIR}/libfs.a -o ${TBD_FILE}
    DEPENDS fs
    COMMENT "Generating .tbd file for fs"
)

add_custom_target(generate_tbd ALL DEPENDS ${TBD_FILE})

add_custom_command(
    OUTPUT ${XCFRAMEWORK_PATH}
    COMMAND xcodebuild -create-xcframework
        -library ${CMAKE_BINARY_DIR}/libfs.a -headers ${CMAKE_CURRENT_SOURCE_DIR}/fs/include
        -output ${XCFRAMEWORK_PATH}
    DEPENDS fs
    COMMENT "Generating .xcframework for fs"
)

add_custom_target(generate_xcframework ALL DEPENDS ${XCFRAMEWORK_PATH})

install(TARGETS fs
    DESTINATION "${CMAKE_BINARY_DIR}/Frameworks"
)
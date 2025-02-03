//
//  _fs.h
//  ScribbleLabApp File System Interface
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

#ifndef _FS_H
#define _FS_H

/**
 * @file _fs.h
 * @brief ScribbleLab File System Interface - Core filesystem operations and utilities
 * 
 * This header provides a comprehensive, cross-platform filesystem interface for the
 * ScribbleLab application. It includes functions for file and directory manipulation,
 * attribute management, permissions handling, and path operations.
 * 
 * Key features:
 * - POSIX-compliant file operations
 * - Comprehensive error handling
 * - Thread-safe implementations
 * - Support for symbolic links
 * - Advanced permission management
 * - Path manipulation utilities
 * 
 * @note All functions are thread-safe unless specifically noted
 * @note Error handling is consistent across all functions
 * @note Memory management is caller's responsibility where noted
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <access.h>

/**
 * @enum fsFileType
 * @brief Comprehensive enumeration representing various file types in the filesystem
 * 
 * This enumeration provides a complete set of file types that can be encountered
 * in the filesystem. Each type is represented by a bit flag, allowing for potential
 * combination of types in future implementations. The types align with standard
 * UNIX/POSIX file type definitions while providing additional flexibility for
 * custom implementations.
 * 
 * @constant FS_FILE_TYPE_UNKNOWN     (0) File type cannot be determined or is invalid
 * @constant FS_FILE_TYPE_REGULAR     (1<<0) Regular file - A normal file that can contain data
 * @constant FS_FILE_TYPE_DIRECTORY   (1<<1) Directory - A container for other files and directories
 * @constant FS_FILE_TYPE_SYMLINK     (1<<2) Symbolic link - A reference to another file or directory
 * @constant FS_FILE_TYPE_FIFO        (1<<3) Named pipe - Inter-process communication channel
 * @constant FS_FILE_TYPE_SOCK        (1<<4) Socket - Endpoint for network communication
 * @constant FS_FILE_TYPE_BLK         (1<<5) Block special file - Buffered device access
 * @constant FS_FILE_TYPE_CHR         (1<<6) Character special file - Unbuffered device access
 * @constant FS_FILE_TYPE_WHT         (1<<7) Whiteout file - Used in layered filesystems
 */
typedef enum {
    FS_FILE_TYPE_UNKNOWN              = 0,
    FS_FILE_TYPE_REGULAR              = (1 << 0),
    FS_FILE_TYPE_DIRECTORY            = (1 << 1),
    FS_FILE_TYPE_SYMLINK              = (1 << 2),
    FS_FILE_TYPE_FIFO                 = (1 << 3),
    FS_FILE_TYPE_SOCK                 = (1 << 4),
    FS_FILE_TYPE_BLK                  = (1 << 5),
    FS_FILE_TYPE_CHR                  = (1 << 6),
    FS_FILE_TYPE_WHT                  = (1 << 7)
} fsFileType;

/**
 * @struct fsFileAttributes
 * @brief Comprehensive structure containing file metadata and attributes
 * 
 * This structure encapsulates essential metadata about a file in the filesystem.
 * It provides information about the file's type, size, and temporal characteristics.
 * The attributes are designed to be efficiently cached and compared for file
 * monitoring and synchronization purposes.
 * 
 * @field type              Type of the file as defined in fsFileType enum
 * @field size             File size in bytes (for regular files)
 * @field creationTime     Unix timestamp of file creation time
 * @field modificationTime Unix timestamp of last content modification
 * 
 * @note For symbolic links, the size field represents the length of the target path
 * @note For directories, the size field might not be meaningful on all systems
 * @note Times are stored as time_t values and may need conversion for display
 */
typedef struct {
    fsFileType type;
    off_t size;
    time_t creationTime;
    time_t modificationTime;
} fsFileAttributes;

bool fsFileExists(const char *path);
fsFileType fsGetFileType(const char *path);

/**
 * @brief Retrieves detailed attributes and metadata for a filesystem entry
 * 
 * Performs a comprehensive analysis of the file or directory, gathering all
 * relevant metadata including file type, size, and timestamps. This function
 * provides detailed information about any filesystem entry while handling
 * special cases and error conditions.
 * 
 * @param path Full path to the filesystem entry. Supports:
 *            - Absolute paths
 *            - Relative paths
 *            - Symbolic links (attributes of target are returned)
 * 
 * @return fsFileAttributes structure containing:
 *         - File type (see fsFileType enum)
 *         - File size in bytes
 *         - Creation timestamp
 *         - Last modification timestamp
 *         Returns zeroed structure if file doesn't exist or is inaccessible
 * 
 * @note Thread-safe implementation
 * @note May require elevated privileges for certain special files
 * @see fsGetFileType() for type-only queries
 * @see fsFileExists() for simple existence checks
 */
fsFileAttributes fsGetFileAttributes(const char *path);

/**
 * @brief Retrieves a list of all files and directories within a specified directory
 * 
 * Performs a detailed directory scan, returning an array of filenames. This function
 * handles various special cases and provides options for filtering and sorting
 * the results.
 * 
 * @param directoryPath Path to the directory to scan. Must be:
 *                     - Accessible with read permissions
 *                     - A valid directory path
 * @param count        Pointer to store the number of entries found
 *                    Will be set to 0 if directory is empty or on error
 * 
 * @return char** Array of strings, each containing a filename
 *         - Array and strings must be freed by caller
 *         - NULL if directory doesn't exist or is inaccessible
 *         - Entries "." and ".." are not included
 *         - Entries are not sorted
 * 
 * @note Memory management:
 *       - Caller must free each string in the array
 *       - Caller must free the array itself
 * @note Thread-safe implementation
 * @see fsGetFileType() to check entry types
 */
char **fsListFilesInDirectory(const char *directoryPath, int *count);


/**
 * @brief Creates a new file with specified initial content
 * 
 * Creates a new file and optionally initializes it with provided data.
 * Handles various edge cases and provides atomic file creation.
 * 
 * @param path Full path where the file should be created
 *            - Must not exist (will not overwrite)
 *            - Parent directory must exist and be writable
 * @param data Initial content to write to the file
 *            - Can be NULL for empty file
 *            - Must be null-terminated if provided
 * 
 * @return true if:
 *         - File was created successfully
 *         - Initial content was written (if provided)
 * @return false if:
 *         - File already exists
 *         - Path is invalid
 *         - Insufficient permissions
 *         - Write error occurred
 * 
 * @note Creates with default permissions (usually 0644)
 * @note Thread-safe implementation
 * @see fsWriteToFile() for writing to existing files
 * @see fsSetFilePermissions() to modify permissions
 */
bool fsCreateFile(const char *path, const char *data);

/**
 * @brief Writes data to an existing file, replacing its current content
 * 
 * Overwrites the entire content of an existing file with new data.
 * Provides atomic writing capabilities and handles various error conditions.
 * 
 * @param path Path to the existing file
 *            - Must be a regular file
 *            - Must be writable
 * @param data New content to write
 *            - Must be null-terminated
 *            - Can be NULL to truncate file
 * 
 * @return true if:
 *         - File was successfully written
 *         - File was truncated (if data was NULL)
 * @return false if:
 *         - File doesn't exist
 *         - Insufficient permissions
 *         - Write error occurred
 *         - File is locked or in use
 * 
 * @note Atomic operation (uses temporary file)
 * @note Preserves original file permissions
 * @see fsAppendToFile() for adding content
 * @see fsCreateFile() for new files
 */
bool fsWriteToFile(const char *path, const char *data);

/**
 * @brief Appends data to the end of an existing file
 * 
 * Adds new content to the end of a file while preserving existing content.
 * Provides atomic append operations and handles concurrent access.
 * 
 * @param path Path to the existing file
 *            - Must be a regular file
 *            - Must be writable
 * @param data Content to append
 *            - Must be null-terminated
 *            - Cannot be NULL
 * 
 * @return true if:
 *         - Data was successfully appended
 * @return false if:
 *         - File doesn't exist
 *         - Insufficient permissions
 *         - Write error occurred
 *         - File is locked or in use
 * 
 * @note Thread-safe implementation
 * @note Atomic operation
 * @see fsWriteToFile() for overwriting content
 */
bool fsAppendToFile(const char *path, const char *data);

/**
 * @brief Reads the entire content of a file into memory
 * 
 * Performs a complete file read operation, handling various file sizes
 * and potential error conditions. Manages memory allocation for the caller.
 * 
 * @param path Path to the file to read
 *            - Must be a regular file
 *            - Must be readable
 * 
 * @return char* Null-terminated string containing file content
 *         - Caller must free the returned string
 *         - NULL if file doesn't exist or is unreadable
 *         - NULL if memory allocation fails
 * 
 * @note Memory management:
 *       - Returned string must be freed by caller
 *       - Large files may require significant memory
 * @note Thread-safe implementation
 * @see fsFileExists() to check file before reading
 */
char *fsReadFile(const char *path);

/**
 * @brief Safely deletes a file from the filesystem
 * 
 * Removes a file with proper error checking and permission validation.
 * Handles special cases and provides secure deletion options.
 * 
 * @param path Path to the file to delete
 *            - Must be a regular file or symbolic link
 *            - Parent directory must be writable
 * 
 * @return true if:
 *         - File was successfully deleted
 *         - File didn't exist
 * @return false if:
 *         - Insufficient permissions
 *         - Path is a directory
 *         - File is locked or in use
 * 
 * @note Does not follow symbolic links
 * @note Thread-safe implementation
 * @see fsDeleteDirectory() for removing directories
 */
bool fsDeleteFile(const char *path);

/**
 * @brief Moves or renames a file to a new location
 * 
 * Performs an atomic move operation, handling cross-device moves
 * and various edge cases. Preserves file attributes and handles
 * existing target files.
 * 
 * @param fromPath Source file path
 *                 - Must exist and be accessible
 * @param toPath   Destination file path
 *                 - Parent directory must exist and be writable
 *                 - Will overwrite existing file
 * 
 * @return true if:
 *         - File was successfully moved
 * @return false if:
 *         - Source doesn't exist
 *         - Insufficient permissions
 *         - Device crossing error
 *         - Path is invalid
 * 
 * @note Atomic operation when possible
 * @note Preserves file attributes
 * @see fsCopyFile() for copying instead of moving
 */
bool fsMoveFile(const char *fromPath, const char *toPath);

/**
 * @brief Creates a copy of a file at a new location
 * 
 * Performs a file copy operation while preserving attributes
 * and handling various special cases and error conditions.
 * 
 * @param fromPath Source file path
 *                 - Must exist and be readable
 * @param toPath   Destination file path
 *                 - Parent directory must exist and be writable
 *                 - Will overwrite existing file
 * 
 * @return true if:
 *         - File was successfully copied
 * @return false if:
 *         - Source doesn't exist
 *         - Insufficient permissions
 *         - Disk space error
 *         - I/O error
 * 
 * @note Preserves file attributes
 * @note May require significant time for large files
 * @see fsMoveFile() for moving instead of copying
 */
bool fsCopyFile(const char *fromPath, const char *toPath);


bool fsCreateDirectory(const char *path);
bool fsDeleteDirectory(const char *path);
bool fsMoveDirectory(const char *fromPath, const char *toPath);
bool fsCopyDirectory(const char *fromPath, const char *toPath);


/**
 * @brief Creates a symbolic link to a target file or directory
 * 
 * Creates a new symbolic link pointing to the specified target.
 * Handles relative and absolute paths, and various error conditions.
 * 
 * @param target   Path to the target file/directory
 *                 - Can be relative or absolute
 *                 - Target doesn't need to exist
 * @param linkPath Path where the symbolic link should be created
 *                 - Parent directory must exist and be writable
 * 
 * @return true if:
 *         - Symbolic link was created successfully
 * @return false if:
 *         - Insufficient permissions
 *         - Link path already exists
 *         - System doesn't support symbolic links
 * 
 * @note Target existence is not verified
 * @note Thread-safe implementation
 * @see fsReadSymbolicLink() to read link target
 */
bool fsCreateSymbolicLink(const char *target, const char *linkPath);

/**
 * @brief Reads the target path of a symbolic link
 * 
 * Retrieves the target path that a symbolic link points to.
 * Handles various error conditions and path types.
 * 
 * @param linkPath Path to the symbolic link
 *                 - Must be a symbolic link
 * @param buffer   Buffer to store the target path
 *                 - Must be pre-allocated
 * @param size     Size of the provided buffer
 *                 - Must be large enough for path
 * 
 * @return true if:
 *         - Target path was successfully read
 * @return false if:
 *         - Path is not a symbolic link
 *         - Buffer is too small
 *         - Link is broken
 * 
 * @note Does not verify target existence
 * @note Thread-safe implementation
 * @see fsCreateSymbolicLink() to create links
 */
bool fsReadSymbolicLink(const char *linkPath, char *buffer, size_t size);

/**
 * @brief Sets permissions for a file or directory
 * 
 * Modifies the permission bits of a filesystem entry.
 * Handles special cases and permission inheritance.
 * 
 * @param path Path to the file or directory
 *            - Must exist and be accessible
 * @param mode Permission mode (chmod style)
 *            - Standard UNIX permission bits
 *            - Special bits (setuid, setgid, sticky)
 * 
 * @return true if:
 *         - Permissions were successfully changed
 * @return false if:
 *         - Insufficient privileges
 *         - Path doesn't exist
 *         - Invalid mode specified
 * 
 * @note May require elevated privileges
 * @note Thread-safe implementation
 * @see fsGetFilePermissions() to query permissions
 */
bool fsSetFilePermissions(const char *path, mode_t mode);

/**
 * @brief Retrieves current permissions of a file or directory
 * 
 * Gets the current permission bits of a filesystem entry.
 * Handles special cases and permission inheritance.
 * 
 * @param path Path to the file or directory
 *            - Must exist and be accessible
 * @param mode Pointer to store the permission mode
 *            - Will contain standard UNIX permission bits
 * 
 * @return true if:
 *         - Permissions were successfully retrieved
 * @return false if:
 *         - Path doesn't exist
 *         - Insufficient privileges
 *         - Invalid pointer provided
 * 
 * @note Thread-safe implementation
 * @see fsSetFilePermissions() to modify permissions
 */
bool fsGetFilePermissions(const char *path, mode_t *mode);


bool fsSetUserPermissions(const char *path, fs_access_mode_t perms);
bool fsSetGroupPermissions(const char *path, fs_access_mode_t perms);
bool fsSetOthersPermissions(const char *path, fs_access_mode_t perms);

bool fsAddFileAccessControlEntry(const char *path, fs_ace_t *ace);
bool fsRemoveFileAccessControlEntry(const char *path, fs_ace_t *ace);
fs_ace_t *fsGetFileAccessControlEntry(const char *path, size_t *count);

/**
 * @brief Extracts the file extension from a path
 * 
 * Analyzes a path string to identify and extract the file extension.
 * Handles various special cases and path formats.
 * 
 * @param path Path to analyze
 *            - Can be just filename or full path
 *            - Must be null-terminated
 * 
 * @return char* Allocated string containing the extension
 *         - Includes the dot (e.g., ".txt")
 *         - NULL if no extension exists
 *         - NULL if path is invalid
 * 
 * @note Memory management:
 *       - Returned string must be freed by caller
 * @note Thread-safe implementation
 * @see fsGetFileName() for full filename
 */
char *fsGetFileExtension(const char *path);

/**
 * @brief Extracts the filename from a path
 * 
 * Parses a path string to extract just the filename component.
 * Handles various path formats and special cases.
 * 
 * @param path Path to analyze
 *            - Can be absolute or relative path
 *            - Must be null-terminated
 * 
 * @return char* Allocated string containing the filename
 *         - Includes the extension
 *         - NULL if path is invalid
 * 
 * @note Memory management:
 *       - Returned string must be freed by caller
 * @note Thread-safe implementation
 * @see fsGetFileExtension() for just the extension
 */
char *fsGetFileName(const char *path);
char *fsGetFileNameWithoutExtension(const char *path);
char *fsGetFileParentDirectory(const char *path);


/**
 * @brief Checks if a file or directory is readable
 * 
 * Verifies read permissions for the current process.
 * Handles various special cases and permission types.
 * 
 * @param path Path to check
 *            - Must exist
 *            - Can be any file type
 * 
 * @return true if:
 *         - File exists and is readable
 * @return false if:
 *         - File doesn't exist
 *         - No read permission
 *         - Path is invalid
 * 
 * @note Checks effective permissions
 * @note Thread-safe implementation
 * @see fsIsWritable() for write permission check
 */
bool fsIsReadable(const char *path);
bool fsIsWritable(const char *path);
bool fsIsExecutable(const char *path);

#endif // _FS_H

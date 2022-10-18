#ifndef _MEMFS_FS_H_
#define _MEMFS_FS_H_

#include <sys/types.h>
#include <sys/stat.h>

/* Initalize FS
 * Create root directory */
int memfs_io_init(void);

/* Create a directory
 * Permission: umask Octal Value (0000)
 */
int memfs_io_create_directory(const char *path, mode_t perms);

/* Remove a directory */
int memfs_io_remove_directory(const char *path);

/* Create a file
 * Permission: umask Octal Value (0000)
 */
int memfs_io_create_file(const char *path, mode_t perms);

/* Remove a file */
int memfs_io_remove_file(const char *path);

/* Get a list of file/dir names in a directory */
char **memfs_io_ls(const char *path);

/* Get Node type
 * 0 -> Directory
 * 1 -> File
 * -1 -> ERROR
 */
int memfs_io_get_node_type(const char *path);

/* Get node attributes */
struct stat *memfs_io_getattr(const char *path);

/* Read File Content */
char *memfs_io_read(const char *path);

/* Write to file */
int memfs_io_write(const char *path, const char *buffer, size_t size);

#endif

#ifndef _MEMFS_FS_INTERNAL_H_
#define _MEMFS_FS_INTERNAL_H_

#include "inode.h"
#include "directory.h"
#include "file.h"

/* Get a specific directory/subdirectory
 * If parent was null, it will traverse from root
 */
struct memfs_directory *
memfs_get_directory_path(const char *path, struct memfs_directory *parent);

/* Get parent directory of path (/parent1/file1) by parsing it */
struct memfs_directory *memfs_get_parent_directory_path(const char *path);

/* Get a file by path */
struct memfs_file *memfs_get_file_path(const char *path);

#endif

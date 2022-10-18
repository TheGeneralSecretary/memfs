#ifndef _MEMFS_FS_FILE_H_
#define _MEMFS_FS_FILE_H_

#include "directory.h"

#include <sys/stat.h>

/* FS File Structure */
struct memfs_file {
		char *name;
		struct memfs_directory *parent;
		struct stat *st;
		char *content;
};

/* Create a new file */
struct memfs_file *memfs_create_file(struct memfs_directory *parent,
									 const char *filename, mode_t mode);

/* Get a file from a directory */
struct memfs_file *memfs_get_file(struct memfs_directory *parent,
								  const char *filename);

/* Check if a file exists within another directory */
int memfs_has_file(struct memfs_directory *parent, const char *filename);

/* Remove a file from a directory */
int memfs_remove_file(struct memfs_directory *parent, const char *filename);

#endif

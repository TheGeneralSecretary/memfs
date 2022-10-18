#ifndef _MEMFS_FS_DIRECTORY_H_
#define _MEMFS_FS_DIRECTORY_H_

#include "inode.h"

#include <sys/stat.h>
#include <sys/types.h>

/* FS Directory Structure */
struct memfs_directory {
		char *name;
		struct memfs_directory *parent;
		struct stat *st;
		struct memfs_inode **nodes;
};

/* Initialize or Get root directory */
struct memfs_directory *memfs_get_root(void);

/* Get number of nodes within a directory */
size_t memfs_node_count(struct memfs_directory *dir);

/* Create a new directory */
struct memfs_directory *memfs_create_directory(struct memfs_directory *parent,
											   const char *dirname,
											   mode_t mode);

/* get a sub directory from directory */
struct memfs_directory *memfs_get_directory(struct memfs_directory *parent,
											const char *dirname);

/* Check if a directory exists within another directory */
int memfs_has_directory(struct memfs_directory *parent, const char *dirname);

/* Recursively remove a directory */
int memfs_remove_directory(struct memfs_directory *parent, const char *dirname);

/* Recursively remove sub directories */
int memfs_remove_subnodes(struct memfs_directory *parent);

#endif

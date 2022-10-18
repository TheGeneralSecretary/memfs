#ifndef _MEMFS_FS_IO_UTILS_H_
#define _MEMFS_FS_IO_UTILS_H_

#include "file.h"

int memfs_trunc(struct memfs_file *file, const char *content);
int memfs_append(struct memfs_file *file, const char *content);

#endif

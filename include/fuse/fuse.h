#ifndef _MEMFS_FUSE_H_
#define _MEMFS_FUSE_H_

#include <sys/types.h>
#include <sys/stat.h>

/* Initalize fuse */
int memfs_fuse_init(int argc, char **argv);

#endif

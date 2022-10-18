#ifndef _MEMFSE_FUSE_OPS_H_
#define _MEMFSE_FUSE_OPS_H_

#define FUSE_USE_VERSION 30
#include <fuse3/fuse.h>

#include <sys/stat.h>
#include <time.h>

int memfs_fuse_getattr(const char *path, struct stat *st,
					   struct fuse_file_info *fi);
int memfs_fuse_readdir(const char *path, void *buffer, fuse_fill_dir_t filler,
					   off_t offset, struct fuse_file_info *fi,
					   enum fuse_readdir_flags flag);
int memfs_fuse_mkdir(const char *path, mode_t mode);
int memfs_fuse_mknod(const char *path, mode_t mode, dev_t rdev);
int memfs_fuse_utimensat(const char *path, const struct timespec tv[2],
						 struct fuse_file_info *fi);
int memfs_fuse_read(const char *path, char *buffer, size_t size, off_t offset,
					struct fuse_file_info *fi);
int memfs_fuse_write(const char *path, const char *buffer, size_t size,
					 off_t offset, struct fuse_file_info *fi);

#endif

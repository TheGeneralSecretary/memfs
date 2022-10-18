#include "fuse_ops.h"
#include "fs/fs.h"

#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <utime.h>

int memfs_fuse_getattr(const char *path, struct stat *st,
					   struct fuse_file_info *fi)
{
		/* 0 = DIR, 1 = FILE, -1 = ERR */
		struct stat *attr = memfs_io_getattr(path);
		if (!attr)
				return -ENOENT;

		memcpy(st, attr, sizeof(struct stat));
		return 0;
}

int memfs_fuse_readdir(const char *path, void *buffer, fuse_fill_dir_t filler,
					   off_t offset, struct fuse_file_info *fi,
					   enum fuse_readdir_flags flag)
{
		filler(buffer, ".", NULL, 0, 0);
		filler(buffer, "..", NULL, 0, 0);

		char **nodenames = memfs_io_ls(path);
		if (!nodenames)
				return 1;

		for (size_t i = 0; nodenames[i] != NULL; i++) {
				if (nodenames[i][0] == '/')
						nodenames[i]++;

				filler(buffer, nodenames[i], NULL, 0, 0);
		}

		return 0;
}

int memfs_fuse_mkdir(const char *path, mode_t mode)
{
		// fuse_ops->mkdir returns 1 on success which is considered "ERROR" by libfuse
		return !(memfs_io_create_directory(path, mode));
}

int memfs_fuse_mknod(const char *path, mode_t mode, dev_t rdev)
{
		return !(memfs_io_create_file(path, mode ^ __S_IFREG));
}

int memfs_fuse_utimensat(const char *path, const struct timespec tv[2],
						 struct fuse_file_info *fi)
{
		struct stat *attr = memfs_io_getattr(path);
		if (!attr)
				return 1;

		attr->st_atime = time(NULL);
		attr->st_mtime = time(NULL);
		return 0;
}

int memfs_fuse_read(const char *path, char *buffer, size_t size, off_t offset,
					struct fuse_file_info *fi)
{
		char *content = memfs_io_read(path);
		if (!content)
				return 1;

		size_t len = strlen(content) + 1;
		if (size > len)
				size = len;

		memcpy(buffer, content, size);
		free(content);
		return size;
}

int memfs_fuse_write(const char *path, const char *buffer, size_t size,
					 off_t offset, struct fuse_file_info *fi)
{
		return memfs_io_write(path, buffer, size);
}

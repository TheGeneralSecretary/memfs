#include "fuse/fuse.h"
#include "fuse_ops.h"

struct fuse_operations fops = { .getattr = memfs_fuse_getattr,
								.readdir = memfs_fuse_readdir,
								.mkdir = memfs_fuse_mkdir,
								.mknod = memfs_fuse_mknod,
								.utimens = memfs_fuse_utimensat,
								.read = memfs_fuse_read,
								.write = memfs_fuse_write };

int memfs_fuse_init(int argc, char **argv)
{
		return fuse_main(argc, argv, &fops, NULL);
}

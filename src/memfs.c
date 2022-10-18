#include "fs/fs.h"
#include "fuse/fuse.h"

#include <clog/clog.h>

int main(int argc, char **argv)
{
		if (!memfs_io_init())
				CLOG_FATAL("FAILED TO INITALIZE MEMFS");

		return memfs_fuse_init(argc, argv);
}

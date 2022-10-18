#ifndef _MEMFS_INODE_H_
#define _MEMFS_INODE_H_

typedef enum { I_NONE = -1, I_DIR = 1, I_FILE = 2 } memfs_inode_t;

/* FS INode Structure */
struct memfs_inode {
		memfs_inode_t type;
		void *node;
};

#endif

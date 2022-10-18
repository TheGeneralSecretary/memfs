#include "directory.h"
#include "file.h"

#include <time.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

struct memfs_directory *memfs_get_root(void)
{
		static struct memfs_directory *root = NULL;
		if (root)
				return root;

		root = malloc(sizeof(struct memfs_directory));
		if (!root)
				return NULL;

		root->name = strdup("/");
		root->parent = NULL;

		root->st = malloc(sizeof(struct stat));
		if (!root->st)
				return NULL;
		memset(root->st, 0, sizeof(struct stat));

		root->st->st_mode = __S_IFDIR | 0755;
		root->st->st_nlink = 2;
		root->st->st_uid = getuid();
		root->st->st_gid = getgid();
		root->st->st_atime = time(NULL);
		root->st->st_mtime = time(NULL);
		root->st->st_ctime = time(NULL);
		root->st->st_size = 4096;

		root->nodes = malloc(sizeof(struct memfs_inode *));
		if (!root->nodes)
				return NULL;

		root->nodes[0] = NULL;

		return root;
}

size_t memfs_node_count(struct memfs_directory *dir)
{
		size_t c = 0;
		if (dir && dir->nodes)
				for (c = 0; dir->nodes[c] != NULL; c++)
						;
		return c;
}

struct memfs_directory *memfs_create_directory(struct memfs_directory *parent,
											   const char *dirname, mode_t mode)
{
		size_t null_idx;
		size_t node_count;
		void *nodes;
		struct memfs_directory *dir = NULL;

		if (!parent)
				return NULL;

		node_count = memfs_node_count(parent) + 1; // Always +1 for NULL

		nodes = realloc(parent->nodes,
						(node_count + 1) * sizeof(struct node *));
		if (!nodes)
				return NULL;

		parent->nodes = nodes;
		null_idx = 0;

		/* Get the Empty(NULL) space */
		for (null_idx = 0; parent->nodes[null_idx] != NULL; null_idx++)
				;

		if (!parent->nodes[null_idx]) {
				dir = malloc(sizeof(struct memfs_directory));
				if (!dir)
						return NULL;

				dir->name = strdup(dirname);
				dir->parent = parent;

				dir->st = malloc(sizeof(struct stat));
				if (!dir->st)
						return NULL;
				memset(dir->st, 0, sizeof(struct stat));

				dir->st->st_mode = mode;
				dir->st->st_nlink = 2;
				dir->st->st_uid = getuid();
				dir->st->st_gid = getgid();
				dir->st->st_atime = time(NULL);
				dir->st->st_mtime = time(NULL);
				dir->st->st_ctime = time(NULL);
				dir->st->st_size = 4096;

				dir->nodes = malloc(sizeof(struct memfs_inode *));
				dir->nodes[0] = NULL;

				parent->nodes[null_idx] = malloc(sizeof(struct memfs_inode));
				parent->nodes[null_idx]->type = I_DIR;
				parent->nodes[null_idx]->node = dir;
				parent->nodes[++null_idx] = NULL;
				parent->st->st_nlink++;
		}

		return dir;
}

struct memfs_directory *memfs_get_directory(struct memfs_directory *parent,
											const char *dirname)
{
		if (parent && parent->nodes)
				for (size_t i = 0; parent->nodes[i] != NULL; i++)
						if (parent->nodes[i]->type == I_DIR &&
							!strcmp(((struct memfs_directory *)parent->nodes[i]
											 ->node)
											->name,
									dirname))
								return parent->nodes[i]->node;
		return NULL;
}

int memfs_has_directory(struct memfs_directory *parent, const char *dirname)
{
		return memfs_get_directory(parent, dirname) != NULL;
}

int memfs_remove_directory(struct memfs_directory *parent, const char *dirname)
{
		size_t idx;
		size_t node_count;

		if (!parent || !parent->nodes)
				return 0;

		if (!memfs_has_directory(parent, dirname))
				return 0;

		for (idx = 0; parent->nodes[idx] != NULL; idx++)
				if (parent->nodes[idx]->type == I_DIR &&
					!strcmp(((struct memfs_directory *)parent->nodes[idx]->node)
									->name,
							dirname))
						break;

		if (!parent->nodes[idx] || !parent->nodes[idx]->node)
				return 0;

		node_count = memfs_node_count(parent) + 1;

		struct memfs_directory *dir = parent->nodes[idx]->node;
		memfs_remove_subnodes(dir);

		/* Free Resources */
		free(parent->nodes[idx]);
		parent->nodes[idx] = NULL;

		struct memfs_inode **shift =
				malloc(node_count * sizeof(struct memfs_inode *));

		size_t j = 0;
		for (size_t i = 0; i < node_count; i++, j++) {
				if (i == idx)
						i++;
				shift[j] = parent->nodes[i];
		}

		shift[j] = NULL;
		free(parent->nodes);
		parent->nodes = shift;
		parent->st->st_nlink--;

		return 1;
}

int memfs_remove_subnodes(struct memfs_directory *parent)
{
		if (!parent)
				return 0;

		size_t node_count = memfs_node_count(parent);

		for (size_t i = 0; i < node_count; i++) {
				switch (parent->nodes[i]->type) {
				case I_DIR:
						memfs_remove_subnodes(parent->nodes[i]->node);
						break;
				case I_FILE: {
						struct memfs_file *fnode = NULL;
						fnode = parent->nodes[i]->node;
						free(fnode->name);
						free(fnode->content);
						free(fnode->st);
						free(fnode);
						fnode = NULL;
						break;
				}
				default:
						break;
				}

				free(parent->nodes[i]);
		}

		free(parent->name);
		free(parent->st);
		free(parent->nodes);
		free(parent);

		return 1;
}

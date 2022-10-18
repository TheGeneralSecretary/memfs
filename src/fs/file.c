#include "file.h"

#include <malloc.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

struct memfs_file *memfs_create_file(struct memfs_directory *parent,
									 const char *filename, mode_t mode)
{
		size_t null_idx;
		size_t node_count;
		void *nodes;
		struct memfs_file *file = NULL;

		if (!parent)
				return NULL;

		node_count = memfs_node_count(parent) + 1;

		nodes = realloc(parent->nodes,
						(node_count + 1) * sizeof(struct memfs_inode *));
		if (!nodes)
				return NULL;

		parent->nodes = nodes;
		null_idx = 0;

		/* Get the Empty(NULL) space */
		for (null_idx = 0; parent->nodes[null_idx] != NULL; null_idx++)
				;

		if (!parent->nodes[null_idx]) {
				file = malloc(sizeof(struct memfs_file));
				if (!file)
						return NULL;

				file->name = strdup(filename);
				file->parent = parent;
				file->content = strdup("");

				file->st = malloc(sizeof(struct stat));
				if (!file->st)
						return NULL;
				memset(file->st, 0, sizeof(struct stat));

				file->st->st_mode = mode;
				file->st->st_nlink = 1;
				file->st->st_uid = getuid();
				file->st->st_gid = getgid();
				file->st->st_atime = time(NULL);
				file->st->st_mtime = time(NULL);
				file->st->st_ctime = time(NULL);
				file->st->st_size = strlen(file->content);

				parent->nodes[null_idx] = malloc(sizeof(struct memfs_inode));
				parent->nodes[null_idx]->type = I_FILE;
				parent->nodes[null_idx]->node = file;
				parent->nodes[++null_idx] = NULL;
				parent->st->st_nlink++;
		}

		return file;
}

struct memfs_file *memfs_get_file(struct memfs_directory *parent,
								  const char *filename)
{
		if (parent && parent->nodes)
				for (size_t i = 0; parent->nodes[i] != NULL; i++)
						if (parent->nodes[i]->type == I_FILE &&
							!strcmp(((struct memfs_file *)parent->nodes[i]->node)
											->name,
									filename))
								return parent->nodes[i]->node;
		return NULL;
}

int memfs_has_file(struct memfs_directory *parent, const char *filename)
{
		return memfs_get_file(parent, filename) != NULL;
}

int memfs_remove_file(struct memfs_directory *parent, const char *filename)
{
		size_t idx;
		size_t node_count;

		if (!parent || !parent->nodes)
				return 0;

		if (!memfs_has_file(parent, filename))
				return 0;

		for (idx = 0; parent->nodes[idx] != NULL; idx++)
				if (parent->nodes[idx]->type == I_FILE &&
					!strcmp(((struct memfs_file *)parent->nodes[idx]->node)
									->name,
							filename))
						break;

		if (!parent->nodes[idx] || !parent->nodes[idx]->node)
				return 0;

		node_count = memfs_node_count(parent) + 1;

		/* Free Resources */
		struct memfs_file *file = parent->nodes[idx]->node;
		free(file->name);
		free(file->content);
		free(file->st);
		file->parent = NULL;
		free(file);
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

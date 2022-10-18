#include "fs/fs.h"
#include "directory.h"
#include "fs_internal.h"

#include <string.h>
#include <malloc.h>

#include <stdio.h>

int memfs_io_init(void)
{
		return memfs_get_root() != NULL;
}

int memfs_io_create_directory(const char *path, mode_t perms)
{
		if (path[0] != '/')
				return 0;

		char *dirname = strrchr(path, '/');
		struct memfs_directory *parent = memfs_get_parent_directory_path(path);
		return (parent &&
				(!memfs_has_directory(parent, dirname) &&
				 (memfs_create_directory(parent, dirname, __S_IFDIR | perms) !=
				  NULL)));
}

int memfs_io_remove_directory(const char *path)
{
		if (path[0] != '/')
				return 0;

		char *dirname = strrchr(path, '/');
		struct memfs_directory *parent = memfs_get_parent_directory_path(path);
		return (parent && (memfs_has_directory(parent, dirname) &&
						   (memfs_remove_directory(parent, dirname))));
}

int memfs_io_create_file(const char *path, mode_t perms)
{
		if (path[0] != '/')
				return 0;

		char *filename = strrchr(path, '/');
		struct memfs_directory *parent = memfs_get_parent_directory_path(path);
		return (parent && (!memfs_has_file(parent, filename) &&
						   (memfs_create_file(parent, filename,
											  __S_IFREG | perms) != NULL)));
}

int memfs_io_remove_file(const char *path)
{
		if (path[0] != '/')
				return 0;

		char *filename = strrchr(path, '/');
		struct memfs_directory *parent = memfs_get_parent_directory_path(path);
		return (parent && (memfs_has_file(parent, filename) &&
						   (memfs_remove_file(parent, filename))));
}

char **memfs_io_ls(const char *path)
{
		if (path[0] != '/')
				return NULL;

		struct memfs_directory *dir = memfs_get_directory_path(path, NULL);
		if (!dir)
				return NULL;

		size_t node_count = memfs_node_count(dir);

		char **nodenames = malloc((node_count + 1) * sizeof(char *));
		if (!nodenames)
				return NULL;

		size_t j = 0;
		for (size_t i = 0; dir->nodes[i] != NULL; i++) {
				switch (dir->nodes[i]->type) {
				case I_DIR: {
						nodenames[j++] = strdup(
								((struct memfs_directory *)dir->nodes[i]->node)
										->name);
						break;
				}
				case I_FILE:
						nodenames[j++] = strdup(
								((struct memfs_file *)dir->nodes[i]->node)
										->name);
						break;
				default:
						break;
				}
		}

		nodenames[j] = NULL;
		return nodenames;
}

int memfs_io_get_node_type(const char *path)
{
		if (path[0] != '/')
				return -1;

		if (!strcmp(path, "/"))
				return I_DIR;

		char *nodename = strrchr(path, '/');
		struct memfs_directory *parent = memfs_get_parent_directory_path(path);

		if (memfs_has_directory(parent, nodename))
				return I_DIR;
		else if (memfs_has_file(parent, nodename))
				return I_FILE;

		return I_NONE;
}

struct stat *memfs_io_getattr(const char *path)
{
		if (path[0] != '/')
				return NULL;

		memfs_inode_t node_type = memfs_io_get_node_type(path);
		if (node_type == I_DIR) {
				struct memfs_directory *dir =
						memfs_get_directory_path(path, NULL);
				if (dir)
						return dir->st;
		} else if (node_type == I_FILE) {
				struct memfs_file *file = memfs_get_file_path(path);
				if (file)
						return file->st;
		}

		return NULL;
}

char *memfs_io_read(const char *path)
{
		if (path[0] != '/')
				return NULL;

		struct memfs_file *file = memfs_get_file_path(path);
		if (!file || !file->content)
				return NULL;

		return strdup(file->content);
}

int memfs_io_write(const char *path, const char *buffer, size_t size)
{
		struct memfs_file *file = memfs_get_file_path(path);
		if (!file)
				return -1;

		if (file->content)
				free(file->content);

		file->content = strndup(buffer, size);
		if (!file->content)
				return -1;

		file->st->st_size = size;
		return size;
}

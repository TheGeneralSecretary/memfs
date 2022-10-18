#include "fs_internal.h"
#include "path_parser.h"

#include <malloc.h>
#include <string.h>

struct memfs_directory *memfs_get_directory_path(const char *path,
												 struct memfs_directory *parent)
{
		struct memfs_directory *curr_dir = parent;
		if (!curr_dir)
				curr_dir = memfs_get_root();

		char **dirs = parse_path(path);

		for (size_t i = 0; dirs[i] != NULL; i++) {
				curr_dir = memfs_get_directory(curr_dir, dirs[i]);
				if (!curr_dir)
						break;
		}

		for (int i = 0; dirs[i] != NULL; i++)
				free(dirs[i]);
		free(dirs);

		return curr_dir;
}

struct memfs_directory *memfs_get_parent_directory_path(const char *path)
{
		char *nodename = strrchr(path, '/');
		char *parent = strdup(path);
		parent[nodename - path] = '\0';
		struct memfs_directory *dir = memfs_get_directory_path(parent, NULL);
		free(parent);
		return dir;
}

struct memfs_file *memfs_get_file_path(const char *path)
{
		char *filename = strrchr(path, '/');
		struct memfs_directory *parent = memfs_get_parent_directory_path(path);
		if (!parent)
				return NULL;

		return memfs_get_file(parent, filename);
}

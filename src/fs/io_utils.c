#include "io_utils.h"

#include <malloc.h>
#include <string.h>
#include <stdio.h>

int memfs_trunc(struct memfs_file *file, const char *content)
{
		if (!file)
				return 0;

		free(file->content);
		file->content = strdup(content);
		return file->content != NULL;
}

int memfs_append(struct memfs_file *file, const char *content)
{
		size_t content_len = strlen(content);
		size_t file_content_len = strlen(file->content);
		size_t len = content_len + file_content_len + 1;
		char *tmp = realloc(file->content, len);
		if (!tmp)
				return 0;

		file->content = tmp;
		int written =
				snprintf(file->content + file_content_len, len, "%s", content);
		return (written >= 0 && written < len);
}

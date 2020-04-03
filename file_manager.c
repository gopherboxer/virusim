#include "file_manager.h"

int create_file(const char * file_path)
{
  FILE * fd = fopen(file_path, "w");

  if (fd == NULL) return 1;

  fclose(fd);
  return 0;
}

int delete_file(const char * path)
{
  return remove(path);
}

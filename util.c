#include "util.h"

char ** split(const char * s, const char d)//, unsigned int * tab_size)
{
  unsigned int tab_size = 0;

  //*tab_size = 0;

  for (int i = 0; i < strlen(s); i++)
  {
    if (s[i] == d) tab_size++;//*tab_size += 1;
  }
  //*tab_size++;
  tab_size++;

  //char ** result = (char **) malloc((tab_size + 1) * sizeof(char *));
  char ** result = (char **) malloc(tab_size * sizeof(char *));

  for (unsigned int i = 0; i < tab_size; i++)
  {
    result[i] = (char *) malloc(20 * sizeof(char));
  }

  int index = 0;

  for (unsigned int i = 0; i < strlen(s); i++)
  {
    if (s[i] == d) index++;
    else strncat(result[index], &s[i], 1);
  }
  return result;
}

char * get_file_content(const char * file_name)
{
  FILE * file_descriptor = fopen(file_name, "r");

  if (!file_descriptor) return "Error: Could not open file.\n";

  fseek (file_descriptor, 0, SEEK_END);
  long length = ftell (file_descriptor);
  fseek (file_descriptor, 0, SEEK_SET);
  char * file_content_buffer = malloc(length);
  //*file_length = length;

  if (file_content_buffer) fread(file_content_buffer, 1, length, file_descriptor);//fread (file_content_buffer, 1, *file_length, file_descriptor);//(file_content_buffer, 1, length, file_descriptor);

  fclose (file_descriptor);
  return file_content_buffer;
}

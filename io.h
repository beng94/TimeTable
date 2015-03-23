#include <stdlib.h>
#include <glib.h>

void read_in (GList **sub_list, const char* file_name);
char** get_tokens (const char* line, const char* separator, int* count);

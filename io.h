#include <stdlib.h>
#include <glib.h>

void read_in (GList **sub_list, const char* file_name);
char** get_tokens (const char* line, const char* separator, int* count);
int print_subject (GList* sub_list, int id);
void print_sub_list(GList* sub_list);
void print_sub_id (GList* sub_list, int sub_id, int les_id);

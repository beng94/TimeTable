#include <stdio.h>
#include "io.h"
#include "data.h"

static void change_commas (char* str, size_t len)
{
    int i = 0;
    while(i < len)
    {
    if(str[i] == '\"')
        {
            i++;
            break;
        }
        i++;
    }

    while(i < len)
    {
        if(str[i] == '\"') break;
        if(str[i] == ';') str[i] = '$';
        i++;
    }
}

static FILE* init_io (const char* file_name)
{
    FILE* f = fopen(file_name, "r");

    if(!f)
    {
        fprintf(stderr, "Cannot open input file");
        exit(EXIT_FAILURE);
    }

    return f;
}

//TODO: It's unsafe
static char* read_line (char* line, size_t size, FILE* f)
{
    return (char*)fgets(line, size, f);
}

char** get_tokens (const char* line, const char* separator, int* count)
{
    (*count) = 1;
    char** result = (char**)malloc(sizeof(char*));

    char* str = strdup(line);
    char* token = strtok(str, separator);
    result[0] = strdup(token);

    while((token = strtok(NULL, separator)) != NULL)
    {
        result = realloc(result, sizeof(*result) * ((*count) + 1));
        result[(*count)] = strdup(token);
        (*count)++;
    }

    free(str);
    return result;
}

static void parse_line (GList **sub_list, const char* line)
{
    int count = 0;
    char** tokens = get_tokens(line, ";", &count);

    add_subject(sub_list, tokens);
}

void read_in (GList **sub_list, const char* file_name)
{
    FILE *f = init_io(file_name);

    char line [500];
    fgets(line, sizeof(line), f); //To avoid headers in input

    while(read_line(line, sizeof(line), f) != NULL)
    {
        //printf("%s\n", line);
        change_commas(line, sizeof(line));
        parse_line(sub_list, line);
    }

    fclose(f);
}

int print_subject (GList* sub_list, int id)
{
    subject *sub = (subject*)g_list_nth_data(sub_list, id);
    if(sub == NULL) return 0;

    printf("%s %s %d %s\n", sub->sub_code, sub->sub_name, sub->credit, sub->sub_type);

    int i = 0;
    lesson *les = NULL;
    while(les = (lesson*)g_list_nth_data(sub->les_list, i))
    {
        printf("%s %s ", les->tutor, les->lang);

        int j = 0;
        interval *intv = NULL;
        while(intv = (interval*)g_list_nth_data(les->time_list, j))
        {
            printf("%s %d %d:%d-%d:%d ",
                    intv->room,
                    intv->day->tm_wday,
                    intv->beg->tm_hour, intv->beg->tm_min,
                    intv->end->tm_hour, intv->end->tm_min);
            j++;
        }
        printf("\n");
        i++;
    }
    getchar();

    return 1;
}

void print_sub_list (GList* sub_list)
{
    int i = 0;
    while(print_subject(sub_list, i)) i++;
}

void print_sub_id (GList* sub_list, int sub_id, int les_id)
{
    subject* sub = (subject*)g_list_nth_data(sub_list, sub_id);
    lesson* les = (lesson*)g_list_nth_data(sub->les_list, les_id);

    printf("%s %s %d %s %s %s",
            sub->sub_code,
            sub->sub_name,
            sub->credit,
            sub->sub_type,
            les->tutor,
            les->lang);

     int j = 0;
    interval *intv = NULL;
    while(intv = (interval*)g_list_nth_data(les->time_list, j))
    {
        printf("%s %d %d:%d-%d:%d ",
                intv->room,
                intv->day->tm_wday,
                intv->beg->tm_hour, intv->beg->tm_min,
                intv->end->tm_hour, intv->end->tm_min);
        j++;
    }
    printf("\n");
}

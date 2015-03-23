#include "data.h"
#include "io.h"
#include <stdio.h>

//TODO: free tokens

static int set_day(const char* str)
{
    if(strcmp(str, "H") == 0) return 0;
    if(strcmp(str, "K") == 0) return 1;
    if(strcmp(str, "SZE") == 0) return 2;
    if(strcmp(str, "CS") == 0) return 3;
    if(strcmp(str, "P") == 0) return 4;

    return -1;
}

static interval* set_time_interval(const char** tokens)
{
    struct tm *beg_time = (struct tm*)malloc(sizeof(struct tm));
    beg_time->tm_hour = atoi(tokens[1]);
    beg_time->tm_min = atoi(tokens[2]);

    struct tm *end_time = (struct tm*)malloc(sizeof(struct tm));
    end_time->tm_hour = atoi(tokens[3]);
    end_time->tm_min = atoi(tokens[4]);

    struct tm *day_time = (struct tm*)malloc(sizeof(struct tm));
    day_time->tm_wday = set_day(tokens[0]);

    interval *new_interval = (interval*)malloc(sizeof(interval));
    new_interval->beg = beg_time;
    new_interval->end = end_time;
    new_interval->day = day_time;
    new_interval->room = strdup(tokens[5]);

    return new_interval;
}

//TODO: free tokens
static GList* parse_occassions (const char* str)
{
    int count = 0;
    char** tokens = get_tokens(str, "$\"", &count);

    GList *time_list = NULL;

    int i;
    for(i = 0; i < count; i++)
    {
        int cnt = 0;
        char** tokens1 = get_tokens(tokens[i], "\":-()", &cnt);
        time_list = g_list_append(time_list, (gpointer)(set_time_interval(tokens1)));
    }

    return time_list;
}

void add_subject (GList **sub_list, char** const tokens)
{
    //Go through sub_list and look for lec_name
    //look for lec_type
    //look for the same occassion
    //TODO: collect the same occassions into a single data

    char* sub_name = strdup(tokens[1]);
    char* sub_type = strdup(tokens[5]);

    lesson *new_lesson = (lesson*)malloc(sizeof(lesson));
    new_lesson->tutor = strdup(tokens[8]);
    new_lesson->lang = strdup(tokens[9]);
    new_lesson->time_list = parse_occassions(tokens[7]);

    GList* tmp_list = NULL;

    int i = 0;
    while(tmp_list = g_list_nth(*sub_list, i))
    {
        subject* sub = (subject*)g_list_nth_data(*sub_list, i);
        if(strcmp(sub->sub_name, sub_name) == 0 &&
            strcmp(sub->sub_type, sub_type) == 0)
        {
            //Add occassion to the list
            printf("existing subject found\n");
            sub->les_list = g_list_append(sub->les_list, (gpointer)new_lesson);
            break;
        }
        i++;
    }

    if(tmp_list == NULL)
    {
        subject* new_sub = (subject*)malloc(sizeof(subject));
        new_sub->sub_code = strdup(tokens[0]);
        new_sub->sub_name = strdup(tokens[1]);
        new_sub->credit = atoi(tokens[2]);
        new_sub->sub_type = strdup(tokens[5]);
        new_sub->les_list = NULL;
        new_sub->les_list = g_list_append(new_sub->les_list, (gpointer)new_lesson);
        *sub_list = g_list_append(*sub_list, (gpointer)new_sub);
    }
}

int print_subject (GList* sub_list, int id)
{
    subject *sub = (subject*)g_list_nth_data(sub_list, id);
    if(sub == NULL) return -1;

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

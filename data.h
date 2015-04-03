#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <stdio.h>
#include "time.h"

#ifndef DATA_H
#define DATA_H

typedef struct interval
{
    struct tm *beg;
    struct tm *end;
    struct tm *day;
    char* room;
} interval;

typedef struct subject
{
    char* sub_code;
    char* sub_name;
    int credit;
    char* sub_type;
    GList *les_list;

} subject;

typedef struct lesson
{
    char* tutor;
    char* lang;
    GList *time_list;
} lesson;

void add_subject (GList** sub_list, char** tokens);
int time_intersects(const interval* interval_a, const interval* interval_b);
int time_interval_intersects (GList *time_list_a, GList *time_list_b);
#endif

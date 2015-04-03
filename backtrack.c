#include <glib.h>
#include "data.h"
#include "backtrack.h"
#include "io.h"

static int collide (GList *back_list, int* id, int index)
{
    subject *sub_a = (subject*)g_list_nth_data(back_list, index);
    lesson *les_a = (lesson*)g_list_nth_data(sub_a->les_list, id[index]);

    for(int i = 0; i < index; i++)
    {
        //if time interval intersects return 1
        subject *sub_b = (subject*)g_list_nth_data(back_list, i);
        lesson *les_b = (lesson*)g_list_nth_data(sub_b->les_list, id[i]);

        if (time_interval_intersects(les_a->time_list, les_b->time_list))
        {
            //printf("collide index: %d\n", i);
            //printf("%s %s - %s %s", sub_a->sub_name, sub_a->sub_type, sub_b->sub_name, sub_b->sub_type);
            return 1;
        }
    }

    return 0;
}

static void step_back(int* id, int index, int cnt)
{
    for(int i = index; i < cnt; i++) id[i] = 0;
}

static gint gcompare (gconstpointer a, gconstpointer b)
{
    subject *sub_a = (subject*)a;
    subject *sub_b = (subject*)b;

    int cnt_a = g_list_length(sub_a->les_list);
    int cnt_b = g_list_length(sub_b->les_list);

    if(cnt_a >cnt_b) return 1;
    if(cnt_a < cnt_b) return -1;
    if(cnt_a == cnt_b) return 0;
}

static gint sort_by_day (gconstpointer a, gconstpointer b)
{
    subject *sub_a = (subject*)a;
    subject *sub_b = (subject*)b;

    lesson *les_a = g_list_nth_data(sub_a->les_list, 0);
    lesson *les_b = g_list_nth_data(sub_b->les_list, 0);

    interval* intv_a = g_list_nth_data(les_a->time_list,0);
    interval* intv_b = g_list_nth_data(les_b->time_list, 0);

    if(intv_a->day->tm_wday > intv_b->day->tm_wday) {return 1;}
    else  if (intv_a->day->tm_wday == intv_b->day->tm_wday)
    {
        if(intv_a->beg->tm_hour > intv_b->beg->tm_hour) return 1;
    }

    return 0;
}

//TODO: implement it differently
static void print_solution (GList* back_list, int* id, int cnt)
{
    for(int i = 0; i < cnt; i++)
    {
        print_sub_id(back_list, i, 0);
    }

    printf("\n");
}

gpointer intv_copy (gconstpointer src, gpointer data)
{
    interval *old = (interval*)src;

    interval *new_intv = (interval*)malloc(sizeof(interval));
    new_intv->beg = (struct tm*)malloc(sizeof(struct tm));
    new_intv->beg->tm_hour = old->beg->tm_hour;
    new_intv->beg->tm_min = old->beg->tm_min;
    new_intv->end = (struct tm*)malloc(sizeof(struct tm));
    new_intv->end->tm_hour = old->end->tm_hour;
    new_intv->end->tm_min = old->end->tm_min;
    new_intv->day = (struct tm*)malloc(sizeof(struct tm));
    new_intv->day->tm_wday = old->day->tm_wday;
    new_intv->room = strdup(old->room);

    return (gpointer)new_intv;
}

gpointer les_copy (gconstpointer src, gpointer data)
{
    lesson *old = (lesson*)src;

    lesson *new_les = (lesson*)malloc(sizeof(lesson));
    new_les->tutor = strdup(old->tutor);
    new_les->lang = strdup(old->lang);
    new_les->time_list = g_list_copy_deep(old->time_list, (GCopyFunc)intv_copy, NULL);

    return (gpointer)new_les;
}

gpointer sub_copy (gconstpointer src, gpointer data)
{
    subject *old = (subject*)src;

    subject *new_sub = (subject*)malloc(sizeof(subject));
    new_sub->sub_code = strdup(old->sub_code);
    new_sub->sub_name = strdup(old->sub_name);
    new_sub->credit = old->credit;
    new_sub->sub_type = old->sub_type;
    new_sub->les_list = g_list_copy_deep(old->les_list, (GCopyFunc)les_copy, NULL);

    return (gpointer)new_sub;
}

GDestroyNotify free_intv (gpointer data)
{
    interval *intv = (interval*)data;
    free(intv->beg);
    free(intv->end);
    free(intv->day);
    free(intv->room);
}

GDestroyNotify free_les (gpointer data)
{
    lesson *les = (lesson*)data;
    free(les->tutor);
    free(les->lang);
    g_list_free_full(les->time_list, (GDestroyNotify)free_intv);
}

GDestroyNotify free_sub (gpointer data)
{
    subject *sub = (subject*)data;
    free(sub->sub_code);
    free(sub->sub_name);
    free(sub->sub_type);
    g_list_free_full(sub->les_list, (GDestroyNotify)free_les);
}


static void solution_found(GList* back_list, int* id, int cnt)
{
    GList *list = NULL;
    list = g_list_copy_deep(back_list, (GCopyFunc)sub_copy, NULL);

    for(int i =0 ; i< cnt; i++)
    {
        subject *tmp_sub = g_list_nth_data(list,i);
        for(int j = 0; j< id[i]; j++)
        {
            gpointer rem = g_list_nth_data(tmp_sub->les_list, 0);
            tmp_sub->les_list  = g_list_remove(tmp_sub->les_list, rem);
        }
    }

    list = g_list_sort(list, sort_by_day);
    print_solution(list, id, cnt);

    //TODO: free list
    //g_list_free_full(list, (GDestroyNotify)free_sub);
}

void back_track (GList **sub_list)
{
    GList *back_list = g_list_copy_deep(*sub_list, (GCopyFunc) sub_copy, NULL);
    back_list = g_list_sort(back_list, gcompare);

    int cnt = g_list_length(back_list);
    int* id = (int*)malloc(sizeof(int) * cnt);

    for(int i = 0; i < cnt; i++) id[i] = 0;

    int index = 0;
    int db = 0;
    while(1)
    {
        subject *tmp = (subject*)g_list_nth_data(back_list, index);

        if(g_list_nth(tmp->les_list, id[index]) == NULL)
        {
            if(index == 0) break;
            step_back(id, index, cnt);
            index--;
            id[index]++;
            continue;
        }

        if(collide(back_list,id,  index))
        {
            id[index]++;
            continue;
        }

        else
        {
            if(index == cnt-1)
            {
                solution_found(back_list, id, cnt);
                db++;
                getchar();
                id[index]++;
                continue;
            }
            else
            {
                index++;
            }
        }
    }

    printf("%d db megoldást találtam\n", db);

    //TODO: free back_list
    g_list_free_full(back_list, free_sub);
    free(id);
}

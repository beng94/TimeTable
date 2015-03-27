#include <glib.h>
#include "data.h"
#include "backtrack.h"
#include "io.h"

static int collide (GList *back_list, int* id, int index)
{
    subject *sub_a = (subject*)g_list_nth_data(back_list, index);
    lesson *les_a = (lesson*)g_list_nth_data(sub_a->les_list, id[index]);

    int i;
    for(i = 0; i < index; i++)
    {
        //if time interval intersects return 1
        subject *sub_b = (subject*)g_list_nth_data(back_list, i);
        lesson *les_b = (lesson*)g_list_nth_data(sub_b->les_list, id[i]);

        if (time_interval_intersects(les_a->time_list, les_b->time_list)) return 1;
    }

    return 0;
}

static void step_back(int* id, int index, int cnt)
{
    int i;
    for(i = index; i < cnt; i++)
    {
        id[i] = 0;
    }
}

static int solution_found()
{
    return 0;
}

static gint Gcompare (gconstpointer a, gconstpointer b)
{
    subject *sub_a = (subject*)a;
    subject *sub_b = (subject*)b;

    int cnt_a = g_list_length(sub_a->les_list);
    int cnt_b = g_list_length(sub_b->les_list);

    if(cnt_a >cnt_b) return 1;
    if(cnt_a < cnt_b) return -1;
    if(cnt_a == cnt_b) return 0;
}

void back_track (GList **sub_list)
{
    //TODO: maybe the copy function doesn't work properly

    GList *back_list = g_list_copy(*sub_list);
    back_list = g_list_sort(*sub_list, Gcompare);
    //print_sub_list(back_list);

    int cnt = g_list_length(back_list);
    int* id = (int*)malloc(sizeof(int) * cnt);

    int i;
    for(i = 0; i < cnt; i++) id[i] = 0;

    int index = 0;

    while(1)
    {
        //printf("%d\n", index);
        subject *tmp = (subject*)g_list_nth_data(back_list, index);
        if(g_list_nth(tmp->les_list, id[index]) == NULL)
        {
            if(index == 0) break;
            step_back(id, index, cnt);
            index--;
            id[index]++;
            //getchar();
            continue;
        }

        if(collide(back_list,id,  index))
        {
            //step_back(id, index, cnt);
            //index--;
            id[index]++;
            //getchar();
            continue;
        }

        else
        {
            //index++;
            if(index == cnt-1)
            {
                //print solution
                print_sub_id(back_list, index, id[index]);
                //printf("sol found\n");
                getchar();
                id[index]++;
                continue;
            }
            else {
                index++;
            }
        }
    }

    //TODO: free back_list
    //TODO: free index array
}

#include <glib.h>
#include <stdio.h>
#include "io.h"
#include "backtrack.h"

const char *file_name = "TimeTable.csv";

int main()
{
    GList *sub_list = NULL;
    read_in(&sub_list, file_name);

    getchar();
    print_sub_list(sub_list);

    back_track(sub_list);

    return 0;
}

#include <time.h>

typedef struct time_interval
{
    struct tm *beg;
    struct tm *end;
} time_interval;

typedef struct occassion
{
    char* room;
    struct tm *day;
    time_interval interval;
} occassion;

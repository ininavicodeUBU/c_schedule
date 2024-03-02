#include <windows.h>
#include <stdio.h>

#include "../include/func.h"





int main()
{
    date_t local_time;
    get_local_time(&local_time);

    // Print or use the current time
    printf("Current Local Time: %d/%d/%d %d:%d\n", local_time.day, local_time.month, local_time.year, local_time.hour, local_time.minute);

    return 0;
}

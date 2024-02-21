#define DESCRIPTION_MAX_LEN 100
#define MAX_EVENTS 50

#define MAX_PATH_LEN 40
#define MAX_SCHEDULES 20

// event fields
// id, day, month, year, hour, minute, description
#define EVENT_FIELDS 7


// un-comment which line correspons to your OS
#define WIN 1
// #define LINUX 1

#ifdef WIN 
    #define PIPE_SEPARATOR "&"

#elif LINUX
    #define PIPE_SEPARATOR ";"

#endif

#define SPACE " "

#define SCHEDULES_PATH "../schedules"
#define FIND_SCH_COMMAND "cd " SPACE SCHEDULES_PATH PIPE_SEPARATOR "ls *.txt"
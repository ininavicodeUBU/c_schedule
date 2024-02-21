#define DESCRIPTION_MAX_LEN 100
#define MAX_EVENTS 50

#define MAX_FILENAME_LEN 40
#define MAX_PATH_LEN 100
#define MAX_SCHEDULES 20

#define MAX_TEXT_INPUT_OPTION_LEN  10

// un-comment which line correspons to your OS
#define WIN 1
// #define LINUX 1

#ifdef WIN 
    #define PIPE_SEPARATOR "&"

#elif LINUX
    #define PIPE_SEPARATOR ";"

#endif

#define MAIN_EXE_TO_SCH_REL_PATH "/../../schedules/"
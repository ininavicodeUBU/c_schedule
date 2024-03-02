// libraries
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <windows.h>
#include <tchar.h>
#include <dirent.h>

// inlcudes
#include "../include/func.h"

// struct date_t data management functions +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void get_local_time(date_t* date)
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    // Format the result string as DD/MM/YYYY HH:mm:ss
    // sprintf(result, "%02d/%02d/%04d %02d:%02d:%02d", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
    date->day = st.wDay;
    date->month = st.wMonth;
    date->year = st.wYear;
    date->minute = st.wMinute;
    date->hour = st.wHour;
}
// -------------------------------------------------------------------------------------------------------------------


// data management functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void get_events_by (date_event_t events_list[], int year, int month, int day, date_event_t out_events[])
{
    
    bool contition_to_save = true;
    int events_count = 0; // if the final count is 0, tell there are no events for this conditions
    int i = 0;
    while (!end_of_event_list(events_list[i]))
    {   
        if (!deleted_event(events_list[i]))
        {
            if (year != -1)
                contition_to_save = (year == events_list[i].date.year);

            if (month != -1)
                contition_to_save *= (month == events_list[i].date.month);

            if (day != -1)
                contition_to_save *= (day == events_list[i].date.day);
            
            if (contition_to_save)
            {
                out_events[events_count].id = events_list[i].id;
                out_events[events_count].date.day = events_list[i].date.day;
                out_events[events_count].date.month = events_list[i].date.month;
                out_events[events_count].date.year = events_list[i].date.year;
                out_events[events_count].date.hour = events_list[i].date.hour;
                out_events[events_count].date.minute = events_list[i].date.minute;
                strcpy(out_events[events_count].description, events_list[i].description);

                events_count++;
            }
        }

        i++;
        
    }

    put_centinela_event(&out_events[events_count]);
        
}

bool end_of_event_list (date_event_t event)
{
    // return true if end of the list
    return (event.id == CENTINELA_END_OF_EVENT_LIST_ID);
    
}

bool deleted_event (date_event_t event)
{
    // return true if the event is deleted
    return (event.id == DELETED_EVENT_OF_THE_LIST);
}

void put_centinela_event (date_event_t *event)
{
    event->id = CENTINELA_END_OF_EVENT_LIST_ID;
}

void delete_event (date_event_t events_list[], int id_to_delete)
{
    int id = 0;
    bool deleted = false;
    while (!end_of_event_list(events_list[id]) || !deleted)
    {
        if (events_list[id].id == id_to_delete)
        {
            events_list[id].id = DELETED_EVENT_OF_THE_LIST;
            deleted = true;  
        } 
        id++;
    }
}
// ---------------------------------------------------------------------------------------------

// menu user_interaction & displaying functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void show_events_by (date_event_t events_list[], int year, int month, int day)
{
    bool condition_to_save_id = true;
    int events_count = 0; // if the final count is 0, tell there are no events for this conditions
    int i = 0;
    while (!end_of_event_list(events_list[i]))
    {   
        if (!deleted_event(events_list[i]))
        {
            if (year != -1)
                condition_to_save_id = (year == events_list[i].date.year);

            if (month != -1)
                condition_to_save_id *= (month == events_list[i].date.month);

            if (day != -1)
                condition_to_save_id *= (day == events_list[i].date.day);
            
            if (condition_to_save_id)
            {
                printf("\nId: %d\nTime: %d/%d/%d - %d:%d\n\tDescription: %s\n", events_list[i].id, events_list[i].date.day, events_list[i].date.month, events_list[i].date.year
                , events_list[i].date.hour, events_list[i].date.minute, events_list[i].description);
                events_count++;
            }
        }

        i++;
        
    }

    if (events_count == 0)
        printf("\nNo events to show\n\n");
}

void show_available_schedules (char schedules[][MAX_FILENAME_LEN])
{
    // default options
    printf("\n[+] -> New schedule\n");
    printf("[-] -> Delete schedule\n");

    // print to the user the available schedules
    short unsigned n_dir = 0;
    while (schedules[n_dir][0] != '\0')
    {
        printf("[%u] -> %s\n", n_dir, schedules[n_dir]);
        n_dir++;
    }
}

void get_events_filtered_by_user (date_event_t events_list[], date_event_t out_filtered_events[])
{
    int noramlized_user_option_show_events_menu;
    char user_option_text_events_menu[MAX_TEXT_INPUT_OPTION_LEN];
    
    do
    {
        printf("Filter by ... ->\n");
            printf("\t[+] - all\n");
            printf("\t[0] - year\n");
            printf("\t[1] - year & month\n");
            printf("\t[2] - year & month & day\n");
        
        fflush2();
        fgets(user_option_text_events_menu, MAX_TEXT_INPUT_OPTION_LEN, stdin);
        
        noramlized_user_option_show_events_menu = normalize_user_input_text(user_option_text_events_menu);

    } while (((noramlized_user_option_show_events_menu < 0) || (noramlized_user_option_show_events_menu > 2)) && (noramlized_user_option_show_events_menu != -2));
    
    if (noramlized_user_option_show_events_menu == -2)
    {
        get_events_by(events_list, -1, -1, -1, out_filtered_events);
    }
    else if (noramlized_user_option_show_events_menu == 0) // year
    {
        // local variables
        int year;
        // end local variables
        printf("\n");

        // select the year
        printf("Intro the year of the events to show: "); scanf(" %d", &year);
        // function -> show events by year
        get_events_by(events_list, year, -1, -1, out_filtered_events);

    } else if (noramlized_user_option_show_events_menu == 1) // year & month
    {
        // local variables
        int year, month;
        // end local variables
        printf("\n");

        // select the year
        printf("Intro the year & month of the events to show MM/YYYY: "); scanf(" %d/%d", &month, &year);
        // function -> show events by year and month
        get_events_by(events_list, year, month, -1, out_filtered_events);

    } else if (noramlized_user_option_show_events_menu == 2) // year & month & day
    {
        // local variables
        int year, month, day;
        // end local variables
        printf("\n");

        // select the year
        printf("Intro the year & month of the events to show DD/MM/YYYY: "); scanf(" %d/%d/%d", &day, &month, &year);
        // function -> show events by year and month and day
        get_events_by(events_list, year, month, day, out_filtered_events);
    }
}

// ---------------------------------------------------------------------------------------------

// specific menu functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void operate_schedule_menu_option (int menu_option, const char schedules_path[], char available_schedules[][MAX_FILENAME_LEN],
date_event_t events_list[], int* events_len, char new_sch_path[])
{
    if (menu_option == -2) // new schedule
    {   
        char new_schedule_name[MAX_FILENAME_LEN];
        char new_schedule_path[MAX_PATH_LEN];
        
        printf("\nWrite the name of the schedule: ");
        fgets(new_schedule_name, MAX_FILENAME_LEN, stdin);
        trim_trailing_whitespace(new_schedule_name);
        sprintf(new_schedule_path, "%s%s", schedules_path, new_schedule_name); strcat(new_schedule_path, ".txt");
        new_file(new_schedule_path);
        file_to_event_list(new_schedule_path, events_list, events_len);

        // writing the output parameter
        strcpy(new_sch_path, new_schedule_name);
    }
    else if (menu_option == -1) // delete schedule
    {
        unsigned schedule_to_delete;
        char delete_schedule_path[MAX_PATH_LEN];

        do
        {
            printf("\nSelect one schedule to delete: ");
            scanf(" %u", &schedule_to_delete);
            sprintf(delete_schedule_path, "%s%s", schedules_path, available_schedules[schedule_to_delete]);
        } while ((schedule_to_delete < 0) || delete_file(delete_schedule_path));

        *events_len = -1; // indicate that the option of the user was to delete a file
    }
    else
    {
        char open_schedule_path[MAX_PATH_LEN];
        sprintf(open_schedule_path, "%s%s", schedules_path, available_schedules[menu_option]);
        file_to_event_list(open_schedule_path, events_list, events_len);
        strcpy(new_sch_path, open_schedule_path);
    }
}

// ---------------------------------------------------------------------------------------------

// user input functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int normalize_user_input_text (char user_input_text[])
{
    int return_int;
    char input_cpy[MAX_TEXT_INPUT_OPTION_LEN];
    strcpy(input_cpy, user_input_text);


    if (is_numeric(input_cpy))
        return_int = (atoi(input_cpy));

    else
    {
        trim_trailing_whitespace(input_cpy);
        if (strcmp(input_cpy, "-") == 0)
            return_int = -1;
        
        else if (strcmp(input_cpy, "+") == 0)
            return_int = -2;
    }


    return return_int;
}

// ----------------------------------------------------------------------------------------------------------

// string functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void trim_trailing_whitespace(char *str) {
    size_t len = strlen(str);

    // Trim trailing whitespace
    size_t end = len - 1;
    while (end > 0 && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
        end--;
    }

    // Null-terminate the trimmed string
    str[end + 1] = '\0';
}

void strcpy_len (char copy[], char paste[], int str_len)
{
    for (int i = 0; i < str_len; i++)
        paste[i] = copy[i];
}

bool is_numeric (char string[])
{
    bool result = true;
    short unsigned i = 0;

    while ((string[i] != '\n') && (result))
    {       
        result = (string[i] >= '0') && (string[i] <= '9');
        i++;
    }

    return result;
}

// -----------------------------------------------------------------------

// file interaction functions +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void file_to_event_list (char filename[], date_event_t event_list[], int *n_events)
{
    // variables
    FILE *f_sch;
    int n_event = 0;
    // end variables

    printf("\nOpening... %s", filename);

    f_sch = fopen(filename, "r");
    if (f_sch == NULL)
    {
        printf("\nERROR: Cannot open the schedule %s", filename);
    }
    else
    {
        // fill the events list with the file data
        while (!feof(f_sch) && (n_event < MAX_EVENTS))
        {
            event_list[n_event].id = (unsigned)n_event;
            fscanf(f_sch, "%d %d %d %d %d \"%255[^\"]\"",
                        &event_list[n_event].date.day,
                        &event_list[n_event].date.month,
                        &event_list[n_event].date.year,
                        &event_list[n_event].date.hour,
                        &event_list[n_event].date.minute,
                        event_list[n_event].description);

            n_event++;
        }

        fclose(f_sch);

        // save the n_event in the n_events output parameter
        *n_events = n_event - 1; // add -1 to the count of events because it always count one extra line to detect the end of the file

        put_centinela_event(&event_list[*n_events]); // add the centinella for the end of the events list
    }
    
    

}

void event_list_to_file (char filename[], date_event_t event_list[])
{
    FILE *f_sch;
    char trimmed_decription[DESCRIPTION_MAX_LEN];
    
    printf("\nSaving data on %s ...\n", filename);

    f_sch = fopen(filename, "w");
    if (f_sch == NULL)
    {
        printf("\nERROR: Cannot open the schedule %s", filename);
    }
    else
    {
        unsigned i = 0;
        while (!end_of_event_list(event_list[i]))
        {
            if (!deleted_event(event_list[i]))
            {
                // this proccess is necessary to fprint the sring without \n, so after that is necessary to add a double quote in the same line
                strcpy(trimmed_decription, event_list[i].description);
                trim_trailing_whitespace(trimmed_decription);
                
                fprintf(f_sch, "%d %d %d %d %d \"%s\"\n", event_list[i].date.day, event_list[i].date.month, event_list[i].date.year
                , event_list[i].date.hour, event_list[i].date.minute, trimmed_decription);

            }

            i++;
        }
        fclose(f_sch);
    }
}

void new_file (char filename[])
{
    FILE* new_file;

    trim_trailing_whitespace(filename);
    new_file = fopen(filename, "w");
    if (new_file == NULL)
    {
        printf("\nERROR: Cannot create the file %s", filename);
    }
    else
    {
        printf("\nFile %s created", filename);
    }
    fclose(new_file);
}

bool delete_file (char filename[])
{
    if (remove(filename) == 0)
    {
        printf("\nFile %s deleted", filename);
        return 0;
    }
    else
    {
        printf("\nERROR: Cannot delete %s", filename);
        return 1;
    }
}

// -----------------------------------------------------------------------

// prompt interaction +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool get_exe_path(char out_path[])
{
    TCHAR path[MAX_PATH];
    bool error = false;

    // Get the path of the executable
    DWORD len = GetModuleFileName(NULL, path, MAX_PATH);
    if (len == 0) {
        // _tprintf(_T("Error getting executable path\n")); // enable to print the error
        error = true;
    }
    else
    {
        // Print the directory
        // _tprintf(_T("Executable directory: %s\n"), path); // enable to print the found path
        strcpy(out_path, path);
    }

    return error;

}

bool ls (char directory[], char dir_list[][MAX_FILENAME_LEN], char file_extension[])
{
    DIR *dir;
    struct dirent *entry;
    bool error = false;

    // Open the current directory
    dir = opendir(directory);
    if (dir == NULL) {
        perror("Error opening directory");
        error = true;
    }
    else
    {
        // Read directory entries
        entry = readdir(dir);
        short unsigned n_entry = 0;

        while (entry != NULL) {
            if (entry->d_type == DT_REG && strstr(entry->d_name, file_extension) != NULL) {  // Check if it is a regular file
                // printf("%s\n", entry->d_name);
                strcpy(dir_list[n_entry], entry->d_name);
                n_entry++;
            }
            entry = readdir(dir);
            
        }

        // add a centinela
        dir_list[n_entry][0] = '\0';

        // Close the directory
        closedir(dir);

    }

    return error;
}

get_prompt_out_t get_prompt_out (char command[])
{
    FILE *prompt_out_file;
    char temp_filename[MAX_PATH_LEN];
    get_prompt_out_t return_pointer = malloc(sizeof(char) * MAX_SCHEDULES * MAX_PATH_LEN);

    // execute a prompt command and save the output in prompt_out_file
    prompt_out_file = popen(command, "r");

    if (prompt_out_file == NULL) {
        perror("Error opening pipe.");
        free(return_pointer);
    } 
    else
    {
        // Read the output of the command into the prompt_out
        int n_filename = 0;

        printf("\n");
        while (fgets(temp_filename, MAX_PATH_LEN, prompt_out_file) != NULL) {
            // save each filename in the table of available_schedules
            strcpy_len(temp_filename, (*return_pointer)[n_filename], MAX_PATH_LEN); // this is an special strcpy, ends the copy by len and not by '\n'
            n_filename++;
        }

        // add a centinella at the first char of the next word
        (*return_pointer)[n_filename][0] = '\0';

        // Close the pipe
        pclose(prompt_out_file); 
    }


    return return_pointer;

}

// -----------------------------------------------------------------------

// others ++++++++++++++++++++++++++++++++++++++++++++++
void fflush2()
{
    char temp;
    scanf("%c", &temp);
}


// -----------------------------------------------------


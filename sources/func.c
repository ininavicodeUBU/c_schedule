// libraries
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <windows.h>
#include <tchar.h>
#include <dirent.h>

// inlcudes
#include "../include/func.h"


// function definitions

void strcpy_len (char copy[], char paste[], int str_len)
{
    for (int i = 0; i < str_len; i++)
        paste[i] = copy[i];
}

// +++++++++++++++++++++
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

//  +++++++++++++++++++++++++
/**
 * @details This function does not support value errors, so month (1 - 12), day (max of the month)
 * @param year: (optional argument -1 to skip)
 * @param month: (optional argument: -1 to skip)
 * @param day: (optional argument: -1 to skip)
*/
void show_events_by (date_event_t events_list[], int events_list_len, int year, int month, int day)
{

    bool condition_to_show = true;
    int events_showed_count = 0; // if the final count is 0, tell there are no events for this conditions
    for (int i = 0; i < events_list_len; i++)
    {
        
        if (year != -1)
            condition_to_show = (year == events_list[i].date.year);

        if (month != -1)
            condition_to_show *= (month == events_list[i].date.month);

        if (day != -1)
            condition_to_show *= (day == events_list[i].date.day);
        
        if (condition_to_show)
        {
            printf("\nTime: %d/%d/%d - %d:%d\n\tDescription: %s\n\n", events_list[i].date.day, events_list[i].date.month, events_list[i].date.year
            , events_list[i].date.hour, events_list[i].date.minute, events_list[i].description);
            events_showed_count++;
        }
        
    }

    if (events_showed_count == 0)
        printf("\nNo events to show\n\n");
}

// +++++++++++++++++++++
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

// ++++++++++++++++++++++

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
    }
    
    

}
// ++++++++++++++++++
void event_list_to_file (char filename[], date_event_t event_list[], int n_events)
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
        for (int i = 0; i < n_events; i++)
        {
            // this proccess is necessary to fprint the sring without \n, so after that is necessary to add a double quote in the same line
            strcpy(trimmed_decription, event_list[i].description);
            trim_trailing_whitespace(trimmed_decription);
            
            fprintf(f_sch, "%d %d %d %d %d \"%s\"\n", event_list[i].date.day, event_list[i].date.month, event_list[i].date.year
            , event_list[i].date.hour, event_list[i].date.minute, trimmed_decription);
            
        }
        fclose(f_sch);
    }
}

// +++++++++++++++++
void show_select_and_import_schedules(date_event_t events_list[], int *n_events, char filename_schedule[])
{
    // get a list of the available schedules
    get_prompt_out_t result = get_prompt_out(FIND_SCH_COMMAND); // "cd ../schedules/ ; ls *.txt"

    int n_schedule = 0;

    // show the option to create a new schedule
    printf("\n[-2] - Delete schedule... ");
    printf("\n[-1] - New schedule...\n");

    while ((*result)[n_schedule][0] != '\0')
    {
        printf("[%d] - %s", n_schedule, (*result)[n_schedule]);
        n_schedule++;
    }

    printf("\n");

    int user_option;
    do
    {
        printf("Select an schedule: "); scanf(" %d", &user_option);
    } while ((user_option < -2) || (user_option >= n_schedule));

    printf("\n");

    char full_path[MAX_PATH_LEN];
    char path_prefix[] = SCHEDULES_PATH;
    char filename[MAX_PATH_LEN];

    if (user_option == -2)
    {
        int delete_schedule;
        do
        {
            printf("\nSelect the schedule to delete: "); scanf(" %d", &delete_schedule);

        } while ((delete_schedule < 0) || (delete_schedule >= n_schedule));

        strcpy(filename, (*result)[delete_schedule]);
        trim_trailing_whitespace(filename);
        strcpy(full_path, strcat(path_prefix, filename));
        trim_trailing_whitespace(full_path);

        delete_file(full_path);

        *n_events = -1; // return this to the output parameter to comunicate the deletion of a file
    }
    else if (user_option == -1) // create a new schedule
    {
        int same_name = 0;

        do
        {
            printf("\n\tIntro the owner of the schedule: ");
            // serial flush
            char temp; scanf("%c", &temp);
            // end of serial flush
            fgets(filename, MAX_PATH_LEN, stdin);

            int i = 0;
            while ((i < n_schedule) && (same_name == 0))
            {
                same_name = strcmp(filename, (*result)[i]);
                i++;
            }
            if (same_name == 0)
            {
                printf("This filename already exists, try another");
            }
                
        } while (same_name == 0);

        trim_trailing_whitespace(filename);
        strcpy(filename, strcat(filename, ".txt"));
        strcpy(full_path, strcat(path_prefix, filename));
        trim_trailing_whitespace(full_path);

        new_file(full_path);

    }
    else // save the name of the selected schedule
    {
        strcpy(filename, (*result)[user_option]);
        strcpy(full_path, strcat(path_prefix, filename));
        trim_trailing_whitespace(full_path);
        
    }

    free(result);

    if (user_option != -2)
    {
        file_to_event_list(full_path, events_list, n_events);
        strcpy(filename_schedule, full_path); // save the filename on the output parameter
    }
    
}

// +++++++++++++++++++++
void new_file (char filename[])
{
    FILE* new_file;
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

void delete_file (char filename[])
{
    if (remove(filename) == 0)
    {
        printf("\nFile %s deleted", filename);
    }
    else
        printf("\nERROR: Cannot delete %s", filename);
}


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



bool ls (char directory[], char dir_list[][MAX_PATH_LEN], char file_extension[])
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
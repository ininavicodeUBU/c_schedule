#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./constants.h"

// types
/**
 * Store a data by
 * (int): minute
 * (int): hour
 * (int): day
 * (int): month
 * (int): year
*/
typedef struct date_t
{
    int minute;
    int hour;
    int day;
    int month;
    int year;
} date_t;

/**
 * @brief Store an event by it's date and a little description
 * (date_t): date
 * (char[]): description
*/
typedef struct date_event_t
{
    int id;
    date_t date;
    char description[DESCRIPTION_MAX_LEN];
} date_event_t;
// end types

// functions


/**
 * @brief This typedef can store the output of the <get_prompt_out> function
*/
typedef char (*get_prompt_out_t)[MAX_SCHEDULES][MAX_PATH_LEN];

/**
 * Execute any command on the prompt and get its output as a list of strings,
 * one string by output row
 * 
 * @example
 * ls folder
 * <prompt output> ->
 * file1.txt
 * file2.txt
 * file3.txt
 * <function output> ->
 * {"file1.txt", "file2.txt", "file3.txt"}
*/
get_prompt_out_t get_prompt_out (char command[]);




// str functions +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * @brief Delete all
 *  , ' '
 *  , '\\n'
 *  , '\\t'
 * At the trail of the string
*/
void trim_trailing_whitespace(char *str);

/**
 * @brief This is a variant of strcpy, it copyies the <copy> to <paste> but
 * with the specifyied len and not by the end of one of the strings
*/
void strcpy_len (char copy[], char paste[], int str_len);


bool is_numeric (char string[]);

// -------------------------------------------------------------------------



// file interaction functions +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/**
 * @brief Saves all the data of a file into a list of events
 * @param n_events: (int*): Output parameter, here is stored the count of the events of the file
*/
void file_to_event_list (char filename[], date_event_t event_list[], int *n_events);

/**
 * @brief Saves a list of events into a file
 * @param n_events (int): Pass the count of the events of the event_list
*/
void event_list_to_file (char filename[], date_event_t event_list[]);

/**
 * @brief a new schedule on ./schedules
 * @param (char[]): Specify the name of the schedule
*/
void new_file (char filename[]);

/**
 * @return (bool) -> error = true; (file does not exists)
*/
bool delete_file (char filename[]);

// ----------------------------------------------------------------------------------------------------------

// menu user_interaction & displaying functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/**
 * @details Displays the values of an events_list filtering by 3 fields
 * @param year: (optional argument -1 to skip)
 * @param month: (optional argument: -1 to skip)
 * @param day: (optional argument: -1 to skip)
 * @pre This function does not support value errors, so month (1 - 12), day (max of the month)
*/
void show_events_by (date_event_t events_list[], int year, int month, int day);

void show_available_schedules (char schedules[][MAX_FILENAME_LEN]);

void get_events_filtered_by_user (date_event_t events_list[], date_event_t out_filtered_events[]);

// ----------------------------------------------------------------------------------------------------------

// data management functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/**
 * @details Gets the values of an events_list filtering by 3 fields
 * @param events_list: (in: date_event_t []) List of the events to filter
 * @param year: (optional argument -1 to skip)
 * @param month: (optional argument: -1 to skip)
 * @param day: (optional argument: -1 to skip)
 * @param out_events (out: int []) List of the events got. The list ends with CENTINELA_END_OF_EVENT_LIST_ID
 * @pre This function does not support value errors, so month (1 - 12), day (max of the month)
*/
void get_events_by (date_event_t events_list[], int year, int month, int day, date_event_t out_events[]);

bool end_of_event_list (date_event_t event);

bool deleted_event (date_event_t event);

void put_centinela_event (date_event_t *event);

void delete_event (date_event_t events_list[], int id_to_delete);

// ---------------------------------------------------------------------------------------------

// specific functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/**
 * @param (out: char[]) If the user selects the option of creating a new schedule, this output parameter returns the name of that new schedule
*/
void operate_schedule_menu_option (int menu_option, const char schedules_path[], char available_schedules[][MAX_FILENAME_LEN],
date_event_t events_list[], int* events_len, char new_sch_path[]);
// ---------------------------------------------------------------------------------------------

// user input functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * @pre Input string has to be numeric or to be {"+", "-"}
*/
int normalize_user_input_text (char user_input_text[]);

// ----------------------------------------------------------------------------------------------------------

// prompt functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * @brief Gets the path where the executable of the program is
 * @param out_path (out: char []): Here is saved the path as string
 * @return (bool) -> error = true
 * @pre <windows.h> <tchar.h>
*/
bool get_exe_path(char out_path[]);

/**
 * @brief Executes ls command on the specifyied dir
 * @param directory (in: char []) ls <directory>
 * @param dir_list (out: char [][]) Here is stored the output of the ls command, 1 string by folder/file found.
 * Centinela of dir_list -> dir_list[...][0] == '\0' is true
 * @param file_extension (in optional: char []) |file_extension = "" to unable| Select an specific extension for the listed files,
 * specifyied with dot -> example: .txt, .bat, .png, ...
 * @return (bool) -> error = true
 * @pre <dirent.h>
*/
bool ls (char directory[], char dir_list[][MAX_FILENAME_LEN], char file_extension[]);

// -------------------------------------------------------------------------------------

// others ++++++++++++++++++++++++++++++++++++++++++++++
void fflush2();
// -----------------------------------------------------

#endif
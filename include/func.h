#ifndef AUX_H
#define AUX_H

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
    date_t date;
    char description[DESCRIPTION_MAX_LEN];
} date_event_t;
// end types

// functions
/**
 * @brief This is a variant of strcpy, it copyies the <copy> to <paste> but
 * with the specifyied len and not by the end of one of the strings
*/
void strcpy_len (char copy[], char paste[], int str_len);

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

/**
 * @brief Delete all
 *  , ' '
 *  , '\\n'
 *  , '\\t'
 * At the trail of the string
*/
void trim_trailing_whitespace(char *str);

/**
 * @brief Show all the schedule_files on ./schedules
 * Let the user select one by index, the function gives an option to create a new schedule
 * Import that file as a list of events
 * @param n_events (int*): Output parameter, here is stored the count of the events of the imported file
 * @param filename_schedule (char[]): Output parameter, here is stored the name of the selected file by the user
*/
void show_select_and_import_schedules(date_event_t events_list[], int *n_events, char filename_schedule[]);

/**
 * @brief Saves all the data of a file into a list of events
 * @param n_events: (int*): Output parameter, here is stored the count of the events of the file
*/
void file_to_event_list (char filename[], date_event_t event_list[], int *n_events);

/**
 * @brief Saves a list of events into a file
 * @param n_events (int): Pass the count of the events of the event_list
*/
void event_list_to_file (char filename[], date_event_t event_list[], int n_events);

/**
 * @brief a new schedule on ./schedules
 * @param (char[]): Specify the name of the schedule
*/
void new_file (char filename[]);
void delete_file (char filename[]);

/**
 * @brief This function does not support value errors, so month (1 - 12), day (max of the month)
 * @param month: (optional argument: -1 to skip)
 * @param day: (optional argument: -1 to skip)
*/
void show_events_by (date_event_t events_list[], int events_list_len, int year, int month, int day);
// end functions

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
bool ls (char directory[], char dir_list[][MAX_PATH_LEN], char file_extension[]);

#endif
// libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// own libraries
#include "../include/constants.h"
#include "../include/func.h"
// end libraries

// global const
const int MONTH_DAYS[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
// end global const


// functions
// end functions

int main ()
{
    /*
    MENU -> 
        [0] - show events ->
            [0] - year
            [1] - year & month
            [2] - year & month & day
                return to MAIN

        
        [1] - add event ->
            return to MAIN

        [2] - change schedule
            [-1] - New schedule
            [0...X] - Select schedule

        [3] - stop program
            return to MAIN
    */

    // local variables    
    int user_option;

    date_event_t event_list[MAX_EVENTS];
    int events_len;

    bool end_program = false;
    bool end_actual_schedule;

    char filename_schedule[MAX_FILENAME_LEN];
    // end local variables

    while (!end_program)
    {
        // reseting variables for the next schedule import
        end_actual_schedule = false;

        // select the schedule file
        show_select_and_import_schedules(event_list, &events_len, filename_schedule);
        printf("\nEvents: %d", events_len);

        if (events_len == -1)
        {
            // the option was to delete an schedule the user has to choose another
            end_actual_schedule = true;
        }
        

        while (!end_actual_schedule)
        {
            // let the user choose an option
            do 
            {
                printf("\nSelect an option ->\n");
                    printf("\t[0] - Show events\n");
                    printf("\t[1] - Add event\n");
                    printf("\t[2] - Change schedule\n");
                    printf("\t[3] - End program\n");
                
                scanf(" %d", &user_option);
            } while ((user_option < 0) || (user_option > 3));

            // separate both menus
            printf("\n");

            if (user_option == 0) // list events of
            {
                if (events_len > 0) // check if there is any event saved
                {
                    do
                    {
                        printf("Show events of the ... ->\n");
                            printf("\t[-1] - all\n");
                            printf("\t[0] - year\n");
                            printf("\t[1] - year & month\n");
                            printf("\t[2] - year & month & day\n");
                        scanf(" %d", &user_option);
                    } while ((user_option < -1) || (user_option > 2));
                    
                    if (user_option == -1)
                    {
                        show_events_by(event_list, events_len, -1, -1, -1);
                    }
                    else if (user_option == 0) // year
                    {
                        // local variables
                        int year;
                        // end local variables
                        printf("\n");

                        // select the year
                        printf("Intro the year of the events to show: "); scanf(" %d", &year);
                        // function -> show events by year
                        show_events_by(event_list, events_len, year, -1, -1);

                    } else if (user_option == 1) // year & month
                    {
                        // local variables
                        int year, month;
                        // end local variables
                        printf("\n");

                        // select the year
                        printf("Intro the year & month of the events to show MM/YYYY: "); scanf(" %d/%d", &month, &year);
                        // function -> show events by year and month
                        show_events_by(event_list, events_len, year, month, -1);

                    } else if (user_option == 2) // year & month & day
                    {
                        // local variables
                        int year, month, day;
                        // end local variables
                        printf("\n");

                        // select the year
                        printf("Intro the year & month of the events to show DD/MM/YYYY: "); scanf(" %d/%d/%d", &day, &month, &year);
                        // function -> show events by year and month and day
                        show_events_by(event_list, events_len, year, month, day);
                    }

                } else
                {
                    printf("There are no events to show");
                }

                        

            } else if (user_option == 1) // add event
            {
                bool valid_input;
                do
                {
                    printf("Introduce the date for the event DD/MM/YYYY: "); 
                    scanf(" %d/%d/%d", &event_list[events_len].date.day, &event_list[events_len].date.month, &event_list[events_len].date.year);

                    valid_input = (event_list[events_len].date.month >= 1) && (event_list[events_len].date.month <= 12);
                    valid_input *= (event_list[events_len].date.day >= 1) && (event_list[events_len].date.day <= MONTH_DAYS[event_list[events_len].date.month - 1]); // add -1 because month are mapped from 0 to 11
                } while (!valid_input);
                
                do
                {
                    printf("\nIntroduce the hour for the event HH:MM: ");
                    scanf(" %d:%d", &event_list[events_len].date.hour, &event_list[events_len].date.minute);

                } while ((event_list[events_len].date.hour < 0) || (event_list[events_len].date.hour > 24) || (event_list[events_len].date.minute < 0) || (event_list[events_len].date.minute > 60));

                printf("\nIntroduce a description for your event: ");
                // stream flush
                char temp;
                scanf("%c", &temp);
                // end stream flush
                fgets(event_list[events_len].description, DESCRIPTION_MAX_LEN, stdin);

                events_len++;

            } else if (user_option == 2) // change schedule
            {

                end_actual_schedule = true;

            } else if (user_option == 3) // end program
            {
                end_actual_schedule = true;
                end_program = true;
            }

        }
        // so before opening another schedule we save the data on it's file
        if (events_len != -1) // if the actual file was not deleted, save it
            event_list_to_file(filename_schedule, event_list, events_len);
        
    }
    
    return 0;
}
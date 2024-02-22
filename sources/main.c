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
    MENU -> 2
        [0] - show events ->
            [0] - year
            [1] - year & month
            [2] - year & month & day
                return to MAIN

        
        [1] - add event ->
            return to MAIN

        [2] - change schedule
            [+] - New schedule
            [-] - Delete schedule
            [0...X] - Select schedule

        [3] - stop program
            return to MAIN
    */

    // setup
    const char SCHEDULES_PATH[MAX_PATH_LEN + MAX_FILENAME_LEN];
    const char MAIN_EXE_PATH[MAX_PATH_LEN];

    get_exe_path((char*)MAIN_EXE_PATH);
    sprintf((char*)SCHEDULES_PATH, "%s%s", (char*)MAIN_EXE_PATH, MAIN_EXE_TO_SCH_REL_PATH);

    
    // local variables    
    char user_option[MAX_TEXT_INPUT_OPTION_LEN];
    int noramlized_user_option;


    date_event_t event_list[MAX_EVENTS];
    int events_len;

    bool end_program = false;
    bool end_actual_schedule;

    // end local variables

    while (!end_program)
    {
        // reseting variables for the next schedule import
        end_actual_schedule = false;

        // select the schedule file
        char available_schedules[MAX_SCHEDULES][MAX_FILENAME_LEN];
        char opened_schedule_path[MAX_FILENAME_LEN];

        ls((char*)SCHEDULES_PATH, available_schedules, ".txt"); // get the available schedules

        show_available_schedules(available_schedules);
        
        // get the user input
        fgets(user_option, MAX_TEXT_INPUT_OPTION_LEN, stdin);

        // normalize the input
        noramlized_user_option = normalize_user_input_text(user_option);

        operate_schedule_menu_option(noramlized_user_option, SCHEDULES_PATH, available_schedules, event_list, &events_len, opened_schedule_path);
                
        if (events_len >= 0)
            printf("\nEvents: %d", events_len);

        if (events_len == -1)
        {
            // the option was to delete an schedule the user has to choose another
            end_actual_schedule = true;
        }

        while (!end_actual_schedule)
        {
            // let the user choose an option
            int user_option_end_prog_menu;
            do 
            {
                printf("\nSelect an option ->\n");
                    printf("\t[0] - Show events\n");
                    printf("\t[1] - Add event\n");
                    printf("\t[2] - Delete event\n");
                    printf("\t[3] - Change schedule\n");
                    printf("\t[4] - End program\n");
                scanf("%d", &user_option_end_prog_menu);

            } while ((user_option_end_prog_menu < 0) || (user_option_end_prog_menu > 4));


            // separate both menus
            printf("\n");

            if (user_option_end_prog_menu == 0) // list events of
            {
                if (events_len > 0) // check if there is any event saved
                {
                    date_event_t filtered_events[MAX_EVENTS];
                    get_events_filtered_by_user(event_list, filtered_events);

                    // then after getting the filtered data -> show the data
                    show_events_by(filtered_events, -1, -1, -1);

                } else
                {
                    printf("There are no events to show");
                }

                        

            } else if (user_option_end_prog_menu == 1) // add event
            {
                // when a new event is added is necessary to re-write the centinella at the new end of the list
                // it's necessary too to add a new id, so this will be the (events_len)
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

                // add the centinela & the new id
                event_list[events_len].id = (events_len);
                put_centinela_event(&event_list[events_len + 1]);

                events_len++;

            } else if (user_option_end_prog_menu == 2) // delete event
            {
                if (events_len > 0) // check if there is any event saved
                {
                    date_event_t filtered_events[MAX_EVENTS];
                    get_events_filtered_by_user(event_list, filtered_events);

                    // then after getting the filtered data -> show the data
                    show_events_by(filtered_events, -1, -1, -1);
                    

                    unsigned id_event_to_delete;
                    do
                    {
                        printf("Enter the id of the event to delete: ");
                        scanf(" %u", &id_event_to_delete);

                    } while ((id_event_to_delete < 0) && (id_event_to_delete >= events_len));

                    delete_event(event_list, id_event_to_delete);

                } else
                {
                    printf("There are no events to delete");
                }

            } else if (user_option_end_prog_menu == 3) // change schedule
            {
                char save_schedule_path[MAX_PATH_LEN];
                event_list_to_file(opened_schedule_path, event_list);
                end_actual_schedule = true;

            } else if (user_option_end_prog_menu == 4) // end program
            {
                char save_schedule_path[MAX_PATH_LEN];
                event_list_to_file(opened_schedule_path, event_list);
                end_actual_schedule = true;
                end_program = true;
            }

        }

        fflush2();
        
    }
    
    return 0;
}
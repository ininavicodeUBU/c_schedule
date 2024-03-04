#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

// personal include
#include "../include/GUI_constants.h"
#include "../include/GUI_func.h"
#include "../include/func.h"
#include "../include/constants.h"


// Global variables ++++++++++++++++++++++++++++++++++++++++++++++++++++
GUI_data_t GUI_data;

// -----------------------------------------------------------------


// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    switch (msg)
    {

        case WM_CREATE:

            break;
    
        case WM_COMMAND:

            // combo boxes +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            if (HIWORD(wParam) == CBN_SELCHANGE) {

                // NO SCHEDULE SELECTED MENU
                // select/delete schedule combo box
                if (GUI_data.menu_state[0] == NO_SCHEDULE_SELECTED)
                {
                    // user selects an schedule
                    if (LOWORD(wParam) == ID_SELECT_SCHEDULE_CBX)
                    {
                        // update the menu state
                        GUI_data.menu_state[0] = SCHEDULE_SELECTED;

                        // Combo box selection changed
                        GUI_data.combo_boxes[ID_SELECT_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET] = GetDlgItem(hwnd, ID_SELECT_SCHEDULE_CBX);

                        // Get the index of the selected item
                        int selectedIndex = SendMessage(GUI_data.combo_boxes[ID_SELECT_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_GETCURSEL, 0, 0);

                        // Get the text of the selected item
                        char buffer[256];
                        SendMessage(GUI_data.combo_boxes[ID_SELECT_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);

                        // then we actualize the selected schedule to the variables (if the selected one is not "None" option)
                        // not null schedule selected
                        if (selectedIndex > 0)
                        {
                            // substract 1 to the index because the first option is "None" so the first schedule is on index=1
                            sprintf(GUI_data.selected_schedule_path, "%s%s", GUI_data.schedules_folder_path, GUI_data.names_of_available_schedules[selectedIndex - 1]);

                            // download the events of the selected schedule and store the max (id - 1) = events_len
                            // to be able to append new events at the end of the list
                            file_to_event_list(GUI_data.selected_schedule_path, GUI_data.last_downloaded_events, &GUI_data.events_max_id);
                            GUI_data.events_max_id--;

                            // hide the items of the <no schedule selected menu>
                            hide_no_schedule_selected_menu(&GUI_data);

                            // show the items for the <schedule selected> menu
                            show_date_showing_combo_boxes(&GUI_data);
                            show_days_of_selected_showing_date(&GUI_data);
                            show_save_button(&GUI_data);

                            // show the elements for the new event
                            show_GUI_event_elements(&GUI_data.GUI_event_new);
                            ShowWindow(GUI_data.buttons[ID_NEW_EVENT], true);

                            paint_days_to_default_color(&GUI_data);

                            // getting the events of the showing date
                            get_events_by(GUI_data.last_downloaded_events, GUI_data.showing_date.year, GUI_data.showing_date.month, -1, GUI_data.events_of_showing_date);
                            
                            paint_days_with_events(&GUI_data);

                            // inform to the user which schedule is selected
                            char str [MAX_PATH_LEN];
                            sprintf(str, "%s", GUI_data.names_of_available_schedules[selectedIndex - 1]);
                            printf("\nname opening %s ", str);
                            // MessageBox(hwnd, str, "Schedule selected", MB_OK);                        
        
                        }
                        
                        
                    }

                    // users selects an schedule from delete combo box
                    else if (LOWORD(wParam) == ID_DELETE_SCHEDULE_CBX)
                    {
                        // Combo box selection changed
                        GUI_data.combo_boxes[ID_DELETE_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET] = GetDlgItem(hwnd, ID_DELETE_SCHEDULE_CBX);

                        // Get the index of the selected item
                        int selectedIndex = SendMessage(GUI_data.combo_boxes[ID_DELETE_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_GETCURSEL, 0, 0);

                        // Get the text of the selected item
                        char buffer[256];
                        SendMessage(GUI_data.combo_boxes[ID_DELETE_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);

                        // index 0 is "None" option
                        if (selectedIndex > 0)
                        {
                            // then delete the selected file
                            char full_path_to_delete [MAX_PATH_LEN];
                            sprintf(full_path_to_delete, "%s%s", GUI_data.schedules_folder_path,
                            GUI_data.names_of_available_schedules[selectedIndex - 1]);

                            printf("\nindex: %d, name of the schedule selected: %s", selectedIndex, GUI_data.names_of_available_schedules[selectedIndex - 1]);
                            delete_file(full_path_to_delete);

                            ls(GUI_data.schedules_folder_path, GUI_data.names_of_available_schedules, ".txt");

                            // reset the combo box to the option "None"
                            refresh_available_schedules_combo_boxes(&GUI_data);

                        }

                    }

                }
                
                // SCHEDULE SELECTED MENU ,undefined state
                // user changes the showing date
                else if (GUI_data.menu_state[0] == SCHEDULE_SELECTED)
                {
                    // actualize the available schedules for the new month/year selected
                    // no matters the [1] of the menu state
                    if (LOWORD(wParam) == ID_MONTH_SHOWING_DATE_CBX || LOWORD(wParam) == ID_YEAR_SHOWING_DATE_CBX)
                    {
                        // in case of having [1] = selected_day, then we have to hide all the elements of the events
                        if (GUI_data.menu_state[1] == DAY_SELECTED)
                        {
                            save_elements_data_to_events(&GUI_data);

                            for (int i = 0; i < MAX_DISPLAYING_EVENTS; i++)
                                hide_GUI_event_elements(&GUI_data.GUI_events_list[i]);
                        }
                        // -----------

                        // changes the month
                        if (LOWORD(wParam) == ID_MONTH_SHOWING_DATE_CBX)
                        {
                            // Combo box selection changed
                            GUI_data.combo_boxes[ID_MONTH_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET] = GetDlgItem(hwnd, ID_MONTH_SHOWING_DATE_CBX);

                            // Get the index of the selected item
                            int selectedIndex = SendMessage(GUI_data.combo_boxes[ID_MONTH_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], CB_GETCURSEL, 0, 0);

                            // Get the text of the selected item
                            char buffer[256];
                            SendMessage(GUI_data.combo_boxes[ID_MONTH_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);

                            // update the changes to the variables
                            GUI_data.showing_date.month = selectedIndex + 1;

                        }
                        
                        // changes the year
                        else if (LOWORD(wParam) == ID_YEAR_SHOWING_DATE_CBX)
                        {
                            // Combo box selection changed
                            GUI_data.combo_boxes[ID_YEAR_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET] = GetDlgItem(hwnd, ID_YEAR_SHOWING_DATE_CBX);

                            // Get the index of the selected item
                            int selectedIndex = SendMessage(GUI_data.combo_boxes[ID_YEAR_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], CB_GETCURSEL, 0, 0);

                            // Get the text of the selected item
                            char buffer[256];
                            SendMessage(GUI_data.combo_boxes[ID_YEAR_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);

                            // update the changes to the variables
                            GUI_data.showing_date.year = selectedIndex + GUI_data.local_time_date.year - 1;
                        }

                        // and actualize the menu, so now we go to <no day selected>
                        GUI_data.menu_state[1] = UNDEFINED_MENU_STATE;
                        get_events_by(GUI_data.last_downloaded_events, GUI_data.showing_date.year, GUI_data.showing_date.month, -1, GUI_data.events_of_showing_date);

                        paint_days_to_default_color(&GUI_data);
                        paint_days_with_events(&GUI_data);
                    }
                } 

            }
            // --------------------------------------------------------------------------------------------------

            switch (LOWORD(wParam))
            {
                // buttons ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                case ID_NEW_SCHEDULE_BUTTON:
                // ########## FUNCTIONALITY NO IMPLEMENTED #####################
                    hide_no_schedule_selected_menu(&GUI_data);

                    // show the input box
                    ShowWindow(GUI_data.text_input_boxes[ID_NEW_SCHEDULE_NAME_INBOX + INDEX_INBOX_OFFSET], true);
                    // show the confirm button
                    ShowWindow(GUI_data.buttons[ID_SAVE_NEW_SCHEDULE_NAME], true);

                    // update the menu
                    GUI_data.menu_state[1] = CREATING_NEW_SCHEDULE;

                    break;

                case ID_SAVE_NEW_SCHEDULE_NAME: 
                    if (true)
                    {
                        // get the name of the file
                        char text_on_box[MAX_FILENAME_LEN];
                        get_in_box_text(GUI_data.text_input_boxes[ID_NEW_SCHEDULE_NAME_INBOX + INDEX_INBOX_OFFSET], text_on_box, sizeof(text_on_box));

                        char full_path_to_save [MAX_PATH_LEN];
                        sprintf(full_path_to_save, "%s%s%s", GUI_data.schedules_folder_path,
                                text_on_box, ".txt");

                        // create the file
                        new_file(full_path_to_save);

                        // get the list of schedules from the directory
                        ls(GUI_data.schedules_folder_path, GUI_data.names_of_available_schedules, ".txt");

                        // reset the combo box to the option "None"
                        refresh_available_schedules_combo_boxes(&GUI_data);

                        // and then show another time the menu of <no selected schedule>
                        show_no_schedule_selected_menu(&GUI_data);

                        // hide the elements of the creating schedule menu
                        ShowWindow(GUI_data.text_input_boxes[ID_NEW_SCHEDULE_NAME_INBOX + INDEX_INBOX_OFFSET], false);
                        ShowWindow(GUI_data.buttons[ID_SAVE_NEW_SCHEDULE_NAME], false);

                        // update the menu state
                        GUI_data.menu_state[1] == UNDEFINED_MENU_STATE;

                        // comunicate to the user
                        MessageBox(hwnd, "Schedule saved", "Hey!", MB_OK);

                    }

                    break;

                case ID_SAVE_SELECTED_SCHEDULE:

                    if (GUI_data.menu_state[0] == SCHEDULE_SELECTED)
                    {

                        if (GUI_data.menu_state[1] == DAY_SELECTED)
                        {
                            // no matters the state of [1] menu variable, but in case of [1] = DAY_SELECTED, it's necessary save the
                            // the events from the combo boxes and input boxes too
                            // hide the elements too
                            // hide the elements for the new event
                            if (GUI_data.menu_state[0] == SCHEDULE_SELECTED && GUI_data.menu_state[1] == DAY_SELECTED)
                            {
                                save_elements_data_to_events(&GUI_data);

                                // the save the changes to the file
                                event_list_to_file(GUI_data.selected_schedule_path, GUI_data.last_downloaded_events);

                                for (int i = 0; i < MAX_DISPLAYING_EVENTS; i++)
                                    hide_GUI_event_elements(&GUI_data.GUI_events_list[i]);

                                hide_GUI_event_elements(&GUI_data.GUI_event_new);
                                ShowWindow(GUI_data.buttons[ID_NEW_EVENT], false);

                            }
                        }

                        // actualize menu state
                        GUI_data.menu_state[0] = NO_SCHEDULE_SELECTED;
                        GUI_data.menu_state[1] = UNDEFINED_MENU_STATE;

                        // save the data of the schedule
                        event_list_to_file(GUI_data.selected_schedule_path, GUI_data.last_downloaded_events);

                        // set another the "None schedule selected option on the combo box"
                        SendMessage(GUI_data.combo_boxes[ID_SELECT_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_SETCURSEL, 0, 0); 
                        
                        MessageBox(hwnd, "Saving actual schedule", "window", MB_OK);
                        hide_selected_schedule_menu(&GUI_data);
                        show_no_schedule_selected_menu(&GUI_data);

                        // ----------------------------------------------------------------------------------------------------------------------------



                    }
                    break;

                case ID_NEW_EVENT:
                    if (GUI_data.menu_state[0] == SCHEDULE_SELECTED)
                    {
                         // append this event to the next available id and increment that id for the next append action
                        int id = ++GUI_data.events_max_id;
                                                            
                        int day, month, year, hour, min;
                        printf("\nSaving id: %d", id);

                        // Get the selected index from the combo box
                        day = SendMessage(GUI_data.GUI_event_new.GUI_elements[0], CB_GETCURSEL, 0, 0) + 1;
                        month = SendMessage(GUI_data.GUI_event_new.GUI_elements[1], CB_GETCURSEL, 0, 0) + 1;
                        year = SendMessage(GUI_data.GUI_event_new.GUI_elements[2], CB_GETCURSEL, 0, 0) + GUI_data.local_time_date.year - 1;
                        hour = SendMessage(GUI_data.GUI_event_new.GUI_elements[3], CB_GETCURSEL, 0, 0);
                        min = SendMessage(GUI_data.GUI_event_new.GUI_elements[4], CB_GETCURSEL, 0, 0);
                        get_in_box_text(GUI_data.GUI_event_new.GUI_elements[5],
                        GUI_data.last_downloaded_events[id].description,
                        sizeof(GUI_data.last_downloaded_events[id].description));

                        GUI_data.last_downloaded_events[id].id = id;
                        GUI_data.last_downloaded_events[id].date.day = day;
                        GUI_data.last_downloaded_events[id].date.month = month;
                        GUI_data.last_downloaded_events[id].date.year = year;
                        GUI_data.last_downloaded_events[id].date.hour = hour;
                        GUI_data.last_downloaded_events[id].date.minute = min;

                        // then add the new centinela at the new end of the events list
                        put_centinela_event(&GUI_data.last_downloaded_events[id + 1]);

                        // then prepare the elements of the new event to get another one (description to "")
                        // GUI_data.last_downloaded_events[id + 1].description = '\0'; // end of the string at the first element
                        char temp[2]; temp[0] = '\0';
                        set_in_box_text(GUI_data.GUI_event_new.GUI_elements[5], temp);

                        // refreshing the GUI ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        // before any changes, save the data of the elements on the events list
                        save_elements_data_to_events(&GUI_data);

                        paint_days_to_default_color(&GUI_data);
                        paint_days_with_events(&GUI_data);
                        paint_selected_day(&GUI_data);

                        refresh_showing_events(&GUI_data);

                        // -----------------------------------------------------------------------------------

                        // and confirm the action to the user
                        MessageBox(hwnd, "New event saved", "window", MB_OK);
                    }

                    break;
                
                // delete event button ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // in the range of the event blocks
                case 300 ... (ID_FIRST_EVENT_BLOCK + ELEMENTS_BY_EVENT_BLOCK * MAX_DISPLAYING_EVENTS):

                    if (GUI_data.menu_state[0] == SCHEDULE_SELECTED && GUI_data.menu_state[1] == DAY_SELECTED)
                    {
                        if ((wParam - ID_FIRST_EVENT_BLOCK) % ELEMENTS_BY_EVENT_BLOCK == 6) 
                        {
                            // getting the block of the delete event button pressed
                            int block_id = (wParam - ID_FIRST_EVENT_BLOCK) / ELEMENTS_BY_EVENT_BLOCK;

                            // delete the event 
                            delete_event(GUI_data.last_downloaded_events, GUI_data.GUI_events_list[0].date_event.id);

                            // refresh the showing events
                            save_elements_data_to_events(&GUI_data);

                            // repaint the month days
                            paint_days_to_default_color(&GUI_data);
                            paint_days_with_events(&GUI_data);
                            paint_selected_day(&GUI_data);

                            refresh_showing_events(&GUI_data);

                        }

                        
                        
                    }

                    break;
                // ----------------------------------------------------------------------------------------------------

                default:
                    // user selects a day for first time on the month
                    // SCHEDULE SELECTED MENU ,undefined state
                    if (GUI_data.menu_state[0] == SCHEDULE_SELECTED && GUI_data.menu_state[1] == UNDEFINED_MENU_STATE)
                    {
                        if (wParam >= ID_FIRST_DAY_OF_MONTH && wParam <= ID_LAST_DAY_OF_MONTH)
                        {
                            // update the menu state
                            GUI_data.menu_state[1] = DAY_SELECTED;

                            // update to the variables the selected day
                            GUI_data.showing_date.day = wParam - ID_FIRST_DAY_OF_MONTH + 1;

                            paint_selected_day(&GUI_data);

                            refresh_showing_events(&GUI_data);
                        }

                        
                        

                        return true; // (break)

                    } 

                    // SCHEDULE SELECTED MENU ,day selected            
                    else if (GUI_data.menu_state[0] == SCHEDULE_SELECTED && GUI_data.menu_state[1] == DAY_SELECTED)
                    {
                        // user selects a day
                        if (wParam >= ID_FIRST_DAY_OF_MONTH && wParam <= ID_LAST_DAY_OF_MONTH)
                        {
                            // before any changes, save the data of the elements on the events list
                            save_elements_data_to_events(&GUI_data);

                            // update to the variables the selected day
                            GUI_data.showing_date.day = wParam - ID_FIRST_DAY_OF_MONTH + 1;

                            paint_days_to_default_color(&GUI_data);
                            paint_days_with_events(&GUI_data);
                            paint_selected_day(&GUI_data);

                            refresh_showing_events(&GUI_data);
                        }

                    }

                    break;


                // --------------------------------------------------------------------------------------------------
            }

            break;

        case WM_DRAWITEM:

            if (wParam == ID_NEW_SCHEDULE_BUTTON) // buttons
            {
                LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                DrawCustomButton(lpdis, RGB(87, 155, 18), "New schedule");
                return true;
            } else if (wParam == ID_SAVE_NEW_SCHEDULE_NAME)
            {
                LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                DrawCustomButton(lpdis, RGB(87, 155, 18), "Save");
                return true;
            } else if (wParam == ID_CANCEL_NEW_SCHEDULE_NAME)
            {
                LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                DrawCustomButton(lpdis, RGB(235, 48, 48), "Cancel");
                return true;
            } else if (wParam == ID_SAVE_SELECTED_SCHEDULE) // buttons
            {
                LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                DrawCustomButton(lpdis, RGB_SAVE_SELECTED_SCHEDULE, "S");
                return true;
            } else if (wParam >= ID_FIRST_DAY_OF_MONTH && wParam <= ID_LAST_DAY_OF_MONTH)
            {
                char text_of_button [3];
                // the text of the button is the ID - offset = 0 but starts in 1 so +1
                sprintf(text_of_button, "%d", wParam - ID_FIRST_DAY_OF_MONTH + 1);

                LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                DrawCustomButton(lpdis, GUI_data.colors_of_buttons[wParam], text_of_button);
                return true;
            } else if (wParam == ID_NEW_EVENT)
            {
                LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                DrawCustomButton(lpdis, RGB_NEW_EVENT, "New event");
            } else if (wParam >= ID_FIRST_EVENT_BLOCK)
            {
                if ((wParam - ID_FIRST_EVENT_BLOCK) % ELEMENTS_BY_EVENT_BLOCK == 6) //  that indicades delete button
                {
                    LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                    DrawCustomButton(lpdis, RGB_DELETE_EVENT, "X");
                }
            }
            
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    
    }
    return 0;
}

int main()
{

    // setup ++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = "MyWindowClass";
    RegisterClass(&wc);

    GUI_init(&GUI_data);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

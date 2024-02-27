#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>


#include "../include/GUI_constants.h"
#include "../include/GUI_func.h"
#include "../include/func.h"
#include "../include/constants.h"


// Global variables ++++++++++++++++++++++++++++++++++++++++++++++++++++
// labels and buttons
HWND buttons_of_the_days[31];
HWND labels_of_menus [MAX_MENUS][MAX_LABELS_BY_MENU];
HWND buttons_of_menus [MAX_MENUS][MAX_BUTTONS_BY_MENU];
HWND buttons_of_schedules [MAX_SCHEDULES];
COLORREF colors_of_the_buttons [MAX_BUTTONS_BY_MENU];
HWND days_combo_box, months_combo_box, year_combo_box;


// menu control
GUI_t GUI;

// schedules path variables
const char SCHEDULES_PATH[MAX_PATH_LEN + MAX_FILENAME_LEN];
const char MAIN_EXE_PATH[MAX_PATH_LEN];


// events list and schedules (last downloaded) 
date_event_t events_of_the_month[MAX_EVENTS], full_events_list[MAX_EVENTS];
char available_schedules[MAX_SCHEDULES][MAX_FILENAME_LEN];
char full_path_actual_schedule[MAX_PATH_LEN];


// -----------------------------------------------------------------





// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    
        case WM_CREATE:
            create_buttons_days(hwnd, buttons_of_the_days);
            create_buttons_schedules(hwnd, buttons_of_schedules);

            create_combo_boxes(hwnd, days_combo_box, months_combo_box, year_combo_box);

            break;

        case WM_COMMAND:
        
            switch (LOWORD(wParam))
            {
                case DELETE_SCHEDULE: 
                    MessageBox(hwnd, "Delete Schedule button_new_sch clicked!", "Button Clicked", MB_OK);

                    break;

                case NEW_SCHEDULE:
                    MessageBox(hwnd, "New Schedule button_new_sch clicked!", "Button Clicked", MB_OK);


                    break;

                default:

                    if (LOWORD(wParam) < 100)
                    {
                        
                        // select and download the schedule file
                        ls((char*)(SCHEDULES_PATH), available_schedules, ".txt");

                        sprintf(full_path_actual_schedule, "%s%s", SCHEDULES_PATH, available_schedules[LOWORD(wParam) - 2]); 

                        int n_events_temp;
                        file_to_event_list(full_path_actual_schedule, full_events_list, &n_events_temp);

                        get_events_by(full_events_list, GUI.showing_date.year, GUI.showing_date.month, -1, events_of_the_month);
                        
                        make_visible_buttons_days(hwnd, buttons_of_the_days);
                        paint_day_buttons (hwnd, buttons_of_the_days, events_of_the_month, colors_of_the_buttons);

                    }
                    else if (LOWORD(wParam) < 200)
                    {
                        char message_str [50];
                        sprintf(message_str, "Day %d clicked", LOWORD(wParam) - 100 + 1);
                        MessageBox(hwnd, message_str, "Button Clicked", MB_OK);
                        
                    } else if ((LOWORD(wParam) < 300))
                    {
                        printf("\nloword: %d", (int)(LOWORD(wParam)));
                    
                        if (LOWORD(wParam) == CMB_BOX_DAYS_ID && HIWORD(wParam) == CBN_SELCHANGE) {
                        // Combo box selection changed
                        HWND comboBox = GetDlgItem(hwnd, 1);

                        // Get the index of the selected item
                        int selectedIndex = SendMessage(comboBox, CB_GETCURSEL, 0, 0);

                        // Get the text of the selected item
                        char buffer[256];
                        SendMessage(comboBox, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);

                        // Display the selected item
                        MessageBox(hwnd, buffer, "Selected Item", MB_OK);
                    }
                        
                    }

            }
            break;

        case WM_DRAWITEM:
            switch(wParam)
            {
                case DELETE_SCHEDULE:
                if (true)
                {
                    LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                    DrawCustomButton(lpdis, RGB(202, 65, 65), "Delete schedule");
                    break;

                }

                case NEW_SCHEDULE:
                if (true) // necessary to create local variables
                {
                    LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                    DrawCustomButton(lpdis, RGB(109, 240, 131), "New schedule");
                    break;  

                }

                default:
                if (true)
                {
                    LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                    char num_to_str [4];
                    sprintf(num_to_str, "%d", wParam - 100 + 1);
                    // assign the color of the respective button
                    DrawCustomButton(lpdis, colors_of_the_buttons[wParam - 100], num_to_str);
                }
                    break;
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
    get_exe_path((char*)MAIN_EXE_PATH);
    sprintf((char*)SCHEDULES_PATH, "%s%s", (char*)MAIN_EXE_PATH, MAIN_EXE_TO_SCH_REL_PATH);

    // ############################################
    // BY DEFAULT WE WILL SET DE GUI DATE SHOWING AS 10/2023 UNTIL THE DATE
    // CAN BE OBTANINED BY THE LOCAL TIME
    // ############################################
    GUI.showing_date.year = 2023;
    GUI.showing_date.month = 10;

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = "MyWindowClass";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        "MyWindowClass",
        "My Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

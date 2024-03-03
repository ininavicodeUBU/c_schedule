#include <windows.h>

#include "../include/GUI_func.h"
#include "../include/GUI_constants.h"


// Function to set the background color of the "New Schedule" button
void SetButtonBackgroundColor(HWND hwndButton, COLORREF color)
{
    SendMessage(hwndButton, BM_SETSTYLE, BS_OWNERDRAW, TRUE);
    DeleteObject((HBRUSH)GetClassLongPtr(hwndButton, GCLP_HBRBACKGROUND));
    SetClassLongPtr(hwndButton, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(color));
}

void DrawCustomButton(LPDRAWITEMSTRUCT lpdis, COLORREF bgColor, char text[])
{
    SetBkColor(lpdis->hDC, bgColor);
    ExtTextOut(lpdis->hDC, 0, 0, ETO_OPAQUE, &lpdis->rcItem, NULL, 0, NULL);
    DrawText(lpdis->hDC, text, -1, &lpdis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


// initialization +++++++++++++++++++++++++++++++++++++++
void GUI_init (GUI_data_t* GUI_data)
{
    // .exe path
    get_exe_path((char*)GUI_data->exe_file_path);

    // get schedules folder
    sprintf(GUI_data->schedules_folder_path, "%s%s", GUI_data->exe_file_path, MAIN_EXE_TO_SCH_REL_PATH);

    // available schedules
    ls(GUI_data->schedules_folder_path, GUI_data->names_of_available_schedules, ".txt");

    // save the local time to get that showing date by default
    get_local_time(&GUI_data->local_time_date);
    get_local_time(&GUI_data->showing_date);

    // initializing variables
    put_centinela_event(&GUI_data->last_downloaded_events[0]);
    put_centinela_event(&GUI_data->events_of_showing_date[0]);

    // menu state
    GUI_data->menu_state[0] = NO_SCHEDULE_SELECTED;


    // generating GUI elements +++++++++++++++++++++++++
    // main screen
    GUI_data->GUI_main_screen = CreateWindowEx(
        0,
        "MyWindowClass",
        "Windows Scheduler",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(GUI_data->GUI_main_screen, SW_SHOWDEFAULT);
    UpdateWindow(GUI_data->GUI_main_screen);

    // generating buttons
    GUI_data->buttons[ID_NEW_SCHEDULE_BUTTON] = CreateWindow(
        "BUTTON",
        "New Schedule",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        10, 10, 120, 30,
        GUI_data->GUI_main_screen,
        (HMENU)ID_NEW_SCHEDULE_BUTTON, // Button ID
        (HINSTANCE)GetWindowLong(GUI_data->GUI_main_screen, GWL_HINSTANCE),
        NULL);
    SetButtonBackgroundColor(GUI_data->buttons[ID_NEW_SCHEDULE_BUTTON], RGB_NEW_SCHEDULE_BUTTON);
    GUI_data->colors_of_buttons[ID_NEW_SCHEDULE_BUTTON] = RGB_NEW_SCHEDULE_BUTTON;

    GUI_data->buttons[ID_SAVE_SELECTED_SCHEDULE] = CreateWindow(
        "BUTTON",
        "S",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        X_SAVE_SELECTED_SCHEDULE, Y_SAVE_SELECTED_SCHEDULE, WIDTH_SAVE_SELECTED_SCHEDULE, HEIGTH_SAVE_SELECTED_SCHEDULE,
        GUI_data->GUI_main_screen,
        (HMENU)ID_SAVE_SELECTED_SCHEDULE, // Button ID
        (HINSTANCE)GetWindowLong(GUI_data->GUI_main_screen, GWL_HINSTANCE),
        NULL);
    SetButtonBackgroundColor(GUI_data->buttons[ID_SAVE_SELECTED_SCHEDULE], RGB_SAVE_SELECTED_SCHEDULE);
    GUI_data->colors_of_buttons[ID_SAVE_SELECTED_SCHEDULE] = RGB_SAVE_SELECTED_SCHEDULE;
    

    GUI_data->buttons[ID_SAVE_NEW_SCHEDULE_NAME] = CreateWindow(
        "BUTTON",
        "Save",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        10, 10, 120, 30,
        GUI_data->GUI_main_screen,
        (HMENU)ID_SAVE_NEW_SCHEDULE_NAME, // Button ID
        (HINSTANCE)GetWindowLong(GUI_data->GUI_main_screen, GWL_HINSTANCE),
        NULL);
    SetButtonBackgroundColor(GUI_data->buttons[ID_SAVE_NEW_SCHEDULE_NAME], RGB_SAVE_NEW_SCHEDULE_NAME);
    GUI_data->colors_of_buttons[ID_SAVE_NEW_SCHEDULE_NAME] = RGB_SAVE_NEW_SCHEDULE_NAME;

    GUI_data->buttons[ID_CANCEL_NEW_SCHEDULE_NAME] = CreateWindow(
        "BUTTON",
        "Save",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        10, 10, 120, 30,
        GUI_data->GUI_main_screen,
        (HMENU)ID_CANCEL_NEW_SCHEDULE_NAME, // Button ID
        (HINSTANCE)GetWindowLong(GUI_data->GUI_main_screen, GWL_HINSTANCE),
        NULL);
    SetButtonBackgroundColor(GUI_data->buttons[ID_CANCEL_NEW_SCHEDULE_NAME], RGB_CANCEL_NEW_SCHEDULE_NAME);
    GUI_data->colors_of_buttons[ID_CANCEL_NEW_SCHEDULE_NAME] = RGB_CANCEL_NEW_SCHEDULE_NAME;

    for (int i = 0; i <= 30; i++) // 31 DAYS of the month (max)
    {
        char name_of_button[3];
        sprintf(name_of_button, "%d", i + 1);
        GUI_data->buttons[i + ID_FIRST_DAY_OF_MONTH] = CreateWindow(
            "BUTTON",
            name_of_button,
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            X_FIRST_DAY_OF_MONTH + (i % 7) * WIDTH_FIRST_DAY_OF_MONTH,
            Y_FIRST_DAY_OF_MONTH + (int)(i / 7) * HEIGTH_FIRST_DAY_OF_MONTH, WIDTH_FIRST_DAY_OF_MONTH, HEIGTH_FIRST_DAY_OF_MONTH,
            GUI_data->GUI_main_screen,
            (HMENU)(i + ID_FIRST_DAY_OF_MONTH),
            (HINSTANCE)GetWindowLong(GUI_data->GUI_main_screen, GWL_HINSTANCE),
            NULL);
    }

   
    
    // generating combo boxes
    GUI_data->combo_boxes[ID_DELETE_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET] = CreateWindow(
        "COMBOBOX",
        NULL,
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VSCROLL | WS_VISIBLE,
        X_DELETE_SCHEDULE_CBX, Y_DELETE_SCHEDULE_CBX, WIDTH_DELETE_SCHEDULE_CBX, HEIGTH_DELETE_SCHEDULE_CBX,
        GUI_data->GUI_main_screen,
        (HMENU)ID_DELETE_SCHEDULE_CBX, // Button ID
        GetModuleHandle(NULL),
        NULL);
    
    GUI_data->combo_boxes[ID_SELECT_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET] = CreateWindow(
        "COMBOBOX",
        NULL,
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VSCROLL | WS_VISIBLE,
        X_SELECT_SCHEDULE_CBX, Y_SELECT_SCHEDULE_CBX, WIDTH_SELECT_SCHEDULE_CBX, HEIGTH_SELECT_SCHEDULE_CBX,
        GUI_data->GUI_main_screen,
        (HMENU)ID_SELECT_SCHEDULE_CBX, // Button ID
        GetModuleHandle(NULL),
        NULL);

    GUI_data->combo_boxes[ID_MONTH_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET] = CreateWindow(
        "COMBOBOX",
        NULL,
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VSCROLL | WS_VISIBLE,
        X_MONTH_SHOWING_DATE_CBX, Y_MONTH_SHOWING_DATE_CBX, WIDTH_MONTH_SHOWING_DATE_CBX, HEIGTH_MONTH_SHOWING_DATE_CBX,
        GUI_data->GUI_main_screen,
        (HMENU)ID_MONTH_SHOWING_DATE_CBX, // Button ID
        GetModuleHandle(NULL),
        NULL);

    GUI_data->combo_boxes[ID_YEAR_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET] = CreateWindow(
        "COMBOBOX",
        NULL,
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VSCROLL | WS_VISIBLE,
        X_YEAR_SHOWING_DATE_CBX, Y_YEAR_SHOWING_DATE_CBX, WIDTH_YEAR_SHOWING_DATE_CBX, HEIGTH_YEAR_SHOWING_DATE_CBX,
        GUI_data->GUI_main_screen,
        (HMENU)ID_YEAR_SHOWING_DATE_CBX, // Button ID
        GetModuleHandle(NULL),
        NULL);

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // adding options to combo boxes
    refresh_available_schedules_combo_boxes(GUI_data);
    const char months_list[12][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    for (int i = 0; i < 12; i++)
        SendMessage(GUI_data->combo_boxes[ID_MONTH_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], CB_ADDSTRING, 0, (LPARAM)months_list[i]);
    
    for (int i = 0; i < 30; i++) // will be displaying 30 years from the local date - 1
    {
        char year_to_str[5];
        sprintf(year_to_str, "%d", i + GUI_data->showing_date.year - 1);
        SendMessage(GUI_data->combo_boxes[ID_YEAR_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], CB_ADDSTRING, 0, (LPARAM)year_to_str);
    }

    // adding the default date by the local time
    SendMessage(GUI_data->combo_boxes[ID_MONTH_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], CB_SETCURSEL, GUI_data->showing_date.month - 1, 0); 
    SendMessage(GUI_data->combo_boxes[ID_YEAR_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], CB_SETCURSEL,  1, 0); 
    // -------------------------------------------------------------------------------------------------------------------------------------------------

    // hiding elements ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ShowWindow(GUI_data->buttons[ID_SAVE_SELECTED_SCHEDULE], false);

    ShowWindow(GUI_data->buttons[ID_SAVE_NEW_SCHEDULE_NAME], false);
    ShowWindow(GUI_data->buttons[ID_CANCEL_NEW_SCHEDULE_NAME], false);

    ShowWindow(GUI_data->combo_boxes[ID_MONTH_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], false);
    ShowWindow(GUI_data->combo_boxes[ID_YEAR_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], false);

    for (int i = ID_FIRST_DAY_OF_MONTH; i <= ID_LAST_DAY_OF_MONTH; i++)
        ShowWindow(GUI_data->buttons[i], false);
    
    // -------------------------------------------------------------------------------------------------------------------------------------------------

    // showing elements
    show_no_schedule_selected_menu(GUI_data);

}

// -----------------------------------------------------

// hiding elements +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void hide_no_schedule_selected_menu (GUI_data_t* GUI_data)
{
    // hiding the combo boxes that does not participate on the <no selected schedule> menu
    ShowWindow(GUI_data->buttons[ID_NEW_SCHEDULE_BUTTON], false);
    ShowWindow(GUI_data->combo_boxes[ID_SELECT_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], false);
    ShowWindow(GUI_data->combo_boxes[ID_DELETE_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], false);
}

void hide_selected_schedule_menu (GUI_data_t* GUI_data)
{
    ShowWindow(GUI_data->buttons[ID_SAVE_SELECTED_SCHEDULE], false);

    for (int i = ID_FIRST_DAY_OF_MONTH; i <= ID_LAST_DAY_OF_MONTH; i++)
        ShowWindow(GUI_data->buttons[i], false);

    ShowWindow(GUI_data->combo_boxes[ID_MONTH_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], false);
    ShowWindow(GUI_data->combo_boxes[ID_YEAR_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], false);
    
}
// -------------------------------------------------------------------------------------------

// showing elements +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void show_date_showing_combo_boxes (GUI_data_t* GUI_data)
{
    ShowWindow(GUI_data->combo_boxes[ID_MONTH_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], true);
    ShowWindow(GUI_data->combo_boxes[ID_YEAR_SHOWING_DATE_CBX + ID_COMBO_BOX_OFFSET], true);
}

void show_days_of_selected_showing_date (GUI_data_t* GUI_data)
{
    int MONTH_DAYS[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    for (int i = ID_FIRST_DAY_OF_MONTH; i < ID_FIRST_DAY_OF_MONTH + MONTH_DAYS[GUI_data->showing_date.month - 1]; i++)
        ShowWindow(GUI_data->buttons[i], true);

}

void show_save_button (GUI_data_t* GUI_data)
{
    ShowWindow(GUI_data->buttons[ID_SAVE_SELECTED_SCHEDULE], true);

}

void show_no_schedule_selected_menu (GUI_data_t* GUI_data)
{

    // hiding the combo boxes that does not participate on the <no selected schedule> menu
    ShowWindow(GUI_data->buttons[ID_NEW_SCHEDULE_BUTTON], true);
    ShowWindow(GUI_data->combo_boxes[ID_SELECT_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], true);
    ShowWindow(GUI_data->combo_boxes[ID_DELETE_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], true);

}
// -------------------------------------------------------------------------------------------

// refreshing ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void refresh_available_schedules_combo_boxes (GUI_data_t* GUI_data)
{
    // reset the content of combo boxes
    SendMessage(GUI_data->combo_boxes[ID_DELETE_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_RESETCONTENT, 0, 0);
    SendMessage(GUI_data->combo_boxes[ID_SELECT_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_RESETCONTENT, 0, 0);

    SendMessage(GUI_data->combo_boxes[ID_DELETE_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_ADDSTRING, 0, (LPARAM)"None");
    SendMessage(GUI_data->combo_boxes[ID_SELECT_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_ADDSTRING, 0, (LPARAM)"None");

    SendMessage(GUI_data->combo_boxes[ID_DELETE_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_SETCURSEL, 0, 0); // sets the default option on "None"
    SendMessage(GUI_data->combo_boxes[ID_SELECT_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_SETCURSEL, 0, 0); // sets the default option on "None"


    for (int i = 0; GUI_data->names_of_available_schedules[i][0] != '\0'; i++)
    {
        SendMessage(GUI_data->combo_boxes[ID_DELETE_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_ADDSTRING, 0, (LPARAM)GUI_data->names_of_available_schedules[i]);
        SendMessage(GUI_data->combo_boxes[ID_SELECT_SCHEDULE_CBX + ID_COMBO_BOX_OFFSET], CB_ADDSTRING, 0, (LPARAM)GUI_data->names_of_available_schedules[i]);
    }
}
// -------------------------------------------------------------------------------------------

// painting ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void paint_days_to_default_color (GUI_data_t* GUI_data)
{
    for (int i = 0; i <= 30; i++) // 31 DAYS of the month (max)
    {
        SetButtonBackgroundColor(GUI_data->buttons[i + ID_FIRST_DAY_OF_MONTH], RGB_DEFAULT_DAY);
        GUI_data->colors_of_buttons[i + ID_FIRST_DAY_OF_MONTH] = RGB_DEFAULT_DAY;
    }
}

void paint_days_with_events (GUI_data_t* GUI_data)
{
    int i = 0;
    while (!end_of_event_list(GUI_data->events_of_showing_date[i]))
    {
        SetButtonBackgroundColor(GUI_data->buttons[GUI_data->events_of_showing_date[i].date.day + ID_FIRST_DAY_OF_MONTH - 1], RGB_DAY_WITH_EVENT);
        GUI_data->colors_of_buttons[GUI_data->events_of_showing_date[i].date.day + ID_FIRST_DAY_OF_MONTH - 1] = RGB_DAY_WITH_EVENT;
        i++;
    }
    
}

// ---------------------------------------------------------------------------------------------

#include <windows.h>

#include "../include/GUI_func.h"


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
void create_buttons_days (HWND hwnd, HWND buttons_of_the_days [])
{
    for (int i = 0; i < 31; ++i)
    {
        char button_text [3];
        sprintf(button_text, "%d", (i + 1));
        buttons_of_the_days[i] = CreateWindow(
            "BUTTON",
            button_text,
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            DAY_BUTTONS_LEFT_MARGIN + (i % 7) * DAY_BUTTONS_WIDTH,
            DAY_BUTTONS_TOP_MARGIN + (int)(i / 7) * DAY_BUTTONS_HEIGHT,
            DAY_BUTTONS_WIDTH, DAY_BUTTONS_HEIGHT,
            hwnd,
            (HMENU)(i + 100), // buttons will start on 100
            (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
            NULL);

            // initialy all the days are not visible
            ShowWindow(buttons_of_the_days[i], SW_HIDE);

    }

}

void create_buttons_schedules (HWND hwnd, HWND buttons_of_schedules [])
{

    // creating the buttons for the available schedules
    const char SCHEDULES_PATH[MAX_PATH_LEN + MAX_FILENAME_LEN];
    const char MAIN_EXE_PATH[MAX_PATH_LEN];

    get_exe_path((char*)MAIN_EXE_PATH);
    sprintf((char*)SCHEDULES_PATH, "%s%s", (char*)MAIN_EXE_PATH, MAIN_EXE_TO_SCH_REL_PATH);
    
    date_event_t event_list[MAX_EVENTS];
    // select the schedule file
    char available_schedules[MAX_SCHEDULES][MAX_FILENAME_LEN];

    ls((char*)(SCHEDULES_PATH), available_schedules, ".txt");

    // new schedule button
    buttons_of_schedules[NEW_SCHEDULE] = CreateWindow(
            "BUTTON",
            "New Schedule",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            NEW_SCH_BUTTONS_LEFT_MARGIN,
            NEW_SCH_BUTTONS_TOP_MARGIN,
            NEW_SCH_BUTTONS_WIDTH, NEW_SCH_BUTTONS_HEIGHT,
            hwnd,
            (HMENU)(NEW_SCHEDULE),
            (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
            NULL);

    SetButtonBackgroundColor(buttons_of_schedules[NEW_SCHEDULE], RGB(109, 240, 131));

    // delete schedule button
    buttons_of_schedules[DELETE_SCHEDULE] = CreateWindow(
            "BUTTON",
            "Delete Schedule",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            DEL_SCH_BUTTONS_LEFT_MARGIN,
            DEL_SCH_BUTTONS_TOP_MARGIN,
            DEL_SCH_BUTTONS_WIDTH, DEL_SCH_BUTTONS_HEIGHT,
            hwnd,
            (HMENU)(DELETE_SCHEDULE),
            (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
            NULL);

    SetButtonBackgroundColor(buttons_of_schedules[DELETE_SCHEDULE], RGB(202, 65, 65));

    // delete schedule button

    int i = 0;
    while ((available_schedules[i][0] != '\0') && (i < USHRT_MAX))
    {
        
        buttons_of_schedules[i] = CreateWindow(
            "BUTTON",
            available_schedules[i],
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            SCH_BUTTONS_LEFT_MARGIN,
            SCH_BUTTONS_TOP_MARGIN + (i * SCH_BUTTONS_HEIGHT),
            SCH_BUTTONS_WIDTH, SCH_BUTTONS_HEIGHT,
            hwnd,
            (HMENU)(i + 2), // the button it of each schedule has an offset of +2
            (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
            NULL);

            i++;

    }
}

void create_combo_boxes (HWND hwnd, HWND months_combo_box, HWND year_combo_box, GUI_t *GUI)
{
    char combo_box_options[41][256];

    // combo box for moths
    for (int i = 0; i < 12; i++)
        sprintf(combo_box_options[i], "%d", i + 1);

    combo_box_options[12][0] = '\0';
    create_combo_box(hwnd, months_combo_box, combo_box_options,
    CMB_BOX_MONTHS_LEFT_MARGIN,
    CMB_BOX_MONTHS_TOP_MARGIN, CMB_BOX_MONTHS_WIDTH, CMB_BOX_MONTHS_HEIGHT, CMB_BOX_MONTHS_ID, GUI->showing_date.month - 1);

    // combo box for days
    for (int i = 0; i < 40; i++)
        sprintf(combo_box_options[i], "%d", 2022 + i);

    combo_box_options[40][0] = '\0';
    create_combo_box(hwnd, year_combo_box, combo_box_options,
    CMB_BOX_YEARS_LEFT_MARGIN,
    CMB_BOX_YEARS_TOP_MARGIN, CMB_BOX_YEARS_WIDTH, CMB_BOX_YEARS_HEIGHT, CMB_BOX_YEARS_ID, GUI->showing_date.year - 2022);
}

void create_combo_box (HWND hwnd, HWND combo_box, char options[][256], unsigned left_margin,
unsigned top_margin, unsigned width, unsigned height, int combo_box_ID, int default_option) // options ends with options[n][0] 
{
    // Crear la casilla desplegable (combobox)
    combo_box = CreateWindow(
        "COMBOBOX",
        NULL,
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VSCROLL | WS_VISIBLE,
        left_margin, top_margin, width,
        height,
        hwnd,
        (HMENU)combo_box_ID,
        GetModuleHandle(NULL),
        NULL);

    // Agregar números del 1 al 12 al combobox
    int i = 0;
    while (options[i][0] != '\0')
    {
        // Agregar el elemento al combobox
        SendMessage(combo_box, CB_ADDSTRING, 0, (LPARAM)options[i]);
        i++;
    }

    // Establecer el primer elemento como seleccionado por defecto
    SendMessage(combo_box, CB_SETCURSEL, default_option, 0);
}
// -----------------------------------------------------

// buttons management ++++++++++++++++++++++++++++
void paint_day_buttons (HWND hwnd, HWND list_of_buttons [], date_event_t events_of_month[], COLORREF list_of_the_color_button [])
{
    int i = 0;
    while (i < 31) // reset all the buttons to its default color
    {
        list_of_the_color_button[i] = RGB(156, 156, 156);
        SetButtonBackgroundColor(list_of_buttons[events_of_month[i].date.day - 1], RGB(156, 156, 156));
        i++;
    }

    i = 0;
    while (!end_of_event_list(events_of_month[i]))
    {
        list_of_the_color_button[events_of_month[i].date.day - 1] = RGB(109, 123, 240);
        // printf("\nSetting to blue %d at day %d", i, events_of_month[i].date.day);
        SetButtonBackgroundColor(list_of_buttons[events_of_month[i].date.day - 1], RGB(109, 123, 240));
        
        i++;
    }

    
    

}

void make_visible_buttons_days (HWND hwnd, HWND buttons_of_the_days [])
{
    for (int i = 0; i < 31; ++i)
    {

        ShowWindow(buttons_of_the_days[i], SW_NORMAL);

    }

}

// -----------------------------------------------------
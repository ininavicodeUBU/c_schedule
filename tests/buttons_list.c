#include <windows.h>
#include "../include/constants.h"
#include <stdbool.h>
#include <limits.h>


/**
 * NOTES OF THE TEST
 * ShowWindow(<button_pointer>, SW_HIDE); -> This enables&ocults the referenced button
 * ShowWindow(<button_pointer>, SW_SHOWNORMAL); -> This disables&shows the refernced button
*/

// global variables ++++++++++++++++++++++++++++++++++++++++
// list of the buttons
HWND buttons_of_menu_select_schedule [MAX_SCHEDULES + 2]; // max buttons are max_schedules + the two options, new & delete
bool button_delete_visible = true;
// ---------------------------------------------------------

// aux functions ++++++++++++++++++++++++++++++++++++++++++++
// Function to change the background color of a button
void SetButtonBackgroundColor(HWND hwndButton, COLORREF color)
{
    SendMessage(hwndButton, BM_SETSTYLE, BS_OWNERDRAW, TRUE);

    DeleteObject((HBRUSH)GetClassLongPtr(hwndButton, GCLP_HBRBACKGROUND));

    SetClassLongPtr(hwndButton, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(color));
}
// -----------------------------------------------------------

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case 0: // Button ID for "New Schedule"
                    MessageBox(hwnd, "New Schedule button_new_sch clicked!", "Button Clicked", MB_OK); // window type (HWND), Message, Window message Title
                    // this button will ocult the Delete schedule button
                    
                    button_delete_visible ^= 1;
                    
                    if (!button_delete_visible)
                        ShowWindow(buttons_of_menu_select_schedule[1], SW_HIDE);
                    else
                        ShowWindow(buttons_of_menu_select_schedule[1], SW_SHOWNORMAL);

                    break;

                case 1: // Delete schedule button
                    MessageBox(hwnd, "Delete button clicked", "Button Clicked", MB_OK);
                        SetButtonBackgroundColor(buttons_of_menu_select_schedule[0], RGB(0, 0, 255));

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


// Step 5: the entry point
int main()
{
    // Step 1: Registering the Window Class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = "MyWindowClass";
    RegisterClass(&wc);

    // Step 2: Creating the Window
    HWND hwnd = CreateWindowEx(
        0,
        "MyWindowClass",
        "My Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
        NULL, NULL, GetModuleHandle(NULL), NULL);


    // Creating the buttons

    // Step 3.1: Create the <new schedule> button_new_sch
    HWND button_new_sch = CreateWindow(
        "BUTTON",
        "New Schedule",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 10, 120, 30,
        hwnd,
        (HMENU)0, // Button ID
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL);

    // Step 3.2: Create the <delete schedule> button delete_sch
    HWND button_delete_sch = CreateWindow(
        "BUTTON",
        "Delete Schedule",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 40, 120, 30, // default is 10, 10, 120, height
        hwnd,
        (HMENU)1, // Button ID
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL);



    // Step 3.1b: Set Font for the button_new_sch (optional)
    HFONT hFont_new_sch = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
    SendMessage(button_new_sch, WM_SETFONT, (WPARAM)hFont_new_sch, MAKELPARAM(TRUE, 0));

    // Step 3.2b: Set Font for the button delete_sch (optional)
    HFONT hFont_delete_sch = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
    SendMessage(button_delete_sch, WM_SETFONT, (WPARAM)hFont_delete_sch, MAKELPARAM(TRUE, 0));

    // So now add all this buttons to the list of buttons of the select schedule menu
    buttons_of_menu_select_schedule[0] = button_new_sch;
    buttons_of_menu_select_schedule[1] = button_delete_sch;


    // Step 3.1c: Display the Button
    // loop to display all the buttons
    for (int button_id = 0; button_id < 2; button_id++)
    {
        ShowWindow(buttons_of_menu_select_schedule[button_id], SW_SHOWNORMAL);
        UpdateWindow(buttons_of_menu_select_schedule[button_id]);
    }

    // Step 3d: Display the Window
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // Step 6: The Message Loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

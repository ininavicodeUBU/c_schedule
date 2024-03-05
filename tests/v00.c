#include <windows.h>
#include "../include/constants.h"
#include <stdbool.h>

// Global variables
HWND buttons_of_menu[MAX_SCHEDULES + 2];
HWND labels_of_menu[10];
HWND button_new_sch;
COLORREF newScheduleButtonColor = RGB(255, 255, 255); // Default color
bool button_delete_visible = true;

// Function to set the background color of the "New Schedule" button
void SetButtonBackgroundColor(HWND hwndButton, COLORREF color)
{
    SendMessage(hwndButton, BM_SETSTYLE, BS_OWNERDRAW, TRUE);
    DeleteObject((HBRUSH)GetClassLongPtr(hwndButton, GCLP_HBRBACKGROUND));
    SetClassLongPtr(hwndButton, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(color));
}

// Function to handle custom drawing of the button
void DrawCustomButton(LPDRAWITEMSTRUCT lpdis, COLORREF bgColor)
{
    SetBkColor(lpdis->hDC, bgColor);
    ExtTextOut(lpdis->hDC, 0, 0, ETO_OPAQUE, &lpdis->rcItem, NULL, 0, NULL);
    DrawText(lpdis->hDC, "New Schedule", -1, &lpdis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    
        case WM_CREATE:
            // Create the "New Schedule" button
            button_new_sch = CreateWindow(
                "BUTTON",
                "New Schedule",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
                10, 10, 120, 30,
                hwnd,
                (HMENU)0,
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);

            // Create three color buttons (Red, Blue, Green)
            for (int i = 0; i < 3; ++i)
            {
                buttons_of_menu[i] = CreateWindow(
                    "BUTTON",
                    i == 0 ? "Red" : (i == 1 ? "Blue" : "Green"),
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    150 + i * 70, 10, 60, 30,
                    hwnd,
                    (HMENU)(i + 1),
                    (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                    NULL);
            }

            labels_of_menu[0] = CreateWindow(
                "BUTTON",
                "hello text",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_TEXT,
                150 + 300, 10, 60, 30,
                hwnd,
                (HMENU)(0),
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);

            EnableWindow(labels_of_menu[0], false);


            break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case 0: // "New Schedule" button
                // MessageBox(hwnd, "New Schedule button_new_sch clicked!", "Button Clicked", MB_OK);
                button_delete_visible ^= 1;

                if (!button_delete_visible)
                {
                    ShowWindow(buttons_of_menu[1], SW_HIDE);
                    EnableWindow(labels_of_menu[0], false);

                }
                  
                else
                {
                    ShowWindow(buttons_of_menu[1], SW_SHOWNORMAL);
                    ShowWindow(labels_of_menu[0], true);

                }

                break;

            case 1: // Red button
                newScheduleButtonColor = RGB(255, 0, 0);
                break;

            case 2: // Blue button
                newScheduleButtonColor = RGB(0, 0, 255);
                break;

            case 3: // Green button
                newScheduleButtonColor = RGB(0, 255, 0);
                break;
            }

            // Repaint the "New Schedule" button with the updated color
            SetButtonBackgroundColor(button_new_sch, newScheduleButtonColor);
            break;

        case WM_DRAWITEM:
            if (wParam == 0) // "New Schedule" button
            {
                LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                DrawCustomButton(lpdis, newScheduleButtonColor);
                return TRUE;
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

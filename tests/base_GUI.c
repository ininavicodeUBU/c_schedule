#include <windows.h>

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
                    break;

                case 1: // Delete schedule button
                    MessageBox(hwnd, "Delete button clicked", "Button Clicked", MB_OK);
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

    // Step 3.1c: Display the Button
    ShowWindow(button_new_sch, SW_SHOWNORMAL);
    UpdateWindow(button_new_sch);

    ShowWindow(button_delete_sch, SW_SHOWNORMAL);
    UpdateWindow(button_delete_sch);

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

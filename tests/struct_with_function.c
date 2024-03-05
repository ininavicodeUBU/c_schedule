#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

HWND g_hButton;
HWND g_hEdit;
HWND combo_box;

void setEditText(HWND in_box, const char* newText) {
    // Convert narrow string to wide string
    int len = MultiByteToWideChar(CP_UTF8, 0, newText, -1, NULL, 0);
    wchar_t* wideText = malloc(len * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, newText, -1, wideText, len);

    // Set the text of the edit control
    SendMessageW(in_box, WM_SETTEXT, 0, (LPARAM)wideText);

    free(wideText);
}

void get_in_box_text(HWND in_box, char* buffer, int bufferSize) {
    // Get the text from the edit control
    int len = GetWindowTextLengthW(in_box) + 1;
    wchar_t* wideBuffer = malloc(len * sizeof(wchar_t));
    GetWindowTextW(in_box, wideBuffer, len);

    // Convert wide string to narrow string
    WideCharToMultiByte(CP_UTF8, 0, wideBuffer, -1, buffer, bufferSize, NULL, NULL);

    free(wideBuffer);
}

HWND create_in_box(HWND hwnd, char* default_text, unsigned X, unsigned Y, unsigned width, unsigned heigth) {
    // Convert narrow string to wide string
    int len = MultiByteToWideChar(CP_UTF8, 0, default_text, -1, NULL, 0);
    wchar_t* wide_default_text = malloc(len * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, default_text, -1, wide_default_text, len);

    // Create an edit control
    HWND in_box = CreateWindowW(
        L"EDIT",
        wide_default_text,
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE,
        X, Y, width, heigth,
        hwnd,
        (HMENU)2,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL);

    free(wide_default_text);
    return in_box;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:

        combo_box = CreateWindow(
            "COMBOBOX",
            NULL,
            CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VSCROLL | WS_VISIBLE,
            200, 10, 100, 100,
            hwnd,
            (HMENU)2, // Button ID
            GetModuleHandle(NULL),
            NULL);

        char name_of_element [40];
        for (int i = 0; i <= 59; i++)
        {
            sprintf(name_of_element, "%d", i+1);
            SendMessage(combo_box, CB_ADDSTRING, 0, (LPARAM)name_of_element);
        }

        
        g_hEdit = create_in_box(hwnd, "cacatua monstruosa", 10, 40, 150, 150);
    

        // Create a button
        g_hButton = CreateWindowW(
            L"BUTTON",       // Predefined class; Unicode assumed
            L"Click me",     // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            10, 10, 100, 30,  // x, y, width, height
            hwnd,             // Parent window
            (HMENU)1,         // Control ID
            (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
            NULL);            // Pointer not needed
        break;

    case WM_COMMAND:
    if (HIWORD(wParam) == BN_CLICKED) {
        if (LOWORD(wParam) == 1) {
            // Handle button click
            char buffer[256];  // Adjust the buffer size as needed
            get_in_box_text(g_hEdit, buffer, sizeof(buffer));
            MessageBoxA(NULL, buffer, "Entered Text", MB_OK);

            // Get the selected index from the combo box
            int selectedIndex = SendMessage(combo_box, CB_GETCURSEL, 0, 0);

            // Display the selected index
            char indexBuffer[10];  // Adjust the buffer size as needed
            sprintf(indexBuffer, "Selected Index: %d", selectedIndex);
            MessageBoxA(NULL, indexBuffer, "Selected Index", MB_OK);
        } else if (LOWORD(wParam) == 2) {
            // Handle button click
            char buffer[256];  // Adjust the buffer size as needed
            get_in_box_text(g_hEdit, buffer, sizeof(buffer));
            MessageBoxA(NULL, buffer, "You entered Text", MB_OK);
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
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main() {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = L"MyWindowClass";
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        L"MyWindowClass",
        L"My Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 200,
        NULL, NULL, GetModuleHandleW(NULL), NULL);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

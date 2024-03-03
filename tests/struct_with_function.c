#include <windows.h>

HWND g_hButton;
HWND g_hEdit;

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

        
        g_hEdit = create_in_box(hwnd, "cacatua monstruosa", 10, 40, 150, 150);
        // Create an edit control
        // g_hEdit = CreateWindowW(
        //     L"EDIT",         // Predefined class; Unicode assumed
        //     L"Type here",    // Default text
        //     WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE,
        //     10, 40, 200, 50,  // x, y, width, height
        //     hwnd,            // Parent window
        //     (HMENU)2,        // Control ID
        //     (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        //     NULL);           // Pointer not needed

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
            } else if (LOWORD(wParam) == 2)
            {
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

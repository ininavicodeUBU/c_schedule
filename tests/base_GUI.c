#include <windows.h>

// Function to handle messages for the main window
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
            // Create a combo box
            HWND comboBox = CreateWindow("ComboBox", NULL, 
                CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE, 
                10, 10, 150, 200, 
                hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

            // Add items to the combo box
            SendMessage(comboBox, CB_ADDSTRING, 0, (LPARAM)"Option 1");
            SendMessage(comboBox, CB_ADDSTRING, 0, (LPARAM)"Option 2");
            SendMessage(comboBox, CB_ADDSTRING, 0, (LPARAM)"Option 3");

            // Set the default selection
            SendMessage(comboBox, CB_SETCURSEL, 0, 0);

            break;
        }

        case WM_COMMAND: {
            if (LOWORD(wParam) == 1 && HIWORD(wParam) == CBN_SELCHANGE) {
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
            break;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }

        default: {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class
    const char* CLASS_NAME = "Sample Window Class";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "ComboBox Example",             // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main() {
    // ... (Window setup code)

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void DrawComboBoxItem(LPDRAWITEMSTRUCT lpdis) {
    // You can add your custom drawing logic here
    // For simplicity, let's draw a red rectangle as a background

    COLORREF backgroundColor = RGB(255, 0, 0); // Red color
    HBRUSH hBrush = CreateSolidBrush(backgroundColor);
    FillRect(lpdis->hDC, &lpdis->rcItem, hBrush);
    DeleteObject(hBrush);

    // Draw the text
    SetTextColor(lpdis->hDC, RGB(0, 0, 0)); // Black text color
    SetBkMode(lpdis->hDC, TRANSPARENT);
    DrawText(lpdis->hDC, "Sample Item", -1, &lpdis->rcItem, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Create a combo box
            HWND comboBox = CreateWindowEx(0, "COMBOBOX", "", WS_CHILD | WS_VISIBLE | CBS_OWNERDRAWFIXED | CBS_DROPDOWNLIST,
                                           50, 50, 150, 200, hwnd, (HMENU)1, NULL, NULL);

            // Add items to the combo box
            SendMessage(comboBox, CB_ADDSTRING, 0, (LPARAM)L"Item 1");
            SendMessage(comboBox, CB_ADDSTRING, 0, (LPARAM)L"Item 2");
            SendMessage(comboBox, CB_ADDSTRING, 0, (LPARAM)L"Item 3");

            break;
        }

        case WM_DRAWITEM: {
            if (wParam == 1) { // Check if the item is from the combo box with ID 1
                LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
                DrawComboBoxItem(lpdis);
                return TRUE; // Indicate that the item was drawn
            }
            break;
        }

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

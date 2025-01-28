// #include <windows.h>
// #include <iostream>
// #include <conio.h>
// using namespace std;
// int main() {
//     char c = getch();
//     while(true){
//         system("cls");
//         // Get the cursor's screen position
//     POINT cursorPos;
//     GetCursorPos(&cursorPos);

//     // Get the handle to the console window
//     HWND consoleWindow = GetConsoleWindow();

//     // Get the console screen buffer information
//     CONSOLE_SCREEN_BUFFER_INFO csbi;
//     GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

//     // Calculate the console buffer dimensions based on the visible window
//     int bufferWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
//     int bufferHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

//     // Get the dimensions of a single character in the console window
//     HDC hdc = GetDC(consoleWindow);
//     TEXTMETRIC tm;
//     GetTextMetrics(hdc, &tm);
//     int charWidth = tm.tmAveCharWidth;
//     int charHeight = tm.tmHeight;
//     ReleaseDC(consoleWindow, hdc);

//     // Get the position of the console window on the screen
//     RECT consoleRect;
//     GetWindowRect(consoleWindow, &consoleRect);

//     // Convert screen coordinates to console buffer coordinates
//     int consoleX = (cursorPos.x - consoleRect.left - 8) / 10;  // Adjust for window borders
//     int consoleY = (cursorPos.y - consoleRect.top - 30) / 18; // Adjust for title bar

//     // Clamp the values to the buffer's visible range
//     consoleX = max(0, min(consoleX, bufferWidth - 1));
//     consoleY = max(0, min(consoleY, bufferHeight - 1));

//     // Output the buffer coordinates
//     std::cout << "Cursor Position in Console Buffer: (" << consoleX << ", " << consoleY << ")" << std::endl;
//     std::cout << "Buffer Dimensions: (" << bufferWidth << ", " << bufferHeight << ")" << std::endl;
//     cout<<charWidth<<' '<<charHeight;
//     Sleep(50);
//     }
    
//     return 0;
// }

#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;
int main() {
    char c = getch();
    while(true){
        system("cls");
         // Get the cursor's screen position
        POINT cursorPos;
        GetCursorPos(&cursorPos);

        // Get the handle to the console window
        HWND consoleWindow = GetConsoleWindow();

        // // Get the console screen buffer information
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

        // Calculate the visible console window dimensions in characters
        int bufferWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int bufferHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        // // Get the client area of the console window (drawable area)
        RECT clientRect;
        GetClientRect(consoleWindow, &clientRect);

        // Calculate the pixel size of the client area
        int clientWidth = clientRect.right - clientRect.left;
        int clientHeight = clientRect.bottom - clientRect.top;

        // // Map screen coordinates to client area coordinates
        // ScreenToClient(consoleWindow, &cursorPos);

        // Convert client area coordinates to console buffer coordinates
        int consoleX = (cursorPos.x * bufferWidth) / clientWidth;
        int consoleY = (cursorPos.y * bufferHeight) / clientHeight;

        // Clamp the values to the buffer's visible range
        consoleX = max(0, min(consoleX, bufferWidth - 1));
        consoleY = max(0, min(consoleY, bufferHeight - 1));

        // Output the buffer coordinates
        std::cout << "Cursor Position in Console Buffer: (" << consoleX << ", " << consoleY << ")" << std::endl;
        std::cout << "Buffer Dimensions: (" << bufferWidth << ", " << bufferHeight << ")" << std::endl;
        Sleep(200);
    }


    return 0;
}

#include <iostream>
#include <windows.h>
#include <conio.h>

int main() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    std::cout<<rows<<' '<<cols<<std::endl;
    char c = getch();
    return 0;
}

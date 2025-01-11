#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

void gotoxy(short x, short y, string s = ""){
    //prints string s with null default value in pos(x, y) of the std output.
    HANDLE h_std = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y};
    SetConsoleCursorPosition(h_std, pos); 
    cout<<s;
}
void ConsoleResize(int width, int height){
    //Resizes console to given dimensions
    string com = "mode con: cols=" + to_string(width) + " lines=" + to_string(height);
    system(com.c_str());
}

template <size_t Rows, size_t Cols> //prints any character consisting of a 2D array of pairs
void PrintHero(const pair<string, string> (&Hero)[Rows][Cols], int row, int col, int x, int y, int diff){   
    for(int i = 0; i < row; i++){
        gotoxy((diff > 0 ? x + 6 : x - 1), y++, "  "); // erase previous cells based on direction
        gotoxy(x, y - 1); //go to current cells
        for(int j = 0; j < col; j++)
            cout<<Hero[i][j].second<<Hero[i][j].first<<Color.Reset;
    }
    cout<<Color.Reset; //Reset output color to default
}
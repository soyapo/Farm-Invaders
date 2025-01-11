//---necessary libraries---
#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <BasicFunc.cpp>
#include <StructValue.cpp>

//---for cin and cout mainly---
using namespace std;

//---to avoid long repetitions---
typedef const string CS;

//---Global Variables---

int main(){
    ConsoleResize(70, 36);
    system("cls");

    int x = 0, y = 0;
    gotoxy(x, y);

    while(char c = getch()){
        if(c == 'p')
            break;
        else if(c == 'a')
            x = max(0, x - 1);
        else if(c == 'd')
            x = min(120, x + 1);

        gotoxy(x, y);
        PrintHero(Heros.Hero, 6, 7, x, y, c - 96 - 1.8 * (c - 'a'));
    }
    return 0;
}
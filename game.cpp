#include <iostream>
#include <string>
#include <conio.h>
#include <chrono>
#include <windows.h>

using namespace std; //for cin and cout mainly
typedef const string CS; //to avoid long repetitions
#define fst ios_base::sync_with_stdio(0); cin.tie(0);

//---structs to tidy up global variables---
struct{
    int minX, maxX;
    int heroX, heroY;
    int bufferX, bufferY;
    int bulletX, bulletY, isBullet;
    int RabbitX, RabbitY, isRabbit[8], RabDir;

    clock_t lastRabbitMove = 0, lastBulletMove;
} Position;

struct{ //unicode blocks
    CS Full   = "\u2588"; //Full cell
    CS Top    = "\u2580"; //Top half cell
    CS Left   = "\u258C"; //Left half cell
    CS Right  = "\u2590"; //Right half cell
    CS Bottom = "\u2584"; //Bottom half cell
    CS QuadLeft  = "\u259A";
    CS QuadRight = "\u259E"; 
    CS LightShade = "\u2591";
    CS DarkShade  = "\u2593";
    CS Bullet[2] = {Full, LightShade};
} Block;

struct{
    pair<pair<int, int>, pair<string, string>>ObsData[300][100];
    string ObsOrder[6][4] = {
        {" ", Block.LightShade, Block.DarkShade, Block.Full},
        {" ", Block.QuadLeft, Block.Bottom, ""},
        {" ", Block.QuadRight, Block.Top, ""},
        {" ", Block.Left, "", ""},
        {" ", Block.Right, "", ""},
        {"", "", "", ""}
    };

} Barrier;

struct{ //ANSI Colors
    CS Reset  = "\033[0m"; //Default value
    CS Yellow = "\033[38;5;226m";
    CS Black  = "\033[38;2;0;0;0m";  
    CS Navy   = "\033[38;2;0;128;255m";
    CS Green  = "\033[38;2;92;172;45m";
    CS Orange = "\033[38;2;255;165;0m";
    CS Brown  = "\033[38;2;146;116;91m";
    CS White  = "\033[38;2;255;255;255m";

    CS OrangeYellow = "\033[38;2;255;165;0;48;5;226m";
    CS NavyYellow   = "\033[38;2;0;128;255;48;5;226m";
    CS NavyOrange   = "\033[38;2;0;128;255;48;2;255;165;0m";
    CS GreenReset   = "\033[38;2;92;172;45;48;2;0m";
    CS OrangeWhite  = "\033[38;2;255;165;0;48;2;255;255;255m";
    CS GreenRed     = "\033[38;2;92;172;45;48;2;255;0;0m";
    CS RedGreen     = "\033[38;2;255;0;0;48;2;92;172;45m";
    CS BrownGreen   = "\033[38;2;146;116;91;48;2;92;172;45m";
} Color;

struct{//character maps
    const pair<string, string> Hero[6][7] = { //main character (intended to resemble a duck =D)
        {{" ", Color.Reset},        {Block.Bottom, Color.Black}, {Block.Bottom, Color.Yellow}, {Block.Bottom, Color.Yellow}, {Block.Bottom, Color.Yellow}, {Block.Bottom, Color.Black}, {" ", Color.Reset}},
        {{Block.Full, Color.Black}, {Block.Full, Color.Yellow},  {Block.Full, Color.Navy},     {Block.Full, Color.Yellow},   {Block.Full, Color.Navy},     {Block.Full, Color.Yellow},  {Block.Full, Color.Black},},
        {{Block.Full, Color.Black}, {Block.Full, Color.Yellow},  {Block.Full, Color.Yellow},   {Block.Bottom, Color.OrangeYellow},   {Block.Full, Color.Yellow},   {Block.Full, Color.Yellow},  {Block.Full, Color.Black},},
        {{" ", Color.Reset},        {Block.Full, Color.Black},   {Block.Full, Color.Yellow},   {Block.Full, Color.Yellow},   {Block.Full, Color.Yellow},   {Block.Full, Color.Black},   {" ", Color.Reset}},
        {{Block.Top, Color.Black }, {Block.Top, Color.Yellow},   {Block.Full, Color.White},    {Block.Full, Color.Yellow},   {Block.Full, Color.White},    {Block.Top, Color.Yellow},   {Block.Top, Color.Black}},
        {{" ", Color.Reset},        {Block.Top, Color.Orange},   {Block.Top, Color.Orange},    {Block.Top, Color.Black},     {Block.Top, Color.Orange},    {Block.Top, Color.Orange},   {" ", Color.Reset}}
    };

    const pair<string, string> Rabbit[2][3][5] = {{
        {{Block.Full, Color.Orange}, {Block.Top, Color.Orange}, {" ", Color.Reset},{Block.Full, Color.Orange},{Block.Top, Color.Orange}},
        {{Block.Full, Color.Orange}, {Block.Top, Color.NavyOrange}, {Block.Top, Color.OrangeWhite}, {Block.Top, Color.NavyOrange}, {Block.Full, Color.Orange}},
        {{" ", Color.Reset}, {Block.Top, Color.Orange}, {Block.Top, Color.Orange}, {Block.Top, Color.Orange}, {" ", Color.Reset}}},

        {{{Block.Top, Color.Orange}, {Block.Full, Color.Orange}, {" ", Color.Reset},{Block.Top, Color.Orange},{Block.Full, Color.Orange}},
        {{Block.Full, Color.Orange}, {Block.Top, Color.NavyOrange}, {Block.Top, Color.OrangeWhite}, {Block.Top, Color.NavyOrange}, {Block.Full, Color.Orange}},
        {{" ", Color.Reset}, {Block.Top, Color.Orange}, {Block.Top, Color.Orange}, {Block.Top, Color.Orange}, {" ", Color.Reset}}}
    };

} Heros;

//---Functions---
void Initialize();
void InitBarrier();
void CreateBarrier(int, int, int);

void gotoxy(short x, short y, string s = "");

template <size_t Rows, size_t Cols> //prints any character consisting of a 2D array of pairs
void PrintHero(const pair<string, string> (&Hero)[Rows][Cols], int row, int col, int x, int y, int diff); 
void PrintRabbit();
void PrintBullet(int&x, int &y);
void PrintDecor();

//---------------------------------------------//

int main(){
    char c = getch();
    Initialize();
    while(true){
        if(_kbhit()){
            char c = getch();
            if(c == 'p')
                break;
            else if(c == 'a'){
                Position.heroX = max(Position.minX, Position.heroX - 1);
                gotoxy(Position.heroX, Position.heroY);
                PrintHero(Heros.Hero, 6, 7, Position.heroX, Position.heroY, 1);
            }
            else if(c == 'd'){
                Position.heroX = min(Position.maxX - 7, Position.heroX + 1);
                gotoxy(Position.heroX, Position.heroY);
                PrintHero(Heros.Hero, 6, 7, Position.heroX, Position.heroY, -1);
            }
            else if(c == '\r' && !Position.isBullet){
                Position.bulletX = Position.heroX + 3, Position.bulletY = Position.heroY - 2, 
                Position.isBullet = 1, Position.lastBulletMove = clock();
            }
        }

        if(Position.isBullet && (clock() - Position.lastBulletMove) >= 30){
            PrintBullet(Position.bulletX, Position.bulletY);
            Position.lastBulletMove = clock();
            gotoxy(Position.heroX, Position.heroY);
        }

        if(clock() - Position.lastRabbitMove >= 500){
            PrintRabbit();
            Position.lastRabbitMove = clock();
            gotoxy(Position.heroX, Position.heroY);
        }
    }
    return 0;
}

void Initialize(){
    system("cls");

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    Position.bufferY = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    Position.bufferX = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    
    Position.minX = (Position.bufferX - 70) / 2;
    Position.maxX = (Position.bufferX - 70) / 2 + 70;

    Position.heroX   = Position.minX + 31, Position.heroY = Position.bufferY - 7;
    Position.bulletX = 0 , Position.bulletY = 0, Position.isBullet = 0;

    Position.RabbitX = Position.minX, Position.RabbitY = 0, Position.RabDir = 1;
    for(int i = 0; i < 8; i++)
        Position.isRabbit[i] = 1;
    
    PrintDecor();
    InitBarrier();
    PrintHero(Heros.Hero, 6, 7, Position.heroX, Position.heroY, 0);
}

void InitBarrier(){
    int type, x, y = Position.bufferY - 11;
    srand(time(0));

    x = Position.minX + 9,
    type = rand() % 3;
    CreateBarrier(x, y, type);

    x += 15,
    type = rand() % 3;
    CreateBarrier(x, y, type);
    
    x += 16,
    type = rand() % 3;
    CreateBarrier(x, y, type);
    
    x += 15,
    type = rand() % 3;
    CreateBarrier(x, y, type);
}

void CreateBarrier(int x, int y, int type){
    int temp = y;
    switch(type){
        case 0:
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 1, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 1, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 1, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x][y++].second.second = Color.Orange;    
            x -= 5;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 5, Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ", Barrier.ObsData[x++][y].second.second = Color.Reset;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 5, Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ", Barrier.ObsData[x++][y].second.second = Color.Reset;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 5, Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ", Barrier.ObsData[x][y++].second.second = Color.Reset;    
            x -= 5;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 2, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Top, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 2, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Top, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Orange;
            Barrier.ObsData[x][y].first.first = 2, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Top, Barrier.ObsData[x][y].second.second = Color.Orange;    
            x -= 5, y -= 2;
            gotoxy(x, y);
            for(int i = y; i <= y + 2; i++){
                for(int j = x; j <= x + 5; j++)
                    cout<<Barrier.ObsData[j][i].second.second<<Barrier.ObsData[j][i].second.first;
                gotoxy(x, ++temp);
            }
            cout<<Color.Reset;
            break;
        case 1:
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Navy;
            Barrier.ObsData[x][y].first.first = 5, Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ", Barrier.ObsData[x++][y].second.second = Color.Reset;
            Barrier.ObsData[x][y].first.first = 4, Barrier.ObsData[x][y].first.second = 1,
            Barrier.ObsData[x][y].second.first = Block.Right, Barrier.ObsData[x++][y].second.second = Color.Navy;
            Barrier.ObsData[x][y].first.first = 3, Barrier.ObsData[x][y].first.second = 1,
            Barrier.ObsData[x][y].second.first = Block.Left, Barrier.ObsData[x++][y].second.second = Color.Navy;
            Barrier.ObsData[x][y].first.first = 5, Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ", Barrier.ObsData[x++][y].second.second = Color.Reset;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x][y++].second.second = Color.Navy;    
            x -= 5;
            Barrier.ObsData[x][y].first.first = 2, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Top, Barrier.ObsData[x++][y].second.second = Color.Navy;
            Barrier.ObsData[x][y].first.first = 2, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Top, Barrier.ObsData[x++][y].second.second = Color.Navy;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Navy;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Navy;
            Barrier.ObsData[x][y].first.first = 2, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Top, Barrier.ObsData[x++][y].second.second = Color.Navy;
            Barrier.ObsData[x][y].first.first = 2, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Top, Barrier.ObsData[x][y++].second.second = Color.Navy;    
            x -= 5;
            Barrier.ObsData[x][y].first.first = 5, Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ", Barrier.ObsData[x++][y].second.second = Color.Reset;
            Barrier.ObsData[x][y].first.first = 5, Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ", Barrier.ObsData[x++][y].second.second = Color.Reset;
            Barrier.ObsData[x][y].first.first = 4, Barrier.ObsData[x][y].first.second = 1,
            Barrier.ObsData[x][y].second.first = Block.Right, Barrier.ObsData[x++][y].second.second = Color.Navy;
            Barrier.ObsData[x][y].first.first = 3, Barrier.ObsData[x][y].first.second = 1,
            Barrier.ObsData[x][y].second.first = Block.Left, Barrier.ObsData[x++][y].second.second = Color.Navy;
            Barrier.ObsData[x][y].first.first = 5, Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ", Barrier.ObsData[x++][y].second.second = Color.Reset;
            Barrier.ObsData[x][y].first.first = 5, Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ", Barrier.ObsData[x][y].second.second = Color.Reset;    
            x -= 5, y -= 2;
            gotoxy(x, y);
            for(int i = y; i <= y + 2; i++){
                for(int j = x; j <= x + 5; j++)
                    cout<<Barrier.ObsData[j][i].second.second<<Barrier.ObsData[j][i].second.first;
                gotoxy(x, ++temp);
            }
            cout<<Color.Reset;
            break;
        case 2:
            Barrier.ObsData[x][y].first.first = 1, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x++][y].second.second = Color.RedGreen;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 1, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x][y++].second.second = Color.GreenReset;    
            x -= 5;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x++][y].second.second = Color.BrownGreen;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x++][y].second.second = Color.BrownGreen;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x][y++].second.second = Color.GreenRed;    
            x -= 5;
            Barrier.ObsData[x][y].first.first = 5, Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ", Barrier.ObsData[x++][y].second.second = Color.Reset;
            Barrier.ObsData[x][y].first.first = 2, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Top, Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Brown;
            Barrier.ObsData[x][y].first.first = 0, Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full, Barrier.ObsData[x++][y].second.second = Color.Brown;
            Barrier.ObsData[x][y].first.first = 2, Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Top, Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 5, Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ", Barrier.ObsData[x][y].second.second = Color.Reset;    
            x -= 5, y -= 2;
            gotoxy(x, y);
            for(int i = y; i <= y + 2; i++){
                for(int j = x; j <= x + 5; j++)
                    cout<<Barrier.ObsData[j][i].second.second<<Barrier.ObsData[j][i].second.first;
                gotoxy(x, ++temp);
            }
            cout<<Color.Reset;
            break;
        default:
            break;
    };
}

void gotoxy(short x, short y, string s){
    //prints string s with null default value in pos(x, y) of the std output.
    HANDLE h_std = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y};
    SetConsoleCursorPosition(h_std, pos); 
    cout<<s;
}

template <size_t Rows, size_t Cols> //prints any character consisting of a 2D array of pairs
void PrintHero(const pair<string, string> (&Hero)[Rows][Cols], int row, int col, int x, int y, int diff){
    gotoxy(x, y);
    for(int i = 0; i < row; i++){
        gotoxy((diff > 0 ? x + 6 : x - 1), y++, "  "); // erase previous cells based on direction
        gotoxy(x, y - 1); //go to current cells

        for(int j = 0; j < col; j++)
            cout<<Hero[i][j].second<<Hero[i][j].first<<Color.Reset;
    }
    gotoxy(0, 0, Color.Green);
    gotoxy((diff > 0 ? x + 6 : x - 1), y - 1, Block.Bottom); // erase previous cells based on direction
    if(x + 6 < Position.maxX)
        cout<<Block.Bottom;
    gotoxy(Position.heroX, Position.heroY + 5, Block.Bottom);
    gotoxy(Position.heroX + 6, Position.heroY + 5, Block.Bottom);
    cout<<Color.Reset; //Reset output color to default
    gotoxy(Position.maxX, Position.bufferY - 2, " ");
}

void PrintRabbit(){
    if((Position.RabDir == 1 && Position.RabbitX == Position.minX + 15) || 
        (Position.RabDir == -1 && Position.RabbitX == Position.minX)){
            for(int i = Position.RabbitY; i < Position.RabbitY + 2; i++){
                gotoxy(Position.minX, i);
                for(int j = 0; j < 70; j++)
                    cout<<" ";
            }
            Position.RabDir *= -1;
            Position.RabbitY++;
    }
    
    for(int i = 0, x = Position.RabbitX; i < 8; i++, x += 7){
        if(Position.isRabbit[i]){
            int y = Position.RabbitY; 
            for(int j = 0; j < 3; j++){
                gotoxy(x, y);
                
                for(int k = 0; k < 5; k++)
                    cout<<Heros.Rabbit[Position.RabbitX % 2][j][k].second<<Heros.Rabbit[Position.RabbitX % 2][j][k].first;

                for(int k = 0; k < 5; k++)
                    gotoxy(x + (Position.RabDir > 0 ? -1 : 5), Position.RabbitY + k, " ");
                y++;
            }
        }
    }

    Position.RabbitX += Position.RabDir;
}

void PrintBullet(int &x, int &y){
    gotoxy(x, y + 1, Color.Yellow + " ");

    if(Barrier.ObsData[x][y].first.second){
        Position.isBullet = 0;
        gotoxy(x, y, Barrier.ObsData[x][y].second.second);
        Barrier.ObsData[x][y].second.first = Barrier.ObsOrder[Barrier.ObsData[x][y].first.first][--Barrier.ObsData[x][y].first.second];
        if(!Barrier.ObsData[x][y].first.second)
            cout<<Color.Reset;
        gotoxy(x, y, Barrier.ObsData[x][y].second.first);
    }
    else{
        gotoxy(x, y--, Block.Bullet[(y % 12) / 6]);
    }
    
    if(Position.isRabbit[x / 7])
        gotoxy(0, 0, to_string(x / 7));

    if(y <= 0){
        Position.isBullet = 0;
        gotoxy(x, y, " ");
        gotoxy(x, y + 1, " ");
    }
    
    cout<<Color.Reset;
}

void PrintDecor(){
    gotoxy(Position.minX, Position.bufferY - 1, Color.Brown);
    for(int i = Position.minX; i < Position.maxX; i++)
        cout<<Block.DarkShade;

    gotoxy(Position.minX, Position.bufferY - 2, Color.Green);
    for(int i = Position.minX; i < Position.maxX; i++)
        cout<<Block.Bottom;
    cout<<Color.Reset;
    
}



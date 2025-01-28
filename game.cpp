#include <iostream>
#include <string>
#include <conio.h>
#include <chrono>
#include <cstdlib>
#include <windows.h>
using namespace std; //for cin and cout mainly
typedef const string CS; //to avoid long repetitions
//---structs to tidy up global variables---
struct{
    int minX, maxX;
    int heroX, heroY;
    int mouseX, mouseY;
    int bufferX, bufferY;
    int bulletX, bulletY, isBullet;
    clock_t lastRabbitMove = 0, lastBulletMove, lastLogoMove;
} Position;
struct{ //unicode blocks
    CS Full       = "\u2588"; //Full cell
    CS Top        = "\u2580"; //Top half cell
    CS Left       = "\u258C"; //Left half cell
    CS Right      = "\u2590"; //Right half cell
    CS Bottom     = "\u2584"; //Bottom half cell
    CS QuadLeft   = "\u259A";
    CS QuadRight  = "\u259E"; 
    CS LightShade = "\u2591";
    CS DarkShade  = "\u2593";
    CS Bullet[2]  = {Full, LightShade};
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
    CS Reset        = "\033[0m"; //Default value
    CS Yellow       = "\033[38;5;226m";
    CS Black        = "\033[38;2;0;0;0m";  
    CS Red          = "\033[38;2;255;0;0m";
    CS Choco        = "\033[38;2;123;63;0m";
    CS Navy         = "\033[38;2;0;128;255m";
    CS Green        = "\033[38;2;92;172;45m";
    CS Orange       = "\033[38;2;255;165;0m";
    CS Brown        = "\033[38;2;146;116;91m";
    CS White        = "\033[38;2;255;255;255m";
    CS Pink         = "\033[38;2;248;197;216m";
    CS OrangeYellow = "\033[38;2;255;165;0;48;5;226m";
    CS BlackWhite   = "\033[38;2;0;0;0;48;2;255;255;255m";
    CS GreenRed     = "\033[38;2;92;172;45;48;2;255;0;0m";
    CS RedGreen     = "\033[38;2;255;0;0;48;2;92;172;45m";
    CS ChocoGreen   = "\033[38;2;123;63;0;48;2;92;172;45m";
    CS RedWhite     = "\033[38;2;255;0;0;48;2;255;255;255m";
    CS WhiteRed     = "\033[38;2;255;255;255;48;2;255;0;0m";
    CS BrownGreen   = "\033[38;2;146;116;91;48;2;92;172;45m";
    CS WhiteBrown   = "\033[38;2;255;255;255;48;2;123;63;0m";
    CS WhiteGreen   = "\033[38;2;255;255;255;48;2;92;172;45m";
    CS NavyPink     = "\033[38;2;0;128;255;48;2;248;197;216m";
    CS PinkWhite    = "\033[38;2;248;197;216;48;2;255;255;255m";
} Color;
struct{//character maps
    const pair<string, string> Hero[6][7] = { //main character (intended to resemble a duck =D)
        {{" ", Color.Reset},        {Block.Bottom, Color.Black}, {Block.Bottom, Color.Yellow}, {Block.Bottom, Color.Yellow},       {Block.Bottom, Color.Yellow}, {Block.Bottom, Color.Black}, {" ", Color.Reset}},
        {{Block.Full, Color.Black}, {Block.Full, Color.Yellow},  {Block.Full, Color.Navy},     {Block.Full, Color.Yellow},         {Block.Full, Color.Navy},     {Block.Full, Color.Yellow},  {Block.Full, Color.Black},},
        {{Block.Full, Color.Black}, {Block.Full, Color.Yellow},  {Block.Full, Color.Yellow},   {Block.Bottom, Color.OrangeYellow}, {Block.Full, Color.Yellow},   {Block.Full, Color.Yellow},  {Block.Full, Color.Black},},
        {{" ", Color.Reset},        {Block.Full, Color.Black},   {Block.Full, Color.Yellow},   {Block.Full, Color.Yellow},         {Block.Full, Color.Yellow},   {Block.Full, Color.Black},   {" ", Color.Reset}},
        {{Block.Top, Color.Black }, {Block.Top, Color.Yellow},   {Block.Full, Color.White},    {Block.Full, Color.Yellow},         {Block.Full, Color.White},    {Block.Top, Color.Yellow},   {Block.Top, Color.Black}},
        {{" ", Color.Reset},        {Block.Top, Color.Orange},   {Block.Top, Color.Orange},    {Block.Top, Color.Black},           {Block.Top, Color.Orange},    {Block.Top, Color.Orange},   {" ", Color.Reset}}
    };
    const pair<string, string> Rabbit[3][3][5] = {{
        {{Block.Full, Color.Pink},  {Block.Top, Color.Pink},     {" ",       Color.Reset},     {Block.Full, Color.Pink},     {Block.Top,  Color.Pink}},
        {{Block.Full, Color.Pink},  {Block.Top, Color.NavyPink}, {Block.Top, Color.PinkWhite}, {Block.Top,  Color.NavyPink}, {Block.Full, Color.Pink}},
        {{" ",        Color.Reset}, {Block.Top, Color.Pink},     {Block.Top, Color.Pink},      {Block.Top,  Color.Pink},     {" ",        Color.Reset}}},

        {{{Block.Top, Color.Pink},  {Block.Full, Color.Pink},     {" ",       Color.Reset},     {Block.Top, Color.Pink},     {Block.Full, Color.Pink}},
        {{Block.Full, Color.Pink},  {Block.Top,  Color.NavyPink}, {Block.Top, Color.PinkWhite}, {Block.Top, Color.NavyPink}, {Block.Full, Color.Pink}},
        {{" ",        Color.Reset}, {Block.Top,  Color.Pink},     {Block.Top, Color.Pink},      {Block.Top, Color.Pink},     {" ",        Color.Reset}}},

        {{{Block.Top,   Color.Pink}, {Block.Bottom, Color.Pink},  {Block.Top,    Color.Pink}, {Block.Bottom, Color.Pink},  {Block.Top,    Color.Pink}},
        {{Block.Bottom, Color.Pink}, {" ",          Color.Reset}, {Block.Full,   Color.Pink}, {" ",          Color.Reset}, {Block.Bottom, Color.Pink}},
        {{Block.Bottom, Color.Pink}, {Block.Top,    Color.Pink},  {Block.Bottom, Color.Pink}, {Block.Top,    Color.Pink},  {Block.Bottom, Color.Pink}}}
    };
    const pair<string, string> Empty[3][5] = {
        {{" ", Color.Reset}, {" ", Color.Reset}, {" ", Color.Reset}, {" ", Color.Reset}, {" ", Color.Reset}},
        {{" ", Color.Reset}, {" ", Color.Reset}, {" ", Color.Reset}, {" ", Color.Reset}, {" ", Color.Reset}},
        {{" ", Color.Reset}, {" ", Color.Reset}, {" ", Color.Reset}, {" ", Color.Reset}, {" ", Color.Reset}}
    };
} Heros;
struct{
    int RabbitX, RabbitY, RabDir;
    pair <int, int> RabGrid[300][100];
    int isRabbit[8];
} Enemy;

//---Functions---
void Game();

void InitMain();
void InitGame();
void InitBarrier();
void CreateBarrier(int, int, int);

string SN(string s, int n);
void getMousePos();
void gotoxy(short x, short y, string s = "");

template <size_t Rows, size_t Cols> //prints any character consisting of a 2D array of pairs
void PrintHero(const pair<string, string> (&Hero)[Rows][Cols], int row, int col, int x, int y, int diff); 
void PrintRabbit();
void PrintBullet(int&x, int &y);
void PrintDecor();

void MainMenu();
void firstText();
void printLogo(int, int, int);
void printButton(int, int, int, string, string, string);
//---------------------------------------------//
int main(){
    char c = getch();
    MainMenu();
    //firstText();
    //Game();
    return 0;
}
void Game(){
    InitGame();
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
                Position.bulletX = Position.heroX + 3, Position.bulletY = Position.heroY - 1, 
                Position.isBullet = 1, Position.lastBulletMove = clock();
            }
        }

        if(Position.isBullet && (clock() - Position.lastBulletMove) >= 30){
            PrintBullet(Position.bulletX, Position.bulletY);
            Position.lastBulletMove = clock();
            gotoxy(Position.heroX, Position.heroY);
        }

        if(clock() - Position.lastRabbitMove >= 1000){
            PrintRabbit();
            Position.lastRabbitMove = clock();
            gotoxy(Position.heroX, Position.heroY);
        }
    }
}
void InitMain(){
    system("cls");
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    Position.bufferY = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    Position.bufferX = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    Position.minX = (Position.bufferX - 70) / 2;
    Position.maxX = (Position.bufferX - 70) / 2 + 70;
}
void InitGame(){
    Position.heroX   = Position.minX + 31, Position.heroY = Position.bufferY - 7;
    Position.bulletX = 0 , Position.bulletY = 0, Position.isBullet = 0;
    //initial position of the leftmost rabbit, directed to move east
    Enemy.RabbitX = Position.minX, Enemy.RabbitY = 0, Enemy.RabDir = 1;

    for(int i = 0; i < 8; i++){
        Enemy.isRabbit[i] = 1;
        for(int x = 7 * i; x < 7 * i + 5; x++)
            for(int y = 0; y < 3; y++)
                Enemy.RabGrid[x + Enemy.RabbitX][y].first = 1, 
                Enemy.RabGrid[x + Enemy.RabbitX][y].second = i;
    }

    PrintDecor();
    InitBarrier();
    PrintHero(Heros.Hero, 6, 7, Position.heroX, Position.heroY, 0);
}
void InitBarrier(){
    srand(time(0));
    int x = Position.minX + 9, y = Position.bufferY - 11, type = rand() % 3;
    CreateBarrier(x, y, type);
    x += 15, type = rand() % 3;
    CreateBarrier(x, y, type);
    x += 16, type = rand() % 3;
    CreateBarrier(x, y, type);
    x += 15, type = rand() % 3;
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
            Barrier.ObsData[x][y].first.first  = 1,            Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first  = 0,            Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x++][y].second.second = Color.RedGreen;
            Barrier.ObsData[x][y].first.first  = 0,            Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full,   Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first  = 0,            Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full,   Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first  = 0,            Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full,   Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first  = 1,            Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x][y++].second.second = Color.Green;    
            x -= 5;
            Barrier.ObsData[x][y].first.first = 0,             Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full,   Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 0,             Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full,   Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 0,             Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x++][y].second.second = Color.BrownGreen;
            Barrier.ObsData[x][y].first.first = 0,             Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x++][y].second.second = Color.BrownGreen;
            Barrier.ObsData[x][y].first.first = 0,             Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full,   Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 0,             Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Bottom, Barrier.ObsData[x][y++].second.second = Color.GreenRed;    
            x -= 5;
            Barrier.ObsData[x][y].first.first = 5,             Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ",          Barrier.ObsData[x++][y].second.second = Color.Reset;
            Barrier.ObsData[x][y].first.first = 2,             Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Top,    Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 0,             Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full,   Barrier.ObsData[x++][y].second.second = Color.Brown;
            Barrier.ObsData[x][y].first.first = 0,             Barrier.ObsData[x][y].first.second = 3,
            Barrier.ObsData[x][y].second.first = Block.Full,   Barrier.ObsData[x++][y].second.second = Color.Brown;
            Barrier.ObsData[x][y].first.first = 2,             Barrier.ObsData[x][y].first.second = 2,
            Barrier.ObsData[x][y].second.first = Block.Top,    Barrier.ObsData[x++][y].second.second = Color.Green;
            Barrier.ObsData[x][y].first.first = 5,             Barrier.ObsData[x][y].first.second = 0,
            Barrier.ObsData[x][y].second.first = " ",          Barrier.ObsData[x][y].second.second = Color.Reset;    
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
string SN(string s, int n){
    string t = "";
    while (n--)
        t += s;
    return t;
}
void getMousePos(int &x, int &y){
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    // Get the console window handle
    HWND consoleWindow = GetConsoleWindow();
    // Get the console screen buffer information
    HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(consoleOutput, &csbi);
    // Get the dimensions of a single character in the console window
    HDC hdc = GetDC(consoleWindow);
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    int charWidth = tm.tmAveCharWidth;
    int charHeight = tm.tmHeight;
    ReleaseDC(consoleWindow, hdc);
    // Get the console window's screen rectangle
    RECT consoleRect;
    GetWindowRect(consoleWindow, &consoleRect);
    // Convert screen coordinates to console buffer coordinates
    x = (cursorPos.x - consoleRect.left - 8) / 10;
    y = (cursorPos.y - consoleRect.top - 30) / 18;
    // Clamp the values to the console buffer dimensions
    x = max(0, min(x, Position.bufferX));
    y = max(0, min(y, Position.bufferY + 3));
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
    //zeroing previous cells
    for(int i = Enemy.RabbitY; i < Enemy.RabbitY + 3; i++){
        if(Enemy.RabDir > 0)
            for(int x = 0, j = Enemy.RabbitX; x < 8; x++, j += 7)
                Enemy.RabGrid[j][i].first = 0,
                Enemy.RabGrid[j][i].second = 0;
        else
            for(int x = 0, j = Enemy.RabbitX + 53; x < 8; x++, j -= 7)
                Enemy.RabGrid[j][i].first = 0,
                Enemy.RabGrid[j][i].second = 0;
    }

    //direction inversion
    if((Enemy.RabDir == 1 && Enemy.RabbitX == Position.minX + 15) || (Enemy.RabDir == -1 && Enemy.RabbitX == Position.minX)){
        for(int i = Enemy.RabbitY; i < Enemy.RabbitY + 2; i++){
            gotoxy(Position.minX, i);
            for(int j = 0; j < 70; j++)
                cout<<' ';
        }
        Enemy.RabbitY++, Enemy.RabDir *= -1;
    }

    Enemy.RabbitX += Enemy.RabDir;

    for(int i = 0; i < 8; i++){
        for(int x = 7 * i; x < 7 * i + 5; x++){
            for(int y = Enemy.RabbitY; y < Enemy.RabbitY + 3; y++){
                Enemy.RabGrid[x + Enemy.RabbitX][y].second = i;
                if(Enemy.isRabbit[i] == 1)
                    Enemy.RabGrid[x + Enemy.RabbitX][y].first = 1;
            }
        }
    }
    //printing the new cells
    for(int i = 0, x = Enemy.RabbitX; i < 8; i++, x += 7){
        for(int j = 0, y = Enemy.RabbitY; j < 3; j++){
            gotoxy(x, y++);

            if(Enemy.isRabbit[i] == 1)
                for(int k = 0; k < 5; k++)
                    cout<<Heros.Rabbit[Enemy.RabbitX % 2][j][k].second<<Heros.Rabbit[Enemy.RabbitX % 2][j][k].first;
            else if(Enemy.isRabbit[i] < 0){
                Enemy.isRabbit[i]--;
                for(int k = 0; k < 5; k++)
                    cout<<Heros.Rabbit[2][j][k].second<<Heros.Rabbit[2][j][k].first;
            }
            else
                for(int k = 0; k < 5; k++)
                    cout<<Heros.Empty[j][k].second<<Heros.Empty[j][k].first;
            
            for(int k = 0; k < 5; k++)    
                    gotoxy(x + (Enemy.RabDir > 0 ? -1 : 5), Enemy.RabbitY + k, " ");

            if(Enemy.isRabbit[i] == -3)
                Enemy.isRabbit[i] = 0;
        }
    }
}
void PrintBullet(int &x, int &y){
    //clear the previous bullet position
    if(y + 1 < Position.heroY)
        gotoxy(x, y + 1, Color.Yellow + " ");

    //Barrier hit sequence
    if(Barrier.ObsData[x][y].first.second){
        Position.isBullet = 0;
        gotoxy(x, y, Barrier.ObsData[x][y].second.second);
        Barrier.ObsData[x][y].second.first = Barrier.ObsOrder[Barrier.ObsData[x][y].first.first][--Barrier.ObsData[x][y].first.second];
        if(!Barrier.ObsData[x][y].first.second)
            cout<<Color.Reset;
        Position.isBullet = 0;
        gotoxy(x, y, Barrier.ObsData[x][y].second.first);
    }

    //enemy hit sequence
    else if(Enemy.RabGrid[x][y].first == 1){
        Enemy.isRabbit[Enemy.RabGrid[x][y].second] = -1, Position.isBullet = 0;
        int left = Enemy.RabbitX + (7 * Enemy.RabGrid[x][y].second);
        for(int j = Enemy.RabbitY; j < Enemy.RabbitY + 3; j++)
            for(int i = left; i < left + 5; i++)
                Enemy.RabGrid[i][j].first = 0;
        gotoxy(x, y, " ");
    }
    else{
        gotoxy(x, y--, Block.Bullet[(y % 24) / 12]);
    }

    if(y < 0){
        Position.isBullet = 0;
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
void MainMenu(){
    InitMain();
    bool button[4] = {0, 0, 0, 0};
    //Background Printing
    for(int i = 0; i < Position.bufferY; i++)
        gotoxy(Position.minX - 1, i, Color.Choco + Block.Full);
    for(int i = 0; i < Position.bufferY; i++)
        gotoxy(Position.minX + 70, i, Color.Choco + Block.Full);
    for(int i = 0; i < Position.bufferY; i++)
        for(int j = 0; j < 70; j++)
            gotoxy(Position.minX + j, i, Color.BrownGreen + " ");

    printButton(Position.minX + 28, Position.bufferY - 17, 2, Color.BlackWhite, Color.WhiteGreen, "START GAME");
    printButton(Position.minX + 28, Position.bufferY - 13, 1, Color.BlackWhite, Color.WhiteGreen, "LEADERBOARDS");
    printButton(Position.minX + 28, Position.bufferY -  9, 3, Color.BlackWhite, Color.WhiteGreen, "SETTINGS");
    printButton(Position.minX + 28, Position.bufferY -  4, 5, Color.RedWhite,   Color.WhiteGreen, "EXIT");
                
    int index = 1;
    bool shart;

    Position.lastLogoMove = clock();
    while(true){
        if(clock() - Position.lastLogoMove> 700){
            index *= -1, Position.lastLogoMove = clock();
            printLogo(Position.minX + 21, 5, index);
        }
        getMousePos(Position.mouseX, Position.mouseY);
        
        //Game
        shart = Position.mouseY <= Position.bufferY - 16 && Position.mouseY >= Position.bufferY - 18 && Position.mouseX >= Position.minX + 26 && Position.mouseX <= Position.minX + 40;
        if(!button[0] && shart){
            button[0] = 1;
            printButton(Position.minX + 28, Position.bufferY -  17, 2, Color.WhiteBrown, Color.ChocoGreen, "START GAME");    
        }
        else if(button[0] && !shart){
            button[0] = 0;
            printButton(Position.minX + 28, Position.bufferY -  17, 2, Color.BlackWhite, Color.WhiteGreen, "START GAME");    
        }
        else if(button[0] && shart && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)){
            cout<<Color.Reset;
            system("cls");
            Game();
        }

        //EXIT
        shart = Position.mouseY <= Position.bufferY - 3 && Position.mouseY >= Position.bufferY - 5 && Position.mouseX >= Position.minX + 26 && Position.mouseX <= Position.minX + 40;
        if(!button[3] && shart){
            button[3] = 1;
            printButton(Position.minX + 28, Position.bufferY -  4, 5, Color.WhiteRed, Color.RedGreen, "EXIT");    
        }
        else if(button[3] && !shart){
            button[3] = 0;
            printButton(Position.minX + 28, Position.bufferY -  4, 5, Color.RedWhite, Color.WhiteGreen, "EXIT");    
        }
        else if(button[3] && shart && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)){
            exit(0);
        }
    }
}
void firstText(){
    int y = 1;
    gotoxy(Position.minX + 2, y, Color.Green);
    string text = "Written and Directed by Soyapo";
    for(int i = 0; i < text.length(); i++){
        cout<<text[i];
        Sleep(70);
    }
    Sleep(2000);
    y += 2;
    gotoxy(Position.minX + 2, y++, Color.Green);
    text = "FARM INVADERS is a work of fiction designed, developed and published by a multicultural and gender-diverse team from a wide range of ethnic, philosophical and religious backgrounds.";
    for(int i = 0; i < text.length(); i++){
        cout<<text[i];
        if(i % 70 == 0 && i)
            gotoxy(Position.minX + 2, y++);
        Sleep(35);
    }
    Sleep(2000);
    y += 2;
    gotoxy(Position.minX + 2, y++, Color.Green);
    text = "To experience the full richness of the game, PLEASE turn your page FULLSCREEN.";
    for(int i = 0; i < text.length(); i++){
        cout<<text[i];
        if(i % 70 == 0 && i)
            gotoxy(Position.minX + 2, y++);
        Sleep(35);
    }
    gotoxy(Position.minX + 2, y++, Color.Green);
    text = "Press ENTER to continue after doing so";
    for(int i = 0; i < text.length(); i++){
        cout<<text[i];
        if(i % 70 == 0 && i)
            gotoxy(Position.minX + 2, y++);
        Sleep(35);
    }
    char c;
    while(c != '\r'){
        Sleep(200);
        c = getch();
    }
    system("cls");
    MainMenu();
}
void printLogo(int x, int y, int index){
    cout<<Color.BrownGreen;
    
    if(index > 0)
        for(int i = 0; i < 5; i++)
            gotoxy(x - 1, y + i, "  ");
    else
        for(int i = 0; i < 5; i++)
            gotoxy(x + 26, y + i, "   ");

    //"FARM", line by line
    gotoxy(x + index, y++, Color.Choco);
    cout<<Block.Bottom<<SN(Block.Full, 4)<<"  "<<Block.Bottom<<SN(Block.Full, 3)<<Block.Bottom<<"  "<<SN(Block.Full, 2)<<SN(Block.Bottom, 3)<<
    "   "<<Block.Bottom<<SN(Block.Full, 3)<<Block.Bottom;
    
    gotoxy(x + index, y++);
    cout<<SN(Block.Full, 2)<<"    "<<SN(Block.Full, 2)<<Block.Top<<" "<<Block.Top<<SN(Block.Full, 2)<<" "<<SN(Block.Full, 2)<<SN(Block.Top, 3)<<
    Block.Full<<" "<<SN(Block.Full, 2)<<Block.Top<<Block.Full<<Block.Top<<SN(Block.Full, 2);
    
    gotoxy(x + index, y++);
    cout<<SN(Block.Full, 5)<<" "<<SN(Block.Full, 2)<<"   "<<SN(Block.Full, 2)<<" "<<SN(Block.Full, 2)<<SN(Block.Bottom, 3)<<Block.Full<<" "<<
    SN(Block.Full, 2)<<" "<<Block.Full<<" "<<SN(Block.Full, 2);
    
    gotoxy(x + index, y++);
    cout<<SN(Block.Full, 2)<<"    "<<SN(Block.Full, 7)<<" "<<SN(Block.Full, 2)<<SN(Block.Top, 2)<<Block.Full<<Block.Bottom<<" "<<SN(Block.Full, 2)<<
    "   "<<SN(Block.Full, 2);
    
    gotoxy(x + index, y++);
    cout<<SN(Block.Full, 2)<<"    "<<SN(Block.Full, 2)<<"   "<<SN(Block.Full, 2)<<" "<<SN(Block.Full, 2)<<"  "<<Block.Top<<Block.Full<<" "<<
    SN(Block.Full, 2)<<"   "<<SN(Block.Full, 2);

    if(index < 0)
        for(int i = 0; i < 6; i++)
            gotoxy(x - 15, y + i, "  ");
    else{
        gotoxy(x + 37, y + 2, "  ");
        for(int i = 0; i < 6; i++)
            gotoxy(x + 41, y + i, "   ");
    }

    //"INVADERS", line by line
    gotoxy(x - 14 - index, ++y, Color.Red);
    cout<<SN(Block.Full, 6)<<" "<<SN(Block.Full, 2)<<"   "<<SN(Block.Full, 2)<<" "<<Block.Full<<Block.Bottom<<"  "<<Block.Bottom<<
    Block.Full<<"  "<<Block.Bottom<<SN(Block.Full, 3)<<Block.Bottom<<"  "<<SN(Block.Full, 2)<<SN(Block.Bottom, 3)<<"  "<<Block.Bottom<<
    SN(Block.Full, 4)<<" "<<SN(Block.Full, 2)<<SN(Block.Bottom, 3)<<"  "<<Block.Bottom<<SN(Block.Full, 5);

    gotoxy(x - 14 - index, ++y);
    cout<<"  "<<SN(Block.Full, 2)<<"   "<<SN(Block.Full, 3)<<"  "<<SN(Block.Full, 2)<<" "<<SN(Block.Full, 2)<<"  "<<SN(Block.Full, 2)<<
    " "<<SN(Block.Full, 2)<<Block.Top<<" "<<Block.Top<<SN(Block.Full, 2)<<" "<<SN(Block.Full, 2)<<"  "<<SN(Block.Full, 2)<<" "<<
    SN(Block.Full, 2)<<"    "<<SN(Block.Full, 2)<<SN(Block.Top, 3)<<Block.Full<<" "<<SN(Block.Full, 2);

    gotoxy(x - 14 - index, ++y);
    cout<<"  "<<SN(Block.Full, 2)<<"   "<<SN(Block.Full, 2)<<" "<<Block.Full<<" "<<SN(Block.Full, 2)<<" "<<SN(Block.Full, 2)<<"  "<<
    SN(Block.Full, 2)<<" "<<SN(Block.Full, 2)<<"   "<<SN(Block.Full, 2)<<" "<<SN(Block.Full, 2)<<"  "<<SN(Block.Full, 2)<<" "<<
    SN(Block.Full, 5)<<" "<<SN(Block.Full, 2)<<SN(Block.Bottom, 3)<<Block.Top<<" "<<SN(Block.Full, 6);

    gotoxy(x - 14 - index, ++y);
    cout<<"  "<<SN(Block.Full, 2)<<"   "<<SN(Block.Full, 2)<<"  "<<SN(Block.Full, 3)<<" "<<SN(Block.Full, 2)<<"  "<<SN(Block.Full, 2)<<
    " "<<SN(Block.Full, 7)<<" "<<SN(Block.Full, 2)<<"  "<<SN(Block.Full, 2)<<" "<<SN(Block.Full, 2)<<"    "<<SN(Block.Full, 2)<<
    SN(Block.Top, 2)<<Block.Full<<Block.Bottom<<"     "<<SN(Block.Full, 2);

    gotoxy(x - 14 - index, ++y);
    cout<<SN(Block.Full, 6)<<" "<<SN(Block.Full, 2)<<"   "<<SN(Block.Full, 2)<<"  "<<Block.Top<<SN(Block.Full, 2)<<Block.Top<<"  "<<
    SN(Block.Full, 2)<<"   "<<SN(Block.Full, 2)<<" "<<SN(Block.Full, 2)<<Block.Bottom<<Block.Full<<Block.Top<<"  "<<Block.Top<<SN(Block.Full, 4)<<
    " "<<SN(Block.Full, 2)<<"  "<<Block.Top<<Block.Full<<" "<<SN(Block.Full, 5)<<Block.Top;
}
void printButton(int x, int y, int indent, string textColor, string roundColor, string text){
    gotoxy(x, y, roundColor + Block.Bottom);
    for(int j = x + 1; j < x + 13; j++)
        gotoxy(j, y, textColor + " ");
    gotoxy(x + 13, y++, roundColor + Block.Bottom);

    for(int j = x; j < x + 14; j++)
        gotoxy(j, y, textColor + " ");
    gotoxy(x + indent, y, text);
    
    gotoxy(x, ++y, roundColor + Block.Top);
    for(int j = x + 1; j < x + 13; j++)
        gotoxy(j, y, textColor + " ");
    gotoxy(x + 13, y, roundColor + Block.Top);
}
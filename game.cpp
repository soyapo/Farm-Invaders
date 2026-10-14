#include <iostream>
#include <string>
#include <conio.h>
#include <chrono>
#include <cstdlib>
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING //not in MinGW 6.3's wincon.h
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
using namespace std; //for cin and cout mainly
typedef const string CS; //to avoid long repetitions

//---structs to tidy up global variables---
struct Position{
    int minX, maxX;
    int heroX, heroY;
    int mouseX, mouseY;
    int bufferX, bufferY;
    int bulletX, bulletY, isBullet;

    clock_t lastRabbitMove = 0, lastBulletMove, lastLogoMove;
    clock_t MissileInterval, lastMissileMove;
} Position;
struct Block{ //unicode blocks
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
struct Barrier{
    pair<pair<int, int>, pair<string, string>>ObsData[300][200];
    string ObsOrder[6][4] = {
        {" ", Block.LightShade, Block.DarkShade, Block.Full},
        {" ", Block.QuadLeft, Block.Bottom, ""},
        {" ", Block.QuadRight, Block.Top, ""},
        {" ", Block.Left, "", ""},
        {" ", Block.Right, "", ""},
        {"", "", "", ""}
    };
} Barrier;
struct Color{ //ANSI Colors
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
struct Heros{//character maps
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
const int MAXROWS = 5; //vertical cap on the enemy formation
struct Enemy{
    int X, Y, Dir, XP;
    int Rows;                       //how many rows the current wave has
    pair <int, int> Grid[300][200]; //collision map: {alive, rowCol id = r*8 + c}
    int Exist[MAXROWS][8];          //per-enemy: 1 alive, <0 dying, 0 gone
};
struct GameStateT{ //live gameplay state
    int score;
    int lives;
    int alive;      //enemies still shootable in this wave
    int wave;       //1-based wave counter
    int waveTotal;  //enemies this wave started with (for pacing)
    bool over;      //player lost
    bool won;       //unused now that waves are endless; kept for EndScreen
} GameState;
struct MissileT{ //enemy bombs raining down
    static const int MAX = 6;
    int x[MAX], y[MAX];
    bool active[MAX];
} Missiles;

//---Globals---
Enemy Rabbit, Chicken;

//---Functions---
void Game();

void InitConsole();
void InitMain();
void InitGame();
void InitBarrier();
void CreateBarrier(int, int, int);

void DrawHUD();
bool DamageBarrier(int x, int y);
void SpawnMissile();
void UpdateMissiles();
void HeroHit();
void SetupWave();
void WaveCleared();
void EndScreen(bool won);

string SN(string s, int n);
void getMousePos(int &x, int &y);
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
    InitConsole(); //enable UTF-8 glyphs + ANSI colors, hide the cursor
    InitMain();    //compute buffer metrics so the intro lands in the right place
    firstText();   //intro -> MainMenu -> Game
    return 0;
}

//---------------------------------------------//

void Game(){
    InitGame();

    Position.lastRabbitMove  = clock() - 5000; //draw the formation on frame one
    Position.lastBulletMove  = clock();
    Position.lastMissileMove = clock();
    clock_t lastSpawn = clock();
    bool quit = false;

    while(!GameState.over && !GameState.won && !quit){
        //---input---
        if(_kbhit()){
            char c = getch();
            if(c == 'p' || c == 27) //'p' or ESC -> back to menu
                quit = true;
            else if(c == 'a' || c == 'A'){
                Position.heroX = max(Position.minX, Position.heroX - 1);
                gotoxy(Position.heroX, Position.heroY);
                PrintHero(Heros.Hero, 6, 7, Position.heroX, Position.heroY, 1);
            }
            else if(c == 'd' || c == 'D'){
                Position.heroX = min(Position.maxX - 7, Position.heroX + 1);
                gotoxy(Position.heroX, Position.heroY);
                PrintHero(Heros.Hero, 6, 7, Position.heroX, Position.heroY, -1);
            }
            else if((c == '\r' || c == ' ') && !Position.isBullet){
                Position.bulletX = Position.heroX + 3, Position.bulletY = Position.heroY - 1,
                Position.isBullet = 1, Position.lastBulletMove = clock();
            }
        }
        if((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !Position.isBullet){
            Position.bulletX = Position.heroX + 3, Position.bulletY = Position.heroY - 1,
            Position.isBullet = 1, Position.lastBulletMove = clock();
        }

        //---player bullet---
        if(Position.isBullet && (clock() - Position.lastBulletMove) >= 30){
            PrintBullet(Position.bulletX, Position.bulletY);
            Position.lastBulletMove = clock();
        }

        //---enemy formation: pace scales with the fraction left + the wave---
        int maxI = 700 - GameState.wave * 40; if(maxI < 250) maxI = 250;
        int minI = 120;
        double frac = GameState.waveTotal > 0 ? (double)GameState.alive / GameState.waveTotal : 0.0;
        int rabbitInterval = minI + (int)((maxI - minI) * frac);
        if(clock() - Position.lastRabbitMove >= rabbitInterval){
            PrintRabbit();
            Position.lastRabbitMove = clock();
            //lose if the bottom of the formation has marched down to the hero
            int bottom = Rabbit.Y + (Rabbit.Rows - 1) * 4 + 2;
            if(bottom >= Position.heroY)
                GameState.over = true;
        }

        //---enemy bombs: more frequent and faster on later waves---
        int spawnInterval = 1000 - GameState.wave * 80; if(spawnInterval < 300) spawnInterval = 300;
        if(clock() - lastSpawn >= (clock_t)spawnInterval){
            SpawnMissile();
            lastSpawn = clock();
        }
        int missileInterval = 55 - GameState.wave * 3; if(missileInterval < 30) missileInterval = 30;
        if(clock() - Position.lastMissileMove >= missileInterval){
            UpdateMissiles();
            Position.lastMissileMove = clock();
        }

        //---wave cleared -> escalate to the next, larger wave (endless)---
        if(GameState.alive <= 0){
            WaveCleared();
            Position.lastRabbitMove  = clock() - 5000; //draw the new wave immediately
            lastSpawn                = clock();
            Position.lastMissileMove = clock();
        }

        gotoxy(Position.heroX, Position.heroY); //park the cursor off the sprites
        Sleep(3); //hand time back to the OS instead of burning a full core
    }

    if(!quit)
        EndScreen(GameState.won);
}
void InitConsole(){
    //Without these, the Unicode block glyphs and every ANSI colour code below
    //render as garbage. This is the single most important fix for the visuals.
    SetConsoleOutputCP(CP_UTF8);                 //make cout emit UTF-8 bytes correctly
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if(GetConsoleMode(h, &mode))
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING); //interpret \033[ codes
    CONSOLE_CURSOR_INFO cci;                      //hide the blinking caret so it stops
    if(GetConsoleCursorInfo(h, &cci)){            //flickering all over the sprites
        cci.bVisible = FALSE;
        SetConsoleCursorInfo(h, &cci);
    }
    //NOTE: do NOT call ios::sync_with_stdio(false) here. This game renders with
    //gotoxy()+cout and never flushes, so an independently-buffered cout would
    //leave the screen blank until the buffer fills. Keeping the default sync
    //with C stdio makes each write reach the interactive console.
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

    //reset live gameplay state for a fresh game
    GameState.score = 0, GameState.lives = 3, GameState.wave = 1;
    GameState.over  = false, GameState.won = false;
    for(int i = 0; i < Missiles.MAX; i++)
        Missiles.active[i] = false;

    SetupWave(); //build the first wave's formation + collision grid

    PrintDecor();
    InitBarrier();
    PrintHero(Heros.Hero, 6, 7, Position.heroX, Position.heroY, 0);
    DrawHUD();
}
void SetupWave(){
    //Wave 1 has 2 rows; each subsequent wave adds one, capped at MAXROWS and
    //by however many rows actually fit above the barriers on this console.
    int wanted = 1 + GameState.wave;
    int fit    = (Position.bufferY - 13 - 2) / 4; //space between HUD and barriers
    if(fit < 1) fit = 1;
    Rabbit.Rows = min(min(wanted, MAXROWS), fit);
    //start the block at the top-left of the play field, marching east
    Rabbit.X = Position.minX, Rabbit.Y = 2, Rabbit.Dir = 1;

    //clear the collision grid across the whole play area first
    for(int gx = Position.minX; gx < Position.minX + 71 && gx < 300; gx++)
        for(int gy = 0; gy < Position.bufferY && gy < 200; gy++)
            Rabbit.Grid[gx][gy].first = 0, Rabbit.Grid[gx][gy].second = 0;

    GameState.alive = 0;
    for(int r = 0; r < Rabbit.Rows; r++){
        int by = Rabbit.Y + r * 4; //3-tall sprites, 1-row gap => stride 4
        for(int c = 0; c < 8; c++){
            Rabbit.Exist[r][c] = 1;
            GameState.alive++;
            for(int cx = 7 * c; cx < 7 * c + 5; cx++)
                for(int ry = by; ry < by + 3; ry++)
                    Rabbit.Grid[cx + Rabbit.X][ry].first  = 1,
                    Rabbit.Grid[cx + Rabbit.X][ry].second = r * 8 + c;
        }
    }
    GameState.waveTotal = GameState.alive;
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
    //Convert the OS cursor position into console cell coordinates.
    //Instead of trusting a font-size API (dwFontSize is unreliable and ignores
    //display scaling), derive the cell size straight from the client rectangle
    //divided by the number of visible cells. Both the cursor and the client
    //rect live in the same pixel space, so this is DPI-correct by construction.
    POINT p;
    GetCursorPos(&p);
    HWND hwnd = GetConsoleWindow();
    ScreenToClient(hwnd, &p); //pixels relative to the client top-left

    RECT rc;
    GetClientRect(hwnd, &rc);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);

    int cols = csbi.srWindow.Right  - csbi.srWindow.Left + 1; //visible columns
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1; //visible rows
    int clientW = rc.right  - rc.left;
    int clientH = rc.bottom - rc.top;

    int cw = (cols > 0 && clientW > 0) ? clientW / cols : 8;  //cell width  (px)
    int ch = (rows > 0 && clientH > 0) ? clientH / rows : 16; //cell height (px)
    if(cw <= 0) cw = 8;
    if(ch <= 0) ch = 16;

    //add the scroll offset so we land in absolute buffer coords (what gotoxy uses)
    x = p.x / cw + csbi.srWindow.Left;
    y = p.y / ch + csbi.srWindow.Top;

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
    const int stride = 4;                              //3 sprite rows + 1 gap
    int blockH = (Rabbit.Rows - 1) * stride + 3;       //full height of the block

    //---(A) clear the collision grid over the block's whole footprint (plus a
    //one-cell margin for the impending step), so no stale "ghost" cells survive
    //a horizontal move or a descent and cause phantom hits---
    for(int ry = Rabbit.Y; ry <= Rabbit.Y + blockH && ry < 200; ry++)
        for(int rx = Rabbit.X - 1; rx <= Rabbit.X + 54 && rx < 300; rx++)
            if(rx >= 0 && ry >= 0)
                Rabbit.Grid[rx][ry].first = 0, Rabbit.Grid[rx][ry].second = 0;

    //---(B) bounce at the edges: wipe the whole block box, drop a row, reverse---
    if((Rabbit.Dir == 1 && Rabbit.X == Position.minX + 15) ||
       (Rabbit.Dir == -1 && Rabbit.X == Position.minX)){
        for(int rr = Rabbit.Y; rr < Rabbit.Y + blockH; rr++){
            gotoxy(Position.minX, rr);
            for(int j = 0; j < 70; j++)
                cout << ' ';
        }
        Rabbit.Y++, Rabbit.Dir *= -1;
    }

    Rabbit.X += Rabbit.Dir;

    //---(C) mark the collision grid at the new position---
    for(int r = 0; r < Rabbit.Rows; r++){
        int by = Rabbit.Y + r * stride;
        for(int c = 0; c < 8; c++)
            for(int cx = 7 * c; cx < 7 * c + 5; cx++)
                for(int ry = by; ry < by + 3; ry++){
                    Rabbit.Grid[cx + Rabbit.X][ry].second = r * 8 + c;
                    Rabbit.Grid[cx + Rabbit.X][ry].first  = (Rabbit.Exist[r][c] == 1);
                }
    }

    //---(D) draw every band---
    for(int r = 0; r < Rabbit.Rows; r++){
        int by = Rabbit.Y + r * stride;
        int f  = (Rabbit.X + r) % 2; //stagger the walk cycle between rows
        for(int c = 0, x = Rabbit.X; c < 8; c++, x += 7){
            for(int j = 0, y = by; j < 3; j++){
                gotoxy(x, y++);

                if(Rabbit.Exist[r][c] == 1)
                    for(int k = 0; k < 5; k++)
                        cout << Heros.Rabbit[f][j][k].second << Heros.Rabbit[f][j][k].first;
                else if(Rabbit.Exist[r][c] < 0){
                    Rabbit.Exist[r][c]--;
                    for(int k = 0; k < 5; k++)
                        cout << Heros.Rabbit[2][j][k].second << Heros.Rabbit[2][j][k].first;
                }
                else
                    for(int k = 0; k < 5; k++)
                        cout << Heros.Empty[j][k].second << Heros.Empty[j][k].first;

                //erase the trailing column for this row only (stay inside the band)
                gotoxy(x + (Rabbit.Dir > 0 ? -1 : 5), by + j, " ");

                if(Rabbit.Exist[r][c] == -3)
                    Rabbit.Exist[r][c] = 0;
            }
        }
    }
    cout << Color.Reset;
}
void PrintBullet(int &x, int &y){
    //clear the trail cell the bullet just vacated (it travels upward)
    if(y + 1 < Position.heroY)
        gotoxy(x, y + 1, " ");

    //Barrier hit sequence (shared with the enemy bombs)
    if(DamageBarrier(x, y)){
        Position.isBullet = 0;
    }

    //enemy hit sequence
    else if(Rabbit.Grid[x][y].first == 1){
        int id   = Rabbit.Grid[x][y].second;      //capture before we zero the grid
        int r    = id / 8, c = id % 8;            //decode row + column
        int left = Rabbit.X + 7 * c;
        int top  = Rabbit.Y + r * 4;
        Rabbit.Exist[r][c] = -1, Position.isBullet = 0; //start the death animation
        GameState.alive--;
        GameState.score += 100 + (Rabbit.Rows - 1 - r) * 20; //back rows worth more
        DrawHUD();
        //zero this enemy's collision cells so it can't be hit twice
        for(int j = top; j < top + 3; j++)
            for(int i = left; i < left + 5; i++)
                Rabbit.Grid[i][j].first = 0;
        //print the explosion sprite across ALL three rows (was a one-row bug:
        //the inner loop variable y never advanced, so only the top row drew)
        for(int j = 0; j < 3; j++){
            gotoxy(left, top + j);
            for(int k = 0; k < 5; k++)
                cout<<Heros.Rabbit[2][j][k].second<<Heros.Rabbit[2][j][k].first;
        }
    }
    else{
        int frame = (y % 24) / 12;                 //flicker the bullet between two glyphs
        gotoxy(x, y, Color.Yellow + Block.Bullet[frame]);
        y--;
    }

    //fizzled out just under the HUD without hitting anything
    if(y < 1){
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

    // for(int i = 0; i < Position.bufferY; i++)
    //     for(int j = 0; j < 70; j++)
    //         gotoxy(Position.minX + j, i, Color.BrownGreen + " ");
}
void DrawHUD(){
    //score (zero padded) on the left, lives as little blocks on the right
    string s = to_string(GameState.score);
    while(s.size() < 5) s = "0" + s;
    gotoxy(Position.minX, 0, Color.Yellow + "SCORE " + Color.White + s);

    gotoxy(Position.minX + 32, 0, Color.Orange + "WAVE " + Color.White + to_string(GameState.wave) + " ");

    string lives = Color.Green + "LIVES " + Color.Yellow;
    for(int i = 0; i < GameState.lives; i++)
        lives += Block.Full;
    lives += "   "; //wipe a stale icon after losing a life
    gotoxy(Position.minX + 58, 0, lives);
    cout << Color.Reset;
}
bool DamageBarrier(int x, int y){
    //erode one layer of a barrier cell; returns true if a shot was absorbed
    if(x < 0 || y < 0 || x >= 300 || y >= 200)
        return false;
    if(Barrier.ObsData[x][y].first.second){ //this cell still has integrity left
        int &lvl = Barrier.ObsData[x][y].first.second;
        Barrier.ObsData[x][y].second.first =
            Barrier.ObsOrder[Barrier.ObsData[x][y].first.first][--lvl];
        gotoxy(x, y, Barrier.ObsData[x][y].second.second + Barrier.ObsData[x][y].second.first);
        cout << Color.Reset;
        return true;
    }
    return false;
}
void SpawnMissile(){
    //a random column that still has an enemy drops a bomb from its lowest one
    int cols[8], n = 0;
    for(int c = 0; c < 8; c++)
        for(int r = 0; r < Rabbit.Rows; r++)
            if(Rabbit.Exist[r][c] == 1){ cols[n++] = c; break; }
    if(n == 0)
        return;
    int slot = -1;
    for(int i = 0; i < Missiles.MAX; i++)
        if(!Missiles.active[i]){ slot = i; break; }
    if(slot < 0)
        return;
    int c = cols[rand() % n];
    int lowest = 0; //deepest living row in that column
    for(int r = 0; r < Rabbit.Rows; r++)
        if(Rabbit.Exist[r][c] == 1)
            lowest = r;
    Missiles.x[slot] = Rabbit.X + 7 * c + 2;      //centre of the 5-wide sprite
    Missiles.y[slot] = Rabbit.Y + lowest * 4 + 3; //just below that enemy
    Missiles.active[slot] = true;
}
void UpdateMissiles(){
    for(int i = 0; i < Missiles.MAX; i++){
        if(!Missiles.active[i])
            continue;
        int x = Missiles.x[i], y = Missiles.y[i];
        gotoxy(x, y, " ");   //erase the old position
        int ny = y + 1;

        if(DamageBarrier(x, ny))                        //chewed up by a barrier
            Missiles.active[i] = false;
        else if(ny >= Position.heroY && ny <= Position.heroY + 5 &&
                x >= Position.heroX && x <= Position.heroX + 6){ //struck the hero
            Missiles.active[i] = false;
            HeroHit();
        }
        else if(ny >= Position.bufferY - 2)             //fizzled into the ground
            Missiles.active[i] = false;
        else{                                           //keep falling
            Missiles.y[i] = ny;
            gotoxy(x, ny, Color.Red + (ny % 2 ? Block.QuadLeft : Block.QuadRight));
            cout << Color.Reset;
        }
    }
}
void HeroHit(){
    GameState.lives--;
    DrawHUD();
    //flash the hero red so the hit reads clearly, then restore the sprite
    for(int r = 0; r < 6; r++){
        gotoxy(Position.heroX, Position.heroY + r);
        for(int c = 0; c < 7; c++)
            cout << Color.Red << Block.Full;
    }
    cout << Color.Reset;
    Sleep(120);
    PrintHero(Heros.Hero, 6, 7, Position.heroX, Position.heroY, 0);
    if(GameState.lives <= 0)
        GameState.over = true;
}
void WaveCleared(){
    //wave bonus + a short banner, then build the next, larger wave
    GameState.score += 200;
    GameState.wave++;

    CS GreenWhite = "\033[38;2;92;172;45;48;2;255;255;255m";
    string msg = " WAVE " + to_string(GameState.wave) + " ";
    int bx = Position.minX + (70 - (int)msg.size()) / 2, by = Position.bufferY / 2 - 1;
    gotoxy(bx, by, GreenWhite + msg);
    cout << Color.Reset;
    Sleep(1100);

    //wipe the enemy zone (HUD on row 0 stays) and any bombs in flight
    for(int rr = 1; rr < Position.bufferY - 13; rr++){
        gotoxy(Position.minX, rr);
        for(int j = 0; j < 70; j++)
            cout << ' ';
    }
    for(int i = 0; i < Missiles.MAX; i++)
        Missiles.active[i] = false;

    SetupWave();
    InitBarrier(); //fresh cover each wave to offset the rising difficulty
    DrawHUD();
    PrintHero(Heros.Hero, 6, 7, Position.heroX, Position.heroY, 0); //redraw over the banner
}
void EndScreen(bool won){
    Sleep(350); //let the final explosion/flash settle
    int boxW = 46, boxH = 5;
    int boxX = Position.minX + (70 - boxW) / 2, boxY = Position.bufferY / 2 - 2;
    for(int r = 0; r < boxH; r++){
        gotoxy(boxX, boxY + r);
        for(int c = 0; c < boxW; c++)
            cout << Color.BlackWhite << " ";
    }
    cout << Color.Reset;

    CS GreenWhite = "\033[38;2;92;172;45;48;2;255;255;255m"; //green text on the white banner
    string title = won ? "*** WAVE CLEARED! ***" : "*** GAME OVER ***";
    gotoxy(boxX + (boxW - (int)title.size()) / 2, boxY + 1,
           (won ? GreenWhite : Color.RedWhite) + title);
    string sc = "FINAL SCORE: " + to_string(GameState.score);
    gotoxy(boxX + (boxW - (int)sc.size()) / 2, boxY + 2, Color.BlackWhite + sc);
    string pr = "Press ENTER to return to the menu";
    gotoxy(boxX + (boxW - (int)pr.size()) / 2, boxY + 3, Color.BlackWhite + pr);
    cout << Color.Reset;

    char c = 0;
    while(c != '\r'){
        if(_kbhit())
            c = getch();
        else
            Sleep(20);
    }
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
                
    int index = 1, first = 1;
    bool shart;

    Position.lastLogoMove = clock();
    while(true){
        if(clock() - Position.lastLogoMove> 700 || first){
            index *= -1, Position.lastLogoMove = clock();
            printLogo(Position.minX + 21, 5, index);
        }
        first = 0;
        getMousePos(Position.mouseX, Position.mouseY);
        
        //Game
        shart = Position.mouseY <= Position.bufferY - 16 && Position.mouseY >= Position.bufferY - 18 && Position.mouseX >= Position.minX + 26 && Position.mouseX <= Position.minX + 40;
        if(!button[0] && shart){
            button[0] = 1;
            printButton(Position.minX + 28, Position.bufferY - 17, 2, Color.WhiteBrown, Color.ChocoGreen, "START GAME");    
        }
        else if(button[0] && !shart){
            button[0] = 0;
            printButton(Position.minX + 28, Position.bufferY - 17, 2, Color.BlackWhite, Color.WhiteGreen, "START GAME");    
        }
        else if(button[0] && shart && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)){
            cout<<Color.Reset;
            system("cls");
            Game();
            while(GetAsyncKeyState(VK_LBUTTON) & 0x8000) //wait for release so we
                Sleep(10);                                //don't instantly re-launch
            return MainMenu(); //InitMain() at its top redraws a fresh menu
        }

        //Leaderboard
        shart = Position.mouseY <= Position.bufferY - 12 && Position.mouseY >= Position.bufferY - 14 && Position.mouseX >= Position.minX + 26 && Position.mouseX <= Position.minX + 40;
        if(!button[1] && shart){
            button[1] = 1;
            printButton(Position.minX + 28, Position.bufferY - 13, 1, Color.WhiteBrown, Color.ChocoGreen, "LEADERBOARDS");    
        }
        else if(button[1] && !shart){
            button[1] = 0;
            printButton(Position.minX + 28, Position.bufferY - 13, 1, Color.BlackWhite, Color.WhiteGreen, "LEADERBOARDS");    
        }
        else if(button[1] && shart && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)){
            // cout<<Color.Reset;
            // system("cls");
            // Game();
        }

        //SETTINGS
        shart = Position.mouseY <= Position.bufferY - 8 && Position.mouseY >= Position.bufferY - 10 && Position.mouseX >= Position.minX + 26 && Position.mouseX <= Position.minX + 40;
        if(!button[2] && shart){
            button[2] = 1;
            printButton(Position.minX + 28, Position.bufferY - 9, 3, Color.WhiteBrown, Color.ChocoGreen, "SETTINGS");    
        }
        else if(button[2] && !shart){
            button[2] = 0;
            printButton(Position.minX + 28, Position.bufferY - 9, 3, Color.BlackWhite, Color.WhiteGreen, "SETTINGS");    
        }
        else if(button[2] && shart && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)){
            // cout<<Color.Reset;
            // system("cls");
            // Game();
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

        Sleep(5); //ease off the CPU; the menu doesn't need to spin at full tilt
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
    char c = 0;
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
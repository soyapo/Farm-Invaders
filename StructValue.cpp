#include <string>
using namespace std;
typedef const string CS;

//---structs to tidy up global variables---
struct{ //unicode blocks
    CS Full = "\u2588"; //Full cell
    CS Bottom = "\u2584"; //Bottom half cell
    CS Top = "\u2580"; //Top half cell
} Block;

struct{ //ANSI Colors
    CS Yellow = "\033[38;5;226m";
    CS Orange = "\033[38;2;255;165;0m";
    CS Navy = "\033[38;2;0;128;255m";
    CS Black = "\033[38;2;0;0;0m";  
    CS White = "\033[38;2;255;255;255m";
    CS Reset = "\033[0m"; //Default value
} Color;

struct{//character maps
    const pair<string, string> Hero[6][7] = { //main character (intended to resemble a duck =D)
        {{" ", Color.Reset},        {Block.Bottom, Color.Black}, {Block.Bottom, Color.Yellow}, {Block.Bottom, Color.Yellow}, {Block.Bottom, Color.Yellow}, {Block.Bottom, Color.Black}, {" ", Color.Reset}},
        {{Block.Full, Color.Black}, {Block.Full, Color.Yellow},  {Block.Full, Color.Navy},     {Block.Full, Color.Yellow},   {Block.Full, Color.Navy},     {Block.Full, Color.Yellow},  {Block.Full, Color.Black},},
        {{Block.Full, Color.Black}, {Block.Full, Color.Yellow},  {Block.Full, Color.Yellow},   {Block.Full, Color.Orange},   {Block.Full, Color.Yellow},   {Block.Full, Color.Yellow},  {Block.Full, Color.Black},},
        {{" ", Color.Reset},        {Block.Full, Color.Black},   {Block.Full, Color.Yellow},   {Block.Full, Color.Yellow},   {Block.Full, Color.Yellow},   {Block.Full, Color.Black},   {" ", Color.Reset}},
        {{Block.Top, Color.Black }, {Block.Top, Color.Yellow},   {Block.Full, Color.White},    {Block.Full, Color.Yellow},   {Block.Full, Color.White},    {Block.Top, Color.Yellow},   {Block.Top, Color.Black}},
        {{" ", Color.Reset},        {Block.Top, Color.Orange},   {Block.Top, Color.Orange},    {Block.Top, Color.Black},     {Block.Top, Color.Orange},    {Block.Top, Color.Orange},   {" ", Color.Reset}}
    };
} Heros;
#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;
#define fst ios_base::sync_with_stdio(0); cin.tie(NULL);

const std::string YELOW = "\033[38;5;226m";  // Bright yellow
const std::string ORANGE = "\033[38;2;255;165;0m";
const std::string DARK_YELLOW = "\033[38;2;204;153;0m";
const string NAVY = "\033[38;5;21m";
const std::string RESET = "\033[0m";
const string block = "\u2588";
const string half = "\u2584";
const string top = "\u2580";
const std::string BLACK = "\033[38;2;0;0;0m";  
const std::string WHITE = "\033[38;2;255;255;255m";
const std::string PASTEL_BLUE = "\033[38;2;0;128;255m";
const std::string PASTEL_YELLOW = "\033[38;2;253;253;150m";
const string duck = u8"\U0001F986";

void gotoxy(short x, short y, string s = ""){
    //prints string s with null default value in pos(x, y) of the std output.
    HANDLE h_std = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y};
    SetConsoleCursorPosition(h_std, pos); 
    cout<<YELOW<<s<<RESET;
}

int main(){
    int x = 0, y = 0;
    cout<<" "<<BLACK<<half<<YELOW<<half<<half<<half<<BLACK<<half<<endl<<RESET;
    cout<<BLACK<<block<<YELOW<<block<<PASTEL_BLUE<<block<<YELOW<<block<<PASTEL_BLUE<<block<<YELOW<<block<<BLACK<<block<<endl<<RESET;
    cout<<BLACK<<block<<YELOW<<block<<block<<ORANGE<<block<<YELOW<<block<<block<<BLACK<<block<<endl<<RESET;
    cout<<" "<<BLACK<<block<<YELOW<<block<<YELOW<<block<<YELOW<<block<<BLACK<<block<<" "<<endl<<RESET;
    cout<<BLACK<<top<<YELOW<<top<<WHITE<<block<<YELOW<<block<<WHITE<<block<<YELOW<<top<<BLACK<<top<<RESET<<endl;
    cout<<" "<<ORANGE<<top<<top<<BLACK<<top<<ORANGE<<top<<top<<endl;
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;

class Chessboard
{
public:
    Chessboard();
    int board_size = 20;
    vector<vector<string>> chess_board;
    vector<vector<string>> board_initialization(void);
    int board_end(void);
    void display(void);
    int drop_chess(int x, int y, int id);

};

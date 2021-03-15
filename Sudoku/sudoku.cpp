#include <stdio.h>
#include <stdlib.h>
#include <tuple> 
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <future>
#include <utility>  
using namespace std;


tuple<int, int> start_pos(vector<vector<int>> arr);
tuple<int, int> get_next(vector<vector<int>> arr, int x, int y);
vector<int> selected_values(vector<vector<int>> arr, int x, int y);
tuple<int, vector<vector<int>>>  try_sudoku(vector<vector<int>>& arr, int x, int y);
void print_sudoku(vector<vector<int>> arr);

int main()
{
    DWORD tStart = GetTickCount();
    int x;
    int y;
    int next_x;
    int next_y;
    int result;
    int vector_length;
    vector<int> candidated_values;
    vector<vector<int>> result_arr;
    std::vector<std::vector<int>> sudoku_grid;
    sudoku_grid.push_back(std::vector<int>{4, 2, 3, 7, 9, 8, 6, 1, 5});
    sudoku_grid.push_back(std::vector<int>{9, 6, 8, 4, 0, 0, 7, 3, 2});
    sudoku_grid.push_back(std::vector<int>{5, 7, 1, 0, 3, 2, 4, 8, 9});
    sudoku_grid.push_back(std::vector<int>{3, 5, 4, 9, 1, 6, 0, 2, 7});
    sudoku_grid.push_back(std::vector<int>{8, 0, 2, 5, 7, 4, 9, 6, 3});
    sudoku_grid.push_back(std::vector<int>{6, 9, 7, 8, 2, 0, 5, 4, 1});
    sudoku_grid.push_back(std::vector<int>{2, 8, 9, 3, 4, 7, 1, 5, 6});
    sudoku_grid.push_back(std::vector<int>{1, 4, 5, 2, 6, 9, 3, 7, 8});
    sudoku_grid.push_back(std::vector<int>{7, 3, 6, 1, 8, 5, 2, 9, 4});
    tie(x, y) = start_pos(sudoku_grid);
    candidated_values = selected_values(sudoku_grid);
    vector_length = candidated_values.size();
    for (int index=0; index < vector_length; index++)
    {
        temp_grid = sudoku_grid;
        temp_grid[x][y] = candidated_values[index];
        tie(next_x, next_y) = get_next(temp_grid, x, y);
        thread t (try_sudoku, temp_grid, next_x, next_y);
        t.join();

    }

    // tie(result, result_arr) = try_sudoku(sudoku_grid, x, y);
    DWORD tEnd = GetTickCount();
    // if (result == 1)
    // {
    //     print_sodoku(sudoku_grid);
    // }
    long tTickCount = tEnd - tStart;
    std::cout << "用了:" << tTickCount << " ticks" << std::endl;
    system("pause");
    return 0;
}

tuple<int, int> start_pos(vector<vector<int>> arr)
{
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            if (arr[x][y] == 0)
            {
                return make_tuple(x, y);
            }
        }
    }
    return make_tuple(-1, -1);
}

tuple<int, int> get_next(vector<vector<int>> arr, int x, int y)
{
    for (int next_y = y + 1; next_y < 9; next_y++)
    {
        if (arr[x][next_y] == 0)
        {
            return make_tuple(x, next_y);
        }
    }
    for (int next_x = x + 1; next_x < 9; next_x++)
    {
        for (int next_y = 0; next_y < 9; next_y++)
        {
            if (arr[next_x][next_y] == 0)
            {
                return make_tuple(next_x, next_y);
            }
        }
    }
    return make_tuple(-1, -1);
}

vector<int> selected_values(vector<vector<int>> arr, int x, int y)
{
    set<int> grid;
    set<int> full_number;
    set<int> full_row;
    set<int> full_col;
    set<int> differ_0;
    set<int> differ_1;
    set<int> differ_2;
    set<int> intersec_0;
    set<int> intersec_1;
    set<int>::iterator pos;
    vector<int> result;
    int i = int(x / 3);
    int j = int(y / 3);
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            grid.insert(arr[3 * i + row][3 * j + col]);
        }
    }
    for (int index = 1; index < 10; index++)
    {
        full_number.insert(index);
    }
    for (int col = 0; col < 9; col++)
    {
        full_row.insert(arr[x][col]);
    }
    for (int row = 0; row < 9; row++)
    {
        full_col.insert(arr[row][y]);
    }
    set_difference(full_number.begin(), full_number.end(), grid.begin(), grid.end(), inserter(differ_0, differ_0.begin()));
    set_difference(full_number.begin(), full_number.end(), full_row.begin(), full_row.end(), inserter(differ_1, differ_1.begin()));
    set_difference(full_number.begin(), full_number.end(), full_col.begin(), full_col.end(), inserter(differ_2, differ_2.begin()));
    set_intersection(differ_0.begin(), differ_0.end(), differ_1.begin(), differ_1.end(), inserter(intersec_0, intersec_0.begin()));
    set_intersection(intersec_0.begin(), intersec_0.end(), differ_2.begin(), differ_2.end(), inserter(intersec_1, intersec_1.begin()));

    for (pos = intersec_1.begin(); pos != intersec_1.end(); pos++)
    {
        result.push_back(*pos);
    }
    grid.clear();
    full_number.clear();
    full_row.clear();
    full_col.clear();
    differ_0.clear();
    differ_1.clear();
    differ_2.clear();
    intersec_0.clear();
    intersec_1.clear();
    return result;
}

tuple<int, vector<vector<int>>> try_sudoku(vector<vector<int>>& arr, int x, int y)
{
    vector<int> values = selected_values(arr, x, y);
    vector<int>::iterator pos;
    int next_x;
    int next_y;
    int end;
    vector<vector<int>> end_arr;
    for (pos = values.begin(); pos != values.end(); pos++)
    {
        arr[x][y] = *pos;
        tie(next_x, next_y) = get_next(arr, x, y);
        if (next_y == -1)
        { 
            print_sodoku(arr)
            return make_tuple(1, arr);
        }
        else
        {
            tie(end, end_arr) = try_sudoku(arr, next_x, next_y);
            if (end == 1)
            {   
                print_sodoku(arr)
                return make_tuple(1, arr);
            }
            arr[x][y] = 0;
        }
    }
    return make_tuple(0, arr);
}

void print_sudoku(vector<vector<int>> arr)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%d ", arr[i][j]);
            if (j % 3 == 2)
            {
                printf("|");
            }
        }
        printf("\n");
        if (i % 3 == 2)
        {
            printf("---------------------\n");
        }
    }
}
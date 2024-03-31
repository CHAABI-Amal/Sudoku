
#include <chrono>
#include <thread>
#include <time.h>
#include <QCoreApplication>
#include "Solve.h"
using namespace std;

int SpeedTime = 100;
#define N 9
int grid2[N][N] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0}
};

Solve::Solve(QObject* parent) :
    QThread(parent = 0)
{
    int test = 0;
}

bool Solve::isPresentInCol(int col, int num) { //check whether num is present in col or not
    for (int row = 0; row < N; row++)
        if (grid2[row][col] == num)
            return true;
    return false;
}
bool Solve::isPresentInRow(int row, int num) { //check whether num is present in row or not
    for (int col = 0; col < N; col++)
        if (grid2[row][col] == num)
            return true;
    return false;
}
bool Solve::isPresentInBox(int boxStartRow, int boxStartCol, int num) {
    //check whether num is present in 3x3 box or not
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid2[row + boxStartRow][col + boxStartCol] == num)
                return true;
    return false;
}
bool Solve::findEmptyPlace(int& row, int& col) { //get empty location and update row and column
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (grid2[row][col] == 0) //marked with 0 is empty
                return true;
    return false;
}
bool Solve::isValidPlace(int row, int col, int num) {
    //when item not found in col, row and current 3x3 box
    return !isPresentInRow(row, num) && !isPresentInCol(col, num) && !isPresentInBox(row - row % 3,
        col - col % 3, num);
}
bool Solve::solveSudoku() {
    int row, col;
    if (!findEmptyPlace(row, col))
        return true; //when all places are filled
    for (int num = 1; num <= 9; num++) { //valid numbers are 1 - 9
        if (isValidPlace(row, col, num)) { //check validation, if yes, put the number in the grid
            grid2[row][col] = num;
            emit NewItem(row, col, num);
            if (mStop == true) return true;
            std::this_thread::sleep_for(std::chrono::milliseconds(SpeedTime));
            
            if (solveSudoku()) //recursively go for other rooms in the grid
                return true;
            grid2[row][col] = 0; //turn to unassigned space when conditions are not satisfied
        }
    }

    
    return false;
}


void Solve::start(int inputGrid[9][9], int speed) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid2[i][j] = inputGrid[i][j];
        }
    }
    if (speed == 0) speed = 1;
    SpeedTime = speed;
    mStop = false;

    bool test = solveSudoku();
    qDebug() << "Solved";
}

void Solve::Stop()
{
    mStop = true;
}
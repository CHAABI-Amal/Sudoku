#include <thread>
#include "Sudoku.h"
#include <chrono>
#include <thread>
#include <time.h>
#include <iostream>

#include <QCoreApplication>
#include <QThread>

int NumSolutionFound = 0;
const int EMPTY = 0;
#define N 9
int grid[N][N] = {
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

int previousgrid[N][N] = {
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

Sudoku::Sudoku(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::SudokuClass)
{
    ui->setupUi(this);
}

Sudoku::~Sudoku()
{
    delete ui;
}

void Sudoku::on_Clear_clicked()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            ui->Table->setItem(i, j, new QTableWidgetItem);
            grid[i][j] = EMPTY;
        }
    }
}


void Sudoku::on_SolveSlow_clicked()
{
    ui->ErrorLabel->setText(" ");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j] = EMPTY;
        }
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            QTableWidgetItem* cell = ui->Table->item(i, j);
            if (cell) {
                if (cell->text().toInt() > 0)
                {
                    if (cell->text().toInt() >= 10)
                    {
                        ui->ErrorLabel->setText("Cell greater than 9");
                        return;
                    }
                    if (isValidPlace(i, j, cell->text().toInt()) == false)
                    {
                        ui->ErrorLabel->setText("Incorrect Board");
                        return;
                    }
                }
                grid[i][j] = cell->text().toInt();
            }
            previousgrid[i][j] = grid[i][j];
        }
    }

    GreyCells();
    connect(this, &Sudoku::on_Stopped, &SolveThread, &Solve::Stop);
    connect(&SolveThread, &Solve::NewItem, this, &Sudoku::DisplaySingleItem);


    QtConcurrent::run(&Solve::start, &this->SolveThread, grid, ui->horizontalSlider->value());
    
    DisplayGrid();
}
void Sudoku::on_Stop_Recussion_clicked()
{
    emit on_Stopped();
}


void Sudoku::on_Solve_clicked()
{
    ui->ErrorLabel->setText(" ");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j] = EMPTY;
        }
    }
    

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            QTableWidgetItem* cell = ui->Table->item(i, j);
            if (cell) {
                if(cell->text().toInt() > 0)
                { 
                    if (cell->text().toInt() >= 10)
                    {
                        ui->ErrorLabel->setText("Cell greater than 9");
                        return;
                    }
                    if (isValidPlace(i, j, cell->text().toInt()) == false)
                    {
                        ui->ErrorLabel->setText("Incorrect Board");
                        return;
                    }
                }
                grid[i][j] = cell->text().toInt();
            }
            previousgrid[i][j] = grid[i][j];
        }
    }
    GreyCells();
    qDebug() << "Solving";
    bool test = solveSudoku();
    qDebug() << "Solved";

    NumSolutionFound = 0;
    bool test1 = solveSudokuNumber();
    
    DisplayGrid();
    
}

void Sudoku::on_Undo_Solve_clicked() {

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j] = previousgrid[i][j];
            ui->Table->setItem(i, j, new QTableWidgetItem);
            if (grid[i][j] > 0) {
                QString str = QString::number(grid[i][j]);
                ui->Table->item(i, j)->setText(str);
                ui->Table->item(i, j)->setBackground(Qt::white);
            }

        }
    }

}



void Sudoku::DisplayGrid()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            ui->Table->setItem(i, j, new QTableWidgetItem);
            if (grid[i][j]>0){
                QString str = QString::number(grid[i][j]);
                ui->Table->item(i, j)->setText(str);
                if (previousgrid[i][j] > 0)
                {
                    ui->Table->item(i, j)->setBackground(Qt::gray);
                }
            }
        }
    }
    update();
}

void Sudoku::GreyCells()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            ui->Table->setItem(i, j, new QTableWidgetItem);
            if (grid[i][j] > 0) {
                QString str = QString::number(grid[i][j]);
                ui->Table->item(i, j)->setText(str);
                ui->Table->item(i, j)->setBackground(Qt::gray);
            }
        }
    }
    update();
}

void Sudoku::DisplaySingleItem(int i, int j, int n)
{
    ui->Table->setItem(i, j, new QTableWidgetItem);
    QString str = QString::number(n);
    ui->Table->item(i, j)->setText(str);
}


bool Sudoku::isPresentInCol(int col, int num) { //check whether num is present in col or not
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}
bool Sudoku::isPresentInRow(int row, int num) { //check whether num is present in row or not
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}
bool Sudoku::isPresentInBox(int boxStartRow, int boxStartCol, int num) {
    //check whether num is present in 3x3 box or not
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[row + boxStartRow][col + boxStartCol] == num)
                return true;
    return false;
}
bool Sudoku::findEmptyPlace(int& row, int& col) { //get empty location and update row and column
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (grid[row][col] == 0) //marked with 0 is empty
                return true;
    return false;
}
bool Sudoku::isValidPlace(int row, int col, int num) {
    //when item not found in col, row and current 3x3 box
    return !isPresentInRow(row, num) && !isPresentInCol(col, num) && !isPresentInBox(row - row % 3,
        col - col % 3, num);
}
bool Sudoku::solveSudoku() {
    int row, col;
    if (!findEmptyPlace(row, col))
        return true; //when all places are filled
    for (int num = 1; num <= 9; num++) { //valid numbers are 1 - 9
        if (isValidPlace(row, col, num)) { //check validation, if yes, put the number in the grid
            grid[row][col] = num;
            if (solveSudoku()) //recursively go for other rooms in the grid
                return true;
            grid[row][col] = 0; //turn to unassigned space when conditions are not satisfied
        }
    }
    return false;
}

bool Sudoku::solveSudokuNumber() {
    int row, col;
    if (!findEmptyPlace(row, col))
        NumSolutionFound++;
        qDebug() << "Number solutions found is " << NumSolutionFound;
        return false; //when all places are filled
    for (int num = 1; num <= 9; num++) { //valid numbers are 1 - 9
        if (isValidPlace(row, col, num)) { //check validation, if yes, put the number in the grid
            grid[row][col] = num;
            if (solveSudoku()) //recursively go for other rooms in the grid
                return true;
            grid[row][col] = 0; //turn to unassigned space when conditions are not satisfied
        }
    }
    return false;
}




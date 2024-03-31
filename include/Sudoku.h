#pragma once


#include "ui_Sudoku.h"
#include "Solve.h"
#include <QThread>

#include <QtConcurrent/QtConcurrent>
#include <QFuture>


class Sudoku : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sudoku(QWidget *parent = Q_NULLPTR);
    ~Sudoku();

    void DisplayGrid();

private:
    Ui::SudokuClass *ui;
    Solve SolveThread;

    bool isPresentInCol(int col, int num);
    bool isPresentInRow(int row, int num);
    bool isPresentInBox(int boxStartRow, int boxStartCol, int num);
    bool findEmptyPlace(int& row, int& col);
    bool isValidPlace(int row, int col, int num);
    bool solveSudoku();
    bool solveSudokuNumber();
    void GreyCells();
    
signals:
    void on_Stopped();

private slots:
    void on_Clear_clicked();
    void on_SolveSlow_clicked();
    void on_Solve_clicked();
    void on_Undo_Solve_clicked();
    void on_Stop_Recussion_clicked();

public slots:
    void DisplaySingleItem(int i, int j, int n);


};



#include "Sudoku.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sudoku w;
    w.show();
    return a.exec();
}

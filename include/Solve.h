
#include <QtWidgets/QMainWindow>
#include <QThread>


class Solve :
    public QThread
{
    Q_OBJECT
public:
    explicit Solve(QObject* parent = 0);

    void start(int inputGrid[9][9], int speed);
    bool solveSudoku();


public slots:
    void Stop();

private:
    bool mStop;
    bool isPresentInCol(int col, int num);
    bool isPresentInRow(int row, int num);
    bool isPresentInBox(int boxStartRow, int boxStartCol, int num);
    bool findEmptyPlace(int& row, int& col);
    bool isValidPlace(int row, int col, int num);

signals:
    void NewGrid(int grid2[9][9]);
    void NewItem(int i, int j, int n);

};
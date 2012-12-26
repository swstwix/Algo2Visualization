#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include <fstream>
#include <queue>
#include <QTextCodec>
#include <algorithm>
#include <iostream>
#include "inputdialog.h"

using namespace std;

namespace Ui {
class Visualization;
}

struct job {
    int a1,a2,a3;
};

struct work {
    int end, spend;
    job j;
    int n;

    work(job j, int end, int spend, int n) {
        this->end = end;
        this->spend = spend;
        this->j = j;
        this->n = n;
    }

    bool operator<(const work w) const
    {
        return this->end > w.end;
    }
};

class Visualization : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Visualization(QWidget *parent = 0);
    QString fileName;
    ~Visualization();
    void startVisualize();
    void virtual paintEvent(QPaintEvent *);
    void DrawReshenie(QPainter&, QString);
    void DrawOneWork(QPainter& q, work w, int res);
    
private slots:
    void on_action_4_triggered();

    void on_action_triggered();

private:
    Ui::Visualization *ui;
};

#endif // VISUALIZATION_H

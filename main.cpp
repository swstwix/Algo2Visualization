#include <QtGui/QApplication>
#include "visualization.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Visualization w;
    w.show();
    
    return a.exec();
}

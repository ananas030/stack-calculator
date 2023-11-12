#include "calculator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calculator w;
    w.setStyleSheet("background-color: #ADD8E6");
    w.resize(300, 300);
    w.show();
    return a.exec();
}

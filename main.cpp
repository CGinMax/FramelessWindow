#include "framelesswindow.h"
#include <QApplication>
#include <QWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FramelessWindow w;
    w.show();

    return a.exec();
}

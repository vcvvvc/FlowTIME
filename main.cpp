#include "widget.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("FlowTime");
    w.resize(320, 428);

    w.show();

    return a.exec();
}

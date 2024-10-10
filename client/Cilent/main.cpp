#include "cilent.h"
#include"index.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Cilent::getInstance().show();
    return a.exec();
}

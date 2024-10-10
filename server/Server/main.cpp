#include "server.h"
#include"operatedb.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OperateDB::getinstance().connect();
    Server::getinstance();
    return a.exec();
}

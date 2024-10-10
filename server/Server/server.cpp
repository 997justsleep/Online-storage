#include "server.h"
#include "ui_server.h"
#include"mytcpserver.h"
#include<QFile>
#include<QDebug>
#include<QTcpServer>

Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    //加载配置文件
    loadConfig();
    //监听
    MyTcpServer::getinstance().listen(QHostAddress(m_strIP),m_usPort);


}

Server::~Server()
{
    delete ui;
}

void Server::loadConfig()
{
    QFile file(":/server.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray baData = file.readAll();
        QString strData = QString(baData);
        QStringList strList = strData.split("\r\n");
        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        m_rootPath = strList.at(2);
        qDebug()<<"ip:"<<m_strIP<<"port:"<<m_usPort<<"rootdir:"<<m_rootPath;
        file.close();
    }else{
        qDebug()<<"打开配置文件失败";
    }
}

Server &Server::getinstance()
{
    static Server instance;
    return instance;
}

QString Server::getRootPath()
{
    return m_rootPath;
}


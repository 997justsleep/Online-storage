#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include"mytcpsocket.h"
#include <QObject>
#include<QTcpServer>
#include<QList>

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static MyTcpServer& getinstance();
    void incomingConnection(qintptr handle);
    void Forward(QString Tarname , PDU* pdu);
public slots:
    void deletesocket(MyTcpSocket* mysocket);

private:
    //实现单例模式
    MyTcpServer();
    MyTcpServer(const MyTcpServer&) = delete;
    MyTcpServer& operator= (MyTcpServer&) = delete;
    //客户端socket指针链表
    QList<MyTcpSocket*> m_TcpSocketList;

};

#endif // MYTCPSERVER_H

#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include"protocol.h"
#include"msghandle.h"
#include <QObject>
#include<QTcpSocket>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    ~MyTcpSocket();
    Msghandle* mh;
    PDU* ReadMsg();
    void SendMsg(PDU* pdu);
    void userOffLine();
    QString log_user_name;
    QByteArray buffer;
signals:
    void offline(MyTcpSocket *mysocket);
public slots:
    void recvMsg();

};

#endif // MYTCPSOCKET_H

#include "mytcpserver.h"
#include"mytcpsocket.h"
#include<QDebug>


MyTcpServer &MyTcpServer::getinstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug()<<"新客户端连接";
    MyTcpSocket *ptcpSocket = new MyTcpSocket;
    //设置socket描述符
    ptcpSocket->setSocketDescriptor(handle);
    //将新的客户端添加到链表中
    m_TcpSocketList.append(ptcpSocket);
    connect(ptcpSocket,&MyTcpSocket::offline,[=]{
        this->deletesocket(ptcpSocket);
    });

}

void MyTcpServer::Forward(QString Tarname, PDU *pdu)
{
    if(Tarname == NULL || !pdu){
        return;
    }
    for(int i = 0;i < m_TcpSocketList.size();i++){
        if(Tarname == m_TcpSocketList.at(i)->log_user_name){
            //向目标Socket发送好友申请
            m_TcpSocketList.at(i)->write((char*)pdu,pdu->uiPDUlen);
            break;
        }
    }
}

void MyTcpServer::deletesocket(MyTcpSocket *mysocket)
{
    m_TcpSocketList.removeOne(mysocket);
    mysocket->deleteLater();
    mysocket = NULL;

    for(int i=0;i<m_TcpSocketList.size();i++){
        qDebug()<<m_TcpSocketList.at(i)->log_user_name;
    }
}


MyTcpServer::MyTcpServer()
{

}

#include "mytcpsocket.h"
#include"operatedb.h"
#include"protocol.h"
#include<QDebug>

MyTcpSocket::MyTcpSocket()
{
    mh = new Msghandle;
    connect(this,&QTcpSocket::readyRead,[this]{
        this->recvMsg();
    });
    connect(this,&QTcpSocket::disconnected,[this]{
        this->userOffLine();
    });
    qDebug()<<"isupload"<<mh->m_isupload;
}

MyTcpSocket::~MyTcpSocket()
{
    delete mh;
}

PDU *MyTcpSocket::ReadMsg()
{
    uint PDUlen = 0;
    this->read((char*)&PDUlen,sizeof(uint));
    PDU* pdu = mkPDU(PDUlen);
    this->read((char*)pdu+sizeof(uint),PDUlen-sizeof(uint));
    return pdu;
}

void MyTcpSocket::SendMsg(PDU *pdu)
{
    if(pdu == NULL){
        return;
    }
    qDebug()<<"sendpdu_type:"<<pdu->uiMsgType
            <<"sendpdu_msglen:"<<pdu->uiMsgLen;
    this->write((char*)pdu,pdu->uiPDUlen);
    free(pdu);
    pdu = NULL;

}


void MyTcpSocket::recvMsg()
{
    qDebug()<<"recvMsg 接收的消息长度："<<this->bytesAvailable();
    QByteArray data = this->readAll();
    buffer.append(data);

    while(buffer.size() >= int(sizeof(PDU))){
        PDU* pdu = (PDU*)buffer.data();
        if(buffer.size() < int(pdu->uiPDUlen)){
            break;
        }
        PDU* pdun = mkPDU(pdu->uiMsgLen+1);
        memcpy(pdun,pdu,pdu->uiPDUlen);
        SendMsg(mh->handlepdu(pdun,log_user_name,this));
        qDebug()<<"recv pdu msg"<<pdun->caMsg
                <<"\n"<<pdu->caData;

        buffer.remove(0,pdun->uiPDUlen);
    }
}

void MyTcpSocket::userOffLine()
{
    OperateDB::getinstance().handleofflie(log_user_name.toStdString().c_str());
    emit offline(this);
}



#include "cilent.h"
#include "ui_cilent.h"
#include"protocol.h"
#include"index.h"
#include<QFile>
#include<QDebug>
#include<QMessageBox>
#include<QHostAddress>
#include<QString>


Cilent::Cilent(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Cilent)
{
    mh = new Msghandle;
    ui->setupUi(this);
    //配置文件
    loadConfig();
    //关联信号和槽函数
    connect(&m_tcpSocket,&QTcpSocket::connected,[this]{
        this->showConnect();
    });
    //连接接收消息的槽函数
    connect(&m_tcpSocket,&QTcpSocket::readyRead,[this]{
        this->recvMsg();
    });
    //链接服务器
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);

}

void Cilent::loadConfig(){
    //定义文件  固定以冒号开头
    QFile file(":/cilent.config");
    //文件以只读形式打开
     if(file.open(QIODevice::ReadOnly)){
         QByteArray baData = file.readAll();
         QString strData = QString(baData);
         qDebug()<<strData;
         QStringList strList = strData.split("\r\n");//按\r和\n形式拆分
         m_strIP = strList.at(0);
         m_usPort = strList.at(1).toUShort();
         qDebug()<<"ip:"<<m_strIP<<"port:"<<m_usPort;
         file.close();
     }
     else{
         qDebug()<<"打开配置失败";
     }
}

void Cilent::showConnect(){
    QMessageBox::information(this,"链接服务器","连接服务器成功");
}

PDU *Cilent::ReadMsg()
{
    uint uiPDULen = 0;
    m_tcpSocket.read((char*)&uiPDULen,sizeof(uint));
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU* pdu = mkPDU(uiMsgLen);
    m_tcpSocket.read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    return pdu;
}

void Cilent::SendMsg(PDU *pdu)
{
    qDebug()<<"sendpdu start."
            <<"\n pdu type"<<pdu->uiMsgType
            <<"\n pdu len"<<pdu->uiPDUlen
            <<"\n pdu msg"<<pdu->caMsg
            <<"\n pdu cadata"<<pdu->caData
            <<"\n pdu cadata+32"<<pdu->caData+32;
    m_tcpSocket.write((char*)pdu,pdu->uiPDUlen);

    free(pdu);
    pdu = NULL;
    qDebug()<<"\n sendpdu end";
}

void Cilent::recvMsg()
{
//    PDU* pdu = ReadMsg();
//    qDebug()<<"recvMsg的消息类型："<<pdu->uiMsgType
//            <<"参数："<<pdu->caData
//            <<"消息："<<pdu->caMsg;
//    mh->handlePdu(pdu);

//    free(pdu);
//    pdu = NULL;
    qDebug()<<"recvMsg 接收的消息长度："<<m_tcpSocket.bytesAvailable();
    QByteArray data = m_tcpSocket.readAll();
    buffer.append(data);

    while(buffer.size() >= int(sizeof(PDU))){
        PDU* pdu = (PDU*)buffer.data();
        if(buffer.size() < int(pdu->uiPDUlen)){
            break;
        }
        PDU* pdun = mkPDU(pdu->uiMsgLen);
        memcpy(pdun,pdu,pdu->uiPDUlen);
        mh->handlePdu(pdun);
        buffer.remove(0,pdun->uiPDUlen);
    }

}

QTcpSocket& Cilent::getTcpSocket(){
    return m_tcpSocket;
}

Cilent& Cilent::getInstance()
{
    //C++11 之后，静态变量是线程安全的
    static Cilent instance;
    return instance;
}

Cilent::~Cilent()
{
    delete mh;
    delete ui;
}

void Cilent::on_register_PB_clicked()
{
    //用一个PDU存储用户名和密码
    QString user = ui->name_LE ->text();
    QString pwd = ui->pwd_LE->text();
    //给出错误提示
    if(user.isEmpty()||pwd.isEmpty()
            ||user.size()>32||pwd.size()>32){
        QMessageBox::information(this,"注册","用户名或密码长度不合法");
        return;
    }
    //要存到有固定大小的数组中，不需要使用柔性数组，所以传参为0
    PDU *user_pwd_pdu = mkPDU(0);
    user_pwd_pdu->uiMsgType = Msg_PDU_Type_REGIST_REQUEST;

    memcpy(user_pwd_pdu->caData,user.toStdString().c_str(),32);
    memcpy(user_pwd_pdu->caData+32,pwd.toStdString().c_str(),32);

    SendMsg(user_pwd_pdu);
    user_pwd_pdu = NULL;
}

void Cilent::on_login_PB_clicked()
{
    QString user = ui->name_LE->text();
    QString pwd = ui->pwd_LE->text();
    if(user.isEmpty()||pwd.isEmpty()){
        QMessageBox::information(this,"登录","请输入正确的用户名或密码");
        return;
    }
    PDU *user_pwd_pdu = mkPDU(0);
    user_pwd_pdu->uiMsgType = Msg_PDU_Type_LOGIN_REQUEST;

    memcpy(user_pwd_pdu->caData,user.toStdString().c_str(),32);
    memcpy(user_pwd_pdu->caData+32,pwd.toStdString().c_str(),32);
    LoginUser_name = user;

    SendMsg(user_pwd_pdu);
    user_pwd_pdu = NULL;
}



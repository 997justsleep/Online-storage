#include "onlineuser.h"
#include "ui_onlineuser.h"
#include<QString>
#include<QDebug>


OnlineUser::OnlineUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUser)
{
    ui->setupUi(this);
}

OnlineUser::~OnlineUser()
{
    delete ui;
}

void OnlineUser::updateOnlineUserWigetList(QStringList userList)
{
    ui->onlineUser_LW->clear();
    ui->onlineUser_LW->addItems(userList);
}

void OnlineUser::on_onlineUser_LW_itemDoubleClicked(QListWidgetItem *item)
{
    QString CurName = Cilent::getInstance().LoginUser_name;
    QString TarName = item->text();

    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = Msg_PDU_Type_AddFriend_REQUEST;
    memcpy(pdu->caData,CurName.toStdString().c_str(),32);
    memcpy(pdu->caData + 32,TarName.toStdString().c_str(),32);
    qDebug()<<"添加好友， 当前用户："<<CurName
            <<"目标用户："<<TarName;
    Cilent::getInstance().SendMsg(pdu);
}

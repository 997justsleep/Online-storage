#include "friend.h"
#include "ui_friend.h"
#include"index.h"
#include"cilent.h"
#include"protocol.h"
#include"onlineuser.h"
#include<QInputDialog>
#include<QMessageBox>
#include<QDebug>


Friend::Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friend)
{
    ui->setupUi(this);
    m_onlineuser = new OnlineUser;
    RefreshFriend();
    m_chat = new Chat;
}

Friend::~Friend()
{
    delete ui;
    delete m_chat;
    delete m_onlineuser;
}

OnlineUser *Friend::getOnlineUser()
{
    return m_onlineuser;
}

Chat *Friend::getChat()
{
    return m_chat;
}

void Friend::RefreshFriend()
{
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = Msg_PDU_Type_ShowFriend_REQUEST;
    memcpy(pdu->caData,Cilent::getInstance().LoginUser_name.toStdString().c_str(),32);
    Cilent::getInstance().SendMsg(pdu);
    pdu = NULL;
}

void Friend::updateFriendWidget(QStringList Friend)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(Friend);
}

QListWidget *Friend::getOnlineFriend()
{
    return ui->listWidget;
}

void Friend::on_findUser_PB_clicked()
{
    QString name = QInputDialog::getText(this,"搜索","用户名");
    qDebug()<<name;
    if(name ==NULL)return;
    PDU* fpdu = mkPDU(0);
    fpdu->uiMsgType=Msg_PDU_Type_FindUser_REQUEST;
    memcpy(fpdu->caData,name.toStdString().c_str(),32);
    Cilent::getInstance().SendMsg(fpdu);
    fpdu=NULL;

}

void Friend::on_onlineUer_PB_clicked()
{
    PDU* opdu = mkPDU(0);
    opdu->uiMsgType = Msg_PDU_Type_OnlineUser_REQUEST;
    Cilent::getInstance().SendMsg(opdu);
    opdu = NULL;
    if(m_onlineuser->isHidden()){
        m_onlineuser->show();
    }
}

void Friend::on_flushFri_PB_clicked()
{
    RefreshFriend();
}

void Friend::on_delFri_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(!pItem){
        QMessageBox::information(this,"删除好友","请选择要删除的好友");
        return;
    }
    QString delName = pItem->text();
    int ret = QMessageBox::question(this,"删除好友",QString("是否确认删除好友 '%1'").arg(delName));
    if(ret != QMessageBox::Yes){
        return;
    }
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = Msg_PDU_Type_DelFriend_REQUEST;
    memcpy(pdu->caData,Cilent::getInstance().LoginUser_name.toStdString().c_str(),32);
    memcpy(pdu->caData+32,delName.toStdString().c_str(),32);
    Cilent::getInstance().SendMsg(pdu);
    pdu = NULL;
}

void Friend::on_talk_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(!pItem){
        return;
    }
    m_chat->ChatName = pItem->text();
    if(m_chat->isHidden()){
        m_chat->show();
        m_chat->setWindowTitle(m_chat->ChatName);
    }
}

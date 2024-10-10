#include "chat.h"
#include "ui_chat.h"
#include"cilent.h"

Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::updateChat_TE(QString msg)
{
    ui->show_TE->append(msg);
}

void Chat::on_send_PB_clicked()
{
    QString msg = ui->input_LE->text();
    PDU* pdu = mkPDU(msg.toStdString().size());
    ui->input_LE->clear();
    pdu->uiMsgType = Msg_PDU_Type_Chat_REQUEST;
    memcpy(pdu->caData,Cilent::getInstance().LoginUser_name.toStdString().c_str(),32);
    memcpy(pdu->caData+32,ChatName.toStdString().c_str(),32);
    memcpy(pdu->caMsg,msg.toStdString().c_str(),msg.toStdString().size());
    Cilent::getInstance().SendMsg(pdu);
    pdu = NULL;
}

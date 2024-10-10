#include "sharefile.h"
#include "ui_sharefile.h"
#include"index.h"

ShareFile::ShareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFile)
{
    ui->setupUi(this);
}

ShareFile::~ShareFile()
{
    delete ui;
}

void ShareFile::updateOnlineFriend_LW()
{
    ui->onlineFriend_LW->clear();
    QListWidget* friend_lw = Index::getinstance().getfriend()->getOnlineFriend();
    for (int i = 0;i < friend_lw->count();i++) {
        QListWidgetItem * friendItem = friend_lw->item(i);
        QListWidgetItem* newItem = new QListWidgetItem(*friendItem);
        ui->onlineFriend_LW->addItem(newItem);
    }
}

void ShareFile::on_Allselect_PB_clicked()
{
    for(int i=0; i<ui->onlineFriend_LW->count();i++){
        ui->onlineFriend_LW->item(i)->setSelected(true);
    }
}

void ShareFile::on_Cancelselect_PB_clicked()
{
    for(int i=0; i<ui->onlineFriend_LW->count();i++){
        ui->onlineFriend_LW->item(i)->setSelected(false);
    }
}

void ShareFile::on_OK_PB_clicked()
{
    QString loguser = Cilent::getInstance().LoginUser_name;
    QString curpath = Index::getinstance().getfile()->m_curPath;
    QString sharefile = Index::getinstance().getfile()->m_shareFileName;
    QString path = curpath + '/' + sharefile;

    QList<QListWidgetItem*>pItems = ui->onlineFriend_LW->selectedItems();
    int friendsize = pItems.size();
    PDU* pdu = mkPDU(friendsize * 32 + path.toStdString().size() + 1);
    pdu->uiMsgType = Msg_PDU_Type_ShareFile_REQUEST;
    memcpy(pdu->caData,loguser.toStdString().c_str(),32);
    memcpy(pdu->caData + 32,&friendsize,sizeof(int));
    for (int i = 0;i < friendsize;i++) {
        memcpy(pdu->caMsg + i*32,pItems.at(i)->text().toStdString().c_str(),32);
    }
    memcpy(pdu->caMsg + friendsize*32,path.toStdString().c_str(),path.toStdString().size());
    Cilent::getInstance().SendMsg(pdu);
}

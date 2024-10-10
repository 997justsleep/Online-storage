#include "file.h"
#include "ui_file.h"
#include"cilent.h"
#include<QFileDialog>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    m_curPath = "./filesys/"+QString("%1").arg(Cilent::getInstance().LoginUser_name);
    m_userPath = m_curPath;
    qDebug()<<"path:"<<m_curPath;
    m_sharefile = new ShareFile;
    FlushFile();
    ui->setupUi(this);
}

void File::FlushFile()
{
    PDU* pdu = mkPDU(m_curPath.toStdString().size()+1);
    pdu->uiMsgType = Msg_PDU_Type_FlushFile_REQUEST;
    memcpy(pdu->caMsg,m_curPath.toStdString().c_str(),m_curPath.toStdString().size());
    qDebug()<<"curpath:   "<<m_curPath;
    Cilent::getInstance().SendMsg(pdu);
}

void File::updateFileList(QList<FileInfo *> pFileInfoList)
{
    qDebug()<<"dpdateFileList Start!"<<"pfilelist_szie"<<pFileInfoList.size();
    m_pFileInfoList.clear();
    m_pFileInfoList = pFileInfoList;
    ui->file_LW->clear();

    foreach(FileInfo* pFileInfo,pFileInfoList){
       QListWidgetItem *pItem = new QListWidgetItem;
       if(pFileInfo->FileType == 0)
           pItem->setIcon(QIcon(QPixmap(":/dir.png")));
       else if(pFileInfo->FileType == 1)
           pItem->setIcon(QIcon(QPixmap(":/file.png")));
       pItem->setText(pFileInfo->FileName);
       qDebug()<<"update_file_list pFileInfo->FileName:"<<pFileInfo->FileName;
       ui->file_LW->addItem(pItem);
    }

}

void File::uploadFile()
{
    QFile file(m_uploadPath);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,"上传文件","打开文件失败");
        return;
    }
    m_isupload = true;
    PDU* datapdu = mkPDU(4096);
    datapdu->uiMsgType = Msg_PDU_Type_UploadFileData_REQUEST;
    qint64 ret = 0;
    while(true){
        ret = file.read(datapdu->caMsg,4096);
        if(ret == 0){
            break;
        }else if(ret < 0){
            QMessageBox::warning(this,"上传文件","读取文件失败");
            break;
        }
        datapdu->uiMsgLen = ret;
        datapdu->uiPDUlen = ret + sizeof(PDU);
        Cilent::getInstance().getTcpSocket().write((char*)datapdu,datapdu->uiPDUlen);
    }
    m_isupload = false;
    file.close();
    free(datapdu);
    datapdu = NULL;
}

void File::DownloadFile(qint64 fileSize)
{
    m_hasdownloadSize = 0;
    m_downloadTotalSize = fileSize;
    m_isdownload = true;
    m_downloadFile.setFileName(m_downloadFilePath);
    if(!m_downloadFile.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,"下载文件","打开文件失败");
    }
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = Msg_PDU_Type_DownloadFileData_REQUEST;
    Cilent::getInstance().SendMsg(pdu);
}

void File::DownloadFileData(char *buffer, qint64 size)
{
    m_downloadFile.write(buffer,size);
    m_hasdownloadSize += size;
    if(m_hasdownloadSize < m_downloadTotalSize){
        return;
    }
    m_downloadFile.close();
    m_isdownload = false;
    QMessageBox::information(this,"下载文件","下载完成");
}

File::~File()
{
    delete ui;
    delete m_sharefile;
}

void File::on_mKDir_PB_clicked()
{
    QString strNewParh = QInputDialog::getText(this,"新建文件夹","文件夹名称");
    qDebug()<<"newPath:"<<strNewParh;
    if(strNewParh.isEmpty()||strNewParh.size()>32){
        QMessageBox::information(this,"新建文件夹","文件夹名字长度不合法");
        return;
    }
    PDU* pdu = mkPDU(m_curPath.toStdString().size()+1);
    pdu->uiMsgType = Msg_PDU_Type_NewPath_REQUEST;
    memcpy(pdu->caData,strNewParh.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_curPath.toStdString().c_str(),m_curPath.toStdString().size());
    qDebug()<<"m_curpath"<<m_curPath;
    Cilent::getInstance().SendMsg(pdu);
    pdu = NULL;
    FlushFile();
}

void File::on_flushFile_PB_clicked()
{
    FlushFile();
}

void File::on_delDir_PB_clicked()
{
    QListWidgetItem* pItem = ui->file_LW->currentItem();
    if(!pItem){
        QMessageBox::information(this,"删除文件夹","请选择要删除的文件夹");
        return;
    }
    QString delDir = pItem->text();
    foreach(FileInfo* it,m_pFileInfoList){
        if(it->FileName ==delDir && it->FileType != 0){
            QMessageBox::information(this,"删除文件夹","选中的不是文件夹");
            return;
        }
    }
    int res = QMessageBox::question(this,"删除文件夹","确认删除"+delDir+"?");
    if(res != QMessageBox::Yes){
        return;
    }
    QString delpath = QString("%1/%2").arg(m_curPath).arg(delDir);
    qDebug()<<"deldir path:"<<delpath;
    PDU* pdu = mkPDU(delpath.toStdString().size()+1);
    pdu->uiMsgType = Msg_PDU_Type_DelDir_REQUEST;
    memcpy(pdu->caMsg,delpath.toStdString().c_str(),delpath.toStdString().size());
    qDebug()<<"deldir:: pdumsg:"<<pdu->caMsg;
    Cilent::getInstance().SendMsg(pdu);
}

void File::on_rename_PB_clicked()
{
    QListWidgetItem* pItem = ui->file_LW->currentItem();
    if(!pItem){
        QMessageBox::information(this,"重命名文件","请选择要重命名的文件");
        return;
    }
    QString oldName = pItem->text();
    QString newName = QInputDialog::getText(this,"重命名文件","文件名称");
    qDebug()<<"newName:"<<newName;
    if(newName.isEmpty()||newName.size()>32){
        QMessageBox::information(this,"重命名文件","文件名字长度不合法");
        return;
    }
    QString newPath = m_curPath;
    PDU* pdu = mkPDU(newPath.toStdString().size());
    pdu->uiMsgType = Msg_PDU_Type_RenameFile_REQUEST;
    memcpy(pdu->caData,oldName.toStdString().c_str(),32);
    memcpy(pdu->caData + 32,newName.toStdString().c_str(),32);
    memcpy(pdu->caMsg,newPath.toStdString().c_str(),newPath.toStdString().size());
    qDebug()<<"rename____:new path:"<<newPath;
    Cilent::getInstance().SendMsg(pdu);
}

void File::on_file_LW_itemDoubleClicked(QListWidgetItem *item)
{
    QString name = item->text();
    m_curPath = m_curPath + '/' + name;
    foreach(FileInfo* it,m_pFileInfoList){
        if(it->FileName ==name && it->FileType != 0){
            QMessageBox::information(this,"进入文件夹","选中的不是文件夹");
            return;
        }
    }
    FlushFile();
}



void File::on_delFile_PB_clicked()
{
    QListWidgetItem* pItem = ui->file_LW->currentItem();
    if(!pItem){
        QMessageBox::information(this,"删除文件","请选择要删除的文件");
        return;
    }
    QString delFile = pItem->text();
    foreach(FileInfo* it,m_pFileInfoList){
        if(it->FileName ==delFile && it->FileType != 1){
            QMessageBox::information(this,"删除文件","选中的不是文件");
            return;
        }
    }
    int res = QMessageBox::question(this,"删除文件","确认删除"+delFile+"?");
    if(res != QMessageBox::Yes){
        return;
    }
    QString delpath = QString("%1/%2").arg(m_curPath).arg(delFile);
    qDebug()<<"delfile path:"<<delpath;
    PDU* pdu = mkPDU(delpath.toStdString().size()+1);
    pdu->uiMsgType = Msg_PDU_Type_DelFile_REQUEST;
    memcpy(pdu->caMsg,delpath.toStdString().c_str(),delpath.toStdString().size());
    memcpy(pdu->caData,delFile.toStdString().c_str(),32);
    qDebug()<<"delfile:: pdumsg:"<<pdu->caMsg;
    Cilent::getInstance().SendMsg(pdu);
}



void File::on_return_PB_clicked()
{
    if(m_curPath == m_userPath){
        QMessageBox::warning(this,"返回上一级","已到顶层目录");
        return;
    }
    int index = m_curPath.lastIndexOf('/');
    m_curPath.remove(index,m_curPath.toStdString().size()-index);
    FlushFile();
}

void File::on_mvFile_PB_clicked()
{
    if(ui->mvFile_PB->text() == "移动文件"){
        QListWidgetItem* pItem = ui->file_LW->currentItem();
        if(pItem == NULL){
            QMessageBox::information(this,"移动文件","请选择要移动的文件");
            return;
        }
        m_mvFileName = pItem->text();
        m_mvFilePath = m_curPath + '/' + m_mvFileName;
        ui->mvFile_PB->setText("确认/取消");
        return;
    }
    QString tarPath;//目标路径
    QString boxMsg;//提示消息
    QListWidgetItem* pItem = ui->file_LW->currentItem();
    int ret;
    if(pItem == NULL){
        ret = QMessageBox::question(this,"移动文件","是否移动到当前路径下？");
    }else{
        boxMsg = pItem->text();
        boxMsg = "将文件移动到" + boxMsg + '?';
        ret = QMessageBox::question(this,"移动文件",boxMsg);
    }
    ui->mvFile_PB->setText("移动文件");
    if(ret == QMessageBox::No){
        return;
    }
    tarPath = m_curPath + '/' + m_mvFileName;
    int mvFilePathSize = m_mvFilePath.toStdString().size();
    int TarFilePathSize = tarPath.toStdString().size();
    PDU* pdu = mkPDU(mvFilePathSize+TarFilePathSize+1);
    pdu->uiMsgType = Msg_PDU_Type_MoveFile_REQUEST;
    memcpy(pdu->caData,&mvFilePathSize,sizeof(int));
    memcpy(pdu->caData + 32,&TarFilePathSize,sizeof(int));
    memcpy(pdu->caMsg,m_mvFilePath.toStdString().c_str(),mvFilePathSize);
    memcpy(pdu->caMsg + mvFilePathSize,tarPath.toStdString().c_str(),TarFilePathSize);
    Cilent::getInstance().SendMsg(pdu);
}

void File::on_upload_PB_clicked()
{
    if(m_isupload){
        QMessageBox::warning(this,"上传文件","已有正在上传的文件");
        return;
    }
    m_uploadPath.clear();
    m_uploadPath = QFileDialog::getOpenFileName();
    qDebug()<<"upload Path: "<<m_uploadPath;
    if(m_uploadPath.isEmpty()){
        return;
    }
    PDU* pdu = mkPDU(m_uploadPath.toStdString().size()+1);
    pdu->uiMsgType = Msg_PDU_Type_UploadFile_REQUEST;

    int index = m_uploadPath.lastIndexOf('/');
    QString fileName = m_uploadPath.right(m_uploadPath.size() - index - 1);
    //此处取文件名时，直接访问string的size即可不需要转tostdstring
    //含有中文的路径在取index时没有转tss，此处转tss会导致文件名取得不对
    qDebug()<<"filename:"<<fileName;
    QFile file(m_uploadPath);
    qint64 fileSize = file.size();
    memcpy(pdu->caData,fileName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,&fileSize,sizeof(qint64));
    memcpy(pdu->caMsg,m_curPath.toStdString().c_str(),m_curPath.toStdString().size());
    Cilent::getInstance().SendMsg(pdu);

}


void File::on_download_PB_clicked()
{
    if(m_isdownload){
        QMessageBox::warning(this,"下载文件","已有文件正在下载");
        return;
    }
    QListWidgetItem* pItem = ui->file_LW->currentItem();
    if(!pItem){
        QMessageBox::warning(this,"下载文件","请选择要下载的文件");
        return;
    }
    m_downloadFilePath.clear();
    m_downloadFilePath = QFileDialog::getSaveFileName();
    if(m_downloadFilePath.isEmpty()){
        QMessageBox::warning(this,"下载文件","请指定下载路径");
        return;
    }
    QString fileName = pItem->text();
    QString downloadPath = QString("%1/%2").arg(m_curPath).arg(fileName);
    PDU* pdu = mkPDU(downloadPath.toStdString().size()+1);
    pdu->uiMsgType = Msg_PDU_Type_DownloadFile_REQUEST;
    memcpy(pdu->caMsg,downloadPath.toStdString().c_str(),downloadPath.toStdString().size());
    qDebug()<<"downloadPath"<<downloadPath;
    Cilent::getInstance().SendMsg(pdu);
}

void File::on_shareFile_PB_clicked()
{
    QListWidgetItem* pItem = ui->file_LW->currentItem();
    if(!pItem){
        QMessageBox::warning(this,"分享文件","请选择要分享的文件");
        return;
    }
    m_sharefile->updateOnlineFriend_LW();
    m_shareFileName = pItem->text();
    if(m_sharefile->isHidden()){
        m_sharefile->show();
    }
}

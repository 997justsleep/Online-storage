#include "msghandle.h"
#include"operatedb.h"
#include"mytcpserver.h"
#include"server.h"
#include<QFileInfo>
#include<QDebug>
#include <QDir>
#include<QString>

Msghandle::Msghandle()
{

}

PDU *Msghandle::handlepdu(PDU *pdu, QString& name, MyTcpSocket *socket)
{
    PDU* retpdu = NULL;
    switch(pdu->uiMsgType){
    case Msg_PDU_Type_REGIST_REQUEST:{
        retpdu = rigster_user(pdu);
        break;
    }
    case Msg_PDU_Type_LOGIN_REQUEST:{
        retpdu = login_user(pdu,name);
        break;
    }
    case Msg_PDU_Type_FindUser_REQUEST:{
        retpdu = find_user(pdu);
        break;
    }
    case Msg_PDU_Type_OnlineUser_REQUEST:{
        retpdu = OnlineUser();
        break;
    }
    case Msg_PDU_Type_AddFriend_REQUEST:{
        retpdu = AddFriend(pdu);
        break;
    }
    case Msg_PDU_Type_AddFriend_AGREE:{
        retpdu = AddFriendAgree(pdu);
        break;
    }
    case Msg_PDU_Type_ShowFriend_REQUEST:{
        retpdu = ShowFriend(pdu);
        break;
    }
    case Msg_PDU_Type_DelFriend_REQUEST:{
        retpdu = DelFriend(pdu);
        break;
    }
    case Msg_PDU_Type_Chat_REQUEST:{
        Chat(pdu);
        break;
    }
    case Msg_PDU_Type_NewPath_REQUEST:{
        retpdu = CreatePath(pdu);
        break;
    }
    case Msg_PDU_Type_FlushFile_REQUEST:{
        retpdu = FlushFile(pdu);
        break;
    }
    case Msg_PDU_Type_DelDir_REQUEST:{
        retpdu = DelDir(pdu);
        break;
    }
    case Msg_PDU_Type_DelFile_REQUEST:{
        retpdu = DelFile(pdu);
        break;
    }
    case Msg_PDU_Type_RenameFile_REQUEST:{
        retpdu = RenameFile(pdu);
        break;
    }
    case Msg_PDU_Type_MoveFile_REQUEST:{
        retpdu = MoveFile(pdu);
        break;
    }
    case Msg_PDU_Type_UploadFile_REQUEST:{
        retpdu = UploadFile(pdu);
        break;
    }
    case Msg_PDU_Type_UploadFileData_REQUEST:{
        retpdu = UploadFileData(pdu);
        break;
    }
    case Msg_PDU_Type_DownloadFile_REQUEST:{
        retpdu = DownloadFile(pdu);
        break;
    }
    case Msg_PDU_Type_DownloadFileData_REQUEST:{
        retpdu = DownloadFileData(socket);
        break;
    }
    case Msg_PDU_Type_ShareFile_REQUEST:{
        retpdu = ShareFile(pdu);
        break;
    }
    case Msg_PDU_Type_ShareFileAgree_REQUEST:{
        retpdu = ShareFileAgree(pdu);
        break;
    }
    default:
        break;
    }
    return retpdu;
}

PDU* Msghandle::rigster_user(PDU*pdu)
{
    char caName[32]={"\0"};
    char capwd[32]={"\0"};
    memcpy(caName,pdu->caData,32);
    memcpy(capwd,pdu->caData+32,32);
    qDebug()<<"caName:"<<caName<<"capwd:"<<capwd;
    bool ret = OperateDB::getinstance().handleRigist(caName,capwd);
    qDebug()<<"registret_1:"<<ret;
    if(ret){
        QDir d;
        d.mkdir(QString("%1/%2").arg(Server::getinstance().getRootPath()).arg(caName));
    }
    PDU *retpdu = mkPDU(0);
    retpdu->uiMsgType=Msg_PDU_Type_REGIST_RESPOND;
    memcpy(retpdu->caData,&ret,sizeof(bool));
    return retpdu;
}

PDU* Msghandle::login_user(PDU *pdu,QString& name)
{
    char caName[32]={"\0"};
    char capwd[32]={"\0"};
    memcpy(caName,pdu->caData,32);
    memcpy(capwd,pdu->caData+32,32);
    qDebug()<<"caName:"<<caName<<"capwd:"<<capwd;
    bool ret = OperateDB::getinstance().handlelogin(caName,capwd);
    qDebug()<<"loginret_1:"<<ret;
    name = caName;
    PDU* retpdu = mkPDU(0);
    retpdu->uiMsgType = Msg_PDU_Type_LOGIN_RESPOND;
    memcpy(retpdu->caData,&ret,sizeof(bool));
    return retpdu;


}

PDU* Msghandle::find_user(PDU *pdu)
{
    char name[32]={"\0"};
    memcpy(name,pdu->caData,32);
    qDebug()<<"name:"<<name;
    int status = OperateDB::getinstance().handlefind(name);
    PDU* fpdu = mkPDU(0);
    fpdu->uiMsgType=Msg_PDU_Type_FindUser_RESPOND;

    memcpy(fpdu->caData,&status,sizeof(int));
    memcpy(fpdu->caData+32,name,32);
    return fpdu;
}

PDU* Msghandle::OnlineUser()
{
    QStringList a =OperateDB::getinstance().handleOnline();
    uint uimsglen = a.size()*32;
    PDU* opdu = mkPDU(uimsglen);
    for(int i=0;i<a.size();i++){
        memcpy(opdu->caMsg + i*32,a.at(i).toStdString().c_str(),32);
    }
    opdu->uiMsgType = Msg_PDU_Type_OnlineUser_RESPOND;
    return opdu;
}

PDU *Msghandle::AddFriend(PDU *pdu)
{
    char CurName[32],TarName[32];
    memcpy(CurName,pdu->caData,32);
    memcpy(TarName,pdu->caData+32,32);
    qDebug()<<"\n\n hadle curname:"<<CurName<<"handle tarname:"<<TarName;
    int ret = OperateDB::getinstance().handleAdd(CurName,TarName);
    qDebug()<<"ret: "<<ret;
    //如果满足要求，则调用server向目标socket发送请求
    if(ret == 1){
        MyTcpServer::getinstance().Forward(TarName,pdu);
    }
    PDU *apdu = mkPDU(0);
    apdu->uiMsgType =Msg_PDU_Type_AddFriend_RESPOND;
    memcpy(apdu->caData,&ret,sizeof(int));
    return apdu;
}

PDU *Msghandle::AddFriendAgree(PDU *pdu)
{
    char CurName[32],TarName[32];
    memcpy(CurName,pdu->caData,32);
    memcpy(TarName,pdu->caData+32,32);
    qDebug()<<"\n\n agree curname:"<<CurName<<"agree tarname:"<<TarName;
    OperateDB::getinstance().handleAddAgree(CurName,TarName);
    PDU *apdu = mkPDU(0);
    apdu->uiMsgType =Msg_PDU_Type_AddFriend_AGREE;
    MyTcpServer::getinstance().Forward(CurName,apdu);
    return apdu;
}

PDU *Msghandle::ShowFriend(PDU *pdu)
{
    char name[32];
    memcpy(name,pdu->caData,32);
    QStringList Friend = OperateDB::getinstance().Showfriend(name);
    int len = Friend.size();
    PDU* retpdu = mkPDU(32*len);
    for(int i=0;i<Friend.size();i++){
       memcpy(retpdu->caMsg+i*32,Friend.at(i).toStdString().c_str(),32);
    }
    retpdu->uiMsgType = Msg_PDU_Type_ShowFriend_RESPOND;
    return retpdu;
}

PDU *Msghandle::DelFriend(PDU *pdu)
{
    char CurName[32],DelName[32];
    memcpy(CurName,pdu->caData,32);
    memcpy(DelName,pdu->caData+32,32);
    bool ret = OperateDB::getinstance().DelFriend(CurName,DelName);
    PDU* retpdu = mkPDU(0);
    retpdu->uiMsgType = Msg_PDU_Type_DelFriend_RESPOND;
    memcpy(retpdu->caData,&ret,sizeof(bool));
    return retpdu;
}

void Msghandle::Chat(PDU *pdu)
{
    char TarName[32];
    memcpy(TarName,pdu->caData+32,32);
    qDebug()<<"talk to:"<<TarName;
    MyTcpServer::getinstance().Forward(QString(TarName),pdu);
}

PDU *Msghandle::CreatePath(PDU *pdu)
{
    char NewPath[32];
    memcpy(NewPath,pdu->caData,32);
    QString CurPath = pdu->caMsg;
    QDir d;
    QString path = CurPath+"/"+NewPath;
    bool res =1;
    qDebug()<<"create path:"<<path;
    //当前路径不存在      新路径已存在    创建文件夹失败
    qDebug()<<"exists path: "<<d.exists(path);
    if(!d.exists(CurPath) || d.exists(path) || !d.mkdir(path)){
        res = 0;
    }
    PDU* retpdu = mkPDU(0);
    retpdu->uiMsgType = Msg_PDU_Type_NewPath_RESPOND;
    memcpy(retpdu->caData,&res,sizeof(bool));
    qDebug()<<"create res: "<<res;
    return retpdu;
}

PDU *Msghandle::FlushFile(PDU *pdu)
{
    char* CurPath = pdu->caMsg;
    QDir dir(CurPath);
    QFileInfoList fileInfolist = dir.entryInfoList();
    int Filecount = fileInfolist.size();
    qDebug()<<"flush filecount:"<<Filecount;
    PDU* retpdu = mkPDU(sizeof(FileInfo)*(Filecount - 2));//不需要显示.和..
    retpdu->uiMsgType = Msg_PDU_Type_FlushFile_RESPOND;

    FileInfo* pFileInfo = NULL;
    QString strFileName;

    for(int i = 0,j=0; i < Filecount; i++){
        strFileName = fileInfolist[i].fileName();
        if(strFileName == QString(".")||strFileName == QString(".."))
            continue;
        pFileInfo = (FileInfo*)retpdu->caMsg + j++;
        memcpy(pFileInfo->FileName,strFileName.toStdString().c_str(),32);
        if(fileInfolist[i].isDir()){
            pFileInfo->FileType = 0;
        }else if(fileInfolist[i].isFile()){
            pFileInfo->FileType = 1;
        }
        qDebug()<<"flush_file fileName:"<<strFileName;
    }
    qDebug()<<"flush file end";
    return retpdu;
}

PDU *Msghandle::DelDir(PDU *pdu)
{
    char* path = pdu->caMsg;
    qDebug()<<"del dir path: "<<path;
    QFileInfo fileInfo(path);
    bool res = 0;
    if(fileInfo.isDir()){
        QDir dir(path);
        res = dir.removeRecursively();
    }
    PDU* retpdu = mkPDU(0);
    memcpy(retpdu->caData,&res,sizeof(bool));
    qDebug()<<"del dir res:"<<res;
    retpdu->uiMsgType = Msg_PDU_Type_DelDir_RESPOND;
    return retpdu;
}

PDU *Msghandle::DelFile(PDU *pdu)
{
    char* path = pdu->caMsg;
    qDebug()<<"del file path: "<<path;
    QFileInfo fileInfo(path);
    QString name = pdu->caData;
    bool res = 0;
    if(fileInfo.isFile()){
        QFile f(path);
        res = f.remove(path);
    }
    PDU* retpdu = mkPDU(0);
    memcpy(retpdu->caData,&res,sizeof(bool));
    qDebug()<<"del file res:"<<res;
    retpdu->uiMsgType = Msg_PDU_Type_DelFile_RESPOND;
    return retpdu;
}

PDU *Msghandle::RenameFile(PDU *pdu)
{
    QString path = pdu->caMsg;
    char oldName[32],newName[32];
    memcpy(oldName,pdu->caData,32);
    memcpy(newName,pdu->caData + 32,32);
    QString oldPath = path + '/' + QString(oldName);
    QString newPath = path + '/' + QString(newName);
    qDebug()<<"oldpath:  "<<oldPath
            <<"newpath:  "<<newPath;
    QDir d;
    bool res = d.rename(oldPath,newPath);
    qDebug()<<"rename  res"<<res;
    PDU* retpdu = mkPDU(0);
    retpdu->uiMsgType = Msg_PDU_Type_RenameFile_RESPOND;
    memcpy(retpdu->caData,&res,sizeof(bool));
    return retpdu;
}

PDU *Msghandle::MoveFile(PDU *pdu)
{
    int mvlen,tarlen;
    memcpy(&mvlen,pdu->caData,sizeof(int));
    memcpy(&tarlen,pdu->caData + 32,sizeof(int));
    char* mvpath = new char[mvlen + 1];
    char* tarpath = new char[tarlen + 1];

    memset(mvpath,'\0',sizeof(mvlen+1));
    memset(tarpath,'\0',sizeof(tarlen+1));

    memcpy(mvpath,pdu->caMsg,mvlen);
    memcpy(tarpath,pdu->caMsg + mvlen,tarlen);

    bool ret = QFile::rename(mvpath,tarpath);
    PDU* retpdu = mkPDU(0);
    retpdu->uiMsgType = Msg_PDU_Type_MoveFile_RESPOND;
    memcpy(retpdu->caData,&ret,sizeof(bool));

    delete[] mvpath;
    delete[] tarpath;
    mvpath = NULL;
    tarpath = NULL;
    return retpdu;
}

PDU *Msghandle::UploadFile(PDU *pdu)
{
    PDU* retpdu = mkPDU(0);
    retpdu->uiMsgType = Msg_PDU_Type_UploadFile_RESPOND;
    int ret = 0;
    if(m_isupload){
        qDebug()<<"upload is ture";
        ret = 1;
        memcpy(retpdu->caData,&ret,sizeof(int));
        return retpdu;
    }
    char FileName[32] = {'\0'};
    qint64 FileSize = 0;
    memcpy(FileName,pdu->caData,32);
    memcpy(&FileSize,pdu->caData+32,sizeof(qint64));
    QString FilePath = QString("%1/%2").arg(pdu->caMsg).arg(FileName);
    m_uploadFile.setFileName(FilePath);
    if(m_uploadFile.open(QIODevice::WriteOnly)){
        m_isupload = true;
        m_fileTotalSize = FileSize;
        m_uploadfileRecSize = 0;
    }else{
        qDebug()<<"file open failed";
        ret = -1;
    }
    memcpy(retpdu->caData,&ret,sizeof(int));
    qDebug()<<"ret"<<ret
            <<"m_isupload"<<m_isupload
            <<"m_fileTotalSize"<<m_fileTotalSize
            <<"m_upfileRecSize"<<m_uploadfileRecSize;
    return retpdu;
}

PDU *Msghandle::UploadFileData(PDU *pdu)
{
    m_uploadFile.write(pdu->caMsg,pdu->uiMsgLen);
    m_uploadfileRecSize += pdu->uiMsgLen;
    if(m_uploadfileRecSize < m_fileTotalSize){
        return NULL;
    }
    bool ret = m_uploadfileRecSize == m_fileTotalSize;
    PDU* retpdu = mkPDU(0);
    memcpy(retpdu->caData,&ret,sizeof(bool));
    retpdu->uiMsgType = Msg_PDU_Type_UploadFileData_RESPOND;
    m_uploadFile.close();
    m_isupload = false;
    return retpdu;
}

PDU *Msghandle::DownloadFile(PDU *pdu)
{
    PDU* retpdu = mkPDU(0);
    retpdu->uiMsgType = Msg_PDU_Type_DownloadFile_RESPOND;
    int ret = 0;
    if(m_isdownload){
        qDebug()<<"download is ture";
        ret = 1;
        memcpy(retpdu->caData,&ret,sizeof(int));
        return retpdu;
    }
    QFileInfo fileinfo(pdu->caMsg);
    qint64 filesize = fileinfo.size();
    m_downloadFile.setFileName(pdu->caMsg);
    if(m_downloadFile.open(QIODevice::ReadOnly)){
        m_isdownload = true;
    }else{
        ret = -1;
    }
    memcpy(retpdu->caData,&ret,sizeof(int));
    memcpy(retpdu->caData + 32,&filesize,sizeof (qint64));
    return retpdu;

}

PDU *Msghandle::DownloadFileData(MyTcpSocket *socket)
{

    PDU* datapdu = mkPDU(4096);
    datapdu->uiMsgType = Msg_PDU_Type_DownloadFileData_RESPOND;
    qint64 ret = 0;
    while(true){
        ret = m_downloadFile.read(datapdu->caMsg,4096);
        if(ret <= 0){
            break;
        }
        datapdu->uiMsgLen = ret;
        datapdu->uiPDUlen = ret + sizeof(PDU);
        socket->write((char*)datapdu,datapdu->uiPDUlen);
    }
    m_isdownload = false;
    m_downloadFile.close();
    free(datapdu);
    datapdu = NULL;
    return NULL;
}

PDU *Msghandle::ShareFile(PDU *pdu)
{
    char name[32] = {'\0'};
    int friendsize = 0;
    memcpy(name,pdu->caData,32);
    memcpy(&friendsize,pdu->caData + 32,sizeof(int));

    int size = friendsize * 32;

    PDU* forwardpdu = mkPDU(pdu->uiMsgLen - size);
    forwardpdu->uiMsgType = pdu->uiMsgType;
    memcpy(forwardpdu->caData,name,32);
    memcpy(forwardpdu->caMsg,pdu->caMsg + size,pdu->uiMsgLen - size);

    char friendName[32] = {'\0'};
    for (int i = 0;i < friendsize;i++) {
        memcpy(friendName,pdu->caMsg + i * 32,32);
        MyTcpServer::getinstance().Forward(friendName,forwardpdu);
    }
    free(forwardpdu);
    forwardpdu = NULL;

    PDU* retpdu = mkPDU(0);
    retpdu->uiMsgType = Msg_PDU_Type_ShareFile_RESPOND;
    return retpdu;
}

PDU *Msghandle::ShareFileAgree(PDU *pdu)
{
    QString RecvPath = QString("%1/%2").arg(Server::getinstance().getRootPath()).arg(pdu->caData);
    QString SharePath = pdu->caMsg;

    int index = SharePath.lastIndexOf('/');
    QString FileName = SharePath.right(SharePath.size() - index - 1);
    RecvPath = RecvPath + '/' + FileName;
    qDebug()<<"recv_path"<<RecvPath
            <<"share_path"<<SharePath;
    QFileInfo fileinfo(SharePath);

    bool ret = true;
    if(fileinfo.isFile()){
        ret = QFile::copy(SharePath,RecvPath);
    }else if(fileinfo.isDir()){
        ret = CopyDir(SharePath,RecvPath);
    }
    qDebug()<<"ret = "<<ret;
    PDU* retpdu = mkPDU(0);
    retpdu->uiMsgType = Msg_PDU_Type_ShareFileAgree_RESPOND;
    memcpy(retpdu->caData,&ret,sizeof(bool));
    return retpdu;
}

bool Msghandle::CopyDir(QString ShareDir, QString RecvDir)
{
    QDir dir;
    dir.mkdir(RecvDir);
    dir.setPath(ShareDir);
    QFileInfoList fileInfoList = dir.entryInfoList();
    bool ret = true;
    QString shareTmp;
    QString recvTmp;
    for(int i=0; i<fileInfoList.size();i++){
        if(fileInfoList[i].isFile()){
          shareTmp = ShareDir + '/' + fileInfoList[i].fileName();
          recvTmp = RecvDir + '/' + fileInfoList[i].fileName();
          if(!QFile::copy(shareTmp,recvTmp)){
              ret = false;
            }
        }else if(fileInfoList[i].isDir()){
            if(fileInfoList[i].fileName() == QString(".") ||
                    fileInfoList[i].fileName() == QString("..")){
                continue;
            }
            shareTmp = ShareDir + '/' + fileInfoList[i].fileName();
            recvTmp = RecvDir + '/' + fileInfoList[i].fileName();
            if(!CopyDir(shareTmp,recvTmp)){
                ret = false;
            }
        }
    }
    return ret;
}

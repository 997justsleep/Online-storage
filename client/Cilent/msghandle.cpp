#include "msghandle.h"
#include"index.h"
#include"cilent.h"
#include"friend.h"
#include"chat.h"
#include<QString>
#include<QMessageBox>


Msghandle::Msghandle()
{

}

void Msghandle::rigster_user(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    if(res){
        QMessageBox::information(&Cilent::getInstance(),"注册","注册成功");
    }else{
        QMessageBox::information(&Cilent::getInstance(),"注册","注册失败");
    }
    return;
}

void Msghandle::login_user(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    if(res){
        Index::getinstance().show();
        Cilent::getInstance().hide();

    }else{
        QMessageBox::information(&Cilent::getInstance(),"登录","请输入正确的用户名和密码");
    }
}

void Msghandle::find_user(PDU *pdu)
{
    int status;
    char TarName[32];
    memcpy(TarName,pdu->caData+32,32);
    qDebug()<<"\n\ntarname"<<TarName;
    memcpy(&status,pdu->caData,sizeof(int));
    if(status == -1){
        QMessageBox::information(&Cilent::getInstance(),"查找",QString("'%1'不存在").arg(TarName));
    }else if(!status){
        QMessageBox::information(&Cilent::getInstance(),"查找",QString("'%1'离线").arg(TarName));
    }else{
        int ret = QMessageBox::information(&Cilent::getInstance(),"查找","用户在线","添加好友","取消");
        if(ret == 0){
            PDU* apdu = mkPDU(0);
            apdu->uiMsgType = Msg_PDU_Type_AddFriend_REQUEST;
            memcpy(apdu->caData,Cilent::getInstance().LoginUser_name.toStdString().c_str(),32);
            memcpy(apdu->caData+32,TarName,32);
            Cilent::getInstance().SendMsg(apdu);
            apdu = NULL;
        }
    }
}

void Msghandle::OnlineUser(PDU* pdu)
{
    uint uiSize = pdu->uiMsgLen / 32;
    char name[32];
    QStringList userlist;
    userlist.clear();
    for(uint i = 0; i<uiSize; i++){
        memcpy(name,pdu->caMsg + i*32 ,32);
        if(QString(name) == Cilent::getInstance().LoginUser_name){
            continue;
        }
        userlist.append(QString(name));
    }
    Index::getinstance().getfriend()->getOnlineUser()->updateOnlineUserWigetList(userlist);
}

void Msghandle::AddFriend(PDU *pdu)
{
    int ret;
    QString ans;
    memcpy(&ret,pdu->caData,sizeof(int));
    qDebug()<<"ret: "<<ret;
    if(ret == -2){
        ans = QString("服务器错误");
    }else if(ret == -1){
        ans = QString("对方已经是你的好友");
    }else if(ret == 0){
        ans = QString("对方离线");
    }else
        ans = QString("发送成功");
    QMessageBox::information(Index::getinstance().getfriend(),"添加好友",ans);
}

void Msghandle::AddFriendRequest(PDU *pdu)
{
    char reqName[32];
    memcpy(reqName,pdu->caData,32);
    int ret =
            QMessageBox::question(Index::getinstance().getfriend(),"好友申请",QString("是否同意'%1'添加您为好友").arg(reqName));
    PDU* rpdu =mkPDU(0);
    memcpy(rpdu->caData,reqName,32);
    memcpy(rpdu->caData+32,Cilent::getInstance().LoginUser_name.toStdString().c_str(),32);
    if(ret != QMessageBox::Yes){
        rpdu->uiMsgType = Msg_PDU_Type_AddFriend_REFUSE;
    }else{
        rpdu->uiMsgType = Msg_PDU_Type_AddFriend_AGREE;
    }
    Cilent::getInstance().SendMsg(rpdu);
    rpdu = NULL;
}

void Msghandle::AddFriendAgree()
{
    QMessageBox::information(Index::getinstance().getfriend(),"添加好友","添加成功");
    Index::getinstance().getfriend()->RefreshFriend();

}

void Msghandle::ShowFriend(PDU *pdu)
{
    QStringList ret;
    char name[32];
    for(uint i=0;i<pdu->uiMsgLen / 32;i++){
        memcpy(name,pdu->caMsg+ i*32,32);
        ret.append(QString(name));
    }
    Index::getinstance().getfriend()->updateFriendWidget(ret);
}

void Msghandle::DelFriend(PDU *pdu)
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(!ret){
        QMessageBox::information(&Index::getinstance(),"删除好友","删除失败");
    }else {
        QMessageBox::information(&Index::getinstance(),"删除好友","删除成功");
    }
    Index::getinstance().getfriend()->RefreshFriend();
}

void Msghandle::Chat_(PDU *pdu){
    Chat* ch = NULL;
    ch = Index::getinstance().getfriend()->getChat();
    char Name[32];
    memcpy(Name,pdu->caData,32);
    Index::getinstance().getfriend()->getChat()->ChatName = QString(Name);
    if(ch->isHidden()){
        ch->setWindowTitle(QString(Name));
        ch->show();
    }
    Index::getinstance().getfriend()->getChat()->updateChat_TE(QString("%1: %2").arg(Name).arg(pdu->caMsg));

}

void Msghandle::CreateNewPath(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    qDebug()<<"create res:"<<res;
    if(!res){
        QMessageBox::information(&Cilent::getInstance(),"创建文件夹","创建失败");
    }
}

void Msghandle::FlushFile(PDU *pdu)
{
    int icount = pdu->uiMsgLen / sizeof(FileInfo);
    qDebug()<<"icount:"<<icount;
    QList<FileInfo*> pFileList;
    for(int i = 0;i < icount;i++){
        FileInfo* pFileInfo = new FileInfo;
        memcpy(pFileInfo,pdu->caMsg + i*sizeof(FileInfo),sizeof(FileInfo));
        pFileList.append(pFileInfo);
        qDebug()<<"第"<<i<<"次循环"<<"List_size"<<pFileList.size();
    }

    Index::getinstance().getfile()->updateFileList(pFileList);

}

void Msghandle::DelDir(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    qDebug()<<"del dir res: "<<res;
    if(res){
        QMessageBox::information(Index::getinstance().getfile(),"删除文件夹","删除成功");
    }else{
        QMessageBox::information(Index::getinstance().getfile(),"删除文件夹","删除失败");
    }
    Index::getinstance().getfile()->FlushFile();

}

void Msghandle::DelFile(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    qDebug()<<"del file res: "<<res;
    if(res){
        QMessageBox::information(Index::getinstance().getfile(),"删除文件","删除成功");
    }else{
        QMessageBox::information(Index::getinstance().getfile(),"删除文件","删除失败");
    }
    Index::getinstance().getfile()->FlushFile();
}

void Msghandle::RenameFile(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    qDebug()<<"rename res: "<<res;
    if(!res){
       QMessageBox::information(Index::getinstance().getfile(),"重命名文件","重命名失败");
    }
    Index::getinstance().getfile()->FlushFile();
}

void Msghandle::MoveFile(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    if(!res){
        QMessageBox::information(Index::getinstance().getfile(),"移动文件","移动失败");
    }
    Index::getinstance().getfile()->FlushFile();
}

void Msghandle::UploadFile(PDU *pdu)
{
    int res;
    memcpy(&res,pdu->caData,sizeof(int));
    if(res == 0){
        Index::getinstance().getfile()->uploadFile();
    }else{
        QMessageBox::information(Index::getinstance().getfile(),"上传文件","上传文件失败");
    }
}

void Msghandle::UploadFileData(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    if(!res){
        QMessageBox::information(Index::getinstance().getfile(),"上传文件","上传失败");
    }else{
        QMessageBox::information(Index::getinstance().getfile(),"上传文件","上传成功");
    }
    Index::getinstance().getfile()->FlushFile();
}

void Msghandle::DownloadFile(PDU *pdu)
{
    int ret;
    memcpy(&ret,pdu->caData,sizeof(int));
    if(ret == 1){
        QMessageBox::warning(Index::getinstance().getfile(),"下载文件","已有文件正在下载");
        return;

    }
    if(ret == -1){
        QMessageBox::warning(Index::getinstance().getfile(),"下载文件","下载文件失败");
        return;
    }
    qint64 filesize;
    memcpy(&filesize,pdu->caData+32,sizeof(qint64));
    Index::getinstance().getfile()->DownloadFile(filesize);

}

void Msghandle::DownloadFileData(PDU *pdu)
{
    char buffer[4096];
    memcpy(buffer,pdu->caMsg,pdu->uiMsgLen);
    qint64 Size;
    memcpy(&Size,&pdu->uiMsgLen,sizeof(qint64));
    Index::getinstance().getfile()->DownloadFileData(buffer,Size);
}

void Msghandle::ShareFile()
{
    QMessageBox::warning(Index::getinstance().getfile(),"分享文件","已分享文件给好友");
}

void Msghandle::ShareFileRequest(PDU *pdu)
{
    QString FilePath = QString(pdu->caMsg);
    int index = FilePath.lastIndexOf('/');
    QString FileName = FilePath.right(FilePath.size() - index - 1);
    int ret = QMessageBox::question(&Index::getinstance(),"分享文件",
                                    QString("是否接收%1分享的文件%2").arg(pdu->caData).arg(FileName));
    if(ret != QMessageBox::Yes){
        return;
    }
    PDU* retpdu = mkPDU(pdu->uiMsgLen);
    retpdu->uiMsgType = Msg_PDU_Type_ShareFileAgree_REQUEST;
    QString Name = Cilent::getInstance().LoginUser_name;
    memcpy(retpdu->caMsg,pdu->caMsg,pdu->uiMsgLen);
    memcpy(retpdu->caData,Name.toStdString().c_str(),32);
    Cilent::getInstance().SendMsg(retpdu);
}

void Msghandle::ShareFileAgree(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    if(!res){
        QMessageBox::information(Index::getinstance().getfile(),"分享文件","分享失败");
    }else{
        QMessageBox::information(Index::getinstance().getfile(),"分享文件","分享成功");
    }
    Index::getinstance().getfile()->FlushFile();
}

void Msghandle::handlePdu(PDU *pdu)
{
    switch(pdu->uiMsgType){
    case Msg_PDU_Type_REGIST_RESPOND :{
        rigster_user(pdu);
        break;
    }
    case Msg_PDU_Type_LOGIN_RESPOND:{
        login_user(pdu);
        break;
    }
    case Msg_PDU_Type_FindUser_RESPOND:{
        find_user(pdu);
        break;
    }
    case Msg_PDU_Type_OnlineUser_RESPOND:{
        OnlineUser(pdu);
        break;
    }
    case Msg_PDU_Type_AddFriend_RESPOND:{
        AddFriend(pdu);
        break;
    }
    case Msg_PDU_Type_AddFriend_REQUEST:{
        AddFriendRequest(pdu);
        break;
    }
    case Msg_PDU_Type_AddFriend_AGREE:{
        AddFriendAgree();
        break;
    }
    case Msg_PDU_Type_ShowFriend_RESPOND:{
        ShowFriend(pdu);
        break;
    }
    case Msg_PDU_Type_DelFriend_RESPOND:{
        DelFriend(pdu);
        break;
    }
    case Msg_PDU_Type_Chat_REQUEST:{
        Chat_(pdu);
        break;
    }
    case Msg_PDU_Type_NewPath_RESPOND:{
        CreateNewPath(pdu);
        break;
    }
    case Msg_PDU_Type_FlushFile_RESPOND:{
        FlushFile(pdu);
        break;
    }
    case Msg_PDU_Type_DelDir_RESPOND:{
        DelDir(pdu);
        break;
    }
    case Msg_PDU_Type_DelFile_RESPOND:{
        DelFile(pdu);
        break;
    }
    case Msg_PDU_Type_RenameFile_RESPOND:{
        RenameFile(pdu);
        break;
    }
    case Msg_PDU_Type_MoveFile_RESPOND:{
        MoveFile(pdu);
        break;
    }
    case Msg_PDU_Type_UploadFile_RESPOND:{
        UploadFile(pdu);
        break;
    }
    case Msg_PDU_Type_UploadFileData_RESPOND:{
        UploadFileData(pdu);
        break;
    }
    case Msg_PDU_Type_DownloadFile_RESPOND:{
        DownloadFile(pdu);
        break;
    }
    case Msg_PDU_Type_DownloadFileData_RESPOND:{
        DownloadFileData(pdu);
        break;
    }
    case Msg_PDU_Type_ShareFile_RESPOND:{
        ShareFile();
        break;
    }
    case Msg_PDU_Type_ShareFile_REQUEST:{
        ShareFileRequest(pdu);
        break;
    }
    case Msg_PDU_Type_ShareFileAgree_RESPOND:{
        ShareFileAgree(pdu);
        break;
    }
    default:
        break;
    }
}

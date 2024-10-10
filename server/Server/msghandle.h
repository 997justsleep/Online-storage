#ifndef MSGHANDLE_H
#define MSGHANDLE_H

#include"protocol.h"
#include <QFile>
#include<QString>

class MyTcpSocket;

class Msghandle
{
public:
    Msghandle();
    PDU* handlepdu(PDU* pdu,QString& name,MyTcpSocket* socket);
    PDU* rigster_user(PDU*pdu);
    PDU* login_user(PDU*pdu,QString& name);
    PDU* find_user(PDU*pdu);
    PDU* OnlineUser();
    PDU* AddFriend(PDU* pdu);
    PDU* AddFriendAgree(PDU* pdu);
    PDU* ShowFriend(PDU*pdu);
    PDU* DelFriend(PDU* pdu);
    void Chat(PDU* pdu);
    PDU* CreatePath(PDU* pdu);
    PDU* FlushFile(PDU* pdu);
    PDU* DelDir(PDU* pdu);
    PDU* DelFile(PDU* pdu);
    PDU* RenameFile(PDU* pdu);
    PDU* MoveFile(PDU* pdu);
    PDU* UploadFile(PDU* pdu);
    PDU* UploadFileData(PDU* pdu);
    PDU* DownloadFile(PDU* pdu);
    PDU* DownloadFileData(MyTcpSocket* socket);
    PDU* ShareFile(PDU* pdu);
    PDU* ShareFileAgree(PDU* pdu);
    bool CopyDir(QString ShareDir,QString RecvDir);

    //上传文件属性
    QFile m_uploadFile;
    bool m_isupload = false;
    qint64 m_fileTotalSize;
    qint64 m_uploadfileRecSize;
    //下载文件属性
    bool m_isdownload = false;
    QFile m_downloadFile;
};

#endif // MSGHANDLE_H

#ifndef MSGHANDLE_H
#define MSGHANDLE_H
#include"protocol.h"
#include<QStringList>

class Msghandle
{
public:
    Msghandle();
    void handlePdu(PDU* pdu);
    void rigster_user(PDU* pdu);
    void login_user(PDU* pdu);
    void find_user(PDU* pdu);
    void OnlineUser(PDU* pdu);
    void AddFriend(PDU* pdu);
    void AddFriendRequest(PDU* pdu);
    void AddFriendAgree();
    void ShowFriend(PDU* pdu);
    void DelFriend(PDU* pdu);
    void Chat_(PDU* pdu);
    void CreateNewPath(PDU* pdu);
    void FlushFile(PDU* pdu);
    void DelDir(PDU* pdu);
    void DelFile(PDU* pdu);
    void RenameFile(PDU* pdu);
    void MoveFile(PDU* pdu);
    void UploadFile(PDU* pdu);
    void UploadFileData(PDU* pdu);
    void DownloadFile(PDU* pdu);
    void DownloadFileData(PDU* pdu);
    void ShareFile();
    void ShareFileRequest(PDU* pdu);
    void ShareFileAgree(PDU* pdu);
};

#endif // MSGHANDLE_H

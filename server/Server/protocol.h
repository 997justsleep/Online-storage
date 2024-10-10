#ifndef PROTOCOL_H
#define PROTOCOL_H

#include<stdlib.h>
#include<unistd.h>
#include<string.h>

typedef unsigned int uint;

enum Msg_PDU_Type{
    Msg_PDU_Type_MIN=0,
    //注册
    Msg_PDU_Type_REGIST_REQUEST,
    Msg_PDU_Type_REGIST_RESPOND,
    //登录
    Msg_PDU_Type_LOGIN_REQUEST,
    Msg_PDU_Type_LOGIN_RESPOND,
    //查找用户
    Msg_PDU_Type_FindUser_REQUEST,
    Msg_PDU_Type_FindUser_RESPOND,
    //在线用户
    Msg_PDU_Type_OnlineUser_REQUEST,
    Msg_PDU_Type_OnlineUser_RESPOND,
    //添加好友
    Msg_PDU_Type_AddFriend_REQUEST,
    Msg_PDU_Type_AddFriend_RESPOND,
    //回应添加好友请求
    Msg_PDU_Type_AddFriend_AGREE,
    Msg_PDU_Type_AddFriend_REFUSE,
    //显示好友
    Msg_PDU_Type_ShowFriend_REQUEST,
    Msg_PDU_Type_ShowFriend_RESPOND,
    //删除好友
    Msg_PDU_Type_DelFriend_REQUEST,
    Msg_PDU_Type_DelFriend_RESPOND,
    //聊天
    Msg_PDU_Type_Chat_REQUEST,
    Msg_PDU_Type_Chat_RESPOND,
    //新建文件夹
    Msg_PDU_Type_NewPath_REQUEST,
    Msg_PDU_Type_NewPath_RESPOND,
    //刷新文件
    Msg_PDU_Type_FlushFile_REQUEST,
    Msg_PDU_Type_FlushFile_RESPOND,
    //删除文件夹
    Msg_PDU_Type_DelDir_REQUEST,
    Msg_PDU_Type_DelDir_RESPOND,
    //删除文件
    Msg_PDU_Type_DelFile_REQUEST,
    Msg_PDU_Type_DelFile_RESPOND,
    //重命名文件
    Msg_PDU_Type_RenameFile_REQUEST,
    Msg_PDU_Type_RenameFile_RESPOND,
    //移动文件
    Msg_PDU_Type_MoveFile_REQUEST,
    Msg_PDU_Type_MoveFile_RESPOND,
    //上传文件
    Msg_PDU_Type_UploadFile_REQUEST,
    Msg_PDU_Type_UploadFile_RESPOND,
    //上传文件数据
    Msg_PDU_Type_UploadFileData_REQUEST,
    Msg_PDU_Type_UploadFileData_RESPOND,
    //下载文件
    Msg_PDU_Type_DownloadFile_REQUEST,
    Msg_PDU_Type_DownloadFile_RESPOND,
    //下载文件数据
    Msg_PDU_Type_DownloadFileData_REQUEST,
    Msg_PDU_Type_DownloadFileData_RESPOND,
    //分享文件
    Msg_PDU_Type_ShareFile_REQUEST,
    Msg_PDU_Type_ShareFile_RESPOND,
    //同意分享文件
    Msg_PDU_Type_ShareFileAgree_REQUEST,
    Msg_PDU_Type_ShareFileAgree_RESPOND,


    Msg_PDU_Type_MAX=0x00fffff
};

struct PDU{//通信协议结构体
    uint uiPDUlen;//总消息长度（对象大小）
    uint uiMsgType;//消息类型
    uint uiMsgLen;//实际消息长度
    char caData[64];//属性
    char caMsg[];//实际消息
};

struct FileInfo{
    char FileName[32];//文件名
    int FileType;//文件类型
};

PDU *mkPDU(uint uiMsgLen);

#endif // PROTOCOL_H

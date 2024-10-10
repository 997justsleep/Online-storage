#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include<QSqlDatabase>

class OperateDB : public QObject
{
    Q_OBJECT
public:
    static OperateDB& getinstance();
    void connect();
    ~OperateDB();
    bool handleRigist(char* name,char* pwd);//注册
    bool handlelogin(char* name,char* pwd);//登录
    void handleofflie(const char* name);//下线
    int handlefind(const char*name);//查找
    int handleAdd(const char* CurName,const char* TarName);//添加
    void handleAddAgree(const char* curname,const char* tarname);//同意添加
    QStringList Showfriend(const char* name);//显示好友
    QStringList handleOnline();//在线
    bool DelFriend(const char*CurName,const char* DelName);//删除好友
private:
    QSqlDatabase m_db;
    explicit OperateDB(QObject *parent = nullptr);
    OperateDB (const OperateDB& )=delete;
    OperateDB& operator=(const OperateDB&) = delete;
signals:

};

#endif // OPERATEDB_H

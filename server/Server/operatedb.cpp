#include "operatedb.h"
#include<QSqlDatabase>
#include<QDebug>
#include <QSqlQuery>


OperateDB &OperateDB::getinstance()
{
    static OperateDB instance;
    return instance;
}

void OperateDB::connect()
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName("QTpro");
    m_db.setUserName("zxr");
    m_db.setPort(3306);
    m_db.setPassword("123456");
    if(m_db.open()){
        qDebug()<<"数据库连接成功";
    }else{
        qDebug()<<"数据库连接失败";
    }
}

OperateDB::~OperateDB()
{
    m_db.close();
}

bool OperateDB::handleRigist(char *name, char *pwd)
{
    //传入指针，先判空
    if(name == NULL || pwd == NULL){
        return false;
    }
    //用一个字符串接收sql语句
    QString sql = QString("select * from user_info where name = '%1'").arg(name);
    //数据库对象 q
    QSqlQuery q;
    //q.exec()可以返回是否执行sql语句 ，q.next()得到下一条记录
    if(!q.exec(sql)||q.next()){
        //当语句执行失败或者有记录，则注册失败
        qDebug()<<"hhhhh";
        return false;
    }else{
        //否则插入注册信息，返回执行结果
        qDebug()<<"aaaaaaaaaaa";
        sql = QString("insert into user_info(name,pwd) values('%1','%2')").arg(name).arg(pwd);
        return q.exec(sql);
    }

}

bool OperateDB::handlelogin(char *name, char *pwd)
{
    if(name == NULL|| pwd == NULL){
        return false;
    }
    QString sql = QString("select *from user_info where name = '%1' and pwd ='%2'").arg(name).arg(pwd);
    QSqlQuery q;
    if(!q.exec(sql)||!q.next()){
        return false;
    }
    else{
        sql = QString("update user_info set online = 1 where name = '%1' ").arg(name);
        return q.exec(sql);
    }
}

void OperateDB::handleofflie(const char *name)
{
    if(name == NULL){
        qDebug()<<"not offline---";
        return;
    }
    else{
        QString sql = QString("update user_info set online = 0 where name = '%1'").arg(name);
        QSqlQuery q;
        qDebug()<<"offline-----";
        q.exec(sql);
    }
}

int OperateDB::handlefind(const char *name)
{
    if(name == NULL){
        return -1;//不存在
    }else{
        QString sql = QString("select online from user_info where name = '%1'").arg(name);
        QSqlQuery q;
        if(!q.exec(sql)||!q.next())
            return -1;//不存在
        else{
            return q.value(0).toInt();//取出值，第0个位置的值，转成整数
        }
    }
}

int OperateDB::handleAdd(const char *CurName, const char *TarName)
{
    if(CurName == NULL || TarName == NULL){
        return -2;
    }
    QString sql = QString(R"(
                          select * from friend
                          where
                          (
                          user_id = (select id from user_info where name = '%1')
                          and
                          friend_id = (select id from user_info where name = '%2')
                          )
                          or
                          (
                          user_id = (select id from user_info where name = '%3')
                          and
                          friend_id = (select id from user_info where name = '%4')
                          )
                          )").arg(CurName).arg(TarName).arg(TarName).arg(CurName);
    qDebug()<<"cur:"<<CurName<<"tar:"<<TarName;
    QSqlQuery q;
    q.exec(sql);
    if(q.next()){
        return -1;//已经是好友
    }

    sql = QString("select online from user_info where name = '%1'").arg(TarName);
    q.exec(sql);
    q.next();
    //返回在线情况
    qDebug()<<"DB ret:"<<q.value(0).toInt();
    return q.value(0).toInt();
}

void OperateDB::handleAddAgree(const char *CurName, const char *TarName)
{
    if(CurName == NULL || TarName == NULL){
        return;
    }
    QString sql = QString(R"(
                          insert into friend(user_id,friend_id)
                          select u1.id,u2.id from user_info u1 ,user_info u2
                          where u1.name = '%1' and u2.name = '%2';
                          )").arg(CurName).arg(TarName);
    qDebug()<<"cur:"<<CurName<<"tar:"<<TarName;
    QSqlQuery q;
    q.exec(sql);
    return;
}

QStringList OperateDB::Showfriend(const char *name)
{
    QStringList Friend;
    if(name == NULL)return Friend;
    QString sql = QString(R"(
                          select name from user_info
                          where id in(
                            select user_id from friend where friend_id = (select id from user_info where name = '%1')
                            union
                            select friend_id from friend where user_id = (select id from user_info where name = '%2')
                          )and online = 1
                          )").arg(name).arg(name);
    QSqlQuery q;
    q.exec(sql);
    while(q.next()){
        Friend.append(q.value(0).toString());
        qDebug()<<"friend:"<<q.value(0).toString();
    }
    return Friend;

}

QStringList OperateDB::handleOnline()
{
    QString sql = QString("select name from user_info where online = 1");
    QSqlQuery q;
    q.exec(sql);
    QStringList a;
    a.clear();
    while(q.next()){
        a.append(q.value(0).toString());
        qDebug()<<"online_user:"<<q.value(0).toString();
    }
    return a;
}

bool OperateDB::DelFriend(const char *CurName, const char *DelName)
{
    if(CurName == NULL || DelName == NULL){
        return 0;
    }
    QString isFriend = QString(R"(
                               (
                               user_id = (select id from user_info where name = '%1')
                               and
                               friend_id = (select id from user_info where name = '%2')
                               )
                               or
                               (
                               user_id = (select id from user_info where name = '%3')
                               and
                               friend_id = (select id from user_info where name = '%4')
                               )
                               )").arg(CurName).arg(DelName).arg(DelName).arg(CurName);
    QString sql = QString("select * from friend where")+isFriend;
    qDebug()<<"cur:"<<CurName<<"del:"<<DelName;
    QSqlQuery q;
    q.exec(sql);
    if(!q.next()){
        return 0;//已经不是好友
    }
    qDebug()<<"删除阶段";
    sql = QString("delete from friend where ")+isFriend;
    return q.exec(sql);
}

OperateDB::OperateDB(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
}

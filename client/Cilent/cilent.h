#ifndef CILENT_H
#define CILENT_H

#include <QWidget>
#include<QTcpSocket>
#include"protocol.h"
#include"msghandle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Cilent; }
QT_END_NAMESPACE

class Cilent : public QWidget
{
    Q_OBJECT

public:
    Msghandle* mh;
    ~Cilent();
    PDU* ReadMsg();
    void SendMsg(PDU* pdu);
    void loadConfig();
    QTcpSocket& getTcpSocket();
    QString LoginUser_name;
    static Cilent& getInstance();//通过一个静态函数得到该类的唯一实例
    QByteArray buffer;
public slots:
    void showConnect();
    void recvMsg();

private slots:

    void on_register_PB_clicked();

    void on_login_PB_clicked();

private:
    //实现单例模式
    //私有化 构造函数，删除 拷贝构造函数 和 赋值运算符重载
    Cilent(QWidget *parent = nullptr);
    Cilent(const Cilent&)=delete;
    Cilent& operator= (const Cilent&)=delete;

    Ui::Cilent *ui;
    QString m_strIP;
    quint16 m_usPort;
    QTcpSocket m_tcpSocket;
};
#endif // CILENT_H

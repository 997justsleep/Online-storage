#ifndef SERVER_H
#define SERVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QWidget
{
    Q_OBJECT

public:

    ~Server();
    void loadConfig();
    static Server& getinstance();
    QString getRootPath();

private:

    Server(QWidget *parent = nullptr);
    Server(const Server& other) = delete;
    Server operator=(const Server& other) = delete;

    Ui::Server *ui;
    QString m_strIP;
    quint16 m_usPort;
    QString m_rootPath;
};
#endif // SERVER_H

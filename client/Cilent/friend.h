#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include"onlineuser.h"
#include"chat.h"

namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = nullptr);
    ~Friend();
    OnlineUser* getOnlineUser();
    Chat* getChat();
    void RefreshFriend();
    void updateFriendWidget(QStringList Friend);
    QListWidget* getOnlineFriend();

private slots:
    void on_findUser_PB_clicked();

    void on_onlineUer_PB_clicked();

    void on_flushFri_PB_clicked();

    void on_delFri_PB_clicked();

    void on_talk_PB_clicked();

private:
    Chat* m_chat;
    Ui::Friend *ui;
    OnlineUser* m_onlineuser;

};

#endif // FRIEND_H

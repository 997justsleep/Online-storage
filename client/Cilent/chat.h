#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include"protocol.h"

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();
    QString ChatName;
    void updateChat_TE(QString msg);

private slots:
    void on_send_PB_clicked();

private:
    Ui::Chat *ui;
};

#endif // CHAT_H

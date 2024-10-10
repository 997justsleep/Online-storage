#ifndef INDEX_H
#define INDEX_H

#include <QWidget>
#include"file.h"
#include"friend.h"

namespace Ui {
class Index;
}

class Index : public QWidget
{
    Q_OBJECT

public:

    ~Index();
    File*getfile();
    Friend*getfriend();
    static Index& getinstance();

private slots:
    void on_Friend_PB_clicked();

    void on_File_PB_clicked();

private:
    explicit Index(QWidget *parent = nullptr);
    Index(Index& other)=delete;
    Index& operator=(const Index&other)=delete;
    Ui::Index *ui;
};

#endif // INDEX_H

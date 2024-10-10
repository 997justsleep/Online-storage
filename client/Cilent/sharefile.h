#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QWidget>

namespace Ui {
class ShareFile;
}

class ShareFile : public QWidget
{
    Q_OBJECT

public:
    explicit ShareFile(QWidget *parent = nullptr);
    ~ShareFile();
    void updateOnlineFriend_LW();
private slots:
    void on_Allselect_PB_clicked();

    void on_Cancelselect_PB_clicked();

    void on_OK_PB_clicked();

private:
    Ui::ShareFile *ui;
};

#endif // SHAREFILE_H

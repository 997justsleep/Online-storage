#include "index.h"
#include "ui_index.h"
#include"cilent.h"

Index::Index(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Index)
{
    ui->setupUi(this);
    this->setWindowTitle(Cilent::getInstance().LoginUser_name);
    this->setFixedSize(800,600);
}

Index::~Index()
{
    delete ui;
}

File *Index::getfile()
{
    return ui->filePage;
}

Friend *Index::getfriend()
{
    return ui->friendPage;
}

Index &Index::getinstance()
{
    static Index instance;
    return instance;
}

void Index::on_Friend_PB_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->Friend_PB->setStyleSheet("QPushButton{border:none;background-color:rgb(255, 255, 255);padding:20px;}");
    ui->File_PB->setStyleSheet("QPushButton{border:none;padding:20px;}");
}

void Index::on_File_PB_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->File_PB->setStyleSheet("QPushButton{border:none;background-color:rgb(255, 255, 255);padding:20px;}");
    ui->Friend_PB->setStyleSheet("QPushButton{border:none;padding:20px;}");
}

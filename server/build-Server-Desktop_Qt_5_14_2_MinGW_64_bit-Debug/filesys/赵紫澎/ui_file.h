/********************************************************************************
** Form generated from reading UI file 'file.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_H
#define UI_FILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_File
{
public:
    QHBoxLayout *horizontalLayout_2;
    QListWidget *file_LW;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *flushFile_PB;
    QPushButton *mKDir_PB;
    QPushButton *delDir_PB;
    QPushButton *delFile_PB;
    QPushButton *mvFile_PB;
    QVBoxLayout *verticalLayout_2;
    QPushButton *return_PB;
    QPushButton *rename_PB;
    QPushButton *shareFile_PB;
    QPushButton *download_PB;
    QPushButton *upload_PB;

    void setupUi(QWidget *File)
    {
        if (File->objectName().isEmpty())
            File->setObjectName(QString::fromUtf8("File"));
        File->resize(521, 374);
        horizontalLayout_2 = new QHBoxLayout(File);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        file_LW = new QListWidget(File);
        file_LW->setObjectName(QString::fromUtf8("file_LW"));

        horizontalLayout_2->addWidget(file_LW);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        flushFile_PB = new QPushButton(File);
        flushFile_PB->setObjectName(QString::fromUtf8("flushFile_PB"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(flushFile_PB->sizePolicy().hasHeightForWidth());
        flushFile_PB->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(flushFile_PB);

        mKDir_PB = new QPushButton(File);
        mKDir_PB->setObjectName(QString::fromUtf8("mKDir_PB"));
        sizePolicy.setHeightForWidth(mKDir_PB->sizePolicy().hasHeightForWidth());
        mKDir_PB->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(mKDir_PB);

        delDir_PB = new QPushButton(File);
        delDir_PB->setObjectName(QString::fromUtf8("delDir_PB"));
        sizePolicy.setHeightForWidth(delDir_PB->sizePolicy().hasHeightForWidth());
        delDir_PB->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(delDir_PB);

        delFile_PB = new QPushButton(File);
        delFile_PB->setObjectName(QString::fromUtf8("delFile_PB"));
        sizePolicy.setHeightForWidth(delFile_PB->sizePolicy().hasHeightForWidth());
        delFile_PB->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(delFile_PB);

        mvFile_PB = new QPushButton(File);
        mvFile_PB->setObjectName(QString::fromUtf8("mvFile_PB"));
        sizePolicy.setHeightForWidth(mvFile_PB->sizePolicy().hasHeightForWidth());
        mvFile_PB->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(mvFile_PB);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        return_PB = new QPushButton(File);
        return_PB->setObjectName(QString::fromUtf8("return_PB"));
        sizePolicy.setHeightForWidth(return_PB->sizePolicy().hasHeightForWidth());
        return_PB->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(return_PB);

        rename_PB = new QPushButton(File);
        rename_PB->setObjectName(QString::fromUtf8("rename_PB"));
        sizePolicy.setHeightForWidth(rename_PB->sizePolicy().hasHeightForWidth());
        rename_PB->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(rename_PB);

        shareFile_PB = new QPushButton(File);
        shareFile_PB->setObjectName(QString::fromUtf8("shareFile_PB"));
        sizePolicy.setHeightForWidth(shareFile_PB->sizePolicy().hasHeightForWidth());
        shareFile_PB->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(shareFile_PB);

        download_PB = new QPushButton(File);
        download_PB->setObjectName(QString::fromUtf8("download_PB"));
        sizePolicy.setHeightForWidth(download_PB->sizePolicy().hasHeightForWidth());
        download_PB->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(download_PB);

        upload_PB = new QPushButton(File);
        upload_PB->setObjectName(QString::fromUtf8("upload_PB"));
        sizePolicy.setHeightForWidth(upload_PB->sizePolicy().hasHeightForWidth());
        upload_PB->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(upload_PB);


        horizontalLayout->addLayout(verticalLayout_2);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(File);

        QMetaObject::connectSlotsByName(File);
    } // setupUi

    void retranslateUi(QWidget *File)
    {
        File->setWindowTitle(QCoreApplication::translate("File", "Form", nullptr));
        flushFile_PB->setText(QCoreApplication::translate("File", "\345\210\267\346\226\260\346\226\207\344\273\266", nullptr));
        mKDir_PB->setText(QCoreApplication::translate("File", "\345\210\233\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        delDir_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266\345\244\271", nullptr));
        delFile_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
        mvFile_PB->setText(QCoreApplication::translate("File", "\347\247\273\345\212\250\346\226\207\344\273\266", nullptr));
        return_PB->setText(QCoreApplication::translate("File", "\350\277\224\345\233\236\344\270\212\344\270\200\347\272\247", nullptr));
        rename_PB->setText(QCoreApplication::translate("File", "\351\207\215\345\221\275\345\220\215", nullptr));
        shareFile_PB->setText(QCoreApplication::translate("File", "\345\210\206\344\272\253\346\226\207\344\273\266", nullptr));
        download_PB->setText(QCoreApplication::translate("File", "\344\270\213\350\275\275\346\226\207\344\273\266", nullptr));
        upload_PB->setText(QCoreApplication::translate("File", "\344\270\212\344\274\240\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class File: public Ui_File {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_H

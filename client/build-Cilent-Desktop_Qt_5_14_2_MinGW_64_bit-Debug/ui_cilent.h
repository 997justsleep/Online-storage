/********************************************************************************
** Form generated from reading UI file 'cilent.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CILENT_H
#define UI_CILENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Cilent
{
public:

    void setupUi(QWidget *Cilent)
    {
        if (Cilent->objectName().isEmpty())
            Cilent->setObjectName(QString::fromUtf8("Cilent"));
        Cilent->resize(800, 600);

        retranslateUi(Cilent);

        QMetaObject::connectSlotsByName(Cilent);
    } // setupUi

    void retranslateUi(QWidget *Cilent)
    {
        Cilent->setWindowTitle(QCoreApplication::translate("Cilent", "Cilent", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cilent: public Ui_Cilent {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CILENT_H

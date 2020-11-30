/********************************************************************************
** Form generated from reading UI file 'show_capture.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOW_CAPTURE_H
#define UI_SHOW_CAPTURE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_show_capture
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *show_pic;
    QPushButton *again;
    QPushButton *ok;

    void setupUi(QWidget *show_capture)
    {
        if (show_capture->objectName().isEmpty())
            show_capture->setObjectName(QString::fromUtf8("show_capture"));
        show_capture->resize(452, 286);
        verticalLayout_2 = new QVBoxLayout(show_capture);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        show_pic = new QLabel(show_capture);
        show_pic->setObjectName(QString::fromUtf8("show_pic"));
        show_pic->setFrameShape(QFrame::Box);
        show_pic->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(show_pic);

        again = new QPushButton(show_capture);
        again->setObjectName(QString::fromUtf8("again"));

        verticalLayout_2->addWidget(again);

        ok = new QPushButton(show_capture);
        ok->setObjectName(QString::fromUtf8("ok"));

        verticalLayout_2->addWidget(ok);


        retranslateUi(show_capture);

        QMetaObject::connectSlotsByName(show_capture);
    } // setupUi

    void retranslateUi(QWidget *show_capture)
    {
        show_capture->setWindowTitle(QCoreApplication::translate("show_capture", "Form", nullptr));
        show_pic->setText(QCoreApplication::translate("show_capture", "<html><head/><body><p><br/></p></body></html>", nullptr));
        again->setText(QCoreApplication::translate("show_capture", "Capture Again", nullptr));
        ok->setText(QCoreApplication::translate("show_capture", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class show_capture: public Ui_show_capture {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOW_CAPTURE_H

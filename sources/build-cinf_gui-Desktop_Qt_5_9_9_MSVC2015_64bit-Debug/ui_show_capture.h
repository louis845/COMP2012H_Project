/********************************************************************************
** Form generated from reading UI file 'show_capture.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOW_CAPTURE_H
#define UI_SHOW_CAPTURE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
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
    QPushButton *ok;

    void setupUi(QWidget *show_capture)
    {
        if (show_capture->objectName().isEmpty())
            show_capture->setObjectName(QStringLiteral("show_capture"));
        show_capture->resize(452, 286);
        verticalLayout_2 = new QVBoxLayout(show_capture);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        show_pic = new QLabel(show_capture);
        show_pic->setObjectName(QStringLiteral("show_pic"));
        show_pic->setFrameShape(QFrame::Box);
        show_pic->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(show_pic);

        ok = new QPushButton(show_capture);
        ok->setObjectName(QStringLiteral("ok"));

        verticalLayout_2->addWidget(ok);


        retranslateUi(show_capture);

        QMetaObject::connectSlotsByName(show_capture);
    } // setupUi

    void retranslateUi(QWidget *show_capture)
    {
        show_capture->setWindowTitle(QApplication::translate("show_capture", "Form", Q_NULLPTR));
        show_pic->setText(QApplication::translate("show_capture", "<html><head/><body><p><br/></p></body></html>", Q_NULLPTR));
        ok->setText(QApplication::translate("show_capture", "OK", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class show_capture: public Ui_show_capture {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOW_CAPTURE_H

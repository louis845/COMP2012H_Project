/********************************************************************************
** Form generated from reading UI file 'input_key.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUT_KEY_H
#define UI_INPUT_KEY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_input_key
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *user_label;
    QLineEdit *user_text;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *password_label;
    QLineEdit *password_text;
    QSpacerItem *horizontalSpacer_4;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *cancel_btn;
    QPushButton *no_ocr_btn;
    QPushButton *ok_btn;
    QSpacerItem *horizontalSpacer_6;
    QWidget *widget_4;
    QLabel *label;

    void setupUi(QWidget *input_key)
    {
        if (input_key->objectName().isEmpty())
            input_key->setObjectName(QString::fromUtf8("input_key"));
        input_key->resize(407, 449);
        widget = new QWidget(input_key);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(40, 260, 321, 61));
        widget->setMinimumSize(QSize(1, 1));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        user_label = new QLabel(widget);
        user_label->setObjectName(QString::fromUtf8("user_label"));

        horizontalLayout->addWidget(user_label);

        user_text = new QLineEdit(widget);
        user_text->setObjectName(QString::fromUtf8("user_text"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(user_text->sizePolicy().hasHeightForWidth());
        user_text->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(user_text);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        widget_2 = new QWidget(input_key);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(40, 314, 319, 71));
        widget_2->setMinimumSize(QSize(0, 0));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        password_label = new QLabel(widget_2);
        password_label->setObjectName(QString::fromUtf8("password_label"));

        horizontalLayout_2->addWidget(password_label);

        password_text = new QLineEdit(widget_2);
        password_text->setObjectName(QString::fromUtf8("password_text"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(150);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(password_text->sizePolicy().hasHeightForWidth());
        password_text->setSizePolicy(sizePolicy1);
        password_text->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(password_text);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        widget_3 = new QWidget(input_key);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(40, 380, 341, 44));
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        cancel_btn = new QPushButton(widget_3);
        cancel_btn->setObjectName(QString::fromUtf8("cancel_btn"));

        horizontalLayout_3->addWidget(cancel_btn);

        no_ocr_btn = new QPushButton(widget_3);
        no_ocr_btn->setObjectName(QString::fromUtf8("no_ocr_btn"));

        horizontalLayout_3->addWidget(no_ocr_btn);

        ok_btn = new QPushButton(widget_3);
        ok_btn->setObjectName(QString::fromUtf8("ok_btn"));

        horizontalLayout_3->addWidget(ok_btn);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        widget_4 = new QWidget(input_key);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setGeometry(QRect(30, 20, 341, 241));
        label = new QLabel(widget_4);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 321, 191));
        label->setPixmap(QPixmap(QString::fromUtf8(":/rsc/transparent_logo.png")));
        label->setAlignment(Qt::AlignCenter);

        retranslateUi(input_key);

        QMetaObject::connectSlotsByName(input_key);
    } // setupUi

    void retranslateUi(QWidget *input_key)
    {
        input_key->setWindowTitle(QCoreApplication::translate("input_key", "Form", nullptr));
        user_label->setText(QCoreApplication::translate("input_key", "Username:", nullptr));
        password_label->setText(QCoreApplication::translate("input_key", "Password:", nullptr));
        cancel_btn->setText(QCoreApplication::translate("input_key", "Cancel", nullptr));
        no_ocr_btn->setText(QCoreApplication::translate("input_key", "Do not use OCR", nullptr));
        ok_btn->setText(QCoreApplication::translate("input_key", "OK", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class input_key: public Ui_input_key {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUT_KEY_H

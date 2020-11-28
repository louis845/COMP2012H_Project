/********************************************************************************
** Form generated from reading UI file 'input_key.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUT_KEY_H
#define UI_INPUT_KEY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_input_key
{
public:
    QVBoxLayout *verticalLayout;
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
    QPushButton *ok_btn;
    QSpacerItem *horizontalSpacer_6;

    void setupUi(QWidget *input_key)
    {
        if (input_key->objectName().isEmpty())
            input_key->setObjectName(QStringLiteral("input_key"));
        input_key->resize(280, 220);
        input_key->setMinimumSize(QSize(280, 220));
        input_key->setMaximumSize(QSize(280, 220));
        verticalLayout = new QVBoxLayout(input_key);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget = new QWidget(input_key);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        user_label = new QLabel(widget);
        user_label->setObjectName(QStringLiteral("user_label"));

        horizontalLayout->addWidget(user_label);

        user_text = new QLineEdit(widget);
        user_text->setObjectName(QStringLiteral("user_text"));

        horizontalLayout->addWidget(user_text);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(input_key);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        password_label = new QLabel(widget_2);
        password_label->setObjectName(QStringLiteral("password_label"));

        horizontalLayout_2->addWidget(password_label);

        password_text = new QLineEdit(widget_2);
        password_text->setObjectName(QStringLiteral("password_text"));
        password_text->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(password_text);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addWidget(widget_2);

        widget_3 = new QWidget(input_key);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        cancel_btn = new QPushButton(widget_3);
        cancel_btn->setObjectName(QStringLiteral("cancel_btn"));

        horizontalLayout_3->addWidget(cancel_btn);

        ok_btn = new QPushButton(widget_3);
        ok_btn->setObjectName(QStringLiteral("ok_btn"));

        horizontalLayout_3->addWidget(ok_btn);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        verticalLayout->addWidget(widget_3);


        retranslateUi(input_key);

        QMetaObject::connectSlotsByName(input_key);
    } // setupUi

    void retranslateUi(QWidget *input_key)
    {
        input_key->setWindowTitle(QApplication::translate("input_key", "Form", Q_NULLPTR));
        user_label->setText(QApplication::translate("input_key", "Username:", Q_NULLPTR));
        password_label->setText(QApplication::translate("input_key", "Password:", Q_NULLPTR));
        cancel_btn->setText(QApplication::translate("input_key", "Cancel", Q_NULLPTR));
        ok_btn->setText(QApplication::translate("input_key", "OK", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class input_key: public Ui_input_key {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUT_KEY_H

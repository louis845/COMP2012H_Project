/********************************************************************************
** Form generated from reading UI file 'begin_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEGIN_WIDGET_H
#define UI_BEGIN_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_begin_widget
{
public:

    void setupUi(QWidget *begin_widget)
    {
        if (begin_widget->objectName().isEmpty())
            begin_widget->setObjectName(QStringLiteral("begin_widget"));
        begin_widget->resize(450, 450);
        begin_widget->setMinimumSize(QSize(450, 450));
        begin_widget->setMaximumSize(QSize(450, 450));

        retranslateUi(begin_widget);

        QMetaObject::connectSlotsByName(begin_widget);
    } // setupUi

    void retranslateUi(QWidget *begin_widget)
    {
        begin_widget->setWindowTitle(QApplication::translate("begin_widget", "begin_widget", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class begin_widget: public Ui_begin_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEGIN_WIDGET_H

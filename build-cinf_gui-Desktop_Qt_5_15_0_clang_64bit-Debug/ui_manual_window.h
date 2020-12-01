/********************************************************************************
** Form generated from reading UI file 'manual_window.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANUAL_WINDOW_H
#define UI_MANUAL_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_manual_window
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *manual_window)
    {
        if (manual_window->objectName().isEmpty())
            manual_window->setObjectName(QString::fromUtf8("manual_window"));
        manual_window->resize(679, 481);
        horizontalLayout = new QHBoxLayout(manual_window);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        treeWidget = new QTreeWidget(manual_window);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        horizontalLayout->addWidget(treeWidget);

        textBrowser = new QTextBrowser(manual_window);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        horizontalLayout->addWidget(textBrowser);


        retranslateUi(manual_window);

        QMetaObject::connectSlotsByName(manual_window);
    } // setupUi

    void retranslateUi(QWidget *manual_window)
    {
        manual_window->setWindowTitle(QCoreApplication::translate("manual_window", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class manual_window: public Ui_manual_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANUAL_WINDOW_H

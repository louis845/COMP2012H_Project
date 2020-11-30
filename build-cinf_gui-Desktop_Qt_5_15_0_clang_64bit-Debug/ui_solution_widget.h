/********************************************************************************
** Form generated from reading UI file 'solution_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOLUTION_WIDGET_H
#define UI_SOLUTION_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_solution_widget
{
public:
    QScrollArea *scrollArea;
    QWidget *steps_widget;
    QWidget *left_widget;
    QTabWidget *tabWidget;
    QWidget *plain_tab;
    QPlainTextEdit *plain_textedit;
    QWidget *ascii_tab;
    QPlainTextEdit *ascii_textedit;
    QScrollArea *scrollAreaIntepretation;
    QWidget *scrollAreaWidgetContents;
    QPushButton *scan_btn;
    QWidget *finish_btn_widget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *next_btn2;
    QPushButton *finish_btn;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *methods_btn;
    QSpacerItem *horizontalSpacer;
    QPushButton *previous_btn;
    QPushButton *next_btn;

    void setupUi(QWidget *solution_widget)
    {
        if (solution_widget->objectName().isEmpty())
            solution_widget->setObjectName(QString::fromUtf8("solution_widget"));
        solution_widget->resize(830, 580);
        solution_widget->setMinimumSize(QSize(830, 580));
        solution_widget->setMaximumSize(QSize(830, 580));
        scrollArea = new QScrollArea(solution_widget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(430, 70, 361, 441));
        scrollArea->setWidgetResizable(true);
        steps_widget = new QWidget();
        steps_widget->setObjectName(QString::fromUtf8("steps_widget"));
        steps_widget->setGeometry(QRect(0, 0, 359, 439));
        scrollArea->setWidget(steps_widget);
        left_widget = new QWidget(solution_widget);
        left_widget->setObjectName(QString::fromUtf8("left_widget"));
        left_widget->setGeometry(QRect(30, 20, 381, 541));
        tabWidget = new QTabWidget(left_widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 220, 361, 311));
        plain_tab = new QWidget();
        plain_tab->setObjectName(QString::fromUtf8("plain_tab"));
        plain_textedit = new QPlainTextEdit(plain_tab);
        plain_textedit->setObjectName(QString::fromUtf8("plain_textedit"));
        plain_textedit->setGeometry(QRect(10, 10, 331, 271));
        tabWidget->addTab(plain_tab, QString());
        ascii_tab = new QWidget();
        ascii_tab->setObjectName(QString::fromUtf8("ascii_tab"));
        ascii_textedit = new QPlainTextEdit(ascii_tab);
        ascii_textedit->setObjectName(QString::fromUtf8("ascii_textedit"));
        ascii_textedit->setGeometry(QRect(10, 10, 331, 271));
        tabWidget->addTab(ascii_tab, QString());
        scrollAreaIntepretation = new QScrollArea(left_widget);
        scrollAreaIntepretation->setObjectName(QString::fromUtf8("scrollAreaIntepretation"));
        scrollAreaIntepretation->setGeometry(QRect(20, 50, 331, 161));
        scrollAreaIntepretation->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 329, 159));
        scrollAreaIntepretation->setWidget(scrollAreaWidgetContents);
        scan_btn = new QPushButton(left_widget);
        scan_btn->setObjectName(QString::fromUtf8("scan_btn"));
        scan_btn->setGeometry(QRect(20, 10, 331, 23));
        finish_btn_widget = new QWidget(solution_widget);
        finish_btn_widget->setObjectName(QString::fromUtf8("finish_btn_widget"));
        finish_btn_widget->setGeometry(QRect(420, 520, 381, 41));
        horizontalLayout_2 = new QHBoxLayout(finish_btn_widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        next_btn2 = new QPushButton(finish_btn_widget);
        next_btn2->setObjectName(QString::fromUtf8("next_btn2"));

        horizontalLayout_2->addWidget(next_btn2);

        finish_btn = new QPushButton(finish_btn_widget);
        finish_btn->setObjectName(QString::fromUtf8("finish_btn"));

        horizontalLayout_2->addWidget(finish_btn);

        widget = new QWidget(solution_widget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(420, 20, 381, 44));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        methods_btn = new QPushButton(widget);
        methods_btn->setObjectName(QString::fromUtf8("methods_btn"));

        horizontalLayout_3->addWidget(methods_btn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        previous_btn = new QPushButton(widget);
        previous_btn->setObjectName(QString::fromUtf8("previous_btn"));

        horizontalLayout_3->addWidget(previous_btn);

        next_btn = new QPushButton(widget);
        next_btn->setObjectName(QString::fromUtf8("next_btn"));

        horizontalLayout_3->addWidget(next_btn);


        retranslateUi(solution_widget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(solution_widget);
    } // setupUi

    void retranslateUi(QWidget *solution_widget)
    {
        solution_widget->setWindowTitle(QCoreApplication::translate("solution_widget", "Form", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(plain_tab), QCoreApplication::translate("solution_widget", "Plain", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(ascii_tab), QCoreApplication::translate("solution_widget", "Asciimath", nullptr));
        scan_btn->setText(QCoreApplication::translate("solution_widget", "Scan", nullptr));
        next_btn2->setText(QCoreApplication::translate("solution_widget", "Compute", nullptr));
        finish_btn->setText(QCoreApplication::translate("solution_widget", "Finished", nullptr));
        methods_btn->setText(QCoreApplication::translate("solution_widget", "Methods", nullptr));
        previous_btn->setText(QCoreApplication::translate("solution_widget", "Previous", nullptr));
        next_btn->setText(QCoreApplication::translate("solution_widget", "Next", nullptr));
    } // retranslateUi

};

namespace Ui {
    class solution_widget: public Ui_solution_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOLUTION_WIDGET_H

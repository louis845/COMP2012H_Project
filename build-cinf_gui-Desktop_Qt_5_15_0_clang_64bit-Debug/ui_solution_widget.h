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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_solution_widget
{
public:
    QWidget *left_widget;
    QPushButton *scan_btn;
    QTabWidget *tabWidget_2;
    QWidget *tab;
    QScrollArea *scrollAreaIntepretation;
    QWidget *scrollAreaWidgetContents;
    QWidget *tab_2;
    QScrollArea *scrollAreaOriginal;
    QWidget *scrollAreaWidgetContents_2;
    QPlainTextEdit *ascii_textedit;
    QWidget *widget_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *methods_btn;
    QSpacerItem *horizontalSpacer;
    QPushButton *previous_btn;
    QPushButton *next_btn;
    QScrollArea *scrollArea;
    QWidget *steps_widget;
    QWidget *finish_btn_widget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *next_btn2;
    QProgressBar *disp_prog;
    QPushButton *finish_btn;
    QTreeWidget *treeWidget;

    void setupUi(QWidget *solution_widget)
    {
        if (solution_widget->objectName().isEmpty())
            solution_widget->setObjectName(QString::fromUtf8("solution_widget"));
        solution_widget->resize(996, 580);
        solution_widget->setMinimumSize(QSize(830, 580));
        solution_widget->setMaximumSize(QSize(3000, 3000));
        left_widget = new QWidget(solution_widget);
        left_widget->setObjectName(QString::fromUtf8("left_widget"));
        left_widget->setGeometry(QRect(20, 12, 398, 556));
        scan_btn = new QPushButton(left_widget);
        scan_btn->setObjectName(QString::fromUtf8("scan_btn"));
        scan_btn->setGeometry(QRect(20, 10, 331, 23));
        tabWidget_2 = new QTabWidget(left_widget);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(10, 50, 361, 181));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        scrollAreaIntepretation = new QScrollArea(tab);
        scrollAreaIntepretation->setObjectName(QString::fromUtf8("scrollAreaIntepretation"));
        scrollAreaIntepretation->setGeometry(QRect(0, 0, 351, 151));
        scrollAreaIntepretation->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 349, 149));
        scrollAreaIntepretation->setWidget(scrollAreaWidgetContents);
        tabWidget_2->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        scrollAreaOriginal = new QScrollArea(tab_2);
        scrollAreaOriginal->setObjectName(QString::fromUtf8("scrollAreaOriginal"));
        scrollAreaOriginal->setGeometry(QRect(0, 0, 351, 151));
        scrollAreaOriginal->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 349, 149));
        scrollAreaOriginal->setWidget(scrollAreaWidgetContents_2);
        tabWidget_2->addTab(tab_2, QString());
        ascii_textedit = new QPlainTextEdit(left_widget);
        ascii_textedit->setObjectName(QString::fromUtf8("ascii_textedit"));
        ascii_textedit->setGeometry(QRect(10, 240, 361, 291));
        widget_2 = new QWidget(solution_widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(410, 12, 398, 556));
        widget = new QWidget(widget_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 381, 44));
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

        scrollArea = new QScrollArea(widget_2);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(10, 50, 371, 441));
        scrollArea->setWidgetResizable(true);
        steps_widget = new QWidget();
        steps_widget->setObjectName(QString::fromUtf8("steps_widget"));
        steps_widget->setGeometry(QRect(0, 0, 369, 439));
        scrollArea->setWidget(steps_widget);
        finish_btn_widget = new QWidget(widget_2);
        finish_btn_widget->setObjectName(QString::fromUtf8("finish_btn_widget"));
        finish_btn_widget->setGeometry(QRect(0, 500, 381, 41));
        horizontalLayout_2 = new QHBoxLayout(finish_btn_widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        next_btn2 = new QPushButton(finish_btn_widget);
        next_btn2->setObjectName(QString::fromUtf8("next_btn2"));

        horizontalLayout_2->addWidget(next_btn2);

        disp_prog = new QProgressBar(finish_btn_widget);
        disp_prog->setObjectName(QString::fromUtf8("disp_prog"));
        disp_prog->setValue(24);

        horizontalLayout_2->addWidget(disp_prog);

        finish_btn = new QPushButton(finish_btn_widget);
        finish_btn->setObjectName(QString::fromUtf8("finish_btn"));

        horizontalLayout_2->addWidget(finish_btn);

        treeWidget = new QTreeWidget(solution_widget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("History"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setGeometry(QRect(810, 20, 161, 531));

        retranslateUi(solution_widget);

        tabWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(solution_widget);
    } // setupUi

    void retranslateUi(QWidget *solution_widget)
    {
        solution_widget->setWindowTitle(QCoreApplication::translate("solution_widget", "Form", nullptr));
        scan_btn->setText(QCoreApplication::translate("solution_widget", "Scan", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab), QCoreApplication::translate("solution_widget", "Intepretation", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QCoreApplication::translate("solution_widget", "Original Image", nullptr));
        methods_btn->setText(QCoreApplication::translate("solution_widget", "Methods", nullptr));
        previous_btn->setText(QCoreApplication::translate("solution_widget", "Previous", nullptr));
        next_btn->setText(QCoreApplication::translate("solution_widget", "Next", nullptr));
        next_btn2->setText(QCoreApplication::translate("solution_widget", "Compute", nullptr));
        finish_btn->setText(QCoreApplication::translate("solution_widget", "Finished", nullptr));
    } // retranslateUi

};

namespace Ui {
    class solution_widget: public Ui_solution_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOLUTION_WIDGET_H

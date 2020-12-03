/********************************************************************************
** Form generated from reading UI file 'solution_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOLUTION_WIDGET_H
#define UI_SOLUTION_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_solution_widget
{
public:
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QWidget *left_widget;
    QVBoxLayout *verticalLayout;
    QPushButton *scan_btn;
    QTabWidget *tabWidget_2;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_4;
    QScrollArea *scrollAreaIntepretation;
    QWidget *scrollAreaWidgetContents;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollAreaOriginal;
    QWidget *scrollAreaWidgetContents_2;
    QLabel *original_image_display;
    QPlainTextEdit *ascii_textedit;
    QLabel *ascii_parser_err;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
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
    QPushButton *jump_btn;
    QTreeWidget *treeWidget;

    void setupUi(QWidget *solution_widget)
    {
        if (solution_widget->objectName().isEmpty())
            solution_widget->setObjectName(QStringLiteral("solution_widget"));
        solution_widget->resize(996, 580);
        solution_widget->setMinimumSize(QSize(830, 580));
        solution_widget->setMaximumSize(QSize(3000, 3000));
        horizontalLayout = new QHBoxLayout(solution_widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        left_widget = new QWidget(solution_widget);
        left_widget->setObjectName(QStringLiteral("left_widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(20);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(left_widget->sizePolicy().hasHeightForWidth());
        left_widget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(left_widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        scan_btn = new QPushButton(left_widget);
        scan_btn->setObjectName(QStringLiteral("scan_btn"));

        verticalLayout->addWidget(scan_btn);

        tabWidget_2 = new QTabWidget(left_widget);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_4 = new QHBoxLayout(tab);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        scrollAreaIntepretation = new QScrollArea(tab);
        scrollAreaIntepretation->setObjectName(QStringLiteral("scrollAreaIntepretation"));
        scrollAreaIntepretation->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 303, 211));
        scrollAreaIntepretation->setWidget(scrollAreaWidgetContents);

        horizontalLayout_4->addWidget(scrollAreaIntepretation);

        tabWidget_2->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        scrollAreaOriginal = new QScrollArea(tab_2);
        scrollAreaOriginal->setObjectName(QStringLiteral("scrollAreaOriginal"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scrollAreaOriginal->sizePolicy().hasHeightForWidth());
        scrollAreaOriginal->setSizePolicy(sizePolicy1);
        scrollAreaOriginal->setMinimumSize(QSize(305, 211));
        scrollAreaOriginal->setWidgetResizable(false);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 303, 210));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(scrollAreaWidgetContents_2->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents_2->setSizePolicy(sizePolicy2);
        original_image_display = new QLabel(scrollAreaWidgetContents_2);
        original_image_display->setObjectName(QStringLiteral("original_image_display"));
        original_image_display->setGeometry(QRect(9, 9, 281, 191));
        scrollAreaOriginal->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_3->addWidget(scrollAreaOriginal);

        tabWidget_2->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget_2);

        ascii_textedit = new QPlainTextEdit(left_widget);
        ascii_textedit->setObjectName(QStringLiteral("ascii_textedit"));

        verticalLayout->addWidget(ascii_textedit);

        ascii_parser_err = new QLabel(left_widget);
        ascii_parser_err->setObjectName(QStringLiteral("ascii_parser_err"));

        verticalLayout->addWidget(ascii_parser_err);


        horizontalLayout->addWidget(left_widget);

        widget_2 = new QWidget(solution_widget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        widget = new QWidget(widget_2);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        methods_btn = new QPushButton(widget);
        methods_btn->setObjectName(QStringLiteral("methods_btn"));

        horizontalLayout_3->addWidget(methods_btn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        previous_btn = new QPushButton(widget);
        previous_btn->setObjectName(QStringLiteral("previous_btn"));

        horizontalLayout_3->addWidget(previous_btn);

        next_btn = new QPushButton(widget);
        next_btn->setObjectName(QStringLiteral("next_btn"));

        horizontalLayout_3->addWidget(next_btn);


        verticalLayout_2->addWidget(widget);

        scrollArea = new QScrollArea(widget_2);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        steps_widget = new QWidget();
        steps_widget->setObjectName(QStringLiteral("steps_widget"));
        steps_widget->setGeometry(QRect(0, 0, 361, 422));
        scrollArea->setWidget(steps_widget);

        verticalLayout_2->addWidget(scrollArea);

        finish_btn_widget = new QWidget(widget_2);
        finish_btn_widget->setObjectName(QStringLiteral("finish_btn_widget"));
        horizontalLayout_2 = new QHBoxLayout(finish_btn_widget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        next_btn2 = new QPushButton(finish_btn_widget);
        next_btn2->setObjectName(QStringLiteral("next_btn2"));

        horizontalLayout_2->addWidget(next_btn2);

        disp_prog = new QProgressBar(finish_btn_widget);
        disp_prog->setObjectName(QStringLiteral("disp_prog"));
        disp_prog->setMaximum(100);
        disp_prog->setValue(0);
        disp_prog->setTextVisible(false);

        horizontalLayout_2->addWidget(disp_prog);

        jump_btn = new QPushButton(finish_btn_widget);
        jump_btn->setObjectName(QStringLiteral("jump_btn"));

        horizontalLayout_2->addWidget(jump_btn);


        verticalLayout_2->addWidget(finish_btn_widget);


        horizontalLayout->addWidget(widget_2);

        treeWidget = new QTreeWidget(solution_widget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("History"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));

        horizontalLayout->addWidget(treeWidget);


        retranslateUi(solution_widget);

        tabWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(solution_widget);
    } // setupUi

    void retranslateUi(QWidget *solution_widget)
    {
        solution_widget->setWindowTitle(QApplication::translate("solution_widget", "Form", Q_NULLPTR));
        scan_btn->setText(QApplication::translate("solution_widget", "Scan", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab), QApplication::translate("solution_widget", "Intepretation", Q_NULLPTR));
        original_image_display->setText(QString());
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QApplication::translate("solution_widget", "Original Image", Q_NULLPTR));
        ascii_parser_err->setText(QString());
        methods_btn->setText(QApplication::translate("solution_widget", "Methods", Q_NULLPTR));
        previous_btn->setText(QApplication::translate("solution_widget", "Previous", Q_NULLPTR));
        next_btn->setText(QApplication::translate("solution_widget", "Next", Q_NULLPTR));
        next_btn2->setText(QApplication::translate("solution_widget", "Compute", Q_NULLPTR));
        jump_btn->setText(QApplication::translate("solution_widget", "Jump To Anwser", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class solution_widget: public Ui_solution_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOLUTION_WIDGET_H

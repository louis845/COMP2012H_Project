#ifndef MATRIXCHOOSERDIALOG_H
#define MATRIXCHOOSERDIALOG_H

#include <QDialog>
#include <QWebEngineView>
#include "parser/parser.h"

namespace Ui {
class MatrixChooserDialog;
}

class MatrixChooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MatrixChooserDialog(const Info& info, const int& outersel, QWidget *parent = nullptr);
    ~MatrixChooserDialog();

    int selected_index;
    const int outer_sel;

private:
    Ui::MatrixChooserDialog *ui;
    QWebEngineView *matrix_display;
    QMenu *select_matrix_menu;
    const Info& info;

    QString to_display_string(const TokNum::TokName& t);
    QString to_display_string(const int& i);

    void user_select_matrix(int index);

    void display(const string& str);
};

#endif // MATRIXCHOOSERDIALOG_H

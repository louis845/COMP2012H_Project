#ifndef DIALOG_KEY_H
#define DIALOG_KEY_H

#include <QDialog>
#include <QPixmap>
#include <string>
using namespace std;

namespace Ui {
class Dialog_key;
}

class Dialog_key : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_key(QPixmap &capture_pic,QWidget *parent = nullptr);
    ~Dialog_key();

private:
    Ui::Dialog_key *ui;
    QPixmap capture;
    string user;
    string password;

signals:
    void ok_cliked(string user,string password);
    void cancel_clicked();
};

#endif // DIALOG_KEY_H

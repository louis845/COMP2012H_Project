#ifndef INPUT_KEY_H
#define INPUT_KEY_H

#include <QWidget>
#include <string>
#include "utils/ocr_api.h"
using namespace std;

namespace Ui {
class input_key;
}

class input_key : public QWidget
{
    Q_OBJECT

public:
    explicit input_key(QWidget *parent = nullptr);
    ~input_key();

private:
    Ui::input_key *ui;
    string user;
    string password;
    void on_ok_button_clicked();

signals:
    void user_input(string user,string password);
    void user_quit();
};

#endif // INPUT_KEY_H

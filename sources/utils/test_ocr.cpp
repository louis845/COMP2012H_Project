#include "ocr_api.h"
#include <iostream>
#include <string>
#include <QCoreApplication>
using namespace std;

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    string filename;
    cout << "Input the relative path for the image for testing." << endl;
    cin >> filename;
    string app_id = "";     // left blank
    string app_key = "";    // left blank
    Ocr::GetInstance().set_app_id(app_id);
    Ocr::GetInstance().set_app_key(app_key);
    auto res = Ocr::GetInstance().Request(filename);
    cout << res.first << endl << res.second << endl;
    cin.get(); cin.get();
    return app.exec();
}
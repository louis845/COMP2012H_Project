#include "ocr_api.h"
#include <iostream>
#include <string>
#include <QCoreApplication>
using namespace std;

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    Ocr::getInstance().testSslSettings();

    string app_id = "";     // user input app_id
    string app_key = "";    // user input app_key

    Ocr::getInstance().setCredentials(app_id, app_key);    // set the credentials

    try { Ocr::getInstance().testConnectivity(); cout << "Connection: OK" << endl;}
    catch (const std::runtime_error& err) { cerr << err.what() << std::endl; }
    catch (...) { cerr << "Unhandled exception" << std::endl; }

    string img_path;        // relative path to the image file
    cout << "Input the relative path for the image for testing." << endl;
    cin >> img_path;

    auto result = Ocr::getInstance().request(img_path);     // pass the image path and get a pair of strings

    cout << result.first << endl;   // the first component is the LaTeX string
    cout << result.second << endl;  // the second component is AsciiMath

    cin.get(); cin.get();
    return app.exec();
}
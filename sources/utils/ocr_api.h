#ifndef OCR_H_
#define OCR_H_

#include <QCoreApplication>
#include <QtNetwork>
#include <QImage>
#include <iostream>
#include <string>
#include <utility>
using std::pair;
using std::string;
using std::cout;
using std::cin;


// Singleton class for OCR
// Use Ocr::GetInstance() to retrieve the unique instance
class Ocr final : public QObject
{
    Q_OBJECT

public:
    static Ocr& GetInstance()
    {
        static Ocr instance;
        return instance;
    }

    pair<string, string> Request(const string& img_path, const bool& using_latex=true);

    inline void set_app_key(const string &app_key)
    {
        this->app_key = app_key;
    }

    inline void set_app_id(const string &app_id)
    {
        this->app_id = app_id;
    }

    void TestSslSettings() const;

private slots:
    void OnFinish(QNetworkReply *reply);

private:
    const string API_URL = "https://api.mathpix.com/v3/text";

    Ocr();
    ~Ocr();
    Ocr(const Ocr&) { }
    Ocr& operator=(const Ocr&);

    QByteArray Img2Base64(QString img_path) const;
    QNetworkRequest InitRequest() const;
    QJsonObject InitJson(QByteArray img_base64, bool using_latex) const;
    QString ParseJson(QByteArray response) const;
    void Post(const string& img_path, const bool& using_latex);

    string app_key{""};
    string app_id{""};
    string result{""};
    // int status_code = 200;
    // QNetworkAccessManager network_mgr;
};

#endif /* OCR_H_ */
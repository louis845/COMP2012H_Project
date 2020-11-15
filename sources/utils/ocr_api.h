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
// Use Ocr::getInstance() to retrieve the unique instance
class Ocr final : public QObject
{
    Q_OBJECT

public:
    static Ocr& getInstance()
    {
        static Ocr instance;
        return instance;
    }

    pair<string, string> request(const string& img_path, const bool& using_latex=true);

    inline void set_app_key(const string &app_key)
    {
        this->app_key = app_key;
    }

    inline void set_app_id(const string &app_id)
    {
        this->app_id = app_id;
    }

    void testSslSettings() const;

private slots:
    void onFinish(QNetworkReply *reply);

private:
    const string API_URL = "https://api.mathpix.com/v3/text";

    Ocr();
    ~Ocr();
    Ocr(const Ocr&) { }
    Ocr& operator=(const Ocr&);

    QByteArray img2Base64(QString img_path) const;
    QNetworkRequest initRequest() const;
    QJsonObject initJson(QByteArray img_base64) const;
    pair<string, string> parseJson(QByteArray response) const;
    void post(const string& img_path);

    string app_key{""};
    string app_id{""};
    pair<string, string> result{"", ""};
    // int status_code = 200;
    // QNetworkAccessManager network_mgr;
};

#endif /* OCR_H_ */
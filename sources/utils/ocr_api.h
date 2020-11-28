#ifndef OCR_H_
#define OCR_H_

#include <QCoreApplication>
#include <QtNetwork>
#include <QImage>
#include <iostream>
#include <string>
#include <utility>


// Singleton class for OCR
// Use Ocr::getInstance() to retrieve the unique instance
class Ocr final
{
public:
    static Ocr& getInstance()
    {
        static Ocr instance;
        return instance;
    }

    // return LaTeX and AsciiMath strings as a pair
    std::pair<std::string, std::string> request(const std::string& img_path);

    inline void setCredentials(const std::string& api_id, const std::string &api_key) 
    { 
        this->api_id = api_id;
        this->api_key = api_key; 
    }

    void testSslSettings() const;

private:
    const std::string API_URL = "https://api.mathpix.com/v3/text";

    Ocr();
    ~Ocr();
    Ocr(const Ocr&) { }
    Ocr& operator=(const Ocr&);

    QByteArray img2Base64(QString img_path) const;
    QNetworkRequest initRequest() const;
    QJsonObject initJson(QByteArray img_base64) const;
    std::pair<std::string, std::string> parseJson(QByteArray response) const;
    void post(const std::string& img_path);

    std::string api_key{""};
    std::string api_id{""};
    std::pair<std::string, std::string> result{"", ""};
    // int status_code = 200;
    // QNetworkAccessManager network_mgr;
};

#endif /* OCR_H_ */
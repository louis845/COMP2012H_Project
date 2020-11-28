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
    std::pair<std::string, std::string> request(const std::string& img_path) const;
    std::pair<std::string, std::string> request(const QPixmap& pixmap) const;

    inline void setCredentials(const std::string& api_id, const std::string &api_key) 
    { 
        this->api_id = api_id;
        this->api_key = api_key; 
    }

    void testConnectivity() const;
    void testSslSettings() const;

private:
    const std::string API_URL = "https://api.mathpix.com/v3/text";

    Ocr() = default;
    ~Ocr() = default;
    Ocr(const Ocr&) = delete;
    Ocr& operator=(const Ocr&) = delete;

    QByteArray img2Base64(const QString& img_path) const;
    QByteArray img2Base64(const QPixmap& pixmap) const;
    QNetworkRequest initRequest() const;
    QJsonObject initJson(const QByteArray& img_base64) const;
    std::pair<std::string, std::string> parseJson(const QByteArray& response) const;
    QByteArray post(const QNetworkRequest& request, const QJsonObject& json) const;

    std::string api_key{""};
    std::string api_id{""};
};

#endif /* OCR_H_ */
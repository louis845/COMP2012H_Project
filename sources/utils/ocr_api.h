#ifndef OCR_H_
#define OCR_H_

#include <QCoreApplication>
#include <QtNetwork>
#include <QImage>
#include <iostream>
#include <string>
#include <utility>


// Class for OCR request, it retrieve an image from a path of accepts a QPixmap
// object, convert it into Base64 encoded strings, and post a http request to 
// the server of Mathpix Snip OCR API, retrieve and parse the response after that
// and finally return the OCR result, in LaTeX and AsciiMath formated strings
//
// Designed using singleton pattern, one can only retrieve the unique instance by 
// calling Ocr::getInstance(), since the whole application should only use one instance
// 
// EXAMPLE:
//          Ocr::getInstance().setCredentials("api_id", "api_key");
//          Ocr::getInstance().testConnectivity();
//          result = Ocr::getInstance().request("path/to/image.jpg");
class Ocr final
{
public:
    static Ocr& getInstance()
    {
        static Ocr instance;
        return instance;
    }

    // Return LaTeX and AsciiMath strings as a pair, first being LaTaX
    std::pair<std::string, std::string> request(const std::string& img_path) const;
    std::pair<std::string, std::string> request(const QPixmap& pixmap) const;

    // set credentials used for OCR API
    inline void setCredentials(const std::string& api_id, const std::string &api_key) 
    { 
        this->api_id = api_id;
        this->api_key = api_key; 
    }

    // Test the connection as well as authorization
    //
    // On success, no exception will be thrown. Otherwise, std::runtime_error
    // will be thrown with error message being HTTP error info, image corruption
    // or not found, unauthorization, etc. 
    void testConnectivity() const;

    // Only for testing, on some platforms and release Qt may have SSL issue
    void testSslSettings() const;

private:
    // Target server url
    const std::string API_URL = "https://api.mathpix.com/v3/text";

    Ocr() = default;
    ~Ocr() = default;
    Ocr(const Ocr&) = delete;
    Ocr& operator=(const Ocr&) = delete;

    // Convert image to Base64 encoded string
    // std::runtime_error will be thrown if no image can be found
    // at the specified path or if the image is corrupted
    QByteArray img2Base64(const QString& img_path) const;
    QByteArray img2Base64(const QPixmap& pixmap) const;

    // Initialize QNetworkRequest object and set the http header
    QNetworkRequest initRequest() const;

    // Construct a QJsonObject with given Base64
    QJsonObject initJson(const QByteArray& img_base64) const;

    // Parse the JSON in response and return a pair of LaTeX AsciiMath strings
    // If response does not contain a valid JSON field, std::runtime_error will be thrown
    std::pair<std::string, std::string> parseJson(const QByteArray& response) const;

    // Do the actual post, std::runtime_error will be thrown if HTTP errors occur
    // with corresponding HTTP status code
    QByteArray post(const QNetworkRequest& request, const QJsonObject& json) const;

    std::string api_key{""};
    std::string api_id{""};
};

#endif /* OCR_H_ */
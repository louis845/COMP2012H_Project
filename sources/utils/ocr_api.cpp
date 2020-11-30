#include "ocr_api.h"
#include <QPixmap>
using std::string;
using std::pair;
using std::to_string;


// Convert the image at given path to base64 encoding string
QByteArray Ocr::img2Base64(const QString& img_path) const
{
    QFile img_file(img_path);
    if (!img_file.exists() || !img_file.open(QIODevice::ReadOnly))
        throw std::runtime_error("image file is not existent or corrupted");

    QImage img(img_path);
    QByteArray ba;
    QBuffer buf(&ba);
    buf.open(QIODevice::WriteOnly);

    img.save(&buf, "JPEG");
    QByteArray img_base64 = ba.toBase64();
    buf.close();
    // std::cout << "base64 for img: " << img_base64.toStdString() << endl;
    return img_base64;
}


QByteArray Ocr::img2Base64(const QPixmap& pixmap) const
{
    QByteArray ba; 
    QBuffer buf(&ba); 
    buf.open(QIODevice::WriteOnly); 
    pixmap.save(&buf, "JPEG");
    
    QByteArray img_base64 = ba.toBase64();
    buf.close();
    return img_base64;
}


// Initialize the header of the request
QNetworkRequest Ocr::initRequest() const
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString::fromStdString(API_URL)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("app_id"), QByteArray::fromStdString(api_id));
    request.setRawHeader(QByteArray("app_key"), QByteArray::fromStdString(api_key));
    return request;
}


// Initialize the JSON field
QJsonObject Ocr::initJson(const QByteArray& img_base64) const
{
    QJsonObject json;
    // json.insert("src", "https://mathpix.com/examples/limit.jpg");
    json.insert("src", "data:image/jpeg;base64," + QString(img_base64));

    QJsonArray formats;
    formats.append("data");
    // formats.append("text");
    // formats.append("html");
    json.insert("formats", QJsonValue(formats));

    QJsonObject data_options;
    data_options.insert("include_latex", true);
    data_options.insert("include_asciimath", true);
    json.insert("data_options", QJsonValue(data_options));

    return json;
}


// parse the JSON field in the HTTP response
// refer to https://docs.mathpix.com/#result-object for the response structure
pair<string, string>Ocr::parseJson(const QByteArray& response) const
{
    QJsonParseError err;
    QJsonDocument json = QJsonDocument::fromJson(response, &err);
    if (err.error != QJsonParseError::NoError)
        throw std::runtime_error("response is not in a valid JSON format.");

    QJsonObject json_object = json.object();
    if (json_object.contains("error"))
        throw std::runtime_error(json_object.value("error").toString().toStdString());

    QString latex_output, asciimath_output;
    QJsonArray array = json_object.value("data").toArray();
    for (int i = 0; i < array.size(); ++i)  // may contain multiple lines of output
    {
        QJsonValue value = array.at(i).toObject().value("value");
        if (array.at(i).toObject().value("type") == "latex")
        {
            if (i != 0) latex_output += R"( \\ )";
            latex_output += value.toString();
        }
            
        else
        {
            if (i != 0) asciimath_output += R"( \ )";
            asciimath_output += value.toString();
        }
    }
    
    return {latex_output.toStdString(), asciimath_output.toStdString()};
}


QByteArray Ocr::post(const QNetworkRequest& request, const QJsonObject& json) const
{
    QNetworkAccessManager mgr;
    QNetworkReply* reply = mgr.post(request, QJsonDocument(json).toJson());
    
    QEventLoop event_loop;
    QObject::connect(&mgr, &QNetworkAccessManager::finished, &event_loop, &QEventLoop::quit);
    event_loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray response = reply->readAll();
        reply->deleteLater();
        return response;
    }
    else
    {
        int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply->deleteLater();
        if (status_code == 401) 
            throw std::runtime_error("401 Unauthorized, please check your API credentials");
        throw std::runtime_error("HTTP error: " + to_string(status_code));
    }
}


pair<string, string> Ocr::request(const string& img_path) const
{
    QByteArray img_base64 = img2Base64(QString::fromStdString(img_path));
    QNetworkRequest request = initRequest();
    QJsonObject json = initJson(img_base64);

    QByteArray response = post(request, json);
    return parseJson(response);
}


pair<string, string> Ocr::request(const QPixmap &img) const
{
    QByteArray img_base64 = img2Base64(img);
    QNetworkRequest request = initRequest();
    QJsonObject json = initJson(img_base64);

    QByteArray response = post(request, json);
    return parseJson(response);
}


void Ocr::testConnectivity() const
{
    QJsonObject json;
    json.insert("src", "https://mathpix.com/examples/limit.jpg");

    QJsonArray formats;
    formats.append("data");
    json.insert("formats", QJsonValue(formats));

    QJsonObject data_options;
    data_options.insert("include_latex", true);
    json.insert("data_options", QJsonValue(data_options));

    parseJson(post(initRequest(), json));
}


// Test the OpenSSL library
void Ocr::testSslSettings() const
{
    std::cout << QSslSocket::sslLibraryBuildVersionString().toStdString() << std::endl;
    std::cout << QSslSocket::supportsSsl() << std::endl;
    std::cout << QSslSocket::sslLibraryVersionString().toStdString() << std::endl;
}

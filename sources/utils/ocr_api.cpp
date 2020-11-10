#include "ocr_api.h"
using std::string;
using std::to_string;

Ocr::Ocr()
{
    // QObject::connect(&network_mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnFinish(QNetworkReply*)));
}

Ocr::~Ocr()
{
    // network_mgr.disconnect();
}


// Convert the image at given path to base64 encoding string
QByteArray Ocr::Img2Base64(QString img_path) const
{
    QFile img_file(img_path);
    if (!img_file.exists() || !img_file.open(QIODevice::ReadOnly))
        throw string("Image file is not existent or corrupted");

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


// Initialize the header of the request
QNetworkRequest Ocr::InitRequest() const
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString::fromStdString(API_URL)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("app_id"), QByteArray::fromStdString(app_id));
    request.setRawHeader(QByteArray("app_key"), QByteArray::fromStdString(app_key));
    return request;
}


// Initialize the JSON field
QJsonObject Ocr::InitJson(QByteArray img_base64, bool using_latex) const
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
    if (using_latex)
        data_options.insert("include_latex", true);
    else
        data_options.insert("include_asciimath", true);
    json.insert("data_options", QJsonValue(data_options));

    return json;
}


// parse the JSON field in the HTTP response
// refer to https://docs.mathpix.com/#result-object for the response structure
QString Ocr::ParseJson(QByteArray response) const
{
    QJsonParseError err;
    QJsonDocument json = QJsonDocument::fromJson(response, &err);
    if (err.error != QJsonParseError::NoError)
        throw string("Error: Response is not in a valid JSON format.");

    QJsonObject json_object = json.object();
    if (json_object.contains("error"))
        throw string("Error: ") + json_object.value("error").toString().toStdString();

    QString ocr_output;
    QJsonArray array = json_object.value("data").toArray();
    for (int i = 0; i < array.size(); ++i)  // may contain multiple lines of output
    {
        QJsonValue value = array.at(i).toObject().value("value");
        ocr_output += value.toString();
    }
    
    return ocr_output;
}


void Ocr::Post(const string& img_path, const bool& using_latex)
{
    QByteArray img_base64 = Img2Base64(QString::fromStdString(img_path));
    QNetworkRequest request = InitRequest();
    QJsonObject json = InitJson(img_base64, using_latex);

    // network_mgr.post(request, QJsonDocument(json).toJson());
}


void Ocr::OnFinish(QNetworkReply* reply)
{
    std::cout << "enter OnFinish\n";

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray response = reply->readAll();
        result = ParseJson(response).toStdString();
    }

    // status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->deleteLater();
    return;
}


// When using_latex = true (default), the output will be in LaTeX format
// Otherwise the output will be in asciiMath 
pair<string, string> Ocr::Request(const string& img_path, const bool& using_latex)
{
    try
    {
        QByteArray img_base64 = Img2Base64(QString::fromStdString(img_path));
        QNetworkRequest request = InitRequest();
        QJsonObject json = InitJson(img_base64, using_latex);

        QNetworkAccessManager mgr;
        QNetworkReply* reply = mgr.post(request, QJsonDocument(json).toJson());
        
        QEventLoop event_loop;
        QObject::connect(&mgr, QNetworkAccessManager::finished, &event_loop, QEventLoop::quit);
        event_loop.exec();

        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray response = reply->readAll();
            result = ParseJson(response).toStdString();
            reply->deleteLater();
            return {"Success", result};
        }
        else
        {
            int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            reply->deleteLater();
            return {"Error", "HTTP error: " + to_string(status_code)};
        }
    } 
    catch(string error)
    {
        return {"Error", error};
    }

    /********************
    try
    {
        Post(img_path, using_latex);
        return {"Success", result};
    }
    catch(string msg)
    {
        return {"Error", msg};
    }
    */
}


// Test the OpenSSL library
void Ocr::TestSslSettings() const
{
    std::cout << QSslSocket::sslLibraryBuildVersionString().toStdString() << std::endl;
    std::cout << QSslSocket::supportsSsl() << std::endl;
    std::cout << QSslSocket::sslLibraryVersionString().toStdString() << std::endl;
}

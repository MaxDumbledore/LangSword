#include "documenttranslatecore.h"

#include <QFile>
#include <QMimeDatabase>
#include <QUrlQuery>
#include <QHttpPart>
#include <QHttpMultiPart>

const QString DocumentTranslateCore::prefixUrl="https://fanyi-api.baidu.com/api/trans/vip/doctrans";

DocumentTranslateCore::DocumentTranslateCore(QString _from,QString _to):
    TranslateCore(_from,_to)
{
}

void DocumentTranslateCore::translate(QString  filePath)
{
    QUrl myUrl(prefixUrl);
    QUrlQuery query;
    query.addQueryItem("appid",appId);
    query.addQueryItem("from",from);
    query.addQueryItem("to",to);
    auto timestamp=QString::number(QDateTime::currentDateTime().toTime_t());
    query.addQueryItem("timestamp",timestamp);
    auto type=QMimeDatabase().mimeTypeForFile(filePath).preferredSuffix();
    query.addQueryItem("type",type);
    QByteArray temp;
    temp.append("appid="+appId+"&from="+from+"&timestamp="+timestamp
                +"&to="+to+"&type="+type+"&");
    qDebug()<<temp;
    auto file=new QFile(filePath);
    qDebug()<<file->open(QIODevice::ReadOnly);
    temp.append(QCryptographicHash::hash(file->readAll(),QCryptographicHash::Md5).toHex());
    temp.append(secretKey);
    QString sign=QCryptographicHash::hash(temp,QCryptographicHash::Md5).toHex();
    query.addQueryItem("sign",sign);

    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"file\";filename=\""+file->fileName()+"\";"));
    file->seek(0);
    part.setBodyDevice(file);
    auto multiPart=new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->append(part);
    myUrl.setQuery(query);
    qDebug()<<myUrl;
    auto reply=naManager->post(QNetworkRequest(myUrl),multiPart);
    file->setParent(multiPart);
    multiPart->setParent(reply);
    //connect(reply,&QNetworkReply:: uploadProgress,[&](qint64 a,qint64 b){qDebug()<<a<<b;});
}

void DocumentTranslateCore::callback(QNetworkReply *reply)
{
    TranslateCore::callback(reply);

    reply->deleteLater();
}

#include "translatecore.h"
#include <QString>
#include <QtNetwork>

const QString TranslateCore::appId="20210105000663879";
const QString TranslateCore::secretKey="N1gS5hAyCBukHXTeeVHP";

std::random_device TranslateCore::rd;
std::default_random_engine TranslateCore::rng(rd());
std::uniform_int_distribution<int> TranslateCore::myRand;

TranslateCore::TranslateCore(QString _from, QString _to):
    naManager(new QNetworkAccessManager(this)),
    from(_from),to(_to)
{
    connect(naManager,&QNetworkAccessManager::finished, this, &TranslateCore::callback);
}

QString TranslateCore::getFrom() const
{
    return from;
}

void TranslateCore::setFrom(const QString &value)
{
    from = value;
}

QString TranslateCore::getTo() const
{
    return to;
}

void TranslateCore::setTo(const QString &value)
{
    to = value;
}

void TranslateCore::callback(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();
    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
        qDebug() << "reason=" << reason.toString();
    if(reply->error() != QNetworkReply::NoError)
        qDebug() << "Failed: " << reply->errorString();
}

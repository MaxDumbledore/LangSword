#include "commontranslatecore.h"

#include <QUrlQuery>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

const QString CommonTranslateCore::prefixUrl="http://api.fanyi.baidu.com/api/trans/vip/translate";

CommonTranslateCore::CommonTranslateCore(QString _from,QString _to):
    TranslateCore(_from,_to),singleLineMode(false)
{
}

void CommonTranslateCore::translate(QString content)
{
    if(content.isEmpty())
    {
        emit resultReceived("");
        return;
    }
    if(singleLineMode)
    {
        content.replace('\n',' ');
        content.remove('\r');
    }
    QUrl myUrl(prefixUrl);
    QUrlQuery query;
    QByteArray temp;
    QString sign, salt=QString::number(myRand(rng));
    temp.append(appId+content+salt+secretKey);
    sign=QCryptographicHash::hash(temp,QCryptographicHash::Md5).toHex();
    query.addQueryItem("appid",appId);
    //query.addQueryItem("q",content);
    query.addQueryItem("from",from);
    query.addQueryItem("to",to);
    query.addQueryItem("salt",salt);
    query.addQueryItem("sign",sign);
    myUrl.setQuery(query.query()+"&q="+QUrl::toPercentEncoding(content));
    qDebug()<<myUrl;
    naManager->get(QNetworkRequest(myUrl));
}

void CommonTranslateCore::changeSingleLineMode()
{
    singleLineMode=!singleLineMode;
}

void CommonTranslateCore::callback(QNetworkReply *reply)
{
    TranslateCore::callback(reply);
    QJsonParseError jsonError;
    auto jsonResult=reply->readAll();
    qDebug()<<jsonResult;
    auto doc=QJsonDocument::fromJson(jsonResult,&jsonError);
    reply->deleteLater();
    if(doc.isNull()||jsonError.error!=QJsonParseError::NoError||!doc.isObject())
        return ;
    auto object=doc.object();
    auto list=object.value("trans_result").toArray();
    auto res=list.at(0).toObject().value("dst").toString();
    for(int i=1;i<list.size();i++)
        res+='\n'+list.at(i).toObject().value("dst").toString();
    emit resultReceived(res);
}

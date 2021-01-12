#include "translatecore.h"
#include <curl/curl.h>
#include <openssl/md5.h>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QUrl>

const char TranslateCore::prefixURL[]="http://api.fanyi.baidu.com/api/trans/vip/translate?";
const char TranslateCore::appId[]="20210105000663879";
const char TranslateCore::secretKey[]="N1gS5hAyCBukHXTeeVHP";

TranslateCore::TranslateCore():
    from("en"),to("zh"),
    rd(),rng(rd())
{
}

QString TranslateCore::translate(QString content)
{
    CURL *curl=curl_easy_init();
    if(curl==nullptr)
        return "";
    int salt=myRand(rng);
    char *myURL=new char [10000]();
    char *sign=new char [10000]();
    char *sSalt=new char [12]();
    strcpy(myURL,prefixURL);
    strcpy(sign,appId);
    strcat(sign,qPrintable(content));
    sprintf(sSalt,"%d",salt);
    strcat(sign,sSalt);
    strcat(sign,secretKey);
    unsigned char *md=new unsigned char [16];
    MD5((unsigned char *)sign,strlen(sign),md);
    char tmp[3]={'\0'};
    char *buf=new char[33]();
    for (int i = 0; i < 16; i++)
    {
        sprintf(tmp,"%2.2x",md[i]);
        strcat(buf,tmp);
    }
    strcat(myURL,"appid=");
    strcat(myURL,appId);
    strcat(myURL,"&q=");
    content=QUrl::toPercentEncoding(content);
    qDebug()<<content<<endl;
    strcat(myURL,qPrintable(content));
    strcat(myURL,"&from=");
    strcat(myURL,qPrintable(from));
    strcat(myURL,"&to=");
    strcat(myURL,qPrintable(to));
    strcat(myURL,"&salt=");
    strcat(myURL,sSalt);
    strcat(myURL,"&sign=");
    strcat(myURL,buf);
    qDebug()<<myURL<<endl;
    curl_easy_setopt(curl,CURLOPT_URL,myURL);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,cb);
    CurlMem chunk{nullptr,0};
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void *)&chunk);
    CURLcode res=curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    delete [] myURL;
    delete [] sign;
    delete [] sSalt;
    delete [] md;
    delete [] buf;
    if(res!=CURLE_OK)
    {
        delete [] chunk.response;
        return "";
    }
    QByteArray s(chunk.response);
    delete [] chunk.response;
    return getResultFromJson(s);
}

size_t TranslateCore::cb(void *data, size_t size, size_t nmemb, void *userp)
{
    auto realsize=size*nmemb;
    auto mem=(CurlMem *)userp;
    auto ptr=(char *)realloc(mem->response, mem->size+realsize+1);
    if(ptr==nullptr)
        return 0;
    mem->response=ptr;
    memcpy(&(mem->response[mem->size]),data,realsize);
    mem->size+=realsize;
    mem->response[mem->size]=0;
    return realsize;
}

QString TranslateCore::getResultFromJson(QByteArray s)
{
    QJsonParseError jsonError;
    QJsonDocument doc=QJsonDocument::fromJson(s,&jsonError);
    if(doc.isNull()||jsonError.error!=QJsonParseError::NoError||!doc.isObject())
        return "";
    QJsonObject object=doc.object();
    auto list=object.value("trans_result").toArray();
    auto res=list.at(0).toObject().value("dst").toString();
    for(int i=1;i<list.size();i++)
        res+='\n'+list.at(i).toObject().value("dst").toString();
    return res;
}

#include "languageconstants.h"

const QString LanguageConstants::defaultSourceLanguage=QObject::tr("English");
const QString LanguageConstants::defaultTargetLanguage=QObject::tr("Chinese");
const QString LanguageConstants::langAuto=tr("Auto-detection");

QMap<QString,QString> LanguageConstants::languageToCode;
QList<QString> LanguageConstants::sourceLanguages;
QList<QString> LanguageConstants::targetLanguages;

const QMap<QString, QString> &LanguageConstants::getLanguageMap()
{
    if(!languageToCode.empty())
        return languageToCode;
    languageToCode[tr("Auto-detection")]="auto";
    languageToCode[tr("Chinese")]="zh";
    languageToCode[tr("English")]="en";
    languageToCode[tr("Cantonese")]="yue";
    languageToCode[tr("Classical Chinese")]="wyw";
    languageToCode[tr("Japanese")]="jp";
    languageToCode[tr("Korean")]="kor";
    languageToCode[tr("French")]="fra";
    languageToCode[tr("Spanish")]="spa";
    languageToCode[tr("Thai")]="th";
    languageToCode[tr("Arabic")]="ara";
    languageToCode[tr("Russian")]="ru";
    languageToCode[tr("Portuguese")]="pt";
    languageToCode[tr("German")]="de";
    languageToCode[tr("Italian")]="it";
    languageToCode[tr("Greek")]="el";
    languageToCode[tr("Dutch")]="nl";
    languageToCode[tr("Polish")]="pl";
    languageToCode[tr("Bulgarian")]="bul";
    languageToCode[tr("Estonian")]="est";
    languageToCode[tr("Danish")]="dan";
    languageToCode[tr("Finland")]="fin";
    languageToCode[tr("Czech")]="cs";
    languageToCode[tr("Romanian")]="rom";
    languageToCode[tr("Slovenian")]="slo";
    languageToCode[tr("Swedish")]="swe";
    languageToCode[tr("Hungarian")]="hu";
    languageToCode[tr("Traditional Chinese")]="cht";
    languageToCode[tr("Vietnamese")]="vie";
    return languageToCode;
}

const QList<QString> &LanguageConstants::getSourceLanguages()
{
    if(!sourceLanguages.empty())
        return sourceLanguages;
    sourceLanguages=getLanguageMap().keys();
    sourceLanguages.removeOne(langAuto);
    sourceLanguages.insert(0,langAuto);
    return sourceLanguages;
}

const QList<QString> &LanguageConstants::getTargetLanguages()
{
    if(!targetLanguages.empty())
        return targetLanguages;
    targetLanguages=getLanguageMap().keys();
    targetLanguages.removeOne(langAuto);
    return targetLanguages;
}

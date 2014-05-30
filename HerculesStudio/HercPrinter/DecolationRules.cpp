#include "DecolationRules.h"
#include "Preferences.h"
#include "HerculesStudio.h"


DecolationRules::DecolationRules() :
    mSeperator(), mAccounting()
{

}

const QString& DecolationRules::name()
{
    return mName;
}

void DecolationRules::setName(const QString &name)
{
    mName = name;
}

QString DecolationRules::filenameTemplate()
{
    return mFileNameTemplate;
}

void DecolationRules::setFileNameTemplate(const QString& name)
{
    mFileNameTemplate = name;
}

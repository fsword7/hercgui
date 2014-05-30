#include "Stationery.h"
#include "HerculesStudio.h"

static const char *paperSizeNames[] =
{
        "Letter",
        "Legal",
        "Ledger/Tabloid",
        "Letter FanFold",
        "Legal FanFold",
        "Std FanFold",
        "A0",
        "A1",
        "A2",
        "A3",
        "A4",
        "A5",
        "Custom"
};

const char *Stationery::mBarsColorNames[] =
{
    "Green",
    "Gray",
    "Light Blue",
    "None",
    "Custom"
};


const int barsColors[][3] =
{
    {0xdd, 0xff, 0xdd}, // Green
    {0xd2, 0xd2, 0xd2}, // Gray
    {0xe0, 0xf0, 0xf0}, // Light Blue
    {0xff, 0xff, 0xff}  // None
};

Stationery::Stationery(const QString& name, const QString& paperSize, const QString& size1, const QString& size2,
                       int topMargin, bool millimeters, bool portrait, const QString& barsColor,
                       const QString& barsRGB, bool Decorated, bool tractorFeedHoles) :
    mName(name),
    mPaperSize(paperSize),
    mSize1(size1),
    mSize2(size2),
    mTopMargin(topMargin),
    mMillimeters(millimeters),
    mPortrait(portrait),
    mBarsColor(barsColor),
    mBarsRGB(barsRGB),
    mDecorated(Decorated),
    mTractorFeedHoles(tractorFeedHoles)
{
}

unsigned int Stationery::paperSizes()
{
    return sizeof(paperSizeNames)/sizeof(char *);
}

const char *Stationery::paperSize(unsigned int i)
{
    return paperSizeNames[i];
}


int Stationery::getBarsColorIndex()
{
    for (int i=0; i<4; i++)
    {
        if (mBarsColor.compare(mBarsColorNames[i]) == 0)
            return i;
    }
    return -1;
}

int * Stationery::getBarsColor(int *fill)
{
    if (mBarsColor.compare("Custom") == 0)
    {
        QString hex = QString("0x") + mBarsRGB.right(6);
        int color = hex.toInt(0,16);
        fill[0] = color >> 16;
        fill[1] = (color >> 8) & 0xff;
        fill[2] = color & 0xff;
        return fill;
    }
    int index = getBarsColorIndex();
    memcpy(fill, barsColors[index], 3*sizeof(int));
    return fill;
}

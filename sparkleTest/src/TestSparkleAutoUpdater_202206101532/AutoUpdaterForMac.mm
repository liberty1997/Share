#include <QDebug>

#include <Sparkle/Sparkle.h>
#include <Sparkle/SPUUpdater.h>

#include "AutoUpdaterForMac.h"

class SparkleAutoUpdater::SparkleAutoUpdaterPrivate
{
public:
    SUUpdater *updater;
    //SPUUpdater *updater;
    //SPUStandardUpdaterController *updater;
};

SparkleAutoUpdater::SparkleAutoUpdater(const QString& feedUrl)
{
    d = new SparkleAutoUpdaterPrivate;
    d->updater = [[SUUpdater sharedUpdater] retain];
    NSURL *url = [NSURL URLWithString:
            [NSString stringWithUTF8String: feedUrl.toUtf8().data()]];
    [d->updater setFeedURL: url];
}

SparkleAutoUpdater::~SparkleAutoUpdater()
{
    [d->updater release];
    delete d;
}

void SparkleAutoUpdater::checkForUpdates()
{
    [d->updater checkForUpdatesInBackground];
    qDebug() << __FUNCTION__;
}

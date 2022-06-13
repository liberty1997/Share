#include <QDebug>

#include <Sparkle/Sparkle.h>
//#include <Sparkle/SPUUpdater.h>

#include "AutoUpdaterForMac.h"

class SparkleAutoUpdater::SparkleAutoUpdaterPrivate
{
public:
    //SUUpdater *updater;
    SPUUpdater *updater;
    //SPUStandardUpdaterController *updater;
};

SparkleAutoUpdater::SparkleAutoUpdater(const QString& feedUrl)
{
    d = new SparkleAutoUpdaterPrivate;
    //d->updater = [[SUUpdater sharedUpdater] retain];
    NSBundle *hostBundle = [NSBundle mainBundle];
    SPUStandardUserDriver *userDriver = [[SPUStandardUserDriver alloc] initWithHostBundle:hostBundle delegate:nil];
    d->updater = [[SPUUpdater alloc] initWithHostBundle:hostBundle applicationBundle:hostBundle userDriver:userDriver delegate:nil];
    NSURL *url = [NSURL URLWithString:
            [NSString stringWithUTF8String: feedUrl.toUtf8().data()]];
    [d->updater setFeedURL: url];
    [d->updater setAutomaticallyChecksForUpdates: false];

    NSError *updaterError = nil;
    [d->updater startUpdater:(&updaterError)];
}

SparkleAutoUpdater::~SparkleAutoUpdater()
{
    [d->updater release];
    delete d;
}

void SparkleAutoUpdater::checkForUpdates()
{
    //[d->updater checkForUpdatesInBackground];

    //[d->updater checkForUpdates:(nil)];
    [d->updater checkForUpdates];
    qDebug() << __FUNCTION__;
}

#ifndef AUTOUPDATERFORMAC_H
#define AUTOUPDATERFORMAC_H

#include <QString>

class SparkleAutoUpdater
{
public:
    SparkleAutoUpdater(const QString& feedUrl);
    ~SparkleAutoUpdater();
    void checkForUpdates();

private:
    class SparkleAutoUpdaterPrivate;
    SparkleAutoUpdaterPrivate *d;
};

#endif // AUTOUPDATERFORMAC_H

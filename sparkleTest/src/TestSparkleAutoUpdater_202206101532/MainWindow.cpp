#include "ui_MainWindow.h"

#include "AutoUpdaterForMac.h"

#include "MainWindow.h"


static const char *APP_CUR_VERSION = "2.2.2";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //updater = new SparkleAutoUpdater("https://github.com/liberty1997/Share/blob/master/sparkleTest/appcast.xml");
    updater = new SparkleAutoUpdater("https://raw.githubusercontent.com/liberty1997/Share/master/sparkleTest/appcast.xml");
}

MainWindow::~MainWindow()
{
    if(updater) {
        delete updater;
        updater = nullptr;
    }

    delete ui;
}


void MainWindow::on_pushButton_clicked(bool checked)
{
    ui->label->setText(checked ? APP_CUR_VERSION : "");
}


void MainWindow::on_pushButton_2_clicked()
{
    if(!updater) {
        qDebug() << "err: nullptr. Failed to init UPDATER.";
        return;
    }

    updater->checkForUpdates();

    /*if(!updater) {
        updater = new SparkleAutoUpdater("https://github.com/liberty1997/Share/blob/master/sparkleTest/appcast.xml");
    }

    updater->checkForUpdates();*/
}

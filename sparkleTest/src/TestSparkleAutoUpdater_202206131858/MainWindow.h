#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SparkleAutoUpdater;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked(bool checked);
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    SparkleAutoUpdater *updater;
};
#endif // MAINWINDOW_H

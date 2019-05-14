#ifndef WINDOW_H
#define WINDOW_H

#include "ui_untitled.h"

class Window : public QWidget, private Ui::Form {
    Q_OBJECT

public:
    explicit Window(QWidget * parent = nullptr);
private:
    QStringList fileNames;
private slots:
    void on_buttonSelect_clicked();
    void on_buttonDeskewing_clicked();
    void on_buttonRun_clicked();
};

#endif // WINDOW_H

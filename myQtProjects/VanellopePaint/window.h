#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QPushButton>
#include <QColorDialog>
#include "renderarea.h"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();

private slots:
    void shapeChanged();
    void penChanged();
    void brushChanged();

private:
    RenderArea* renderArea;
    QLabel* shapeLabel;
    QLabel* penColorLabel;
    QLabel* penWidthLabel;
    QLabel* penStyleLabel;
    QLabel* penCapLabel;
    QLabel* penJoinLabel;
    QLabel* brushStyleLabel;
    QComboBox* shapeComboBox;
    QPushButton* penColorPushButton;
    QColorDialog* colorDialog;
    QSlider* penWidthSlider;
    QComboBox* penStyleComboBox;
    QComboBox* penCapComboBox;
    QComboBox* penJoinComboBox;
    QComboBox* brushStyleComboBox;
    // Eraser ?
};
#endif // WINDOW_H

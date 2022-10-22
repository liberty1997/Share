#include "MyGraphicsView.h"

#include <QGraphicsScene>
#include <QMouseEvent>

MyGraphicsView::MyGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setMouseTracking(true);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    auto viewPos = event->pos();
    emit mouseMove(viewPos, this->mapToScene(viewPos));
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Equal || event->key() == Qt::Key_Plus) {
        scale(1.2, 1.2);
    }

    if(event->key() == Qt::Key_Minus) {
        scale(0.8, 0.8);
    }
}

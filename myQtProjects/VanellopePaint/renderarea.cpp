#include "renderarea.h"
#include <QPainter>
#include <QColor>
#include <QRect>

RenderArea::RenderArea(QWidget* parent) : QWidget(parent)
{
    shape = Shape::Line;

    setBackgroundRole((QPalette::Base)); // to render the background to white
    setAutoFillBackground(true);
}

QSize RenderArea::sizeHint() const
{
    return QSize(900, 480);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(240, 240);
}

void RenderArea::setShape(Shape shape)
{
    this->shape = shape;
    //update();
}

void RenderArea::setPen(const QPen &pen)
{
    this->pen = pen;
    //update();
}

void RenderArea::setBrush(const QBrush &brush)
{
    this->brush = brush;
    //update();
}

//void RenderArea::mousePressEvent(QMouseEvent* event)
//{
////    currentPoint = event->pos();
////    pointVector.push_back(currentPoint);
//    pointVector.clear();
//}

void RenderArea::mouseMoveEvent(QMouseEvent* event)
{
    currentPoint = event->pos();
    pointVector.push_back(currentPoint);

    update();
    //painterIndex = pointVector.size();
}

void RenderArea::mouseReleaseEvent(QMouseEvent* event)
{
//    currentPoint = event->pos();
//    pointVector.push_back(currentPoint);
    pointVector.clear();
}

void RenderArea::paintEvent(QPaintEvent* event)
{
    // 把此次mouseMove时捕获的点组成QLine存入QVector
    for(int i = 1; i < pointVector.size(); ++i)
    {
        MyLine myLine(QLine(pointVector.at(i - 1), pointVector.at(i)),
                      QPen(pen));
        lineVector.push_back(myLine);
    }

    QPainter painter(this);
//    painter.setPen(pen);
//    painter.setBrush(brush);

    // 在QWidget上面画时，每次会清除QWidget，所以得把所有line都存下来，
    // 然后每次画一遍所有line
    for(QVector<MyLine>::const_iterator iter = lineVector.constBegin(); iter != lineVector.constEnd(); ++iter)
    {
        painter.setPen(iter->pen);
        painter.drawLine(iter->line);
    }

//    if(pointVector.size() > painterIndex)
//        painterIndex = pointVector.size();


//    if(pointVector.size() == 1)
//        painter.drawPoint(pointVector.at(0));
//    else if(pointVector.size() > 1)
//    {
//        for(int i = 1; i < pointVector.size(); ++i)
//        {
//            painter.drawLine(pointVector.at(i - 1), pointVector.at(i));
//            //painter.drawPoint(i);
//        }
//    }
}

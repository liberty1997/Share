#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QPen>
#include <QLine>
#include <QQueue>
#include <QSize>
#include <QVector>
#include <QPainter>

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    enum Shape {Point, Line, Rect};

    RenderArea(QWidget* parent = 0);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setShape(Shape shape);
    void setPen(const QPen& pen);
    void setBrush(const QBrush& brush);

protected:
    //void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    Shape shape;
    QPen pen;
    QBrush brush;

    QPoint currentPoint;
    QVector<QPoint> pointVector;
    struct MyLine
    {
        MyLine(QLine l, QPen p) : line(l), pen(p)
        {
        }

        MyLine()
        {
            line = QLine(); // "Constructs a null line."
            pen = QPen(); // "Constructs a default black solid line pen with 1 width."
        }

        QLine line;
        QPen pen;
    };

    QVector<MyLine> lineVector;
};

#endif // RENDERAREA_H

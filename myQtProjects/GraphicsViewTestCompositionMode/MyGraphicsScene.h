#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>

class MyGraphicsScene : public QGraphicsScene
{
public:
    MyGraphicsScene(QObject *parent = nullptr);
    void setCount(int count);
    int getCount() const { return m_count; }

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    int m_count { 0 };
};

#endif // MYGRAPHICSSCENE_H

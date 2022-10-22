#include "MyGraphicsScene.h"

#include <QDebug>
#include <QPainter>
#include <QPixmap>

MyGraphicsScene::MyGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

void MyGraphicsScene::setCount(int count)
{
    m_count = count;
    update();
}

void MyGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);

    /* CompositionMode 即
     * 有三张图： src、dst、ret
     * 在ret中先以QPainter::CompositionMode_Source画transparent，
     * 再在ret中以QPainter::CompositionMode_SourceOver画dst，
     * 再在ret中以某种CompositionMode画src，
     * 最后呈现在ret中的图即为Composition后的结果
     */

    // 画一个src图
    QPixmap pixmapSrc(QSize(300, 300));
    pixmapSrc.fill(Qt::transparent);
    QPainter p(&pixmapSrc);
    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(QColor(150, 175, 239));
    p.drawRect(QRect(50, 50, 200, 200));
    p.end();

    // 准备dst图
    QPixmap pixmapDst(QSize(300, 300));
    pixmapDst.fill(Qt::transparent);
    p.begin(&pixmapDst);
    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing);
    // 画dst图的图形
    p.setBrush(QColor(241, 201, 112));
    p.drawEllipse(QPointF(50, 50), 50, 50);

    // 再以某种CompositionMode把src图画到dst上
    p.setCompositionMode(QPainter::CompositionMode_SourceOut);
    p.drawPixmap(pixmapDst.rect(), pixmapSrc);
    p.end();

    // 呈现dst图，亦即最终结果
    painter->drawPixmap(QRect(-100, -100, 300, 200), pixmapDst);

    qDebug() << "exposed rect: " << rect;
    qDebug() << "scene rect: " << sceneRect();

    // -- 画个甜甜圈
    QPen    pen;
    pen.setWidth(1); //线宽
    pen.setColor(Qt::red); //划线颜色
    pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等
    pen.setCapStyle(Qt::FlatCap);//线端点样式
    pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
    painter->setPen(pen);
    //线性渐变
    QLinearGradient linearGrad(0, 0, 100, 0);//从左到右,
    linearGrad.setColorAt(0, Qt::yellow);//起点颜色
    linearGrad.setColorAt(1, Qt::green);//终点颜色
    linearGrad.setSpread(QGradient::PadSpread);  //展布模式
    painter->setBrush(linearGrad);
    //设置复合模式
    painter->setCompositionMode(QPainter::CompositionMode_Difference);
    for (int i = 0; i < m_count; i++)
    {
        painter->drawEllipse(QPoint(50, 0), 50, 50);
        painter->rotate(10);
    }
}

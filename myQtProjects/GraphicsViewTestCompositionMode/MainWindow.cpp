#include "ui_MainWindow.h"

#include "MyGraphicsScene.h"
#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->graphicsView, &MyGraphicsView::mouseMove,
            this, [this](QPoint viewPos, QPointF scenePos) {
        ui->label_2->setText(QString("%1, %2").arg(viewPos.x()).arg(viewPos.y()));
        ui->label_2->adjustSize();
        ui->label_4->setText(QString("%1, %2").arg(scenePos.x()).arg(scenePos.y()));
        ui->label_4->adjustSize();
    });

    m_scene = new MyGraphicsScene(ui->graphicsView);
    m_scene->setSceneRect(QRectF(-100, -100, 200, 200));
    ui->graphicsView->setScene(m_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
//    auto view = new MyGraphicsView;
//    view->setScene(m_scene);
//    view->show();
    m_scene->setCount(m_scene->getCount() + 1);
}


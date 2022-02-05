#include "window.h"
#include <QGridLayout>
#include <QString>
#include <QColor>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QConicalGradient>

/*  "it is a good programming practice to give each role their own name" */
const int IdRole = Qt::ItemDataRole::UserRole;

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    renderArea = new RenderArea;

    shapeLabel = new QLabel(tr("形状："));
    shapeComboBox = new QComboBox;
    shapeComboBox->addItem(tr("点"), RenderArea::Shape::Point);
    shapeComboBox->addItem(tr("线"), RenderArea::Shape::Line);
    shapeComboBox->addItem(tr("矩形"), RenderArea::Shape::Rect);

    penColorLabel = new QLabel(tr("颜色："));
    penColorPushButton = new QPushButton(QString("选色"), this);
    colorDialog = new QColorDialog(Qt::black);
    colorDialog->setVisible(false);

    penWidthLabel = new QLabel(tr("粗细："));
    penWidthSlider = new QSlider(this);
    penWidthSlider->setOrientation(Qt::Horizontal);
    penWidthSlider->setMinimum(1);
    penWidthSlider->setMaximum(20);
    penWidthSlider->setSingleStep(1);

    penStyleLabel = new QLabel(tr("画笔样式："));
    penStyleComboBox = new QComboBox;
    penStyleComboBox->addItem(tr("Solid"), static_cast<int>(Qt::SolidLine));
    penStyleComboBox->addItem(tr("Dash"), static_cast<int>(Qt::DashLine));
    penStyleComboBox->addItem(tr("Dot"), static_cast<int>(Qt::DotLine));
    penStyleComboBox->addItem(tr("Dash Dot"), static_cast<int>(Qt::DashDotLine));
    penStyleComboBox->addItem(tr("Dash Dot Dot"), static_cast<int>(Qt::DashDotDotLine));
    penStyleComboBox->addItem(tr("None"), static_cast<int>(Qt::NoPen));

    penCapLabel = new QLabel(tr("Pen Cap："));
    penCapComboBox = new QComboBox;
    penCapComboBox->addItem(tr("Flat"), Qt::FlatCap);
    penCapComboBox->addItem(tr("Square"), Qt::SquareCap);
    penCapComboBox->addItem(tr("Round"), Qt::RoundCap);

    penJoinLabel = new QLabel(tr("Pen Join："));
    penJoinComboBox = new QComboBox;
    penJoinComboBox->addItem(tr("Miter"), Qt::MiterJoin);
    penJoinComboBox->addItem(tr("Bevel"), Qt::BevelJoin);
    penJoinComboBox->addItem(tr("Round"), Qt::RoundJoin);

    brushStyleLabel = new QLabel(tr("Brush："));
    brushStyleComboBox = new QComboBox;
    brushStyleComboBox->addItem(tr("Linear Gradient"), static_cast<int>(Qt::LinearGradientPattern));
    brushStyleComboBox->addItem(tr("Radial Gradient"), static_cast<int>(Qt::RadialGradientPattern));
    brushStyleComboBox->addItem(tr("Conical Gradient"), static_cast<int>(Qt::ConicalGradientPattern));
    brushStyleComboBox->addItem(tr("Texture"), static_cast<int>(Qt::TexturePattern));
    brushStyleComboBox->addItem(tr("Solid"), static_cast<int>(Qt::SolidPattern));
    brushStyleComboBox->addItem(tr("Horizontal"), static_cast<int>(Qt::HorPattern));
    brushStyleComboBox->addItem(tr("Vertical"), static_cast<int>(Qt::VerPattern));
    brushStyleComboBox->addItem(tr("Cross"), static_cast<int>(Qt::CrossPattern));
    brushStyleComboBox->addItem(tr("Backward Diagonal"), static_cast<int>(Qt::BDiagPattern));
    brushStyleComboBox->addItem(tr("Forward Diagonal"), static_cast<int>(Qt::FDiagPattern));
    brushStyleComboBox->addItem(tr("Diagonal Cross"), static_cast<int>(Qt::DiagCrossPattern));
    brushStyleComboBox->addItem(tr("Dense 1"), static_cast<int>(Qt::Dense1Pattern));
    brushStyleComboBox->addItem(tr("Dense 2"), static_cast<int>(Qt::Dense2Pattern));
    brushStyleComboBox->addItem(tr("Dense 3"), static_cast<int>(Qt::Dense3Pattern));
    brushStyleComboBox->addItem(tr("Dense 4"), static_cast<int>(Qt::Dense4Pattern));
    brushStyleComboBox->addItem(tr("Dense 5"), static_cast<int>(Qt::Dense5Pattern));
    brushStyleComboBox->addItem(tr("Dense 6"), static_cast<int>(Qt::Dense6Pattern));
    brushStyleComboBox->addItem(tr("Dense 7"), static_cast<int>(Qt::Dense7Pattern));
    brushStyleComboBox->addItem(tr("None"), static_cast<int>(Qt::NoBrush));

    connect(shapeComboBox, SIGNAL(activated(int)), this, SLOT(shapeChanged()));
    connect(penColorPushButton, SIGNAL(clicked()), colorDialog, SLOT(show()));
    connect(colorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(penChanged()));
    connect(penWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(penChanged()));
    connect(penStyleComboBox, SIGNAL(activated(int)), this, SLOT(penChanged()));
    connect(penCapComboBox, SIGNAL(activated(int)), this, SLOT(penChanged()));
    connect(penJoinComboBox, SIGNAL(activated(int)), this, SLOT(penChanged()));
    connect(brushStyleComboBox, SIGNAL(activated(int)), this, SLOT(brushChanged()));

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    mainLayout->addWidget(shapeLabel, 2, 0, Qt::AlignRight);
    mainLayout->addWidget(shapeComboBox, 2, 1);
    mainLayout->addWidget(penColorLabel, 3, 0, Qt::AlignRight);
    mainLayout->addWidget(penColorPushButton, 3, 1);
    mainLayout->addWidget(penWidthLabel, 4, 0, Qt::AlignRight);
    mainLayout->addWidget(penWidthSlider, 4, 1);
    mainLayout->addWidget(penStyleLabel, 5, 0, Qt::AlignRight);
    mainLayout->addWidget(penStyleComboBox, 5, 1);
    mainLayout->addWidget(penCapLabel, 6, 0, Qt::AlignRight);
    mainLayout->addWidget(penCapComboBox, 6, 1);
    mainLayout->addWidget(penJoinLabel, 7, 0, Qt::AlignRight);
    mainLayout->addWidget(penJoinComboBox, 7, 1);
    mainLayout->addWidget(brushStyleLabel, 8, 0, Qt::AlignRight);
    mainLayout->addWidget(brushStyleComboBox, 8, 1);
    setLayout(mainLayout);

    void shapeChanged();
    void penChanged();
    void brushChanged();

    setWindowTitle(tr("VanellopePaint (Derived From \"Basic Drawing Example\")"));
}

Window::~Window()
{
}

void Window::shapeChanged()
{
    RenderArea::Shape shape = RenderArea::Shape(shapeComboBox->itemData(shapeComboBox->currentIndex(), IdRole).toInt());

    renderArea->setShape(shape);
}

void Window::penChanged()
{
    int width = penWidthSlider->value();
    QColor color = colorDialog->currentColor();
    Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(penStyleComboBox->currentIndex(), IdRole).toInt());
    Qt::PenCapStyle cap = Qt::PenCapStyle(penCapComboBox->itemData(penCapComboBox->currentIndex(), IdRole).toInt());
    Qt::PenJoinStyle join = Qt::PenJoinStyle(penJoinComboBox->itemData(penJoinComboBox->currentIndex(), IdRole).toInt());

    renderArea->setPen(QPen(color, width, style, cap, join));
}

void Window::brushChanged()
{
    Qt::BrushStyle style = Qt::BrushStyle(brushStyleComboBox->itemData(brushStyleComboBox->currentIndex(), IdRole).toInt());
    if(style == Qt::LinearGradientPattern)
    {
        QLinearGradient linearGradient(0, 0, 100, 100);
        linearGradient.setColorAt(0.0, Qt::white);
        linearGradient.setColorAt(0.2, Qt::yellow);
        linearGradient.setColorAt(1.0, Qt::red);
        renderArea->setBrush(linearGradient);
    }
    else if(style == Qt::RadialGradientPattern)
    {
        QRadialGradient radialGradient(50, 50, 50, 70, 70);
        radialGradient.setColorAt(0.0, Qt::white);
        radialGradient.setColorAt(0.2, Qt::green);
        radialGradient.setColorAt(1.0, Qt::black);
        renderArea->setBrush(radialGradient);
    }
    else if (style == Qt::ConicalGradientPattern)
    {
        QConicalGradient conicalGradient(50, 50, 150);
        conicalGradient.setColorAt(0.0, Qt::white);
        conicalGradient.setColorAt(0.2, Qt::green);
        conicalGradient.setColorAt(1.0, Qt::black);
        renderArea->setBrush(conicalGradient);
    }
    /*
    else if(style == Qt::TexturePattern)
    {
        renderArea->setBrush(QBrush(QPixmap(":/images/brick.png")));
    }
    */
    else
    {
        renderArea->setBrush((QBrush(Qt::blue, style)));
    }
}










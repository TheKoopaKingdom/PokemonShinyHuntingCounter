#include <QPainter>

#include "counter.h"
#include "ui_Counter.h"
#include "about.h"
#include "gradient.h"

Counter::Counter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Counter)
{
    //set up ui
    ui->setupUi(this);

    //set window icon and title
    this->setWindowIcon(QIcon(":/icon.png"));
    this->setWindowTitle("Shiny Hunting Counter");

    //set maximum spinbox
    //afaik, 2147483647 is the largest 32-bit number
    ui->encounterBox->setMaximum(2147483647);
    ui->fontSize->setMaximum(2147483647);
    ui->outlineWidth->setMaximum(2147483647);
    ui->xBox->setMaximum(2147483647);
    ui->yBox->setMaximum(2147483647);
    ui->gradTransPoint->setMaximum(2147483647);

    //set default values
    ui->fontSize->setValue(30);
    ui->xBox->setValue(50);
    ui->yBox->setValue(50);
    ui->outlineWidth->setValue(2);
    solidColor = Qt::black;

    //reset count
    count = 0;
}

Counter::~Counter()
{
    delete ui;
}

//painting
void Counter::paintEvent(QPaintEvent *event)
{
    //make the canvas
    QPainter canvas(this);

    //turn on anti-aliasing (higher quality)
    canvas.setRenderHints(QPainter::HighQualityAntialiasing);

    //grab font info
    QFont font = ui->font->currentText();
    font.setPointSize(ui->fontSize->value());

    //make the text path
    QPainterPath textPath;
    textPath.addText(x, y, font, QString::number(count));

    //draw the text
    canvas.drawPath(textPath);

    //gradient
    if(ui->gradientBox->isChecked())
    {
        //fill in the text using the pattern
        canvas.fillPath(textPath, makeGradient(x, y, ui->gradTransPoint->value(), gradColor1, gradColor2));
    }

    //default to solid color
    else
    {
        //make pattern based off of solid color (default is black)
        QBrush pattern(solidColor);

        //fill in the text using the pattern
        canvas.fillPath(textPath, pattern);
    }

    //outline
    if (ui->useOutline->isChecked())
    {
        //make stroke path
        QPainterPathStroker outlineStrokePath;
        //set width of stroke path
        outlineStrokePath.setWidth(ui->outlineWidth->value());
        //make path based off of stroke path
        QPainterPath outline(outlineStrokePath.createStroke(textPath));

        //use outside stroke instead of center
        QPainterPath finalStroke = outline.subtracted(textPath);

        //grab color for outline
        QBrush brush(outlineColor);

        //fill in the path
        canvas.fillPath(finalStroke, brush);
    }
}

//about button
void Counter::on_aboutButton_clicked()
{
    //create about window
    About *aboutWindow = new About();

    //show
    aboutWindow->show();
}
#include "roundprocessbar.h"
#include <QDebug>

RoundProcessBar::RoundProcessBar(QWidget *parent)
    :QWidget(parent)
    ,m_value(0.0)
    ,m_startAngle(90)
    ,m_max(100)
{
    //设置默认外圈宽度
    setOuterWide(18);
    //设置默认内圈宽度
    setInnerWide(16);
    //设置外圈颜色
    setOuterColor(QColor(233,248,248));
    //设置内圈颜色
    setInnerColor(QColor(133, 243, 244));
    //设置文字颜色
    setTextColor(QColor(49,177,190));
}

RoundProcessBar::~RoundProcessBar()
{

}

double RoundProcessBar::outerWide() const
{
    return m_outerWide;
}

void RoundProcessBar::setOuterWide(double newOuterWide)
{
    m_outerWide = newOuterWide;
}

double RoundProcessBar::innerWide() const
{
    return m_innerWide;
}

void RoundProcessBar::setInnerWide(double newInnerWide)
{
    m_innerWide = newInnerWide;
}

const QColor &RoundProcessBar::outerColor() const
{
    return m_outerColor;
}

void RoundProcessBar::setOuterColor(const QColor &newOuterColor)
{
    m_outerColor = newOuterColor;
}

const QColor &RoundProcessBar::innerColor() const
{
    return m_innerColor;
}

void RoundProcessBar::setInnerColor(const QColor &newInnerColor)
{
    m_innerColor = newInnerColor;
}

const QColor &RoundProcessBar::textColor() const
{
    return m_textColor;
}

void RoundProcessBar::setTextColor(const QColor &newTextColor)
{
    m_textColor = newTextColor;
}

double RoundProcessBar::value() const
{
    return m_value;
}

void RoundProcessBar::setValue(double newValue)
{
    m_value = newValue;
    repaint();
}

void RoundProcessBar::paintOuterBar(QPainter &p)
{
    QPen pen;
    pen.setWidth(m_outerWide);
    pen.setColor(m_outerColor);
    p.setPen(pen);
    QRectF r(m_squareStart,m_squareStart,m_squareWide,m_squareWide);
    p.drawEllipse(r);
}

void RoundProcessBar::paintInnerBar(QPainter &p)
{
    QPen pen;
    pen.setWidth(m_innerWide);
    pen.setColor(m_innerColor);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    p.setPen(pen);
    QRectF r(m_squareStart,m_squareStart,m_squareWide,m_squareWide);

    //画圆弧的时候是以16分之1度为单位，所以这里要乘以16
    int startAngle = m_startAngle * 16;
    int spanAngle = m_value / m_max * 360 * 16 * -1;                //逆时针旋转乘以-1
    p.drawArc(r,startAngle,spanAngle);

}

void RoundProcessBar::paintText(QPainter &p)
{
    p.setPen(m_textColor);
    p.setFont(QFont("Microsoft YaHei",22));
    p.drawText(m_squareStart,m_squareStart,m_squareWide,m_squareWide,Qt::AlignCenter,
                             QString::number(m_value/m_max * 100,'f',1)+"%");
}

void RoundProcessBar::caculateSquare()
{
    int minWide = qMin(width(),height());
    double barWide = qMax(m_outerWide,m_innerWide);
    //控件宽度 = 2 * 偏移距离 + 用来画圆的矩形长度
    m_squareWide = minWide - barWide - 2;
    m_squareStart = barWide / 2 + 1;
}

double RoundProcessBar::max() const
{
    return m_max;
}

void RoundProcessBar::setMax(double newMax)
{
    m_max = newMax;
}

void RoundProcessBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    paintOuterBar(painter);
    paintInnerBar(painter);
    paintText(painter);
}

void RoundProcessBar::resizeEvent(QResizeEvent *event)
{
    //根据内外圈跨度设置控件得最小大小
    if(m_outerWide > m_innerWide)
        setMinimumSize(m_outerWide * 4,m_outerWide * 4);
    else
        setMinimumSize(m_innerWide * 4,m_innerWide * 4);

    caculateSquare();
}

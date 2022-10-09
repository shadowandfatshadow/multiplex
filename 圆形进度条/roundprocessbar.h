#pragma once

#include <QWidget>
#include <QPainter>

class RoundProcessBar : public QWidget
{
    Q_OBJECT

public:
    RoundProcessBar(QWidget* parent = nullptr);
    ~RoundProcessBar();

    double outerWide() const;
    void setOuterWide(double newOuterWide);

    double innerWide() const;
    void setInnerWide(double newInnerWide);

    const QColor &outerColor() const;
    void setOuterColor(const QColor &newOuterColor);

    const QColor &innerColor() const;
    void setInnerColor(const QColor &newInnerColor);

    const QColor &textColor() const;
    void setTextColor(const QColor &newTextColor);

    double value() const;
    void setValue(double newValue);

    double max() const;
    void setMax(double newMax);

private:
    void paintOuterBar(QPainter& p);    //绘制外圈
    void paintInnerBar(QPainter& p);    //绘制内圈
    void paintText(QPainter& p);        //绘制文字
    void caculateSquare();              //计算矩形信息，需要是矩形，在画弧的时候才能是圆

private:
    int m_startAngle;               //初始角度，大部分是从顶部开始，也就是得先转个90度
    double m_value;
    double m_max;

    double m_squareStart;           //矩形左上角
    double m_squareWide;            //举行宽度

    double m_outerWide;             //外圈宽度
    double m_innerWide;             //内圈宽度
    QColor m_outerColor;            //外圈颜色
    QColor m_innerColor;            //内圈颜色
    QColor m_textColor;             //文字颜色


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
};

#include "shadowwidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

ShadowWidget::ShadowWidget(QWidget *parent)
    : QWidget{parent}
{

}

void ShadowWidget::paintEvent(QPaintEvent *event)
{
    if (isMaximized())
    {
        return;
    }
    int nShadowsWidth = 6;
    int nRadius = 3;
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(nShadowsWidth, nShadowsWidth, this->width() - nShadowsWidth * 2, this->height() - nShadowsWidth * 2, nRadius, nRadius);
    QPainter painter(this);
    QColor color(0, 0, 0, 200);
    for (int i = 0; i < nShadowsWidth; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(nShadowsWidth - i, nShadowsWidth - i, this->width() - (nShadowsWidth - i) * 2, this->height() - (nShadowsWidth - i) * 2, nRadius + i, nRadius + i);
        color.setAlpha(100 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

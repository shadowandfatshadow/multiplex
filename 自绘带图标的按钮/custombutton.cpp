#include "custombutton.h"
#include <QPainter>
#include <QStyleOptionToolButton>
#include <QFontMetrics>

CustomButton::CustomButton(QWidget *parent)
    :QToolButton(parent)
{

}

void CustomButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);             //让qss先生效

    //绘制准备工作,启用反锯齿
    QPainter painter(this);
    painter.save();

    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRect rect = this->rect();
    int rectW = rect.width();
    int rectH = rect.height();

    int padding_left = (rectW - (m_iconSize.width() + m_space + m_textSize.width())) /2;

    QRect iconRect = QRect(rect.left() + padding_left,rect.top() + (rectH - m_iconSize.height())/2,m_iconSize.width(),m_iconSize.height());
    QRect textRect = QRect(iconRect.right() + m_space,rect.top() + (rectH - m_textSize.height())/2,m_textSize.width(),m_textSize.height());

    QPixmap p = m_icon.pixmap(m_iconSize,
                              QIcon::Normal,
                              isChecked() ?  QIcon::On : QIcon::Off);

    painter.drawPixmap(iconRect, p);

    painter.setPen(Qt::green);
    painter.drawText(textRect, m_text);

    painter.restore();
}

const QFont &CustomButton::font() const
{
    return m_font;
}

void CustomButton::setFont(const QFont &newFont)
{
    m_font = newFont;
    QFontMetrics f(m_font);
    m_textSize = QSize(f.width(m_text),f.height());
}

int CustomButton::space() const
{
    return m_space;
}

void CustomButton::setSpace(int newSpace)
{
    m_space = newSpace;
}

const QSize &CustomButton::iconSize() const
{
    return m_iconSize;
}

void CustomButton::setIconSize(const QSize &newIconSize)
{
    m_iconSize = newIconSize;
}

void CustomButton::setIconSize(int w, int h)
{
    m_iconSize = QSize(w,h);
}

const QIcon &CustomButton::icon() const
{
    return m_icon;
}

void CustomButton::setIcon(const QIcon &newIcon)
{
    m_icon = newIcon;
}

const QString &CustomButton::text() const
{
    return m_text;
}

void CustomButton::setText(const QString &newText)
{
    m_text = newText;

    QFontMetrics f(m_font);
    m_textSize = QSize(f.width(m_text),f.height());

}

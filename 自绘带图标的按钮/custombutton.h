#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QToolButton>

class CustomButton : public QToolButton
{
    Q_OBJECT
public:
    CustomButton(QWidget *parent = nullptr);

    // QWidget interface
    const QIcon &icon() const;
    void setIcon(const QIcon &newIcon);
    const QString &text() const;
    void setText(const QString &newText);

    const QSize &iconSize() const;
    void setIconSize(const QSize &newIconSize);
    void setIconSize(int w,int h);

    int space() const;
    void setSpace(int newSpace);

    const QFont &font() const;
    void setFont(const QFont &newFont);

protected:
    void paintEvent(QPaintEvent *event);

    QIcon m_icon;
    QSize m_iconSize = QSize(16,16);
    QString m_text;
    QSize m_textSize;
    QFont m_font;
    int m_space = 10;                           //图片与文字的间隔
};

#endif // CUSTOMBUTTON_H

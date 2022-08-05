#ifndef SHADOWWIDGET_H
#define SHADOWWIDGET_H

#include <QWidget>

class ShadowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShadowWidget(QWidget *parent = nullptr);

signals:


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // SHADOWWIDGET_H

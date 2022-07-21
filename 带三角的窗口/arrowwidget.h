#pragma once

#include <QWidget>
#include "commonutil_global.h"

class COMMONUTIL_EXPORT ArrowWidget : public QWidget
{
	Q_OBJECT

public:
	//三角形的位置
	enum TRIANGLEDIR
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
	};


	ArrowWidget(QWidget* parent);
	~ArrowWidget();

	void setTriangleInfo(int width, int height,TRIANGLEDIR dir = TRIANGLEDIR::TOP);		//设置三角形宽和高
	void setCenterWidget(QWidget* widget);				//设置中间区域的widget

	void setBackgroundColor(QColor color);
	QPainterPath freshMaskRegion();		//创建窗体遮罩

	void setRoundRadius(int radius)
	{
		m_roundRadius = radius;
	}

private:
	int			m_startPos;				//三角起始位置
	int			m_triangleWidth;		//小三角的宽度
	int			m_triangleHeight;		//小三角的高度
	TRIANGLEDIR m_triangleDir;			//三角朝向
	int			m_roundRadius;			//圆角半径

protected:
	virtual void resizeEvent(QResizeEvent* event) override;
};

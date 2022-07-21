#pragma once

#include <QWidget>
#include "commonutil_global.h"

class COMMONUTIL_EXPORT ArrowWidget : public QWidget
{
	Q_OBJECT

public:
	//�����ε�λ��
	enum TRIANGLEDIR
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
	};


	ArrowWidget(QWidget* parent);
	~ArrowWidget();

	void setTriangleInfo(int width, int height,TRIANGLEDIR dir = TRIANGLEDIR::TOP);		//���������ο�͸�
	void setCenterWidget(QWidget* widget);				//�����м������widget

	void setBackgroundColor(QColor color);
	QPainterPath freshMaskRegion();		//������������

	void setRoundRadius(int radius)
	{
		m_roundRadius = radius;
	}

private:
	int			m_startPos;				//������ʼλ��
	int			m_triangleWidth;		//С���ǵĿ��
	int			m_triangleHeight;		//С���ǵĸ߶�
	TRIANGLEDIR m_triangleDir;			//���ǳ���
	int			m_roundRadius;			//Բ�ǰ뾶

protected:
	virtual void resizeEvent(QResizeEvent* event) override;
};

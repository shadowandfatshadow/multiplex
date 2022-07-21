#include "arrowwidget.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QBitmap>

ArrowWidget::ArrowWidget(QWidget* parent)
	: QWidget(parent)
	, m_startPos(80)
	, m_triangleWidth(10)
	, m_triangleHeight(5)
	, m_triangleDir(TRIANGLEDIR::TOP)
	, m_roundRadius(4)
{
	setBackgroundColor(Qt::white);
}

ArrowWidget::~ArrowWidget()
{
}

void ArrowWidget::setTriangleInfo(int width, int height, TRIANGLEDIR dir)
{
	m_triangleWidth = width;
	m_triangleHeight = height;
	m_triangleDir = dir;

}

void ArrowWidget::setCenterWidget(QWidget* widget)
{
	QHBoxLayout* hMainLayout = new QHBoxLayout(this);
	hMainLayout->addWidget(widget);
	hMainLayout->setSpacing(0);
	hMainLayout->setContentsMargins(10, 0, 10, 0);
}

void ArrowWidget::setBackgroundColor(QColor color)
{
	QPalette pal;
	pal.setColor(QPalette::Window, color);
	setAutoFillBackground(true);
	setPalette(pal);
}

QPainterPath ArrowWidget::freshMaskRegion()
{
	if (m_triangleDir == TRIANGLEDIR::TOP || m_triangleDir == TRIANGLEDIR::BOTTOM)
	{
		m_startPos = (this->width() - m_triangleWidth) / 2;
	}
	else
	{
		m_startPos = (this->height() - m_triangleHeight) / 2;
	}

	QPainterPath maskRegion;
	if (m_triangleDir == TRIANGLEDIR::TOP)
	{
		QPolygon trianglePolygon;
		trianglePolygon << QPoint(m_startPos, m_triangleHeight);
		trianglePolygon << QPoint(m_startPos + m_triangleWidth / 2, 0);
		trianglePolygon << QPoint(m_startPos + m_triangleWidth, m_triangleHeight);

		maskRegion.addRoundedRect(QRect(0, m_triangleHeight, width(), height() - m_triangleHeight),m_roundRadius,m_roundRadius);
		maskRegion.addPolygon(trianglePolygon);
	}
	else if (m_triangleDir == TRIANGLEDIR::BOTTOM)
	{
		QPolygon trianglePolygon;
		trianglePolygon << QPoint(m_startPos, this->height() - m_triangleHeight);
		trianglePolygon << QPoint(m_startPos + m_triangleWidth / 2, this->height());
		trianglePolygon << QPoint(m_startPos + m_triangleWidth, this->height() - m_triangleHeight);

		maskRegion.addRoundedRect(QRect(0, 0, width(), height() - m_triangleHeight), m_roundRadius, m_roundRadius);
		maskRegion.addPolygon(trianglePolygon);
	}
	else if (m_triangleDir == TRIANGLEDIR::LEFT)
	{
		QPolygon trianglePolygon;
		trianglePolygon << QPoint(m_triangleHeight, m_startPos);
		trianglePolygon << QPoint(0 , m_startPos + m_triangleWidth / 2);
		trianglePolygon << QPoint(m_triangleHeight, m_startPos + m_triangleWidth);

		maskRegion.addRoundedRect(QRect(m_triangleHeight, 0, width() - m_triangleHeight, height()), m_roundRadius, m_roundRadius);
		maskRegion.addPolygon(trianglePolygon);
	}
	else if (m_triangleDir == TRIANGLEDIR::RIGHT)
	{
		QPolygon trianglePolygon;
		trianglePolygon << QPoint(width() - m_triangleHeight, m_startPos);
		trianglePolygon << QPoint(width(), m_startPos + m_triangleWidth / 2);
		trianglePolygon << QPoint(width() - m_triangleHeight, m_startPos + m_triangleWidth);

		maskRegion.addRoundedRect(QRect(0, 0, width() - m_triangleHeight, height()), m_roundRadius, m_roundRadius);
		maskRegion.addPolygon(trianglePolygon);
	}
	
	return maskRegion;
}

void ArrowWidget::resizeEvent(QResizeEvent* event)
{
	// 绘制自定义形状的窗口
	QBitmap bitMap(this->width(), this->height());	// 定义一个窗口大小的位图
	bitMap.fill();									// 填充整个位图
	QPainter painter(&bitMap);						// 定义绘图设备
	painter.setBrush(Qt::black);                  
	painter.setPen(Qt::NoPen);                    
	painter.setRenderHint(QPainter::Antialiasing);	// 反走样

	QPainterPath drawPath = freshMaskRegion();

	painter.drawPath(drawPath);						// 绘制圆角矩形

	setMask(bitMap);								// 将圆角作为遮罩
}


#pragma once

#include <QDialog>
#include "commonutil_global.h"

namespace Ui {
	class SimplePromptBox;
}


class COMMONUTIL_EXPORT SimplePromptBox : public QDialog
{
	Q_OBJECT

public:
	enum Level									//提示等级
	{
		SUCCESS,
		INFO,
		WARN,
		ERR,
	};

	SimplePromptBox(QWidget *parent = Q_NULLPTR);
	~SimplePromptBox();

	SimplePromptBox(QString content, Level level = INFO, QWidget* parent = nullptr);

	void init();
	void initUI();
	void connectSignalSlot();

	void setContent(QString content);
	void setLevel(Level level);

	void vCenterContent();								//垂直居中文本

private:
	Ui::SimplePromptBox* ui;

	double m_scaleX, m_scaleY;
protected:
	virtual void showEvent(QShowEvent*) override;

};

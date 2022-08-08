#pragma once

#include <QDialog>
#include "commonutil_global.h"

namespace Ui {
	class PromptBox;
}

class COMMONUTIL_EXPORT PromptBox : public QDialog
{
	Q_OBJECT

public:

	enum Level									//��ʾ�ȼ�
	{	
		SUCCESS,
		INFO,
		WARN,
		ERR,
	};

	~PromptBox();
	PromptBox(QString title = "", Level level = INFO, QString content = "", QString confirmBtn = "", QString cancelBtn = "", QWidget* parent = nullptr);


	void init();
	void initUI();
	void connectSignalSlot();

	void setTitle(QString title);
	void setLevel(Level level);
	void setContent(QString content);
	void vCenterContent();								//��ֱ�����ı�

private:
	Ui::PromptBox* ui;


	double m_scaleX, m_scaleY;
protected:
	virtual void showEvent(QShowEvent*) override;

};

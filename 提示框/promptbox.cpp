#include "ui_promptbox.h"
#include "promptbox.h"
#include "adaptreslution.h"
#include "Common.h"
#include <QPixmap>
#include <QTextFrame>

PromptBox::PromptBox(QString title /*= ""*/, Level level /*= INFO*/, QString content /*= ""*/, QString confirmBtn /*= ""*/, QString cancelBtn /*= ""*/, QWidget* parent /*= nullptr*/)
	:QDialog(parent)
{
	ui = new Ui::PromptBox;
	ui->setupUi(this);

	init();

	setTitle(title);
	setLevel(level);
	setContent(content);
	
	confirmBtn.isEmpty() ? ui->btn_confirm->setVisible(false) : ui->btn_confirm->setText(confirmBtn);
	cancelBtn.isEmpty() ? ui->btn_cancel->setVisible(false) : ui->btn_cancel->setText(cancelBtn);
}

PromptBox::~PromptBox()
{
}

void PromptBox::init()
{
	setWindowFlag(Qt::FramelessWindowHint);


	initUI();
	connectSignalSlot();
}

void PromptBox::initUI()
{
	int width = 0;
	int height = 0;
	DeskTopGeometry::getSize(width, height);
	m_scaleX = width / 1920.0;
	m_scaleY = height / 1080.0;

	//适配分辨率
	AdaptResolution(this, m_scaleX, m_scaleY);

}

void PromptBox::connectSignalSlot()
{
	connect(ui->btn_close, &QToolButton::clicked, this, [&]() {close(); });

}

void PromptBox::setTitle(QString title)
{
	ui->l_title->setText(title);
}

void PromptBox::setLevel(Level level)
{
	QPixmap pixmap;

	switch (level)
	{
	case PromptBox::SUCCESS:
		pixmap.load(":/PromptBox/PromptBox/success.png");
		break;
	case PromptBox::INFO:
		pixmap.load(":/PromptBox/PromptBox/info.png");
		break;
	case PromptBox::WARN:
		pixmap.load(":/PromptBox/PromptBox/warn.png");
		break;
	case PromptBox::ERR:
		pixmap.load(":/PromptBox/PromptBox/error.png");
		break;
	default:
		break;
	}

	pixmap.scaled(32 * m_scaleX, 32 * m_scaleY, Qt::KeepAspectRatio);
	ui->l_icon->setPixmap(pixmap);

}

void PromptBox::setContent(QString content)
{
	ui->tb_content->setText(content);
}

void PromptBox::vCenterContent()
{
	int textHeight = ui->tb_content->document()->size().height();			//文本的高度
	if (textHeight == 0)
		return;

	int height = ui->tb_content->size().height();							//控件的高度

	QTextFrame* pFrame = ui->tb_content->document()->rootFrame();			//获取当前文本的frame
	QTextFrameFormat frameFormat = pFrame->frameFormat();

	int topMargin = (int)frameFormat.topMargin();							//获取当前的topmargin
	int docBoundingBoxHeight = textHeight - topMargin;						//document的bounddingbox 的 高度

	if (height < docBoundingBoxHeight)
	{
		topMargin = 2;														//设置为常规的margin
	}
	else
	{
		topMargin = (height - docBoundingBoxHeight) / 2 - 2;
	}

	frameFormat.setTopMargin(topMargin);
	//需要设置border才有效
	frameFormat.setBorder(1);
	frameFormat.setBorderBrush(QColor(0xffffff));
	pFrame->setFrameFormat(frameFormat);
}

void PromptBox::showEvent(QShowEvent*)
{
	vCenterContent();
}

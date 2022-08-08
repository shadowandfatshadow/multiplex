#include "ui_simplepromptbox.h"
#include "simplepromptbox.h"
#include "adaptreslution.h"
#include "Common.h"
#include <QPixmap>
#include <QTextFrame>

SimplePromptBox::SimplePromptBox(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::SimplePromptBox;
	ui->setupUi(this);

	init();
}

SimplePromptBox::SimplePromptBox(QString content, Level level /*= INFO*/, QWidget* parent /*= nullptr*/)
	:QDialog(parent)
{
	ui = new Ui::SimplePromptBox;
	ui->setupUi(this);

	init();

	setContent(content);
	setLevel(level);
}

SimplePromptBox::~SimplePromptBox()
{
}

void SimplePromptBox::init()
{
	setWindowFlag(Qt::FramelessWindowHint);

	initUI();
	connectSignalSlot();
}

void SimplePromptBox::initUI()
{
	int width = 0;
	int height = 0;
	DeskTopGeometry::getSize(width, height);
	m_scaleX = width / 1920.0;
	m_scaleY = height / 1080.0;

	//适配分辨率
	AdaptResolution(this, m_scaleX, m_scaleY);
}

void SimplePromptBox::connectSignalSlot()
{
	connect(ui->btn_close, &QToolButton::clicked, this, &QDialog::close);
}

void SimplePromptBox::setContent(QString content)
{
	ui->te_content->setText(content);
}

void SimplePromptBox::setLevel(Level level)
{
	QPixmap pixmap;

	switch (level)
	{
	case Level::SUCCESS:
		pixmap.load(":/PromptBox/PromptBox/success.png");
		break;
	case Level::INFO:
		pixmap.load(":/PromptBox/PromptBox/info.png");
		break;
	case Level::WARN:
		pixmap.load(":/PromptBox/PromptBox/warn.png");
		break;
	case Level::ERR:
		pixmap.load(":/PromptBox/PromptBox/error.png");
		break;
	default:
		break;
	}
	ui->l_icon->setScaledContents(true);
	ui->l_icon->setPixmap(pixmap);
}

void SimplePromptBox::vCenterContent()
{
	int textHeight = ui->te_content->document()->size().height();			//文本的高度
	if (textHeight == 0)
		return;

	int height = ui->te_content->size().height();							//控件的高度

	QTextFrame* pFrame = ui->te_content->document()->rootFrame();			//获取当前文本的frame
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
	//需要设置border才有效，所以这里把border颜色设置成和控件背景一个颜色
	frameFormat.setBorder(1);
	frameFormat.setBorderBrush(QColor(0xffffff));
	pFrame->setFrameFormat(frameFormat);

	//控件高度也改变一下
	QTextDocument* document = ui->te_content->document();
	ui->te_content->setFixedHeight(document->size().height() + 2);
}

void SimplePromptBox::showEvent(QShowEvent*)
{
	vCenterContent();
}

#include "loghandler.h"


#pragma region LogHandlerPrivate {

//初始化static变量
QMutex LogHandlerPrivate::logMutex;
QFile* LogHandlerPrivate::logFile = nullptr;
QTextStream* LogHandlerPrivate::logOut = nullptr;

LogHandlerPrivate::LogHandlerPrivate()
{
	logDir.setPath("log");

	openLogFile();

	// 定时刷新日志输出到文件
	flushLogFileTimer.setInterval(1000); 
	flushLogFileTimer.start();
	QObject::connect(&flushLogFileTimer, &QTimer::timeout, [] {
		QMutexLocker locker(&LogHandlerPrivate::logMutex);
		if (nullptr != logOut) {
			logOut->flush();
		}
	});
}


LogHandlerPrivate::~LogHandlerPrivate()
{
	if (nullptr != logFile) {
		logFile->flush();
		logFile->close();
		delete logOut;
		delete logFile;

		logOut = nullptr;
		logFile = nullptr;
	}
}

void LogHandlerPrivate::openLogFile()
{
	if (!logDir.exists())
	{
		logDir.mkpath(".");
	}
	QString logPah = logDir.absoluteFilePath(QDate::currentDate().toString("yyyy-MM-dd.log"));

	if (logFile == nullptr)
	{
		logFile = new QFile(logPah);
		logOut = (logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) ? new QTextStream(logFile) : nullptr;
		if (logOut != nullptr)
		{
			logOut->setCodec("UTF-8");
		}
	}

}

void LogHandlerPrivate::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	QMutexLocker locker(&LogHandlerPrivate::logMutex);
	QString level;
	switch (type)
	{
	case QtDebugMsg:
		level = "DEBUG";
		break;
	case QtWarningMsg:
		level = "WARN";
		break;
	case QtCriticalMsg:
		level = "ERROR";
		break;
	case QtFatalMsg:
		level = "FATAL";
		break;
	case QtInfoMsg:
		level = "INFO";
		break;
	default:
		break;
	}

	if (LogHandlerPrivate::logOut == nullptr)
		return;

	//输出到日志文件
	QString fileName = context.file;
	int index = fileName.lastIndexOf(QDir::separator());
	fileName = fileName.mid(index + 1);

	(*LogHandlerPrivate::logOut) << QString("%1 - [%2] (%3:%4, %5): %6\n")
		.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
		.arg(level).arg(fileName).arg(context.line).arg(context.function).arg(msg);

}

#pragma endregion }

#pragma region LogHandler {
void LogHandler::installMessageHandler()
{
	QMutexLocker locker(&LogHandlerPrivate::logMutex);
	if (d == nullptr)
	{
		d = new LogHandlerPrivate;
		qInstallMessageHandler(LogHandlerPrivate::messageHandler);					//qt安装消息处理函数
	}
}

void LogHandler::uninstallMessageHandler()
{
}

LogHandler::LogHandler()
	:d(nullptr)
{}

#pragma endregion}



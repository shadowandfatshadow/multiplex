#pragma once

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMutexLocker>
#include <QDateTime>
#include <QTimer>
#include <QTextStream>

struct LogHandlerPrivate
{
	LogHandlerPrivate();
	~LogHandlerPrivate();

	//打开或创建日志文件
	void openLogFile();

	//消息处理函数
	static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

	//成员属性
	QDir logDir;					//日志文件夹
	QTimer flushLogFileTimer;		//刷新输出到日志文件的定时器

	static QFile* logFile;			//日志文件
	static QTextStream* logOut;		//输出到日志文件的stream
	static QMutex logMutex;			//同步使用的锁
};


class LogHandler 
{
public:
	void installMessageHandler();					//安装消息处理器
	void uninstallMessageHandler();					//取消安装消息处理器并释放资源

	static LogHandler& get()
	{
		static LogHandler m_handler;
		return m_handler;
	}

private:
	LogHandler();

	LogHandlerPrivate* d;
};

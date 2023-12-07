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

	//�򿪻򴴽���־�ļ�
	void openLogFile();

	//��Ϣ������
	static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

	//��Ա����
	QDir logDir;					//��־�ļ���
	QTimer flushLogFileTimer;		//ˢ���������־�ļ��Ķ�ʱ��

	static QFile* logFile;			//��־�ļ�
	static QTextStream* logOut;		//�������־�ļ���stream
	static QMutex logMutex;			//ͬ��ʹ�õ���
};


class LogHandler 
{
public:
	void installMessageHandler();					//��װ��Ϣ������
	void uninstallMessageHandler();					//ȡ����װ��Ϣ���������ͷ���Դ

	static LogHandler& get()
	{
		static LogHandler m_handler;
		return m_handler;
	}

private:
	LogHandler();

	LogHandlerPrivate* d;
};

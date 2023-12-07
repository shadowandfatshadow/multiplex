#include "qreplytimeout.h"

QReplyTimeout::QReplyTimeout(QNetworkReply* reply, const int timeout)
	: QObject(reply) 
{
	Q_ASSERT(reply);
	if (reply && reply->isRunning()) {  // 启动单次定时器
		QTimer::singleShot(timeout, this, SLOT(onTimeout()));
	}
}

QReplyTimeout::~QReplyTimeout()
{}

void QReplyTimeout::onTimeout()
{
	QNetworkReply* reply = static_cast<QNetworkReply*>(parent());
	if (reply->isRunning()) {
		reply->abort();
		reply->deleteLater();
		emit timeout();
	}
}

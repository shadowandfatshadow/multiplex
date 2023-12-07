#pragma once

#include <QObject>
#include <QTimer>
#include <QNetworkReply>

class QReplyTimeout  : public QObject
{
	Q_OBJECT

public:
	QReplyTimeout(QNetworkReply* reply, const int timeout);
	~QReplyTimeout();

signals:
	void timeout();

private slots:
	void onTimeout();
};

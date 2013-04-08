#ifndef QTXMOCKCORE_MOCKIODEVICE_H
#define QTXMOCKCORE_MOCKIODEVICE_H

#include "mockcoreglobal.h"
#include <QtCore>

QTX_BEGIN_NAMESPACE


class MockIODevice : public QIODevice
{
	Q_OBJECT
	
public:
	MockIODevice(const QString & fileName);
	virtual ~MockIODevice();
	
	bool open(OpenMode mode);
	
	const QByteArray & output() const;
	
signals:
	void opened();
	
protected:
	virtual qint64 readData(char* data, qint64 maxSize);
	virtual qint64 writeData(const char* data, qint64 maxSize);
	
private:
	bool recvNextChunk();
	
private:
	QString _fileName;
	quint32 _chunk;
	
	QByteArray _input;
	QByteArray _output;
};


QTX_END_NAMESPACE

#endif // QTXMOCKCORE_MOCKIODEVICE_H

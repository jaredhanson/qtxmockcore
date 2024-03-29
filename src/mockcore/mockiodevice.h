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
    
protected:
    virtual qint64 readData(char* data, qint64 maxSize);
    virtual qint64 writeData(const char* data, qint64 maxSize);
    
private slots:
    void doIO();
    
private:
    bool readChunk();
    
private:
    QString mFileName;
    quint32 mChunk;
    
    QByteArray mInput;
    QByteArray mOutput;
};


QTX_END_NAMESPACE

#endif // QTXMOCKCORE_MOCKIODEVICE_H

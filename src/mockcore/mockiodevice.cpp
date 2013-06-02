#include "mockiodevice.h"

QTX_BEGIN_NAMESPACE


MockIODevice::MockIODevice(const QString & fileName)
    : mFileName(fileName),
      mChunk(0)
{
}

MockIODevice::~MockIODevice()
{
}

bool MockIODevice::open(OpenMode mode)
{
    bool rv = QIODevice::open(mode);
    if (mode & QIODevice::ReadOnly)
    {
        // The IO device is being opened in read mode (or read/write mode).
        // Read data from the file source into the input buffer.
        while (recvNextChunk());
    }
    
    return rv;
}

const QByteArray & MockIODevice::output() const
{
    return mOutput;
}

qint64 MockIODevice::readData(char* data, qint64 maxSize)
{
    // The request is complete, there can never be more bytes available.  Return
    // -1 to indicate this fact to the I/O subsystem.
    if (mInput.size() == 0)
        return -1;
    
    qint64 size = maxSize;
    if (mInput.size() < maxSize)
        size = mInput.size();
        
    memcpy(data, mInput.data(), size);
    mInput.remove(0, size);
    return size;
}

qint64 MockIODevice::writeData(const char* data, qint64 maxSize)
{
    mOutput.append(data, maxSize);
    return maxSize;
}

bool MockIODevice::recvNextChunk()
{
    QFileInfo finfo(mFileName);
    
    QString name = finfo.path();
    name.append( "/" );
    name.append( finfo.baseName() );
    name.append( "." );
    name.append( QString::number(mChunk) );
    name.append( "." );
    name.append( finfo.completeSuffix() );
    mChunk++;
    
    if (QFile::exists(name))
    {
        QFile file(name);
        file.open(QIODevice::ReadOnly);

        mInput = file.readAll();
        
        emit readyRead();
        return true;
    }
    
    return false;
}


QTX_END_NAMESPACE

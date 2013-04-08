#include "mockiodevice.h"

QTX_BEGIN_NAMESPACE


MockIODevice::MockIODevice(const QString & fileName)
	: _fileName(fileName),
	  _chunk(0)
{
}

MockIODevice::~MockIODevice()
{
}

bool MockIODevice::open(OpenMode mode)
{
	bool rv = QIODevice::open(mode);
	
	emit opened();
	
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
	return _output;
}

qint64 MockIODevice::readData(char* data, qint64 maxSize)
{
	// The request is complete, there can never be more bytes available.  Return
	// -1 to indicate this fact to the I/O subsystem.
	if (_input.size() == 0)
		return -1;
	
	qint64 size = maxSize;
	if (_input.size() < maxSize)
		size = _input.size();
		
	memcpy(data, _input.data(), size);
	_input.remove(0, size);
	
	return size;
}

qint64 MockIODevice::writeData(const char* data, qint64 maxSize)
{
	_output.append(data, maxSize);
	
	return maxSize;
}

bool MockIODevice::recvNextChunk()
{
	QFileInfo finfo(_fileName);
	
	QString name = finfo.path();
	name.append( "/" );
	name.append( finfo.baseName() );
	name.append( "." );
	name.append( QString::number(_chunk) );
	name.append( "." );
	name.append( finfo.completeSuffix() );
	_chunk++;
	
	//Logger::trace(QString("  name: %1").arg(name));
	
	if (QFile::exists(name))
	{
		QFile file(name);
		file.open(QIODevice::ReadOnly);

		_input = file.readAll();
		
		emit readyRead();
		return true;
	}
	
	return false;
}


QTX_END_NAMESPACE

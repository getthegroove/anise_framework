#include "filedata.h"

#include <QFile>
#include <QIODevice>

//------------------------------------------------------------------------------
// Constructor and Destructor

CFileData::CFileData()
    : CData()
    , m_bytes()
    , m_binary_data(true)
{

}


//------------------------------------------------------------------------------
// Public Functions

bool CFileData::readFile(QString filename, bool binary)
{
    QFile file(filename);
    m_binary_data = binary;

    QIODevice::OpenMode flags = QIODevice::ReadOnly;
    if(!binary) {
        flags |= QIODevice::Text;
    }

    if(!file.open(flags)) {
        return false;
    }

    // Read the entire file into m_bytes.
    m_bytes = file.readAll();

    return true;
}

bool CFileData::isDataBinary()
{
    return m_binary_data;
}

const QByteArray &CFileData::getBytes() const
{
    return m_bytes;
}


//------------------------------------------------------------------------------
// Private Functions


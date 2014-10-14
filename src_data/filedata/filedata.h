#ifndef FILEDATA_H
#define FILEDATA_H

#include "data/data.h"
#include <QByteArray>

class CFileData: public CData
{
  public:
    explicit CFileData();
    bool readFile(QString filename, bool binary);
    bool isDataBinary();
    const QByteArray &getBytes();

  private:
    QByteArray m_bytes;
    bool m_binary_data;
};

#endif // FILEDATA_H


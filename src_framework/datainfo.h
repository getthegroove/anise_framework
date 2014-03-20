#ifndef DATAINFO_H
#define DATAINFO_H

#include <QString>

template<class T>
class CDataInfo
{
  private:
    QString m_name;

  public:
    explicit CDataInfo()
    {
        m_name = name();
    }

    static const char *name()
    {
        return T::name();
    }
};

#endif // DATAINFO_H

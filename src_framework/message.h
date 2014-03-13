#ifndef MESSAGE_H
#define MESSAGE_H

#include <QSharedDataPointer>
#include <QString>

class CMessage
{
  private:
    QString m_name;

  public:
    explicit CMessage();

  private:
    //QSharedPointer<CMessageData> m_message_data;

};

#endif // MESSAGE_H

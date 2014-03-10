#include "gate.h"
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CGate::CGate(QString name_id, QString msg_type, QObject *parent)
    : QObject(parent)
    , m_name_id(name_id)
    , m_msg_type(msg_type)
{

}

//------------------------------------------------------------------------------
// Public Functions


//------------------------------------------------------------------------------
// Public Slots

void CGate::processMessage(QString msg_type)
{
    // Compare the message types to see if it's the expected type.
    // TODO: Replace this with the Message class and logic.
    if(m_msg_type != msg_type) {
        // Messages do not match.
        qDebug() << "CGate::processMessage() Error:"
                 << "Unexpected Message received.";
        return;
    }

    //m_message_queue.enqueue(QSharedPointer<CMessage>(new Message()));
}


//------------------------------------------------------------------------------
// Private Functions


//------------------------------------------------------------------------------
// Private Slots


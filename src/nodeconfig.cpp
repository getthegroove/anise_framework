#include "nodeconfig.h"
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CNodeConfig::CNodeConfig()
{

}


//------------------------------------------------------------------------------
// Public Functions

bool CNodeConfig::setParameter(QString key, QVariant value) const
{
    // Key exists?
    if(!m_parameters_map.contains(key)) {
        qDebug() << "CNodeConfig::setParameter() Error:"
                 << "The specified parameter field does not exist"
                 << "in the template.";
        return false;
    }

    SParameter param = m_parameters_map[key];

    // Verify that the supplied value matches the required value.
    if(value.type() != param.type) {
        qDebug() << "CNodeConfig::setParameter() Error:"
                 << "Parameter types do not match.";
        return false;
    }

    param.value = value;

    return true;
}

void CNodeConfig::addFilename(QString key, QString name, QString description)
{
    struct SParameter param;
    param.name = name;
    param.type = QVariant::String;
    param.description = description;

    m_parameters_map.insert(key, param);
}

int CNodeConfig::addInputBox(bool sync/* = true */)
{
    m_input_boxes.append(SBoxTemplate(sync));
    return m_input_boxes.size() - 1;
}

void CNodeConfig::addInputGate(int gate_box_id, QString name, QString msg_type)
{
    m_input_boxes[gate_box_id].gates.append(SGateTemplate(name, msg_type));
}


int CNodeConfig::addOutputBox(bool sync/* = true */)
{
    m_output_boxes.append(SBoxTemplate(sync));
    return m_output_boxes.size() - 1;
}

void CNodeConfig::addOutputGate(int gate_box_id, QString name, QString msg_type)
{
    m_output_boxes[gate_box_id].gates.append(SGateTemplate(name, msg_type));
}

const CNodeConfig::SParameter *CNodeConfig::getParameter(QString key)
{
    if(!m_parameters_map.contains(key)) {
        qDebug() << "CNodeConfig::getParameter() Error:"
                 << "The parameter" << key << "was not found";
        return nullptr;
    }

    return &m_parameters_map[key];
}

const QList<CNodeConfig::SBoxTemplate> &CNodeConfig::getInputBoxTemplate() const
{
    return m_input_boxes;
}

const QList<CNodeConfig::SBoxTemplate> &CNodeConfig::getOutputBoxTemplate() const
{
    return m_output_boxes;
}


//------------------------------------------------------------------------------
// Public Slots


//------------------------------------------------------------------------------
// Private Functions


//------------------------------------------------------------------------------
// Private Slots



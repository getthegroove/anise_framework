#include "nodeconfig.h"
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CNodeConfig::CNodeConfig()
    : m_name("Node")
{

}


//------------------------------------------------------------------------------
// Public Functions

void CNodeConfig::setName(QString name)
{
    m_name = name;
}

QString CNodeConfig::getName() const
{
    return m_name;
}

bool CNodeConfig::setParameter(QString key, QVariant value) const
{
    // Key exists?
    if(!m_parameter_template_map.contains(key)) {
        qDebug() << "CNodeConfig::setParameter() Error:"
                 << "The parameter" << key << "has not been defined"
                 << "in the configuration template." << endl;
        return false;
    }

    SParameterTemplate param_template = m_parameter_template_map[key];

    // Verify that the supplied value matches the required value.
    if(value.type() != param_template.type) {
        qDebug() << "CNodeConfig::setParameter() Error:"
                 << "The value specified for" << key
                 << "has an incorrect type." << endl;
        return false;
    }

    param_template.value = value;

    return true;
}

void CNodeConfig::addFilename(QString key, QString name, QString description)
{
    struct SParameterTemplate param_template;
    param_template.name = name;
    param_template.type = QVariant::String;
    param_template.description = description;

    m_parameter_template_map.insert(key, param_template);
}

void CNodeConfig::addInput(QString name, QString msg_type)
{
    m_input_templates.append(SGateTemplate(name, msg_type));
}

void CNodeConfig::addOutput(QString name, QString msg_type)
{
    m_output_templates.append(SGateTemplate(name, msg_type));
}

const CNodeConfig::SParameterTemplate *CNodeConfig::getParameter(QString key)
{
    if(!m_parameter_template_map.contains(key)) {
        qDebug() << "CNodeConfig::getParameter() Error:"
                 << "The parameter" << key << "has not been found." << endl;
        return nullptr;
    }

    return &m_parameter_template_map[key];
}

const QList<CNodeConfig::SGateTemplate> &CNodeConfig::getInputTemplates() const
{
    return m_input_templates;
}

const QList<CNodeConfig::SGateTemplate> &CNodeConfig::getOutputTemplates() const
{
    return m_output_templates;
}


//------------------------------------------------------------------------------
// Public Slots


//------------------------------------------------------------------------------
// Private Functions


//------------------------------------------------------------------------------
// Private Slots



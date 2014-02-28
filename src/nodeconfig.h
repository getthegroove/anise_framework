#ifndef NODECONFIG_H
#define NODECONFIG_H

#include <QMap>
#include <QVariant>

class CNodeConfig
{
  public:
    // The structure that defined the details of a parameter.
    struct SParameter {
        QString name;
        QVariant::Type type; // Type expected for the value.
        QVariant value;      // The value this parameter will hold.
        QString description;
        SParameter()
            : name()
            , type(QVariant::Invalid)
            , value()
            , description() {}
    };
    struct SGate {
        QString name;
        QString msg_type;
        SGate(QString p_name, QString p_msg_type)
            : name(p_name)
            , msg_type(p_msg_type) {}
    };
    struct SGateBox {
        bool sync;
        QList<SGate> gates;
        SGateBox(bool p_sync = true): sync(p_sync) {}
    };

  private:
    // The collection of configuration parameters of the Node.
    // ... It's mutable to allow the user of the Node clases to modify
    // ... the value type of the parameters while disallowing the addition
    // ... or deletion of parameters to the configuration template.
    mutable QMap<QString, SParameter> m_parameters_map;
    mutable QList<SGateBox> m_input_boxes;
    mutable QList<SGateBox> m_output_boxes;

  public:
    CNodeConfig();

    bool setParameter(QString key, QVariant value) const;

    // Facilities for adding different parameters to the configuration
    // ... template.
    void addFilename(QString key, QString name, QString description);

    // Facilities for defining the need of gate boxes and gates.
    // Create a GateBox and return it's gate id.
    int addInputBox(bool sync = true);
    void addInputGate(int gate_box_id, QString name, QString msg_type);
    int addOutputBox(bool sync = true);
    void addOutputGate(int gate_box_id, QString name, QString msg_type);
};

#endif // NODECONFIG_H

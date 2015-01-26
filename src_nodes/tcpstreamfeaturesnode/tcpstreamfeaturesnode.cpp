#include "tcpstreamfeaturesnode.h"
#include "data/datafactory.h"
#include "data/messagedata.h"
#include <ctime>
#include <QDebug>
#include <QDateTime>
#include <tabledata/tabledata.h>
#include <tcpstreamsdata/tcpstreamsdata.h>


//------------------------------------------------------------------------------
// Constructor and Destructor

CTcpStreamFeaturesNode::CTcpStreamFeaturesNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CTcpStreamFeaturesNode::configure(CNodeConfig &config)
{
    // Add parameters
    config.addInt("timezone", "GMT time", "The timezone value.", 0);
    config.addBool("split_dest_ip", "Split Destination IP",
                   "Split each octet of the destination IP address as different features.",
                   false);
    config.addUInt("dest_ip_split_number", "Destination IP Features",
                   "The number of octets to use as attributes, starting from the "
                   "least significant to the most significant [1-4]", 4);
    config.addBool("split_src_ip", "Split Source IP",
                   "Split each octet of the source IP address as different attributes.",
                   false);
    config.addUInt("src_ip_split_number", "Source IP Features",
                   "The number of octets to use as attributes, starting from the "
                   "least significant to the most significant [1-4]", 4);
    config.addUInt("word_count", "Words to Extract",
                  "The number of words to extract from the data stream as they appear.",
                  8);
    config.addUInt("word_length", "Maximum Length of a Word",
                   "The number of characters a word can be matched to.", 16);

    // Add the gates.
    config.addInput("in", "tcpstreams");
    config.addOutput("out", "table");
}


//------------------------------------------------------------------------------
// Protected Functions

bool CTcpStreamFeaturesNode::start()
{
    m_table = QSharedPointer<CTableData>(
                static_cast<CTableData *>(createData("table")));

    if(!m_table.isNull()) {
        // Optimize the column allocation space for the table.
        qint32 cols = 9;
        // Set the table headers at the same time.
        m_table->addHeader("Date");
        m_table->addHeader("Time");

        // DEST IP
        if(getConfig().getParameter("split_dest_ip")->value.toBool()) {
            qint32 length = getConfig().
                    getParameter("dest_ip_split_number")->value.toInt();
            for(qint32 i = length - 1; i >= 0; --i) {
                m_table->addHeader(QString("DA%1").arg(i));
            }
            cols += length;
        }
        else {
            m_table->addHeader("DA");
            ++cols;
        }

        // Destination Port
        m_table->addHeader("DP");

        // SRC IP
        if(getConfig().getParameter("split_src_ip")->value.toBool()) {
            qint32 length = getConfig().
                    getParameter("src_ip_split_number")->value.toInt();
            for(qint32 i = length - 1; i >= 0; --i) {
                m_table->addHeader(QString("SA%1").arg(i));
            }
            cols += length;
        }
        else {
            m_table->addHeader("SA");
            ++cols;
        }

        // Source Port
        m_table->addHeader("SP");

        // Stream duration (in seconds)
        m_table->addHeader("DUR");

        // The first flags, second to last and last flags.
        m_table->addHeader("F1");
        m_table->addHeader("F2");
        m_table->addHeader("F3");

        // The size of the data.
        m_table->addHeader("LEN");

        // WORDS
        qint32 words = getConfig().getParameter("word_count")->value.toInt();
        for(qint32 i = 1; i <= words; ++i) {
            m_table->addHeader(QString("W%1").arg(i));
        }
        cols += words;

        m_table->setCols(cols);

        return true;
    }
    else {
        return false;
    }
}

void CTcpStreamFeaturesNode::data(QString gate_name, const CConstDataPointer &data)
{
    Q_UNUSED(gate_name);

    // Process framework messages.
    if(data->getType() == "message") {
        auto pmsg = data.staticCast<const CMessageData>();
        QString msg = pmsg->getMessage();
        qDebug() << "Received message:" << msg;
        if(msg == "error") {
            commitError("out", "Could not get tcp streams.");
            return;
        }
    }
    else if(data->getType() == "tcpstreams") {
        auto tcp_streams = data.staticCast<const CTcpStreamsData>();

        // Optimize the row allocation space for the table.
        m_table->setRows(tcp_streams->totalStreamsCount());

        // Process the closed streams.
        auto streams_it = tcp_streams->getClosedStreams().constBegin();
        for(; streams_it != tcp_streams->getClosedStreams().constEnd(); ++streams_it) {
            extractFeatures(*streams_it);
        }
        // Process the open streams.
        streams_it = tcp_streams->getOpenStreams().constBegin();
        for(; streams_it != tcp_streams->getOpenStreams().constEnd(); ++streams_it) {
            extractFeatures(*streams_it);
        }

        // Sort by date and time (fields 0 and 1).
        m_table->sort(0, 1);

        commit("out", m_table);
    }
}

void CTcpStreamFeaturesNode::extractFeatures(const CTcpStream &tcp_stream)
{
    QList<QVariant> &row = m_table->newRow();

    // The Attributes being added:
    // DATE TIME DEST_IP DEST_PORT SRC_IP SRC_PORT DUR F1 F2 F3 LEN W1 ... W8

    // Date & Time
    QDateTime datetime;
    QVariant timezone_offset = getConfig().getParameter("timezone")->value;
    // Timezone parameter is in hours, we need it in seconds.
    datetime.setOffsetFromUtc(timezone_offset.toInt() * 3600);
    datetime.setTime_t(tcp_stream.start_time);
    QString time_string = datetime.toString(QString("MM/dd/yyyy hh:mm:ss"));
    QStringList split_time = time_string.split(" ");
    // Append time.
    row << split_time.at(0);
    row << split_time.at(1);

    // Destination Address
    QVariant split_dest = getConfig().getParameter("split_dest_ip")->value;
    if(split_dest.toBool()) {
        // Split each octet of the destination addrs as a different attribute.
        QVariant dest_octets = getConfig().
                getParameter("dest_ip_split_number")->value;
        qint32 bits = 24 - ((4 - dest_octets.toInt()) * 8);
        for(qint32 i = bits; i >= 0; i -= 8) {
            qint32 octet = (tcp_stream.destination_addr >> i) & 0xFF;
            // Path the octet with 0s to make its length 3.
            QString octet_string = QString("%1").arg(octet, 3, 10, QChar('0'));
            // Append an octet.
            row << octet_string;
        }
    }
    else {
        qCritical() << "Destination non-splitting not implemented!";
        row << "0.0.0.0";
    }

    // Destination Port
    row << tcp_stream.destination_port;

    // Source Address
    QVariant split_src = getConfig().getParameter("split_src_ip")->value;
    if(split_src.toBool()) {
        // Split each octet of the source addrs as different attributes.
        QVariant src_octets = getConfig().getParameter("src_ip_split_number")->value;
        qint32 bits = 24 - ((4 - src_octets.toInt()) * 8);
        for(qint32 i = bits; i >= 0; i -= 8) {
            qint32 octet = (tcp_stream.source_addr >> i) & 0xFF;
            // Path the octet with 0s to make its length 3.
            QString octet_string = QString("%1").arg(octet, 3, 10, QChar('0'));
            // Append an octet.
            row << octet_string;
        }
    }
    else {
        qCritical() << "Source feature non-splitting not implemented!";
        row << "0.0.0.0";
    }

    // Source Port
    row << tcp_stream.source_port;

    // Stream duration (in seconds)
    row << static_cast<qint32>(tcp_stream.finish_time - tcp_stream.start_time);

    // The first flags, second to last and last flags.
    row << buildFlagsString(tcp_stream.flags_first);
    row << buildFlagsString(tcp_stream.flags_before_last);
    row << buildFlagsString(tcp_stream.flags_last);

    // The size of the data.
    row << tcp_stream.data_size;

    // The words to extract from the data stream.
    QStringList string_list = extractStrings(tcp_stream.data);
    for(int i = 0; i < string_list.size(); ++i) {
        row << string_list.at(i);
    }
}

QString CTcpStreamFeaturesNode::buildFlagsString(quint8 flags)
{
    QString str(".");
    if(flags & 128) str.append('1');
    if(flags & 64) str.append('0');
    if(flags & 32) str.append('U');
    if(flags & 16) str.append('A');
    if(flags & 8) str.append('P');
    if(flags & 4) str.append('R');
    if(flags & 2) str.append('S');
    if(flags & 1) str.append('F');

    return str;
}

QStringList CTcpStreamFeaturesNode::extractStrings(const QVector<quint8> &data)
{
    QStringList strings;
    QString string("");
    QVariant limit_param = getConfig().getParameter("word_count")->value;
    quint32 word_length = getConfig().getParameter("word_length")->value.toUInt();
    quint32 limit = limit_param.toUInt();

    // Extract up to 'limit' strings.
    for(qint32 i = 0; i < data.size() && limit > 0; ++i) {
        // Found a space, null byte or non-printable char and there is a word?
        if(data[i] > '~' || data[i] == ' ')
        {
            // Treat any non-printable character as a word separators.
            if(!string.isEmpty()) {
                // Add the word to the list and reset the string.
                string.push_front('.'); // Every string starts with a '.'
                strings << string.left(word_length + 1);
                --limit;
                string = "";
            }
        }
        // Found a line break.
        else if(data[i] == '\n') {
            string.append("^");
            string.push_front('.');
            strings << string.left(word_length + 1);
            --limit;
            string = "";
        }
        // If character is unprintable add a ^ and make it printable.
        else if(data[i] < ' ' || data[i] == '^') {
            string.append('^');
            string.append(static_cast<char>(data[i] + 64));
        }
        else {
            // Add the printable character to the current string.
            string.append(static_cast<char>(data[i]));
        }
    }

    // Add any printable characters still remaining.
    if(limit > 0 && !string.isEmpty()) {
        strings << string.left(word_length + 1);
        --limit;
    }

    // Add empty strings until we reach the limit.
    for(; limit > 0; --limit) {
        strings << ".";
    }

    return strings;
}


#include "leradnode.h"
#include "data/datafactory.h"
#include "data/messagedata.h"
#include <QDebug>
#include <QList>
#include <tabledata/tabledata.h>


//------------------------------------------------------------------------------
// Constructor and Destructor

CLeradNode::CLeradNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CLeradNode::configure(CNodeConfig &config)
{
    // Set the node Description
    config.setDescription("The LERAD Anomaly Detection Algorithm: "
                          "Build rules that describe the normal pattern "
                          "of given data.");

    // Add parameters
    //config.addFilename("file", "Input File", "File to be read from disk.");
    config.addUInt("rseed", "Random Seed", "The random seed to feed the "
                   "random number generator at the start.", 666);
    config.addUInt("sample_size", "Training Samples", "Number of samples to use "
                   "for training LERAD.", 100);
    config.addUInt("pairs_to_match", "Number of Pairs to Match",
                   "Number of pairs to match for building rules", 1000);
    config.addUInt("max_rules_per_pair", "Maximum Rules per Pair",
                   "Maximum number of rules to create for each pair.", 4);

    // Add the gates.
    config.addInput("in", "table");
    //config.addOutput("out", "misc");
}


//------------------------------------------------------------------------------
// Protected Functions

bool CLeradNode::start()
{
    return true;
}

void CLeradNode::data(QString gate_name, const CConstDataPointer &data)
{
    Q_UNUSED(gate_name);

    qDebug() << "Data received.";

    if(data->getType() == "message") {
        // Process framework messages.
        auto pmsg = data.staticCast<const CMessageData>();
        QString msg = pmsg->getMessage();
        qDebug() << "Received message:" << msg;
        if(msg == "error") {
            commitError("out", "Could not get tcp file data.");
            return;
        }
    }
    else if(data->getType() == "table") {
        // Process table data.
        auto table = data.staticCast<const CTableData>();
        if(!table.isNull()) {
            lerad(table);
        }
        else {
            commitError("out", "No valid table received.");
        }
    }
}

void CLeradNode::lerad(const QSharedPointer<const CTableData> &table)
{
    // Seed the algorithm.
    qsrand(getConfig().getParameter("rseed")->value.toUInt());

    // Helpers for translating nominals to strings and viceversa.
    QList<QString> n2s;
    n2s.append("*");
    n2s.append("?");
    QMap<QString, Nominal> s2n;
    s2n["*"] = 0;
    s2n["?"] = 1;

    // Number of attributes
    qint32 attribute_count = table->getColCount();
    if(attribute_count < 2) {
        qWarning() << "LERAD needs at least 2 attributes to create rules.";
        return;
    }

    // 2- Read tuples from the table to build a dataset.
    QList<QList<Nominal>> dataset;

    qint32 rows = table->getRowCount();
    for(qint32 j = 0; j < rows; ++j) {
        // Get the row.
        const QList<QVariant> &row = table->getRow(j);
        qint32 i;
        // Iterate the attributes of the row.
        Antecedent t;
        t.reserve(attribute_count);
        for(i = 0; i < attribute_count; ++i) {
            // Convert attribute to nominal.
            QString attr = row[i].toString();
            Nominal n = s2n[attr];
            if(n == 0) {
                n = n2s.size();
                n2s.append(attr);
                s2n[attr] = n;
            }
            t.append(n);
        }
        dataset.append(t);
    }
    qDebug() << "LERAD:: Dataset size:" << dataset.size();

    if(dataset.size() < 2) {
        qWarning() << "LERAD:: Cannot work with less than two tuples.";
        return;
    }

    // 3- Select random samples from the dataset for preliminary training.
    qint32 sample_size = getConfig().getParameter("sample_size")->value.toInt();
    QList<qint32> samples;
    samples.reserve(sample_size);

    qint32 r = qMin(dataset.size(), sample_size);
    for(qint32 i = 0; r > 0 && i < dataset.size(); ++i) {
        qint32 rand_num = rnd() % (dataset.size() - i);
        if(rand_num < r) {
            --r;
            samples.append(i);
        }
    }

    // 4- Construct ruleset.
    // The key is a rule, coded 0="*", 1="?", 2 or more = constrained
    // attribute.  Rules are constructed by sampling 2 tuples
    // in the dataset and constructing up to 4 rules for 4 randomly
    // selected matching attributes in random order where the first is "?"
    // and the other 3 are the anteceedents.

    qint32 ruleset_size = getConfig().
            getParameter("pairs_to_match")->value.toInt();
    qint32 max_rules = getConfig().
            getParameter("max_rules_per_pair")->value.toInt();

    //Ruleset ruleset;
    QList<CRule> ruleset;
    ruleset.reserve(ruleset_size * max_rules * 0.85);
    Antecedent antecedent;
    antecedent.reserve(attribute_count);
    for(qint32 j = 0; j < attribute_count; ++j) antecedent.append(0);

    for(qint32 i = 0; i < ruleset_size; ++i) {
        // Pick random pair of tuples from sample set.
        qint32 r1 = rnd() % samples.size();
        qint32 r2 = rnd() % (samples.size() - 1);
        if(r1 == r2) r2 = samples.size() - 1;
        r1 = samples[r1];
        r2 = samples[r2];

        // Generate rules by matching attribute values.
        for(qint32 j = 0; j < attribute_count; ++j) antecedent[j] = 0;

        qint32 count = 0;

        for(qint32 j = 0; j < attribute_count && count < max_rules; ++j) {
            qint32 r3 = rnd() % attribute_count;
            if(dataset[r1][r3] == dataset[r2][r3]) {
                if(count == 0) {
                    // The first match becomes an consequent (marked with a '?').
                    antecedent[r3] = 1; // = s2n["?"];
                    count = 1;
                    // Insert the rule into the ruleset.
                    ruleset.append(CRule(antecedent));
                }
                else if(antecedent[r3] == 0) {
                    // Other matches are antecedents
                    antecedent[r3] = dataset[r1][r3];
                    ++count;
                    // Add new rule.
                    ruleset.append(CRule(antecedent));
                }
            }
        }
    }
    qDebug() << "LERAD:: Initial Rules:" << ruleset.size();

    // 5- Estimate the support of each rule using the samples.
    QList<CRule>::iterator it;
    for(it = ruleset.begin(); it != ruleset.end(); ++it) {
        qint32 i = 0;
        for(; i < samples.size(); ++i) {
            qint32 r = samples[i];
            qint32 j = 0;
            for(; j < attribute_count; ++j) {
                // Check if the rule applies.
                if((*it).antecedent[j] >= 2 &&
                        (*it).antecedent[j] != dataset[r][j]) {
                    break;
                }
            }
            if(j == attribute_count) {
                // The rule does apply.
                (*it).consequent.add(dataset[r][(*it).i_consequent]);
            }
        }
    }

    // 6- Estimate n/r again so that each tuple attribute is predicted by
    // only the rule with the highest n/r from before
    std::sort(ruleset.begin(), ruleset.end());

    QList<QList<bool>> cover;
    cover.reserve(sample_size);
    for(qint32 i = 0; i < sample_size; ++i) {
        QList<bool> tlist;
        tlist.reserve(attribute_count);
        for(qint32 j = 0; j < attribute_count; ++j) {
            tlist.append(false);
        }
        cover.append(tlist);
    }

    // If there is a rule predicting attribute database[sample[i]][j].
    for(it = ruleset.begin(); it != ruleset.end(); ++it) {
        CRule &r = *it;
        r.consequent.clear();

        for(qint32 j = 0; j < sample_size; ++j) {
            if(!cover[j][r.i_consequent] && r.match(dataset[samples[j]])) {
                r.consequent.add(dataset[samples[j]][r.i_consequent]);
                cover[j][r.i_consequent] = true;
            }
        }
    }

    // 6.1- Discard unsupported rules
    it = ruleset.begin();
    while(it != ruleset.end()) {
        if(it->consequent.n == 0) {
            it = ruleset.erase(it);
        }
        else {
            ++it;
        }
    }
    qDebug() << "LERAD:: Pruned Rules:" << ruleset.size();

    // 7- Calculate exact support for top rules on entire training set
    std::sort(ruleset.begin(), ruleset.end());
    // Reset the cover.
    cover.clear();
    cover.reserve(dataset.size());
    for(qint32 i = 0; i < dataset.size(); ++i) {
        QList<bool> tlist;
        tlist.reserve(attribute_count);
        for(qint32 j = 0; j < attribute_count; ++j) {
            tlist.append(false);
        }
        cover.append(tlist);
    }

    qint32 now = 0;
    qint32 validation_index = dataset.size() * 9;
    bool rule_deleted = false;

    it = ruleset.begin();
    while(it != ruleset.end()) {
        CRule &rule = *it;
        rule.consequent.clear();
        now = 0;
        rule_deleted = false;

        QList<QList<Nominal>>::iterator tuple_it;
        qint32 tuple_index = 0;
        for(tuple_it = dataset.begin(), tuple_index = 0;
                tuple_it != dataset.end();
                ++tuple_it, ++tuple_index) {
            QList<Nominal> &tuple = *tuple_it;
            ++now;
            if(rule.match(tuple)) {
                cover[tuple_index][rule.i_consequent] = true;
                if(rule.consequent.add(tuple[rule.i_consequent]) == 1) {
                    // Update time.
                    rule.consequent.t = now;
                    // Remove rules that trigger False Positives.
                    if(tuple_index  * 10 > validation_index) {
                        it = ruleset.erase(it);
                        rule_deleted = true;
                        break;
                    }
                }
            }
        }
        if(!rule_deleted) {
            ++it;
        }
    }

    // 8- Print coverage
    qDebug() << "LERAD:: Coverage:";
    for(qint32 i = 0; i < attribute_count; ++i) {
        qint32 count = 0;
        for(qint32 j = 0; j < cover.size(); ++j) {
            if(cover[j][i]) {
                ++count;
            }
        }
        qDebug() << "LERAD::" << table->getHeader()[i] << "=" << count;
    }
}




























#ifndef VSCODEPROJECTSRUNNER_H
#define VSCODEPROJECTSRUNNER_H

#include <KRunner/AbstractRunner>

class VSCodeProjectsRunner : public Plasma::AbstractRunner {
Q_OBJECT

public:
    VSCodeProjectsRunner(QObject *parent, const QVariantList &args);

    ~VSCodeProjectsRunner() override;

    void reloadConfiguration() override;

    KConfigGroup config;

    QMap<QString, QString> projects;

    Plasma::QueryMatch addMatch(const QString &text, const QString &data, float relevance);

protected Q_SLOTS:

    void init() override;


public: // Plasma::AbstractRunner API
    void match(Plasma::RunnerContext &context) override;

    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;
};

#endif

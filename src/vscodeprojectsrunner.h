#ifndef VSCODEPROJECTSRUNNER_H
#define VSCODEPROJECTSRUNNER_H

#include <KRunner/AbstractRunner>
#include <QRegularExpression>

struct VSCodeProject {
    int position;
    QString name;
    QString path;
};

class VSCodeProjectsRunner : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
    VSCodeProjectsRunner(QObject *parent, const KPluginMetaData &data, const QVariantList &args);
    void reloadConfiguration() override;
    void match(KRunner::RunnerContext &context) override;
    void run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) override;

private:
    KRunner::QueryMatch createMatch(const QString &text, const QString &data, double relevance);
    QList<VSCodeProject> loadProjects(const QString &dirName);
    const QRegularExpression nameQueryRegex = QRegularExpression(QStringLiteral("^(vs)?code( (?<query>.+))?$"));
    QList<VSCodeProject> projects;
    bool projectNameMatches, appNameMatches;
};

#endif

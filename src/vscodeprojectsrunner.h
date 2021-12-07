#ifndef VSCODEPROJECTSRUNNER_H
#define VSCODEPROJECTSRUNNER_H

#include <KRunner/AbstractRunner>
#include <QFileSystemWatcher>
#include <QRegularExpression>

struct VSCodeProject {
    int position;
    QString name;
    QString path;
};

class VSCodeProjectsRunner : public Plasma::AbstractRunner {
Q_OBJECT

public:
    VSCodeProjectsRunner(QObject *parent, const KPluginMetaData &data, const QVariantList &args);

    const QRegularExpression nameQueryRegex = QRegularExpression(QStringLiteral("^(vs)?code( (?<query>.+))?$"));
    QList<VSCodeProject> projects;
    bool projectNameMatches, appNameMatches;

    Plasma::QueryMatch createMatch(const QString &text, const QString &data, double relevance);

public: // Plasma::AbstractRunner API
    void reloadConfiguration() override;

    void match(Plasma::RunnerContext &context) override;

    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;
private:
    QList<VSCodeProject> loadProjects(const QString &dirName);
};

#endif

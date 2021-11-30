#ifndef VSCODEPROJECTSRUNNER_H
#define VSCODEPROJECTSRUNNER_H

#include <KRunner/AbstractRunner>
#include <QFileSystemWatcher>
#include <QRegularExpression>

class VSCodeProject {
public:
    int position;
    QString name;
    QString path;

    VSCodeProject(int position, const QString &name, const QString &path) : position(position), name(name), path(path) {}
};

class VSCodeProjectsRunner : public Plasma::AbstractRunner {
Q_OBJECT

public:
    VSCodeProjectsRunner(QObject *parent, const KPluginMetaData &data, const QVariantList &args);

    ~VSCodeProjectsRunner() override;

    const QIcon icon = QIcon::fromTheme(QStringLiteral("code_runner"));
    const QRegularExpression nameQueryRegex = QRegularExpression(QStringLiteral("^(vs)?code( (?<query>.+))?$"));
    QList<VSCodeProject> projects;
    bool projectNameMatches, appNameMatches;

    Plasma::QueryMatch createMatch(const QString &text, const QString &data, double relevance);

public: // Plasma::AbstractRunner API
    void reloadConfiguration() override;

    void match(Plasma::RunnerContext &context) override;

    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;
};

#endif

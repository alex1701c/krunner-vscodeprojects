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
    VSCodeProjectsRunner(QObject *parent, const QVariantList &args);

    ~VSCodeProjectsRunner() override;

    const QIcon icon = QIcon::fromTheme("code_runner");
    const QRegularExpression nameQueryRegex = QRegularExpression("^(?:vs)?code( +[^ ]*)? *");
    QFileSystemWatcher watcher;
    QList<VSCodeProject> projects;
    bool projectNameMatches, appNameMatches;

    Plasma::QueryMatch createMatch(const QString &text, const QString &data, double relevance);

protected Q_SLOTS:

    void init() override;

    void reloadPluginConfiguration(const QString &path = "");

public: // Plasma::AbstractRunner API
    void match(Plasma::RunnerContext &context) override;

    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;
};

#endif

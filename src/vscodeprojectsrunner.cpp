#include "vscodeprojectsrunner.h"

// KF
#include <KLocalizedString>
#include <QtCore>
#include <KSharedConfig>

VSCodeProjectsRunner::VSCodeProjectsRunner(QObject *parent, const KPluginMetaData &data, const QVariantList &args)
        : Plasma::AbstractRunner(parent, data, args) {
    setObjectName(QStringLiteral("VSCodeProjectsRunner"));
}

VSCodeProjectsRunner::~VSCodeProjectsRunner() = default;

void VSCodeProjectsRunner::reloadConfiguration() {
    const QString projectManagerRoot = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Code/User/globalStorage/alefragnani.project-manager/";

    // Saved projects
    QFile file(projectManagerRoot + "projects.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QString content = file.readAll();
        const QJsonDocument d = QJsonDocument::fromJson(content.toLocal8Bit());
        if (d.isArray()) {
            int position = d.array().size();
            const auto array = d.array();
            projects.clear();

            for (const auto &item : array) {
                const auto obj = item.toObject();
                if (obj.value(QStringLiteral("enabled")).toBool()) {
                    --position;
                    const QString projectPath = obj.value(QStringLiteral("rootPath")).toString()
                                                    .replace(QLatin1String("$home"), QDir::homePath());
                    projects.append(VSCodeProject(position, obj.value(QStringLiteral("name")).toString(), projectPath));
                }
            }
        }
    }
    // git indexted projects
    QFile gitIndexFile(projectManagerRoot + "projects_cache_git.json");
    if (gitIndexFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QString content = gitIndexFile.readAll();
        const QJsonDocument d = QJsonDocument::fromJson(content.toLocal8Bit());
        if (d.isArray()) {
            int prevCount = projects.count();
            int position = d.array().size();
            const auto array = d.array();
            for (const auto &item : array) {
                const auto obj = item.toObject();
                projects.append(
                    VSCodeProject(position + prevCount, obj.value(QStringLiteral("name")).toString(), obj.value(QStringLiteral("fullPath")).toString()));
            }
        }
    }

    appNameMatches = config().readEntry("appNameMatches", true);
    projectNameMatches = config().readEntry("projectNameMatches", true);
}

void VSCodeProjectsRunner::match(Plasma::RunnerContext &context) {
    if (!context.isValid()) return;
    const QString term = context.query();

    if (projectNameMatches && (term.size() > 2 || context.singleRunnerQueryMode())) {
        for (const auto &project : qAsConst(projects)) {
            if (project.name.startsWith(term, Qt::CaseInsensitive)) {
                context.addMatch(createMatch("Open " + project.name, project.path, (double) term.length() / project.name.length()));
            }
        }
    }
    if (appNameMatches) {
        const auto match = nameQueryRegex.match(term);
        if (!match.hasMatch()) return;
        const QString projectQuery = match.captured(QStringLiteral("query"));
        for (const auto &project: qAsConst(projects)) {
            if (project.name.startsWith(projectQuery, Qt::CaseInsensitive)) {
                context.addMatch(
                        createMatch("Open " + project.name, project.path, (double) project.position / 20)
                );
            }
        }
    }
}

Plasma::QueryMatch VSCodeProjectsRunner::createMatch(const QString &text, const QString &data, double relevance) {
    auto match = Plasma::QueryMatch(this);
    match.setText(text);
    match.setData(data);
    match.setRelevance(relevance);
    match.setIcon(icon);
    return match;
}

void VSCodeProjectsRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)

    QProcess::startDetached(QStringLiteral("code"), {match.data().toString()});
}


K_EXPORT_PLASMA_RUNNER_WITH_JSON(VSCodeProjectsRunner, "vscodeprojectsrunner.json")

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "vscodeprojectsrunner.moc"


#include "vscodeprojectsrunner.h"

// KF
#include <KLocalizedString>
#include <QtCore>
#include <KSharedConfig>

VSCodeProjectsRunner::VSCodeProjectsRunner(QObject *parent, const KPluginMetaData &data, const QVariantList &args)
        : Plasma::AbstractRunner(parent, data, args) {
    setObjectName(QStringLiteral("VSCodeProjectsRunner"));
    if (!QStandardPaths::findExecutable(QStringLiteral("code")).isEmpty()) {
        projects << loadProjects(QStringLiteral("Code"));
    }
    if (!QStandardPaths::findExecutable(QStringLiteral("codium")).isEmpty()) {
        projects << loadProjects(QStringLiteral("VSCodium"));
    }
}

void VSCodeProjectsRunner::reloadConfiguration() {


    appNameMatches = config().readEntry("appNameMatches", true);
    projectNameMatches = config().readEntry("projectNameMatches", true);
}

void VSCodeProjectsRunner::match(Plasma::RunnerContext &context) {
    if (!context.isValid()) return;
    const QString term = context.query();

    if (projectNameMatches && (term.size() > 2 || context.singleRunnerQueryMode())) {
        for (const auto &project : qAsConst(projects)) {
            if (project.name.startsWith(term, Qt::CaseInsensitive)) {
                if (QFileInfo::exists(project.path)) {
                    context.addMatch(createMatch("Open " + project.name, project.path, (double) term.length() / project.name.length()));
                }
            }
        }
    }
    if (appNameMatches) {
        const auto match = nameQueryRegex.match(term);
        if (!match.hasMatch()) return;
        const QString projectQuery = match.captured(QStringLiteral("query"));
        for (const auto &project: qAsConst(projects)) {
            if (project.name.startsWith(projectQuery, Qt::CaseInsensitive)) {
                if (QFileInfo::exists(project.path)) {
                    context.addMatch(
                            createMatch("Open " + project.name, project.path, (double) project.position / 20)
                    );
                }
            }
        }
    }
}

Plasma::QueryMatch VSCodeProjectsRunner::createMatch(const QString &text, const QString &data, double relevance) {
    auto match = Plasma::QueryMatch(this);
    QUrl idUrl;
    idUrl.setScheme(id());
    idUrl.setPath(data);
    match.setId(idUrl.toString());
    match.setText(text);
    match.setData(data);
    match.setRelevance(relevance);
    match.setIcon(icon());
    return match;
}

void VSCodeProjectsRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)

    QString executable = QStringLiteral("code");
    if (!QStandardPaths::findExecutable(QStringLiteral("codium")).isEmpty()) {
        executable = QStringLiteral("codium");
    }
    QProcess::startDetached(executable, {match.data().toString()});
}

QList<VSCodeProject> VSCodeProjectsRunner::loadProjects(const QString &dirName) {
    QList<VSCodeProject> projects;
    // Saved projects

    const QString projectManagerRoot = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/"+ dirName +"/User/globalStorage/alefragnani.project-manager/";
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
                    projects.append(VSCodeProject{position, obj.value(QStringLiteral("name")).toString(), projectPath});
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
                const QString projectPath = obj.value(QStringLiteral("fullPath")).toString();
                projects.append(
                    VSCodeProject{position + prevCount, obj.value(QStringLiteral("name")).toString(), projectPath});
            }
        }
    }

    // recently opened projects from vscode
    const QString vscodeJsonFileName = QStandardPaths::locate(QStandardPaths::ConfigLocation, dirName + "/storage.json");
    QFile vscodeJsonFile(vscodeJsonFileName);
    if (vscodeJsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QString content = vscodeJsonFile.readAll();
        QJsonObject root = QJsonDocument::fromJson(content.toLocal8Bit()).object();
        auto paths = root.value(QLatin1String("openedPathsList")).toObject().value(QLatin1String("entries")).toArray();
        for (const auto &pathObj : paths) {
            QString fileOrFolderUri = pathObj.toObject().value(QLatin1String("folderUri")).toString();
            if (fileOrFolderUri.isEmpty()) {
                fileOrFolderUri = pathObj.toObject().value(QLatin1String("fileUri")).toString();
            }
            const QString localFile = QUrl(fileOrFolderUri).toLocalFile();
            if (!localFile.isEmpty()) {
                projects.append(VSCodeProject{1, QFileInfo(localFile).fileName(), localFile});
            }
        }
    }

    return projects;
}



K_EXPORT_PLASMA_RUNNER_WITH_JSON(VSCodeProjectsRunner, "vscodeprojectsrunner.json")

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "vscodeprojectsrunner.moc"


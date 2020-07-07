#include "vscodeprojectsrunner.h"

// KF
#include <KLocalizedString>
#include <QtCore>
#include <KSharedConfig>
#include <krunner_version.h>
#include "utilities.h"

VSCodeProjectsRunner::VSCodeProjectsRunner(QObject *parent, const QVariantList &args)
        : Plasma::AbstractRunner(parent, args) {
    setObjectName(QStringLiteral("VSCodeProjectsRunner"));
}

VSCodeProjectsRunner::~VSCodeProjectsRunner() = default;

void VSCodeProjectsRunner::init() {
    initializeConfigFile();
    // Add file watcher for config
    watcher.addPath( QDir::homePath() + "/.config/krunnerplugins/vscoderunnerrc");
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &VSCodeProjectsRunner::reloadPluginConfiguration);

    reloadPluginConfiguration();
}

void VSCodeProjectsRunner::reloadPluginConfiguration(const QString &path) {
    const auto config = KSharedConfig::openConfig(QStringLiteral("krunnerplugins/vscoderunnerrc"))
                        ->group("Config");

    // If the file gets edited with a text editor, it often gets replaced by the edited version
    // https://stackoverflow.com/a/30076119/9342842
    if (!path.isEmpty()) {
        if (QFile::exists(path)) {
            watcher.addPath(path);
        }
    }

    const QString filePath = config.readEntry("path", QDir::homePath() +
                                "/.config/Code/User/globalStorage/alefragnani.project-manager/projects.json");
    QFile file(filePath);
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

    appNameMatches = config.readEntry("appNameMatches", true);
    projectNameMatches = config.readEntry("projectNameMatches", true);
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


#if KRUNNER_VERSION >= QT_VERSION_CHECK(5, 72, 0)
K_EXPORT_PLASMA_RUNNER_WITH_JSON(VSCodeProjectsRunner, "plasma-runner-vscodeprojectsrunner.json")
#else
K_EXPORT_PLASMA_RUNNER(vscodeprojectsrunner, VSCodeProjectsRunner)
#endif

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "vscodeprojectsrunner.moc"


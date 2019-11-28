#include "vscodeprojectsrunner.h"

// KF
#include <KLocalizedString>
#include <QtGui/QtGui>
#include <KSharedConfig>

VSCodeProjectsRunner::VSCodeProjectsRunner(QObject *parent, const QVariantList &args)
        : Plasma::AbstractRunner(parent, args) {
    setObjectName(QStringLiteral("VSCodeProjectsRunner"));
}

VSCodeProjectsRunner::~VSCodeProjectsRunner() = default;

void VSCodeProjectsRunner::init() {
    const QString configFolder = QDir::homePath() + "/.config/krunnerplugins/";
    const QDir configDir(configFolder);
    if (!configDir.exists()) configDir.mkpath(configFolder);
    // Create file
    QFile configFile(configFolder + "vscoderunnerrc");
    if (!configFile.exists()) {
        configFile.open(QIODevice::WriteOnly);
        configFile.close();
    }
    // Add file watcher for config
    watcher.addPath(configFolder + "vscoderunnerrc");
    connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(reloadPluginConfiguration(QString)));

    reloadPluginConfiguration();
}

void VSCodeProjectsRunner::reloadPluginConfiguration(const QString &path) {
    const auto config = KSharedConfig::openConfig(QDir::homePath() + "/.config/krunnerplugins/vscoderunnerrc")
            ->group("Config");

    // If the file gets edited with a text editor, it often gets replaced by the edited version
    // https://stackoverflow.com/a/30076119/9342842
    if (!path.isEmpty()) {
        if (QFile::exists(path)) {
            watcher.addPath(path);
        }
    }

    const QString filePath =
            config.readEntry("path", QDir::homePath() +
                                     "/.config/Code/User/globalStorage/alefragnani.project-manager/projects.json");
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QString content = file.readAll();
        const QJsonDocument d = QJsonDocument::fromJson(content.toLocal8Bit());
        if (d.isArray()) {
            int position = d.array().size();
            projects.clear();

            for (const auto &item:d.array()) {
                const auto obj = item.toObject();
                if (obj.value("enabled").toBool()) {
                    --position;
                    projects.append(VSCodeProject(position, obj.value("name").toString(), obj.value("rootPath").toString()));
                }
            }
        }
    }

    appNameMatches = config.readEntry("appNameMatches", "true") == "true";
    projectNameMatches = config.readEntry("projectNameMatches", "true") == "true";
}

void VSCodeProjectsRunner::match(Plasma::RunnerContext &context) {
    if (!context.isValid()) return;
    const QString term = context.query();
    QList<Plasma::QueryMatch> matches;

    if (projectNameMatches) {
        for (const auto &project:projects) {
            if (project.name.startsWith(term, Qt::CaseInsensitive)) {
                matches.append(createMatch("Open " + project.name, project.path, (double) term.length() / project.name.length()));
            }
        }
    }
    if (appNameMatches) {
        if (term.startsWith("vscode") || term.startsWith("code")) {
            nameQueryRegex.indexIn(term);
            for (const auto &project:projects) {
                if (project.name.startsWith(nameQueryRegex.capturedTexts().at(1), Qt::CaseInsensitive)) {
                    matches.append(
                            createMatch("Open " + project.name, project.path, (double) project.position / 20)
                    );
                }
            }
        }
    }

    context.addMatches(matches);
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

    QProcess::startDetached("code", QStringList() << match.data().toString());
}


K_EXPORT_PLASMA_RUNNER(vscodeprojectsrunner, VSCodeProjectsRunner)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "vscodeprojectsrunner.moc"


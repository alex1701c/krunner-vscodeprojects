/*
   Copyright 2019 by Alex <alexkp12355@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

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


void VSCodeProjectsRunner::match(Plasma::RunnerContext &context) {
    if (!context.isValid()) return;
    const QString term = context.query();
    QList<Plasma::QueryMatch> matches;

    if (config.readEntry("projectNameMatches", "true") == "true") {
        for (const auto &key:projects.keys()) {
            if (key.startsWith(term, Qt::CaseInsensitive)) {
                matches.append(addMatch("Open " + key, projects.value(key), (float) term.length() / key.length()));
            }
        }
    }
    if (config.readEntry("programNameMatches", "true") == "true") {
        if (term.startsWith("vscode") || term.startsWith("code")) {
            QRegExp exp("(?:vs)?code ([^ ]*) *");
            exp.indexIn(term);
            if (exp.capturedTexts().size() == 2) {
                for (const auto &key:projects.keys()) {
                    if (key.startsWith(exp.capturedTexts().at(1), Qt::CaseInsensitive)) {
                        matches.append(
                                addMatch("Open " + key, projects.value(key), (float) term.length() / key.length() + 5)
                        );
                    }
                }
            } else {
                for (const auto &key:projects.keys()) {
                    matches.append(
                            addMatch("Open " + key, projects.value(key), (float) term.length() / key.length() + 5)
                    );
                }
            }
        }
    }

    context.addMatches(matches);
}

void VSCodeProjectsRunner::init() {
    config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("VSCodeProjects");
    reloadConfiguration();
}

void VSCodeProjectsRunner::reloadConfiguration() {
    QString filePath = config.readEntry("path",
                                        QDir::homePath() +
                                        "/.config/Code/User/globalStorage/alefragnani.project-manager/projects.json");
    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = file.readAll();
    QJsonDocument d = QJsonDocument::fromJson(content.toLocal8Bit());

    for (const auto &item:d.array()) {
        const auto obj = item.toObject();
        projects.insert(obj.value("name").toString(), obj.value("rootPath").toString());
    }
}

Plasma::QueryMatch VSCodeProjectsRunner::addMatch(const QString &text, const QString &data, const float relevance) {
    auto match = Plasma::QueryMatch(this);
    match.setText(text);
    match.setData(data);
    match.setRelevance(relevance);
    match.setIconName("code");
    return match;
}

void VSCodeProjectsRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)

    QProcess::startDetached("code", QStringList() << match.data().toString());
}


K_EXPORT_PLASMA_RUNNER(vscodeprojectsrunner, VSCodeProjectsRunner)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "vscodeprojectsrunner.moc"

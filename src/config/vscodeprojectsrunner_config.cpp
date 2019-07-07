/******************************************************************************
 *   Copyright 2019 by Alex <alexkp12355@gmail.com>                           *
 *                                                                            *
 *  This library is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as published  *
 *  by the Free Software Foundation; either version 2 of the License or (at   *
 *  your option) any later version.                                           *
 *                                                                            *
 *  This library is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 *  Library General Public License for more details.                          *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this library; see the file COPYING.LIB.                        *
 *  If not, see <http://www.gnu.org/licenses/>.                               *
 *****************************************************************************/

#include "vscodeprojectsrunner_config.h"
#include <KSharedConfig>
#include <KPluginFactory>
#include <krunner/abstractrunner.h>
#include <QtCore/QDir>
#include <QDebug>
#include <QtWidgets/QFileDialog>

K_PLUGIN_FACTORY(VSCodeProjectsRunnerConfigFactory,
                 registerPlugin<VSCodeProjectsRunnerConfig>("kcm_krunner_vscodeprojectsrunner");)

VSCodeProjectsRunnerConfigForm::VSCodeProjectsRunnerConfigForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

VSCodeProjectsRunnerConfig::VSCodeProjectsRunnerConfig(QWidget *parent, const QVariantList &args) : KCModule(parent,
                                                                                                             args) {
    m_ui = new VSCodeProjectsRunnerConfigForm(this);
    auto *layout = new QGridLayout(this);
    layout->addWidget(m_ui, 0, 0);

    config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("VSCodeProjects");

    m_ui->showProjectsByApplication->setChecked(config.readEntry("appNameMatches", "true") == "true");
    m_ui->showProjectsByName->setChecked(config.readEntry("projectNameMatches", "true") == "true");
    m_ui->fileLabel->setText(
            config.readEntry("path",
                             QDir::homePath() +
                             "/.config/Code/User/globalStorage/alefragnani.project-manager/projects.json"
            )
    );

    connect(m_ui->showProjectsByApplication, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->showProjectsByName, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->fileChooserButton, SIGNAL(clicked(bool)), this, SLOT(changed()));

    connect(m_ui->fileChooserButton, SIGNAL(clicked(bool)), this, SLOT(fileChooserDialog()));


    load();

}

void VSCodeProjectsRunnerConfig::save() {
    config.writeEntry("appNameMatches", m_ui->showProjectsByApplication->isChecked() ? "true" : "false");
    config.writeEntry("projectNameMatches", m_ui->showProjectsByName->isChecked() ? "true" : "false");
    config.writeEntry("path", m_ui->fileLabel->text());

    emit changed();
}

void VSCodeProjectsRunnerConfig::defaults() {
    m_ui->showProjectsByApplication->setChecked(true);
    m_ui->showProjectsByName->setChecked(true);
    m_ui->fileLabel->setText(
            QDir::homePath() + "/.config/Code/User/globalStorage/alefragnani.project-manager/projects.json"
    );
    emit changed(true);
}

void VSCodeProjectsRunnerConfig::fileChooserDialog() {
    QString jsonFile = QFileDialog::getOpenFileName(this, tr("Select file"), "", tr("Json File (*.json)"));
    if (!jsonFile.isEmpty()) {
        m_ui->fileLabel->setText(jsonFile);
    }
}


#include "vscodeprojectsrunner_config.moc"

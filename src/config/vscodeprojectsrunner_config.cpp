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

    const KConfigGroup config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("VSCodeProjects");

    m_ui->showProjectsByApplication->setChecked(config.readEntry("programNameMatches", "true") == "true");
    m_ui->showProjectsByName->setChecked(config.readEntry("projectNameMatches", "true") == "true");
    m_ui->fileLabel->setText(
            config.readEntry("path",
                             QDir::homePath() +
                             "/.config/Code/User/globalStorage/alefragnani.project-manager/projects.json"
            )
    );

    connect(m_ui->showProjectsByApplication, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->showProjectsByApplication, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->fileChooserButton, SIGNAL(clicked(bool)), this, SLOT(changed()));

    connect(m_ui->fileChooserButton, SIGNAL(clicked(bool)), this, SLOT(fileChooserDialog()));


    load();

}

void VSCodeProjectsRunnerConfig::save() {

    KCModule::save();

    emit changed();
}

void VSCodeProjectsRunnerConfig::defaults() {

    emit changed(true);
}

void VSCodeProjectsRunnerConfig::fileChooserDialog() {
    QString jsonFile = QFileDialog::getOpenFileName(this, tr("Select file"), "", tr("Json File (*.json)"));
    if (!jsonFile.isEmpty()) {
        m_ui->fileLabel->setText(jsonFile);
    }
}


#include "vscodeprojectsrunner_config.moc"

#include "vscodeprojectsrunner_config.h"
#include <KSharedConfig>
#include <KPluginFactory>
#include <krunner/abstractrunner.h>
#include <QtCore/QDir>
#include <QtWidgets/QFileDialog>

K_PLUGIN_FACTORY(VSCodeProjectsRunnerConfigFactory,
                 registerPlugin<VSCodeProjectsRunnerConfig>(QStringLiteral("kcm_krunner_vscodeprojectsrunner"));)

VSCodeProjectsRunnerConfigForm::VSCodeProjectsRunnerConfigForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

VSCodeProjectsRunnerConfig::VSCodeProjectsRunnerConfig(QWidget *parent, const QVariantList &args)
    : KCModule(parent, args) {
    m_ui = new VSCodeProjectsRunnerConfigForm(this);
    auto *layout = new QGridLayout(this);
    layout->addWidget(m_ui, 0, 0);
    config = KSharedConfig::openConfig(QStringLiteral("krunnerrc"))->group("Runners").group("vscodeprojectsrunner");

    m_ui->showProjectsByApplication->setChecked(config.readEntry("appNameMatches", true));
    m_ui->showProjectsByName->setChecked(config.readEntry("projectNameMatches", true));
    m_ui->fileLabel->setText(config.readEntry("baseDir", QDir::homePath() + "/.config/Code/User/globalStorage/alefragnani.project-manager"));
    const auto markChanged = [this](){Q_EMIT changed(true);};
    connect(m_ui->showProjectsByApplication, &QPushButton::clicked, this, markChanged);
    connect(m_ui->showProjectsByName, &QPushButton::clicked, this, markChanged);
    connect(m_ui->fileChooserButton, &QPushButton::clicked, this, markChanged);
    connect(m_ui->fileChooserButton, &QPushButton::clicked, this, &VSCodeProjectsRunnerConfig::fileChooserDialog);
}

void VSCodeProjectsRunnerConfig::save() {
    config.writeEntry("appNameMatches", m_ui->showProjectsByApplication->isChecked());
    config.writeEntry("projectNameMatches", m_ui->showProjectsByName->isChecked());
    config.writeEntry("baseDir", m_ui->fileLabel->text());
    config.sync();

    Q_EMIT changed(false);
}

void VSCodeProjectsRunnerConfig::defaults() {
    m_ui->showProjectsByApplication->setChecked(true);
    m_ui->showProjectsByName->setChecked(true);
    m_ui->fileLabel->setText(QDir::homePath() + "/.config/Code/User/globalStorage/alefragnani.project-manager");
    emit changed(true);
}

void VSCodeProjectsRunnerConfig::fileChooserDialog() {
    const QString jsonFile = QFileDialog::getExistingDirectory(this, tr("Choose basedir where projects.json file is located"));
    if (!jsonFile.isEmpty()) {
        m_ui->fileLabel->setText(jsonFile);
    }
}


#include "vscodeprojectsrunner_config.moc"

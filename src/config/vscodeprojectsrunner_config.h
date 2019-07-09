#ifndef VSCodeProjectsRunnerCONFIG_H
#define VSCodeProjectsRunnerCONFIG_H

#include "ui_vscodeprojectsrunner_config.h"
#include <KCModule>
#include <KConfigCore/KConfigGroup>

class VSCodeProjectsRunnerConfigForm : public QWidget, public Ui::VSCodeProjectsRunnerConfigUi {
Q_OBJECT

public:
    explicit VSCodeProjectsRunnerConfigForm(QWidget *parent);
};

class VSCodeProjectsRunnerConfig : public KCModule {
Q_OBJECT

public:
    explicit VSCodeProjectsRunnerConfig(QWidget *parent = nullptr, const QVariantList &args = QVariantList());

public Q_SLOTS:

    void save() override;

    void defaults() override;

    void fileChooserDialog();

private:
    VSCodeProjectsRunnerConfigForm *m_ui;

    KConfigGroup config;
};

#endif

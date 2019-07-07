/******************************************************************************
 *  Copyright %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>                         *
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

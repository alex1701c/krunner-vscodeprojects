add_definitions(-DTRANSLATION_DOMAIN=\"plasma_runner_org.kde.vscodeprojectsrunner\")

configure_file(vscodeprojectsrunner.json.in vscodeprojectsrunner.json)
set(vscodeprojectsrunner_SRCS vscodeprojectsrunner.cpp)

add_library(krunner_vscodeprojectsrunner MODULE ${vscodeprojectsrunner_SRCS})
target_link_libraries(krunner_vscodeprojectsrunner Qt::Gui KF${QT_MAJOR_VERSION}::Runner KF${QT_MAJOR_VERSION}::ConfigCore KF${QT_MAJOR_VERSION}::I18n)

install(TARGETS krunner_vscodeprojectsrunner DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/kf${QT_MAJOR_VERSION}/krunner/)
# Credit for icon: https://github.com/microsoft/vscode
set(KDE_INSTALL_ICONDIR "/usr/share/pixmaps/" CACHE STRING "Default icon directory")
install(FILES code_runner.svg DESTINATION ${KDE_INSTALL_ICONDIR})

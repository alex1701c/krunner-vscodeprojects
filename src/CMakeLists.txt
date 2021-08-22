add_definitions(-DTRANSLATION_DOMAIN=\"plasma_runner_org.kde.vscodeprojectsrunner\")

set(vscodeprojectsrunner_SRCS vscodeprojectsrunner.cpp)

add_library(krunner_vscodeprojectsrunner MODULE ${vscodeprojectsrunner_SRCS})
target_link_libraries(krunner_vscodeprojectsrunner KF5::Runner KF5::I18n)

install(TARGETS krunner_vscodeprojectsrunner DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/kf5/krunner/)
# Credit for icon: https://github.com/microsoft/vscode
set(KDE_INSTALL_ICONDIR "/usr/share/pixmaps/" CACHE STRING "Default icon directory")
install(FILES code_runner.svg DESTINATION ${KDE_INSTALL_ICONDIR})

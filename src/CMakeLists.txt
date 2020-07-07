add_definitions(-DTRANSLATION_DOMAIN=\"plasma_runner_org.kde.vscodeprojectsrunner\")

set(vscodeprojectsrunner_SRCS vscodeprojectsrunner.cpp)

add_library(krunner_vscodeprojectsrunner MODULE ${vscodeprojectsrunner_SRCS})
target_link_libraries(krunner_vscodeprojectsrunner KF5::Runner KF5::I18n)

set(kcm_krunner_vscodeprojectsrunner_SRCS
        config/vscodeprojectsrunner_config.cpp
        )

ki18n_wrap_ui(kcm_krunner_vscodeprojectsrunner_SRCS config/vscodeprojectsrunner_config.ui)
add_library(kcm_krunner_vscodeprojectsrunner MODULE ${kcm_krunner_vscodeprojectsrunner_SRCS})
target_link_libraries(kcm_krunner_vscodeprojectsrunner
        Qt5::Core
        Qt5::Gui
        KF5::CoreAddons
        KF5::ConfigCore
        KF5::I18n
        KF5::ConfigWidgets
        KF5::Runner
        )

add_dependencies(krunner_vscodeprojectsrunner kcm_krunner_vscodeprojectsrunner)


install(TARGETS kcm_krunner_vscodeprojectsrunner DESTINATION ${PLUGIN_INSTALL_DIR})
install(TARGETS krunner_vscodeprojectsrunner DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/krunner/)
install(FILES config/plasma-runner-vscodeprojectsrunner_config.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
if(KF5Runner_VERSION VERSION_LESS 5.72.0)
    install(FILES plasma-runner-vscodeprojectsrunner.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
else()
    kcoreaddons_desktop_to_json(krunner_vscodeprojectsrunner plasma-runner-vscodeprojectsrunner.desktop)
endif()
# Credit for icon: https://github.com/microsoft/vscode
set(ICON_INSTALL_DIR "/usr/share/pixmaps/" CACHE STRING "Default icon directory")
install(FILES code_runner.svg DESTINATION ${ICON_INSTALL_DIR})

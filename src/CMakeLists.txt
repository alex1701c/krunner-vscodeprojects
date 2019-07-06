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


install(TARGETS krunner_vscodeprojectsrunner kcm_krunner_vscodeprojectsrunner DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-runner-vscodeprojectsrunner.desktop config/plasma-runner-vscodeprojectsrunner_config.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

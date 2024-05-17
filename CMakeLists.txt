cmake_minimum_required(VERSION 3.16)
project(vscodeprojectsrunner)
set(CMAKE_PROJECT_VERSION  "1.2.0")
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/alex1701c/krunner-vscodeprojects")
set(CMAKE_PROJECT_AUTHOR "alex1701c")
set(CMAKE_PROJECT_AUTHOR_EMAIL "alex1701c.dev@gmx.net")

set(QT_MIN_VERSION 5.15.0)
set(KF_MIN_VERSION 5.105.0)
find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH})

include(QtVersionOption)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMDeprecationSettings)
include(FeatureSummary)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS Core Gui)
find_package(KF${QT_MAJOR_VERSION} ${KF_MIN_VERSION} REQUIRED COMPONENTS I18n Runner Config)

ecm_set_disabled_deprecation_versions(
	KF ${KF_MIN_VERSION}
	QT ${QT_MIN_VERSION}
)
add_subdirectory(src)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

# CPack configuration
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Krunner Plugin which allows you to open your recent VSCode projects")
set(CPACK_PACKAGE_NAME krunner-${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION "${CMAKE_PROJECT_VERSION}")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CMAKE_PROJECT_AUTHOR}")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "x86_64")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
set(CPACK_RPM_PACKAGE_LICENSE "LGPL 3")

# Then names of dependencies and paths might differ
if (CPACK_DEBIAN) # -DCPACK_DEBIAN=true -DKDE_INSTALL_QTPLUGINDIR=/usr/lib/x86_64-linux-gnu/qt5/plugins
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libkf5runner5 (>=${KF5Runner_VERSION})")
	set(CPACK_DEBIAN_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Debian.deb")
	set(CPACK_GENERATOR "DEB")
elseif(CPACK_FEDORA AND BUILD_WITH_QT6) # -DCPACK_FEDORA=true -DKDE_INSTALL_QTPLUGINDIR=/usr/lib64/qt6/plugins -DBUILD_WITH_QT6=ON
	set(CPACK_RPM_PACKAGE_REQUIRES "kf6-krunner >= ${KF6Runner_VERSION}")
	set(CPACK_RPM_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Fedora.rpm")
	set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/lib64/qt6;/usr/lib64/qt6/plugins;/usr/lib64/qt6/plugins/kf6;/usr/lib64/qt6/plugins/kf6/krunner;/usr/share/kservices6;/usr/share/icons")
	set(CPACK_GENERATOR "RPM")
elseif(CPACK_FEDORA AND NOT BUILD_WITH_QT6) # -DCPACK_FEDORA=true -DKDE_INSTALL_QTPLUGINDIR=/usr/lib64/qt5/plugins
	set(CPACK_RPM_PACKAGE_REQUIRES "kf5-krunner >= ${KF5Runner_VERSION}")
	set(CPACK_RPM_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Fedora.rpm")
	set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/lib64/qt5;/usr/lib64/qt5/plugins;/usr/lib64/qt5/plugins/kf5;/usr/lib64/qt5/plugins/kf5/krunner;/usr/share/kservices5;/usr/share/icons")
	set(CPACK_GENERATOR "RPM")
elseif(CPACK_OPENSUSE) # -DCPACK_OPENSUSE=true -DKDE_INSTALL_QTPLUGINDIR=/usr/lib64/qt5/plugins
	set(CPACK_RPM_PACKAGE_REQUIRES "libKF5Runner5 >= ${KF5Runner_VERSION}")
	set(CPACK_RPM_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-OpenSUSE.rpm")
	set(CPACK_GENERATOR "RPM")
endif()
# We always have to explicitely set the generator when using cpack, otherwise
# the different paths of the distros might get messed up
unset(CPACK_DEBIAN CACHE)
unset(CPACK_FEDORA CACHE)
unset(CPACK_OPENSUSE CACHE)

# These scripts will kill krunner and when it is invoked next time the plugins will be loaded
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/postinst;${CMAKE_CURRENT_SOURCE_DIR}/postrm")
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/postinst")
set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/postrm")

INCLUDE(CPack)

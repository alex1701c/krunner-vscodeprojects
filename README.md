## VSCode Projects Runner
This plugin uses the [Project Manager plugin](https://github.com/alefragnani/vscode-project-manager) from VSCode to read the projects.

## Required Dependencies (KF5)

Debian/Ubuntu:  
`sudo apt install cmake extra-cmake-modules build-essential libkf5runner-dev libkf5textwidgets-dev qtdeclarative5-dev gettext`

openSUSE  
`sudo zypper install cmake extra-cmake-modules libQt5Widgets5 libQt5Core5 libqt5-qtlocation-devel ki18n-devel ktextwidgets-devel kservice-devel krunner-devel gettext-tools kconfigwidgets-devel`  
Fedora  
`sudo dnf install cmake extra-cmake-modules kf5-ki18n-devel kf5-kservice-devel kf5-krunner-devel kf5-ktextwidgets-devel kf5-kconfigwidgets-devel qt5-qtdeclarative-devel gettext`  

## Build instructions

For archlinux users there is [AUR](https://aur.archlinux.org/packages/krunner-vscodeprojects/) or [AUR (git)](https://aur.archlinux.org/packages/krunner-vscodeprojects-git/).  

The easiest way to install is:  
`curl https://raw.githubusercontent.com/alex1701c/krunner-vscodeprojects/master/install.sh | bash`  

Or you can clone the repository and run the install.sh script.

After this you should see your runner in the system settings:

systemsettings5 (Head to "Search")

You can also launch KRunner via Alt-F2 and you will find your runner.  
Credit for icon: https://github.com/microsoft/vscode

## Screenshots

### Overview of projects
![Overview of projects](https://raw.githubusercontent.com/alex1701c/Screenshots/master/VSCodeProjectsRunner/app_overview.png)

### Search for project
![Overview of projects](https://raw.githubusercontent.com/alex1701c/Screenshots/master/VSCodeProjectsRunner/search_project.png)

### Search from overview  
This might be useful if you have other search plugins   
![Overview of projects](https://raw.githubusercontent.com/alex1701c/Screenshots/master/VSCodeProjectsRunner/app_filter.png)

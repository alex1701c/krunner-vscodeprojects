

## Required Dependencies

This plugin use the Project Manager plugin from VSCode to read the projects. 

Debian/Ubuntu:  
`sudo apt install cmake extra-cmake-modules build-essential libkf5runner-dev libkf5textwidgets-dev qtdeclarative5-dev gettext`

openSUSE  
`sudo zypper install cmake extra-cmake-modules libQt5Widgets5 libQt5Core5 libqt5-qtlocation-devel ki18n-devel ktextwidgets-devel kservice-devel krunner-devel gettext-tools`  
Fedora  
`sudo dnf install cmake extra-cmake-modules kf5-ki18n-devel kf5-kservice-devel kf5-krunner-devel kf5-ktextwidgets-devel gettext`  

## Build instructions

The easiest way to install is:  
`curl https://raw.githubusercontent.com/alex1701c/VSCodeProjectsRunner/master/install.sh | bash`

```
git clone https://github.com/alex1701c/VSCodeProjectsRunner
cd VSCodeProjectsRunner
mkdir -p build
cd build
cmake -DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins` ..
make
sudo make install
sudo curl https://raw.githubusercontent.com/vscode-icons/vscode-icons/master/icons/file_type_vscode.svg -o /usr/share/icons/code.svg
```

Restart krunner to load the runner (in a terminal type: kquitapp5 krunner;kstart5 krunner )

After this you should see your runner in the system settings:

systemsettings5 (Head to "Search")

You can also launch KRunner via Alt-F2 and you will find your runner.

## Screenshots

### Overview of projects
![Overview of projects](https://raw.githubusercontent.com/alex1701c/Screenshots/master/VSCodeProjectsRunner/app_overview.png)

### Search for project
![Overview of projects](https://raw.githubusercontent.com/alex1701c/Screenshots/master/VSCodeProjectsRunner/search_project.png)

### Search from overview  
This might be useful if you have other search plugins   
![Overview of projects](https://raw.githubusercontent.com/alex1701c/Screenshots/master/VSCodeProjectsRunner/app_filter.png)
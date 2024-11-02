## VSCode Projects Runner
This plugin uses the [Project Manager plugin](https://github.com/alefragnani/vscode-project-manager) from VSCode to read the projects.

## Required Dependencies

<details>
<summary><b>Debian/Ubuntu</b></summary>

Plasma5:  
```bash install-ubuntu-plasma5
sudo apt install git cmake extra-cmake-modules build-essential libkf5config-dev libkf5i18n-dev libkf5runner-dev libkf5dbusaddons-bin
```
Plasma6:  
```bash install-ubuntu-plasma6
sudo apt install git cmake extra-cmake-modules build-essential libkf6config-dev libkf6i18n-dev libkf6runner-dev libkf6dbusaddons-bin
```

</details>

<details>
<summary><b>OpenSUSE</b></summary>

Plasma5:  
```bash install-opensuse-plasma5
sudo zypper install git cmake extra-cmake-modules ki18n-devel kconfig-devel krunner-devel kconfigwidgets-devel kcmutils-devel kdbusaddons-tools
```
Plasma6:  
```bash install-opensuse-plasma6
sudo zypper install git cmake kf6-extra-cmake-modules kf6-ki18n-devel kf6-kconfig-devel kf6-krunner-devel kf6-kdbusaddons-tools
```

</details>

<details>
<summary><b>Fedora</b></summary>

Plasma5:  
```bash install-fedora-plasma5
sudo dnf install git cmake extra-cmake-modules kf5-ki18n-devel kf5-kconfig-devel kf5-krunner-devel kf5-kcmutils-devel
```
Plasma6:  
```bash install-fedora-plasma6
sudo dnf install git cmake extra-cmake-modules kf6-ki18n-devel kf6-kconfig-devel kf6-krunner-devel kf6-kcmutils-devel
```

</details>


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

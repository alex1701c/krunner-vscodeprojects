void initializeConfigFile() {
    const QString configFolder = QDir::homePath() + "/.config/krunnerplugins/";
    const QDir configDir(configFolder);
    if (!configDir.exists()) configDir.mkpath(configFolder);
    // Create file
    QFile configFile(configFolder + "vscoderunnerrc");
    if (!configFile.exists()) {
        configFile.open(QIODevice::WriteOnly);
        configFile.close();
    }
}
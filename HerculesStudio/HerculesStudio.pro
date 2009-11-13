TEMPLATE = app
TARGET = HerculesStudio
INCLUDEPATH += HercConfiguration \
    HercStudio \
    ui \
    HercUtilities
UI_DIR = ui
MOC_DIR = moc
OBJECTS_DIR = obj

# Input
HEADERS += HercStudio/FontPreferences.h \
    HercStudio/FontPreferences.h \
    HercConfiguration/DevicesRename.h \
    HercConfiguration/DevicesPane.h \
    HercConfiguration/DeviceTypes.h \
    HercConfiguration/DeviceMenuProcessor.h \
    HercStudio/MainWindow.h \
    HercStudio/Arguments.h \
    HercConfiguration/CardReaderProperties.h \
    HercConfiguration/ConfigFile.h \
    HercConfiguration/ConfigLine.h \
    HercConfiguration/Configuration.h \
    HercConfiguration/ConfigurationEditor.h \
    HercConfiguration/ConsoleProperties.h \
    HercConfiguration/CtcProperties.h \
    HercConfiguration/DasdProperties.h \
    HercConfiguration/DeviceConfigLine.h \
    HercConfiguration/DeviceConfigView.h \
    HercConfiguration/DeviceListView.h \
    HercConfiguration/DevicesWidget.h \
    HercConfiguration/Extended_Ui_CardReaderProperties.h \
    HercConfiguration/GenericDeviceProperties.h \
    HercConfiguration/NewReaderFile.h \
    HercConfiguration/PrinterProperties.h \
    HercConfiguration/PunchProperties.h \
    HercConfiguration/ReaderFileListView.h \
    HercConfiguration/StatusUpdateCollector.h \
    HercConfiguration/SysgProperties.h \
    HercConfiguration/SystemConfigLine.h \
    HercConfiguration/TapeProperties.h \
    HercConfiguration/TerminalProperties.h \
    HercConfiguration/VisualizedDeviceEntry.h \
    HercStudio/CommandLine.h \
    HercStudio/Environment.h \
    HercStudio/Fonts.h \
    HercStudio/HelpAbout.h \
    HercStudio/HerculesExecutor.h \
    HercStudio/HerculesStudio.h \
    HercStudio/LogRunner.h \
    HercStudio/MainPanel.h \
    HercStudio/Mips.h \
    HercStudio/NamedPipe.h \
    HercStudio/PanelButton.h \
    HercStudio/Preferences.h \
    HercStudio/PreferencesWin.h \
    HercStudio/Psw.h \
    HercStudio/Recovery.h \
    HercStudio/Regs32.h \
    HercStudio/Regs64.h \
    HercStudio/Runner.h \
    HercStudio/StatusRunner.h \
    HercStudio/StringTokenizer.h \
    HercStudio/SynchronizedQueue.h \
    HercStudio/Watchdog.h \
    HercUtilities/Dasdcat.h \
    HercUtilities/Dasdconv.h \
    HercUtilities/Dasdcopy.h \
    HercUtilities/DasdInit.h \
    HercUtilities/DasdIsup.h \
    HercUtilities/DasdLoad.h \
    HercUtilities/Dasdls.h \
    HercUtilities/DasdTab.h \
    HercUtilities/LineEditWithFocus.h \
    HercUtilities/SystemUtils.h \
    HercUtilities/UtilityExecutor.h \
    HercUtilities/UtilityRunner.h
FORMS += HercConfiguration/DevicesRename.ui \
    HercUtilities/DasdInit.ui \
    HercStudio/MainWindow.ui \
    HercConfiguration/Devices.ui \
    HercConfiguration/CardReaderProperties.ui \
    HercConfiguration/Configuration.ui \
    HercConfiguration/ConsoleProperties.ui \
    HercConfiguration/CtcProperties.ui \
    HercConfiguration/DasdProperties.ui \
    HercConfiguration/hercconfiguration.ui \
    HercConfiguration/PrinterProperties.ui \
    HercConfiguration/PunchProperties.ui \
    HercConfiguration/TapeProperties.ui \
    HercConfiguration/TerminalProperties.ui \
    HercStudio/helpabout.ui \
    HercStudio/preferenceswin.ui \
    HercStudio/Recovery.ui \
    HercUtilities/Dasdcat.ui \
    HercUtilities/Dasdconv.ui \
    HercUtilities/Dasdcopy.ui \
    HercUtilities/DasdIsup.ui \
    HercUtilities/DasdLoad.ui \
    HercUtilities/Dasdls.ui
SOURCES += HercStudio/FontPreferences.cpp \
    HercStudio/FontPreferences.cpp \
    HercConfiguration/DevicesRename.cpp \
    HercConfiguration/DevicesPane.cpp \
    HercConfiguration/DeviceTypes.cpp \
    HercConfiguration/DeviceMenuProcessor.cpp \
    HercStudio/MainWindow.cpp \
    HercStudio/Arguments.cpp \
    HercConfiguration/CardReaderProperties.cpp \
    HercConfiguration/ConfigFile.cpp \
    HercConfiguration/ConfigLine.cpp \
    HercConfiguration/Configuration.cpp \
    HercConfiguration/ConfigurationEditor.cpp \
    HercConfiguration/ConsoleProperties.cpp \
    HercConfiguration/CtcProperties.cpp \
    HercConfiguration/DasdProperties.cpp \
    HercConfiguration/DeviceConfigLine.cpp \
    HercConfiguration/DeviceConfigView.cpp \
    HercConfiguration/DeviceListView.cpp \
    HercConfiguration/DevicesWidget.cpp \
    HercConfiguration/Extended_Ui_CardReaderProperties.cpp \
    HercConfiguration/GenericDeviceProperties.cpp \
    HercConfiguration/NewReaderFile.cpp \
    HercConfiguration/PrinterProperties.cpp \
    HercConfiguration/PunchProperties.cpp \
    HercConfiguration/ReaderFileListView.cpp \
    HercConfiguration/StatusUpdateCollector.cpp \
    HercConfiguration/SysgProperties.cpp \
    HercConfiguration/SystemConfigLine.cpp \
    HercConfiguration/TapeProperties.cpp \
    HercConfiguration/TerminalProperties.cpp \
    HercConfiguration/VisualizedDeviceEntry.cpp \
    HercStudio/CommandLine.cpp \
    HercStudio/Environment.cpp \
    HercStudio/Fonts.cpp \
    HercStudio/HelpAbout.cpp \
    HercStudio/HerculesExecutor.cpp \
    HercStudio/LogRunner.cpp \
    HercStudio/main.cpp \
    HercStudio/MainPanel.cpp \
    HercStudio/Mips.cpp \
    HercStudio/NamedPipe.cpp \
    HercStudio/PanelButton.cpp \
    HercStudio/Preferences.cpp \
    HercStudio/PreferencesWin.cpp \
    HercStudio/Psw.cpp \
    HercStudio/Recovery.cpp \
    HercStudio/Regs32.cpp \
    HercStudio/Regs64.cpp \
    HercStudio/Runner.cpp \
    HercStudio/StatusRunner.cpp \
    HercStudio/SynchronizedQueue.cpp \
    HercStudio/Watchdog.cpp \
    HercUtilities/Dasdcat.cpp \
    HercUtilities/Dasdconv.cpp \
    HercUtilities/Dasdcopy.cpp \
    HercUtilities/DasdInit.cpp \
    HercUtilities/DasdIsup.cpp \
    HercUtilities/DasdLoad.cpp \
    HercUtilities/Dasdls.cpp \
    HercUtilities/DasdTab.cpp \
    HercUtilities/LineEditWithFocus.cpp \
    HercUtilities/SystemUtils.cpp \
    HercUtilities/UtilityExecutor.cpp \
    HercUtilities/UtilityRunner.cpp
RESOURCES += HercStudio/HercStudio.qrc
target.path = /usr/local/bin
INSTALLS += target

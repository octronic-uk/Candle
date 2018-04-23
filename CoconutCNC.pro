#-------------------------------------------------
#
# Project created by QtCreator 2014-05-23T17:51:21
#
#-------------------------------------------------

QT       = core gui opengl serialport sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32: {
    QT += winextras
    DEFINES += WINDOWS
    QMAKE_LFLAGS += "-Wl,--large-address-aware"
    QMAKE_CXXFLAGS_DEBUG += -g3 -pg
    QMAKE_LFLAGS_DEBUG += -pg -lgmon
}

unix:!macx {
    DEFINES += UNIX #GL_GLEXT_PROTOTYPES
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/libs\'"
}

contains(QT_CONFIG, opengles.) {
    warning("GL ES detected. VAO will be disabled.")
    DEFINES += GLES
    INSTALLS += target
    target.path = /opt/
}

unix {
    QMAKE_CXXFLAGS += -std=c++0x
}

TARGET = CoconutCNC
TEMPLATE = app
VERSION = 0.01a
#RC_ICONS += images/coconut.ico
ICON = CoconutCNC.icns

DEFINES += sNan=\"65536\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TRANSLATIONS += \
    translations/coconut_es.ts \
    translations/coconut_fr.ts \
    translations/coconut_pt.ts \
    translations/coconut_ru.ts

SOURCES +=\
    Main.cpp \
    Model/Gcode/GcodeTableModel.cpp \
    Model/Gcode/Parser/ArcProperties.cpp \
    Model/Gcode/Parser/GcodeParser.cpp \
    Model/Gcode/Parser/LineSegment.cpp \
    Model/Gcode/Parser/PointSegment.cpp \
    Model/StatusModel.cpp \
    Model/Gcode/GcodeFileModel.cpp \
    Model/RecentFilesModel.cpp \
    Model/Settings/AbstractSettingsModel.cpp \
    Model/Settings/Ini/IniFileSettingsModel.cpp \
    Model/Settings/Settings.cpp \
    Model/Grbl/GrblMachineModel.cpp \
    Model/Gcode/GcodeCommand.cpp \
    Model/Grbl/GrblResponse.cpp \
    Model/Gcode/Parser/GcodeViewParser.cpp \
    Model/Settings/Sql/SqlSettingsModel.cpp \
    Model/Settings/Sql/AbstractDatabaseRecord.cpp \
    \
    Model/Settings/ToolHolder/ToolHolder.cpp \
    Model/Settings/ToolHolder/ToolHolderGeometry.cpp \
    Model/Settings/ToolHolder/ToolHolderGeometryTableModel.cpp \
    Model/Settings/ToolHolder/ToolHolderListModel.cpp \
    \
    Controller/Settings/ToolHolderFormController.cpp \
    Controller/Settings/ToolFormController.cpp \
    \
    Model/Settings/Tool/Tool.cpp \
    Model/Settings/Tool/ToolGeometry.cpp \
    Model/Settings/Tool/ToolGeometryTableModel.cpp \
    Model/Settings/Tool/ToolListModel.cpp \
    \
    Model/RecentFile.cpp \
    View/Drawers/GridDrawer.cpp \
    View/Widgets/GLWidget.cpp \
    View/Drawers/GcodeDrawer.cpp \
    View/Drawers/OriginDrawer.cpp \
    View/Drawers/SelectionDrawer.cpp \
    View/Drawers/ShaderDrawable.cpp \
    View/Drawers/ToolDrawer.cpp \
    Controller/Settings/ConnectionFormController.cpp \
    Controller/Settings/InterfaceFormController.cpp \
    Controller/Settings/MachineFormController.cpp \
    Controller/ControlFormController.cpp \
    Controller/ProgramFormController.cpp \
    Controller/StateFormController.cpp \
    Controller/UserCommandsFormController.cpp \
    Controller/VisualisationFormController.cpp \
    Controller/AbstractFormController.cpp \
    Controller/MainFormController.cpp \
    Controller/Settings/SettingsFormController.cpp \
    Controller/JogFormController.cpp \
    Controller/ConsoleFormController.cpp \
    Controller/AboutFormController.cpp \
    Utils/IndexOutOfBoundsException.cpp \
    Utils/GcodeCommandNotFoundException.cpp \
    Model/Settings/Profile/Profile.cpp \
    Model/Settings/Profile/ProfilesListModel.cpp \
    Model/Settings/ConnectionSettings.cpp \
    Model/Settings/InterfaceSettings.cpp \
    Model/Settings/MachineSettings.cpp \
    Controller/Settings/NameDialogController.cpp \
    Model/Gcode/GcodeMarkerListModel.cpp \
    Controller/Settings/GrblConfigurationFormController.cpp \
    Model/Grbl/GrblConfigurationModel.cpp \
    Controller/OverrideFormController.cpp \
    View/Widgets/Camera.cpp \
    View/Drawers/SafePositionDrawer.cpp

HEADERS  += \
    Model/Gcode/Parser/ArcProperties.h \
    Model/Gcode/Parser/GcodeParser.h \
    Model/Gcode/Parser/LineSegment.h \
    Model/Gcode/Parser/PointSegment.h \
    Model/Gcode/GcodeTableModel.h \
    Model/Settings/Ini/IniFileSettingsModel.h \
    Model/StatusModel.h \
    Model/Gcode/GcodeFileModel.h \
    Model/RecentFilesModel.h \
    Model/Settings/AbstractSettingsModel.h \
    Model/SerialBaudRate.h \
    Model/Grbl/GrblMachineModel.h \
    Model/Gcode/GcodeCommand.h \
    Model/Grbl/GrblResponse.h \
    Model/Grbl/GrblMachineState.h \
    Model/Gcode/Parser/GcodeViewParser.h \
    Model/Settings/Sql/SqlSettingsModel.h \
    Model/Settings/Profile/ProfilesListModel.h \
    Model/Settings/Profile/Profile.h \
    \
    Model/Settings/ToolHolder/ToolHolder.h \
    Model/Settings/ToolHolder/ToolHolderGeometryTableModel.h \
    Model/Settings/ToolHolder/ToolHolderGeometry.h \
    Model/Settings/ToolHolder/ToolHolderListModel.h \
    Controller/Settings/ToolHolderFormController.h \
\
    Model/RecentFile.h \
    View/Drawers/GridDrawer.h \
    View/Drawers/GcodeDrawer.h \
    View/Drawers/OriginDrawer.h \
    View/Drawers/SelectionDrawer.h \
    View/Drawers/ShaderDrawable.h \
    View/Drawers/ToolDrawer.h \
    View/Widgets/GLWidget.h \
    Controller/JogFormController.h \
    Controller/AboutFormController.h \
    Controller/ConsoleFormController.h \
    Controller/Settings/SettingsFormController.h \
    Controller/MainFormController.h \
    Controller/AbstractFormController.h \
    Controller/CancelException.h \
    Controller/Settings/ConnectionFormController.h \
    Controller/Settings/InterfaceFormController.h \
    Controller/Settings/MachineFormController.h \
    Controller/Settings/ToolFormController.h \
    Controller/ControlFormController.h \
    Controller/ProgramFormController.h \
    Controller/StateFormController.h \
    Controller/UserCommandsFormController.h \
    Controller/VisualisationFormController.h \
    Utils/IndexOutOfBoundsException.h \
    Utils/GcodeCommandNotFoundException.h \
    Utils/Interpolation.h \
    Utils/Util.h \
    Model/Settings/Sql/AbstractDatabaseRecord.h \
    Model/Settings/Tool/Tool.h \
    Model/Settings/Tool/ToolGeometry.h \
    Model/Settings/Tool/ToolGeometryTableModel.h \
    Model/Settings/Tool/ToolListModel.h \
    \
    Model/Settings/Settings.h \
    Model/Settings/Sql/SqlStrings.h \
    Model/Settings/ConnectionSettings.h \
    Model/Settings/InterfaceSettings.h \
    Model/Settings/MachineSettings.h \
    Controller/Settings/NameDialogController.h \
    Model/Gcode/GcodeMarkerListModel.h \
    Controller/Settings/GrblConfigurationFormController.h \
    Model/Grbl/GrblConfigurationModel.h \
    Controller/OverrideFormController.h \
    View/Widgets/Camera.h \
    View/Drawers/SafePositionDrawer.h

FORMS    += \
    View/AboutForm.ui \
    View/ConsoleForm.ui \
    View/ControlForm.ui \
    View/FeedForm.ui \
    View/JogForm.ui \
    View/MainForm.ui \
    View/ProgramForm.ui \
    View/Settings/SettingsForm.ui \
    View/StateForm.ui \
    View/UserCommandsForm.ui \
    View/VisualisationForm.ui \
    View/Settings/ConnectionSettingsForm.ui \
    View/Settings/ToolHolderForm.ui \
    View/Settings/InterfaceForm.ui \
    View/Settings/MachineForm.ui \
    View/Settings/ToolForm.ui \
    View/Settings/NameDialog.ui \
    View/Settings/GrblConfigurationForm.ui \
    View/OverrideForm.ui

DEFINES += _USE_MATH_DEFINES
#DEFINES += QT_SHAREDPOINTER_TRACK_POINTERS

RESOURCES += \
    Resources/Shaders.qrc \
    Resources/Images.qrc \
    Resources/Misc.qrc

CONFIG += c++11

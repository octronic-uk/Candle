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
    target.path = /home/pi
}

unix {
    QMAKE_CXXFLAGS += -std=c++0x
}

TARGET = CocoanutCNC
TEMPLATE = app
VERSION = 1.0.12
RC_ICONS += images/cocoanut.ico

DEFINES += sNan=\"65536\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TRANSLATIONS += \
    translations/cocoanut_es.ts \
    translations/cocoanut_fr.ts \
    translations/cocoanut_pt.ts \
    translations/cocoanut_ru.ts

SOURCES +=\
    Main.cpp \
    Controller/MainFormController.cpp \
    Controller/SettingsFormController.cpp \
    Controller/JogFormController.cpp \
    Controller/ConsoleFormController.cpp \
    Controller/AboutFormController.cpp \
    Model/Tables/GcodeTableModel.cpp \
    Model/Tables/HeightMapTableModel.cpp \
    Model/Parser/ArcProperties.cpp \
    Model/Parser/GcodeParser.cpp \
    Model/Parser/GcodePreprocessorUtils.cpp \
    Model/Parser/GcodeViewParse.cpp \
    Model/Parser/LineSegment.cpp \
    Model/Parser/PointSegment.cpp \
    View/Drawers/GcodeDrawer.cpp \
    View/Drawers/HeightMapBorderDrawer.cpp \
    View/Drawers/HeightMapGridDrawer.cpp \
    View/Drawers/HeightMapInterpolationDrawer.cpp \
    View/Drawers/OriginDrawer.cpp \
    View/Drawers/SelectionDrawer.cpp \
    View/Drawers/ShaderDrawable.cpp \
    View/Drawers/ToolDrawer.cpp \
    Controller/ControlFormController.cpp \
    Controller/FeedFormController.cpp \
    Controller/HeightMapFormController.cpp \
    Controller/ProgramFormController.cpp \
    Controller/SpindleFormController.cpp \
    Controller/StateFormController.cpp \
    Controller/UserCommandsFormController.cpp \
    Controller/VisualisationFormController.cpp \
    Controller/AbstractFormController.cpp \
    Model/SerialPortModel.cpp \
    Model/StatusModel.cpp \
    Model/GcodeFileModel.cpp \
    Model/HeightMapFileModel.cpp \
    Model/RecentFilesModel.cpp \
    Model/RecentHeightMapFilesModel.cpp \
    Model/GcodeItem.cpp \
    View/Widgets/GLWidget.cpp \
    Model/Settings/AbstractSettingsModel.cpp \
    Model/Settings/IniFileSettingsModel.cpp \
    View/Drawers/GridDrawer.cpp


HEADERS  += \
    Controller/JogFormController.h \
    Controller/AboutFormController.h \
    Controller/ConsoleFormController.h \
    Controller/SettingsFormController.h \
    Controller/MainFormController.h \
    Model/Parser/ArcProperties.h \
    Model/Parser/GcodeParser.h \
    Model/Parser/GcodePreprocessorUtils.h \
    Model/Parser/GcodeViewParse.h \
    Model/Parser/LineSegment.h \
    Model/Parser/PointSegment.h \
    Model/Tables/HeightMapTableModel.h \
    Model/Tables/GcodeTableModel.h \
    View/Drawers/GcodeDrawer.h \
    View/Drawers/HeightMapBorderDrawer.h \
    View/Drawers/HeightMapGridDrawer.h \
    View/Drawers/HeightMapInterpolationDrawer.h \
    View/Drawers/OriginDrawer.h \
    View/Drawers/SelectionDrawer.h \
    View/Drawers/ShaderDrawable.h \
    View/Drawers/ToolDrawer.h \
    View/Widgets/GLWidget.h \
    Utils/Interpolation.h \
    Utils/Util.h \
    Utils/MachineState.h \
    Controller/ControlFormController.h \
    Controller/FeedFormController.h \
    Controller/HeightMapFormController.h \
    Controller/ProgramFormController.h \
    Controller/SpindleFormController.h \
    Controller/StateFormController.h \
    Controller/UserCommandsFormController.h \
    Controller/VisualisationFormController.h \
    Controller/AbstractFormController.h \
    Model/SerialPortModel.h \
    Model/Settings/IniFileSettingsModel.h \
    Model/StatusModel.h \
    Model/GcodeFileModel.h \
    Model/HeightMapFileModel.h \
    Model/RecentFilesModel.h \
    Model/RecentHeightMapFilesModel.h \
    Controller/CommandAttributes.h \
    Controller/CommandQueue.h \
    Controller/CancelException.h \
    Model/GcodeItem.h \
    Model/GcodeItemState.h \
    Model/Settings/AbstractSettingsModel.h \
    View/Drawers/GridDrawer.h \
    Model/SerialBaudRate.h

FORMS    += \
    View/AboutForm.ui \
    View/ConsoleForm.ui \
    View/ControlForm.ui \
    View/FeedForm.ui \
    View/HeightMapForm.ui \
    View/JogForm.ui \
    View/MainForm.ui \
    View/ProgramForm.ui \
    View/SettingsForm.ui \
    View/SpindleForm.ui \
    View/StateForm.ui \
    View/UserCommandsForm.ui \
    View/VisualisationForm.ui

DEFINES += _USE_MATH_DEFINES

RESOURCES += \
    Resources/shaders.qrc \
    Resources/images.qrc

CONFIG += c++11

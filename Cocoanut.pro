#-------------------------------------------------
#
# Project created by QtCreator 2014-05-23T17:51:21
#
#-------------------------------------------------

QT       = core gui opengl serialport
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

TARGET = Cocoanut
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
    Model/Tables/GCodeTableModel.cpp \
    Model/Tables/HeightMapTableModel.cpp \
    Model/Parser/ArcProperties.cpp \
    Model/Parser/GCodeParser.cpp \
    Model/Parser/GCodePreprocessorUtils.cpp \
    Model/Parser/GCodeViewParse.cpp \
    Model/Parser/LineSegment.cpp \
    Model/Parser/PointSegment.cpp \
    View/Drawers/GCodeDrawer.cpp \
    View/Drawers/HeightMapBorderDrawer.cpp \
    View/Drawers/HeightMapGridDrawer.cpp \
    View/Drawers/HeightMapInterpolationDrawer.cpp \
    View/Drawers/OriginDrawer.cpp \
    View/Drawers/SelectionDrawer.cpp \
    View/Drawers/ShaderDrawable.cpp \
    View/Drawers/ToolDrawer.cpp \
    View/Widgets/Widget.cpp \
    View/Widgets/StyledToolButton.cpp \
    View/Widgets/ScrollArea.cpp \
    View/Widgets/GroupBox.cpp \
    View/Widgets/GLWidget.cpp \
    View/Widgets/ComboBox.cpp \
    View/Widgets/ColorPicker.cpp \
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
    Model/SettingsModel.cpp \
    Model/StatusModel.cpp \
    Model/GCodeFileModel.cpp \
    Model/HeightMapFileModel.cpp \
    Model/RecentFilesModel.cpp \
    Model/RecentHeightMapFilesModel.cpp \
    Model/GcodeItem.cpp


HEADERS  += \
    Controller/JogFormController.h \
    Controller/AboutFormController.h \
    Controller/ConsoleFormController.h \
    Controller/SettingsFormController.h \
    Controller/MainFormController.h \
    Model/Parser/ArcProperties.h \
    Model/Parser/GCodeParser.h \
    Model/Parser/GCodePreprocessorUtils.h \
    Model/Parser/GCodeViewParse.h \
    Model/Parser/LineSegment.h \
    Model/Parser/PointSegment.h \
    Model/Tables/HeightMapTableModel.h \
    Model/Tables/GCodeTableModel.h \
    View/Drawers/GCodeDrawer.h \
    View/Drawers/HeightMapBorderDrawer.h \
    View/Drawers/HeightMapGridDrawer.h \
    View/Drawers/HeightMapInterpolationDrawer.h \
    View/Drawers/OriginDrawer.h \
    View/Drawers/SelectionDrawer.h \
    View/Drawers/ShaderDrawable.h \
    View/Drawers/ToolDrawer.h \
    View/Widgets/ColorPicker.h \
    View/Widgets/ComboBox.h \
    View/Widgets/GLWidget.h \
    View/Widgets/GroupBox.h \
    View/Widgets/ScrollArea.h \
    View/Widgets/StyledToolButton.h \
    View/Widgets/Widget.h \
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
    Model/SettingsModel.h \
    Model/StatusModel.h \
    Model/GCodeFileModel.h \
    Model/HeightMapFileModel.h \
    Model/RecentFilesModel.h \
    Model/RecentHeightMapFilesModel.h \
    Controller/CommandAttributes.h \
    Controller/CommandQueue.h \
    Controller/CancelException.h \
    Model/GcodeItem.h

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

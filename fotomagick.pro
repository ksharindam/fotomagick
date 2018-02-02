
TEMPLATE = app
TARGET = fotomagick
DEPENDPATH += .
INCLUDEPATH += /usr/include/GraphicsMagick /usr/include/opencv2
LIBS += -lGraphicsMagick++ -lopencv_core -lopencv_imgproc
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
OBJECTS_DIR = build
mytarget.commands += $${QMAKE_MKDIR} build

# Input
HEADERS += canvas.h main.h exif.h filters.h \
        tools/tools.h \
        tools/iscissor_tool.h \
        tools/grabcut_tool.h
SOURCES += canvas.cpp main.cpp exif.cpp filters.cpp \
        tools/tools.cpp \
        tools/iscissor_tool.cpp \
        tools/grabcut_tool.cpp
FORMS += mainwindow.ui
RESOURCES += resources.qrc

# install
INSTALLS += target
target.path = /usr/local/bin


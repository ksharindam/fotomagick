
TEMPLATE = app
TARGET = fotomagick
DEPENDPATH += .
INCLUDEPATH += /usr/include/GraphicsMagick
LIBS += -lGraphicsMagick++
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
OBJECTS_DIR = build
mytarget.commands += $${QMAKE_MKDIR} build

# Input
HEADERS += canvas.h main.h exif.h filters.h \
        tools/tools.h \
        tools/iscissor_tool.h

SOURCES += canvas.cpp main.cpp exif.cpp filters.cpp \
        tools/tools.cpp \
        tools/iscissor_tool.cpp

FORMS += mainwindow.ui
RESOURCES += resources.qrc

# install
INSTALLS += target
target.path = /usr/local/bin


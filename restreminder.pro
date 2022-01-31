HEADERS       = \
    main_windows.h \
    setting.h
SOURCES       = main.cpp \
                main_windows.cpp \
                setting.cpp
RESOURCES     = restreminder.qrc

QT += widgets
requires(qtConfig(combobox))

# install
target.path = /home/davidfan/Work/work-rest-reminder/restreminder
INSTALLS += target

QMAKE_CXXFLAGS += -std=c++11

DISTFILES += \
    setting.json

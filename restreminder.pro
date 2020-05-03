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
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/desktop/restreminder
INSTALLS += target

QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -mmacosx-version-min=10.7
QMAKE_LFLAGS += -mmacosx-version-min=10.7

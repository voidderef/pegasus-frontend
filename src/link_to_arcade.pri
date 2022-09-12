# Link the project that includes this file to the Arcade (backend)

# based on the auto-generated code by Qt Creator

win32:CONFIG(release, debug|release): LIBS += "-L$${TOP_BUILDDIR}/src/arcade/release/"
else:win32:CONFIG(debug, debug|release): LIBS += "-L$${TOP_BUILDDIR}/src/arcade/debug/"
else:unix: LIBS += "-L$${TOP_BUILDDIR}/src/arcade/"

LIBS += -larcade
INCLUDEPATH += \
    "$${TOP_SRCDIR}/src" \
    "$${TOP_SRCDIR}/src/arcade"
DEPENDPATH += \
    "$${TOP_SRCDIR}/src/arcade"

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += "$${TOP_BUILDDIR}/src/arcade/release/libarcade.a"
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += "$${TOP_BUILDDIR}/src/arcade/debug/libarcade.a"
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += "$${TOP_BUILDDIR}/src/arcade/release/arcade.lib"
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += "$${TOP_BUILDDIR}/src/arcade/debug/arcade.lib"
else:unix: PRE_TARGETDEPS += "$${TOP_BUILDDIR}/src/arcade/libarcade.a"

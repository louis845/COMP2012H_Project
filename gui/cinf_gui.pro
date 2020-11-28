QT       += core gui webengine webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../sources/math/Field.cpp \
    ../sources/math/R.cpp \
    ../sources/math/Ring.cpp \
    ../sources/math/double/Double.cpp \
    ../sources/math/double/DoubleComplex.cpp \
    ../sources/math/fraction/Fraction.cpp \
    ../sources/math/linear/CoupledOperations.cpp \
    ../sources/math/linear/LinOpsSteps.cpp \
    ../sources/math/linear/LinearOperations.cpp \
    ../sources/math/long/Long.cpp \
    ../sources/math/long/LongComplex.cpp \
    ../sources/math/long/mpz_wrapper.cpp \
    ../sources/math/polynomial/Polynomial.cpp \
    ../sources/math/tools.cpp \
    ../sources/steps/Step.cpp \
    ../sources/steps/StepsHistory.cpp \
    ../sources/utils/math_wrapper.cpp \
    ../sources/utils/ocr_api.cpp \
    ../sources/utils/test_ocr.cpp \
    input_key.cpp \
    main.cpp \
    begin_widget.cpp \
    show_capture.cpp \
    solution_widget.cpp \
    window_capture.cpp

HEADERS += \
    ../sources/math/Field.h \
    ../sources/math/R.h \
    ../sources/math/Ring.h \
    ../sources/math/double/Double.h \
    ../sources/math/double/DoubleComplex.h \
    ../sources/math/fraction/Fraction.h \
    ../sources/math/linear/CoupledOperations.h \
    ../sources/math/linear/GenDiagonalization.h \
    ../sources/math/linear/InvariantOperations.h \
    ../sources/math/linear/LinOpsSteps.h \
    ../sources/math/linear/LinearOperations.h \
    ../sources/math/linear/Orthogonalisation.h \
    ../sources/math/long/Long.h \
    ../sources/math/long/LongComplex.h \
    ../sources/math/long/LongType.h \
    ../sources/math/long/mpz_wrapper.h \
    ../sources/math/polynomial/Polynomial.h \
    ../sources/math/tools.h \
    ../sources/steps/Step.h \
    ../sources/steps/StepsHistory.h \
    ../sources/utils/math_wrapper.h \
    ../sources/utils/ocr_api.h \
    begin_widget.h \
    input_key.h \
    show_capture.h \
    solution_widget.h \
    window_capture.h \

FORMS += \
    begin_widget.ui \
    input_key.ui \
    show_capture.ui \
    solution_widget.ui

INCLUDEPATH += \
    ../sources/ \
    ../libraries/gmp_621_msvc_x64 \
    ../libraries/Armadillo/include \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libraries/gmp_621_msvc_x64/ -llibgmp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libraries/gmp_621_msvc_x64/ -llibgmp

DEPENDPATH += ../libraries/gmp_621_msvc_x64

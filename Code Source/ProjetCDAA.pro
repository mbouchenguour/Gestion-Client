QT       += gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    associationinteractiontodo.cpp \
    bdd.cpp \
    contact.cpp \
    editcontact.cpp \
    formajoutcontact.cpp \
    gestionait.cpp \
    gestioncontact.cpp \
    gestiondate.cpp \
    gestioninteraction.cpp \
    interaction.cpp \
    main.cpp \
    mainwindow.cpp \
    rechercheinteractions.cpp \
    recherchetodos.cpp \
    todo.cpp

HEADERS += \
    associationinteractiontodo.h \
    bdd.h \
    contact.h \
    editcontact.h \
    formajoutcontact.h \
    gestionait.h \
    gestioncontact.h \
    gestiondate.h \
    gestioninteraction.h \
    interaction.h \
    mainwindow.h \
    rechercheinteractions.h \
    recherchetodos.h \
    todo.h \
    date/*.h

FORMS += \
    editcontact.ui \
    formajoutcontact.ui \
    mainwindow.ui \
    rechercheinteractions.ui \
    recherchetodos.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Ressources.qrc

DISTFILES +=

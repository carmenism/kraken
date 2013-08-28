#-------------------------------------------------
#
# Project created by QtCreator 2012-12-21T13:15:22
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = KRAKEN
TEMPLATE = app

#LIBS += -lglut
#LIBS += -LC:\glut

#INCLUDEPATH = c:/Carmen/thesis/lib

SOURCES += main.cpp\
        ms_prod_mainwindow.cpp \
    PredationModuleTypeI.cpp \
    Parameters.cpp \
    ModelFormParameters.cpp \
    ModelComponent.cpp \
    Model.cpp \
    InteractionMatrix.cpp \
    HarvestModuleExploitRate.cpp \
    HarvestModuleEffortQ.cpp \
    GrowthModuleLogistic.cpp \
    CompetitionModuleWithinGroup.cpp \
    CompetitionModuleBetweenGroup.cpp \
    PredationModuleTypeIII.cpp \
    PredationModuleTypeII.cpp \
    TimeSeriesObservations.cpp \
    MeasureOfFitness.cpp \
    GA_Estimator.cpp \
    HarvestModuleCatch.cpp \
    CompetitionModuleWithinGroup_FillToGuildK.cpp \
    CompetitionBetweenSpeciesNoK.cpp \
    GrowthModuleLinear.cpp \ 
	Shape.cpp \
	Rectangle.cpp \
	MyQGLWidget.cpp

HEADERS  += ms_prod_mainwindow.h \
    PredationModuleTypeIII.h \
    PredationModuleTypeII.h \
    PredationModuleTypeI.h \
    Parameters.h \
    ModelFormParameters.h \
    ModelComponent.h \
    InteractionMatrix.h \
    HarvestModuleExploitRate.h \
    HarvestModuleEffortQ.h \
    GrowthModuleLogistic.h \
    CompetitionModuleWithinGroup.h \
    CompetitionModuleBetweenGroup.h \
    TimeSeriesObservations.h \
    MeasureOfFitness.h \
    Model.h \
    GA_Estimator.h \
    ui_ms_prod_mainwindow.h \
    HarvestModuleCatch.h \
    CompetitionModuleWithinGroup_FillToGuildK.h \
    MersenneTwister.h \
    CompetitionModuleBetweenSpeciesNoK.h \
    GrowthModuleLinear.h \ 
	Shape.h \
	Rectangle.h \
	MyQGLWidget.h

FORMS    += ms_prod_mainwindow.ui

OTHER_FILES += \
    MS_PROD_MAIN_IMAGE.jpg

RESOURCES += \
    MS_PROD_resource_QT.qrc

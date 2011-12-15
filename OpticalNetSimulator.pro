# #####################################################################
# Automatically generated by qmake (2.01a) vie abr 15 23:44:34 2011
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . \
    /usr/local/include/aleph
LIBS += -L/usr/local/lib \
    -lAleph \
    -lnana \
    -lc \
    -lgsl \
    -lgslcblas \
    -lgmp \
    -lmpfr \
    -lasprintf
CONFIG += warn_off

# Input
HEADERS += \
    grid.H \
    misc.H \
    package.H \
    grid_window.H \
    grid_panel.H \
    grid_panel_listener.H \
    simulator.H \
    sim_listener.H \
    statistics_per_element.H \
    statistics.H \
    configuration.H \
    configuration_panel.H \
    statistics_panel.H \
    statistics_shower_panel.H \
    minimum_deflection.H
SOURCES += \
    grid_window.C \
    grid_panel.C \
    simulator.C \
    statistics_per_element.C \
    statistics.C \
    grid.C \
    configuration.C \
    configuration_panel.C \
    statistics_panel.C \
    statistics_shower_panel.C \
    main.C
FORMS += grid_form.ui \
    configuration_form.ui \
    statistics_form.ui \
    statistics_shower_form.ui

OTHER_FILES += \
    simulator.cfg





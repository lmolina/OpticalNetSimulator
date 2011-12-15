# ifndef GRID_WINDOW_H
# define GRID_WINDOW_H

# include <QtGui>

# include <ui_grid_form.h>

# include <grid_panel.H>
# include <grid_panel_listener.H>
# include <sim_listener.H>
# include <configuration_panel.H>
# include <statistics_panel.H>

class Configuration_Panel;
class Statistics_Panel;
class Grid_Panel;

class Grid_Window : public QWidget, Grid_Panel_Listener, Sim_Listener
{
Q_OBJECT

  Simulator * simulator;

  Configuration_Panel * conf_panel;

  Statistics_Panel * stat_panel;

  Configuration * conf;

  Routing_Algorithm routing_algorithm;

public:
  Grid_Panel * panel;

  bool editable;

  Ui_Grid_Form ui;

  explicit Grid_Window(QWidget * parent = 0);

  ~Grid_Window();

  void panel_clicked();

  void step();

  void start();

  void stop();

  void closeEvent(QCloseEvent *);

public slots:
  void check_in_clicked();

  void check_out_clicked();

  void execute();

  void configure();

  void configuration_changed();

  void view_statistics();
};

# endif // GRID_WINDOW_H

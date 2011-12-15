# ifndef STATISTICS_PANEL_H
# define STATISTICS_PANEL_H

# include <QWidget>
# include <QMessageBox>

# include <ui_statistics_form.h>

# include <statistics.H>
# include <grid_window.H>
# include <statistics_shower_panel.H>

class Grid_Window;
class Statistics_Shower_Panel;

class Statistics_Panel : public QWidget
{
  Q_OBJECT

  Statistics * statistics;

  Grid & grid;
  DynDlist<Package> & packages;

  Statistics_Shower_Panel * shower_node;
  Statistics_Shower_Panel * shower_package;
  Statistics_Shower_Panel * shower_general;

public:
  Ui_Statistics_Form ui;

  explicit Statistics_Panel(Grid &, DynDlist<Package> &, QWidget *parent = 0);

  void closeEvent(QCloseEvent *);

signals:

public slots:

  void show_statistics_to_node();

  void show_statistics_to_package();

  void show_general_statistics();

};

# endif // STATISTICS_PANEL_H

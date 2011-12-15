# ifndef GRID_PANEL_H
# define GRID_PANEL_H

# include <QtGui>

# include <simulator.H>
# include <grid_panel_listener.H>
# include <tpl_dynDlist.H>

class Grid_Panel : public QWidget
{
Q_OBJECT

  Grid & grid;

  Grid::Node * selected;

  int x_scale;
  int y_scale;

  int x_init;
  int y_init;

  DynDlist<Grid_Panel_Listener *> listeners;

public:
  explicit Grid_Panel(Grid &, QWidget * parent = 0);

  Grid::Node *& get_selected() { return selected; }

  void paintEvent(QPaintEvent *);

  void mousePressEvent(QMouseEvent *);

  void add_listener(Grid_Panel_Listener *);

  void fire_event();
};

# endif // GRID_PANEL_H

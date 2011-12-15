# ifndef CONFIGURATION_PANEL_H
# define CONFIGURATION_PANEL_H

# include <QWidget>
# include <QMessageBox>

# include <ui_configuration_form.h>
# include <configuration.H>
# include <grid_window.H>

class Grid_Window;

class Configuration_Panel : public QWidget
{
  Q_OBJECT

  Ui_Configuracion ui;

  Configuration * conf;

  bool conf_change();

public:
  explicit Configuration_Panel(QWidget *parent = 0);

signals:
  void change();

public slots:
  void btn_accept_pressed();

  void btn_cancel_pressed();

  void btn_make_default_pressed();

  void closeEvent(QCloseEvent *);
};

#endif // CONFIGURATION_PANEL_H

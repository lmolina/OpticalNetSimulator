# ifndef STATISTICS_SHOWER_PANEL_H
# define STATISTICS_SHOWER_PANEL_H

# include <QWidget>

# include <ui_statistics_shower_form.h>

# include <statistics_panel.H>
# include <statistics.H>

# include <fstream>

class Statistics_Panel;

enum Statistics_To_Show
{
  Nodes,
  Packages,
  General,
  Num_Statistics_To_Show
};

class Statistics_Shower_Panel : public QWidget
{
  Q_OBJECT

  Ui_Shower_Statistics_Form ui;
  Statistics_To_Show st;

  Statistics * statistics;

  Statistics_Per_Element * spe;

public:
  explicit Statistics_Shower_Panel(const Statistics_To_Show &,
                                   const std::string &,
                                   QWidget * parent = 0,
                                   void * info = 0);

  void closeEvent(QCloseEvent *);

signals:

public slots:

  void save();

};

# endif // STATISTICS_SHOWER_PANEL_H

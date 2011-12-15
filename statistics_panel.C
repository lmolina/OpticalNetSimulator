# include <statistics_panel.H>

Statistics_Panel::Statistics_Panel(Grid & g, DynDlist<Package> & p, QWidget *parent)
  : QWidget(parent, Qt::Tool), grid(g), packages(p)
{
  ui.setupUi(this);
  statistics = Statistics::get_instance();

  for (Grid::Node_Iterator it(grid); it.has_current(); it.next())
    {
      Grid::Node * curr = it.get_current();
      ui.combo_nodes->addItem(QString(curr->to_string().c_str()));
    }

  for (DynDlist<Package>::Iterator it(packages); it.has_current(); it.next())
    {
      Package & curr = it.get_current();
      ui.combo_packages->addItem(QString(curr.to_string().c_str()));
    }

  connect(ui.btn_view_nodes, SIGNAL(clicked()), this, SLOT(show_statistics_to_node()));
  connect(ui.btn_view_packages, SIGNAL(clicked()), this, SLOT(show_statistics_to_package()));
  connect(ui.btn_general, SIGNAL(clicked()), this, SLOT(show_general_statistics()));
}

void Statistics_Panel::closeEvent(QCloseEvent * evt)
{
  if (not parent())
    return;

  Grid_Window * window = static_cast<Grid_Window *>(parent());
  window->editable = true;
  window->ui.btn_start->setEnabled(true);
  window->ui.chk_in->setEnabled(true);
  window->ui.chk_out->setEnabled(true);
  window->ui.btn_configure->setEnabled(true);
  window->ui.btn_statistics->setEnabled(true);
}

void Statistics_Panel::show_statistics_to_node()
{
  shower_node = new Statistics_Shower_Panel(Nodes, ui.combo_nodes->currentText().toStdString(), this, &grid);
  ui.btn_view_nodes->setEnabled(false);
  shower_node->show();
}

void Statistics_Panel::show_statistics_to_package()
{
  shower_package = new Statistics_Shower_Panel(Packages, ui.combo_packages->currentText().toStdString(), this, &packages);
  ui.btn_view_packages->setEnabled(false);
  shower_package->show();
}

void Statistics_Panel::show_general_statistics()
{
  shower_general = new Statistics_Shower_Panel(General, std::string(), this);
  ui.btn_general->setEnabled(false);
  shower_general->show();
}

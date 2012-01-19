# include <grid_window.H>

Grid_Window::Grid_Window(QWidget * parent) :
  QWidget(parent), simulator(NULL), editable(true)
{
  conf = Configuration::get_instance();

  try
    {
      conf->load();
    }
  catch(...)
    {
      conf->save();
    }

  routing_algorithm = conf->get_routing_algorithm();
  ui.setupUi(this);
  simulator = new Simulator();
  panel = new Grid_Panel(simulator->get_grid(), this);
  ui.vlayout->addWidget(panel, Qt::AlignCenter);
  panel->add_listener(this);
  simulator->add_listener(this);
  connect(ui.chk_in, SIGNAL(clicked()), this, SLOT(check_in_clicked()));
  connect(ui.chk_out, SIGNAL(clicked()), this, SLOT(check_out_clicked()));
  connect(ui.btn_start, SIGNAL(clicked()), this, SLOT(execute()));
  connect(ui.btn_configure, SIGNAL(clicked()), this, SLOT(configure()));
  connect(ui.btn_statistics, SIGNAL(clicked()), this, SLOT(view_statistics()));
}

Grid_Window::~Grid_Window()
{
  delete simulator;
}

void Grid_Window::panel_clicked()
{
  if (not editable)
    return;
  Grid::Node * s = panel->get_selected();
  if (s == NULL)
    {
      ui.chk_in->setChecked(false);
      ui.chk_in->setEnabled(false);
      ui.chk_out->setChecked(false);
      ui.chk_out->setEnabled(false);
    }
  else
    {
      ui.chk_in->setChecked(s->is_in());
      ui.chk_in->setEnabled(true);
      ui.chk_out->setChecked(s->is_out());
      ui.chk_out->setEnabled(true);
    }
}

void Grid_Window::check_in_clicked()
{
  Grid::Node * s = panel->get_selected();
  s->is_in() = ui.chk_in->isChecked();
  if (s->is_in())
    simulator->add_entrance(s);
  else
    simulator->remove_entrance(s);
  panel->update();
}

void Grid_Window::check_out_clicked()
{
  Grid::Node * s = panel->get_selected();
  s->is_out() = ui.chk_out->isChecked();
  if (s->is_out())
    simulator->add_exit(s);
  else
    simulator->remove_exit(s);
  panel->update();
}

void Grid_Window::step()
{
  panel->update();
}

void Grid_Window::start()
{
  editable = false;
  ui.btn_start->setEnabled(false);
  ui.chk_in->setEnabled(false);
  ui.chk_out->setEnabled(false);
  ui.btn_configure->setEnabled(false);
  ui.btn_statistics->setEnabled(false);
}

void Grid_Window::stop()
{
  editable = true;
  ui.btn_start->setEnabled(true);
  ui.btn_configure->setEnabled(true);
  ui.btn_statistics->setEnabled(true);
  ui.chk_in->setEnabled(panel->get_selected() != NULL);
  ui.chk_out->setEnabled(panel->get_selected() != NULL);
}

void Grid_Window::execute()
{
  // TODO: Routing_Algorithm deberia ser conocido por el simulador y no por Grid_Window
  try
  {
      switch (routing_algorithm)
      {
      case A_Minimum_Deflection:
          simulator->step<Minimum_Deflection>();
          break;
      case A_Dijkstra_Routing:
          simulator->step<DijkstraRouting>();
          break;
      default:
        QMessageBox::critical(this,
                              "Error",
                              "Algoritmo de enrutamiento invalido");
      }
      QMessageBox::information(this,
                               "Trabajo realizado",
                               "La corrida de la simulacion ha finalizado, haga click en el boton estadisticas para ver los resultados");
    }
  catch(const std::exception & e)
    {
      QMessageBox::critical(this, "Error", e.what());
    }
}

void Grid_Window::configure()
{
  conf_panel = new Configuration_Panel(this);
  connect(conf_panel, SIGNAL(change()), this, SLOT(configuration_changed()));
  editable = false;
  ui.btn_start->setEnabled(false);
  ui.chk_in->setEnabled(false);
  ui.chk_out->setEnabled(false);
  ui.btn_configure->setEnabled(false);
  conf_panel->show();
}

void Grid_Window::configuration_changed()
{
  delete simulator;
  simulator = new Simulator();
  delete panel;
  panel = new Grid_Panel(simulator->get_grid());
  ui.vlayout->addWidget(panel, Qt::AlignCenter);
  panel->add_listener(this);
  simulator->add_listener(this);
  ui.btn_statistics->setEnabled(false);
}

void Grid_Window::view_statistics()
{
  stat_panel = new Statistics_Panel(simulator->get_grid(), simulator->get_packages_list(), this);
  editable = false;
  ui.btn_start->setEnabled(false);
  ui.chk_in->setEnabled(false);
  ui.chk_out->setEnabled(false);
  ui.btn_configure->setEnabled(false);
  ui.btn_statistics->setEnabled(false);
  stat_panel->show();
}

void Grid_Window::closeEvent(QCloseEvent * evt)
{
  if (QMessageBox::question(this,
                            QString("Seguridad"),
                            QString("Seguro que desea salir?"),
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    {
      evt->ignore();
      return;
    }

}

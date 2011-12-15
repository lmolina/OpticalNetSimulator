# include <configuration_panel.H>

Configuration_Panel::Configuration_Panel(QWidget *parent)
  : QWidget(parent, Qt::Tool)
{
  conf = Configuration::get_instance();
  setEnabled(true);

  ui.setupUi(this);
  ui.txt_width->setText(QString().setNum(conf->get_grid_width()));
  ui.txt_height->setText(QString().setNum(conf->get_grid_height()));
  ui.spin_num_it->setValue(conf->get_num_iterations());
  ui.txt_mu->setText(QString().setNum(conf->get_mu()));
  ui.txt_width->setValidator(new QIntValidator());
  ui.txt_height->setValidator(new QIntValidator());
  ui.txt_mu->setValidator(new QDoubleValidator());
  connect(ui.btn_accept, SIGNAL(clicked()), this, SLOT(btn_accept_pressed()));
  connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(btn_cancel_pressed()));
  connect(ui.btn_make_default, SIGNAL(clicked()), this, SLOT(btn_make_default_pressed()));
}

bool Configuration_Panel::conf_change()
{
  if (ui.txt_width->text().isEmpty())
    {
      QMessageBox::critical(this, QString("Error"), QString("Debe seleccionar un ancho para la grid"));
      ui.txt_width->setFocus();
      return false;
    }
  if (ui.txt_height->text().isEmpty())
    {
      QMessageBox::critical(this, QString("Error"), QString("Debe seleccionar una altura para la grid"));
      ui.txt_height->setFocus();
      return false;
    }

  if (ui.spin_num_it->value() < 1)
    {
      QMessageBox::critical(this, QString("Error"), QString("El numero de iteraciones debe ser mayor que cero"));
      ui.spin_num_it->setFocus();
      return false;
    }

  if (ui.txt_mu->text().isEmpty())
    {
      QMessageBox::critical(this, QString("Error"), QString("Debe seleccionar la media para la generacion de paquetes"));
      ui.txt_mu->setFocus();
      return false;
    }
  conf->set_grid_width(ui.txt_width->text().toULong());
  conf->set_grid_height(ui.txt_height->text().toULong());
  conf->set_num_iterations(ui.spin_num_it->value());
  conf->set_mu(ui.txt_mu->text().toDouble());
  emit change();
  return true;
}

void Configuration_Panel::btn_accept_pressed()
{
  if (not conf_change())
    return;
  this->close();
}

void Configuration_Panel::btn_cancel_pressed()
{
  this->close();
}

void Configuration_Panel::btn_make_default_pressed()
{
  if (not conf_change())
    return;
  conf->save();
  QMessageBox::information(this, QString("Informacion"), QString("Se han establecidos los nuevos valores como predeterminados"));
}

void Configuration_Panel::closeEvent(QCloseEvent *)
{
  if (not parent())
    return;

  Grid_Window * window = static_cast<Grid_Window *>(parent());
  window->editable = true;
  window->ui.btn_start->setEnabled(true);
  window->ui.btn_configure->setEnabled(true);
  Grid::Node * selected = window->panel->get_selected();
  window->ui.chk_in->setEnabled(selected != NULL);
  window->ui.chk_out->setEnabled(selected != NULL);
  if (selected)
    {
      window->ui.chk_in->setChecked(selected->is_in());
      window->ui.chk_out->setChecked(selected->is_out());
    }
  else
    {
      window->ui.chk_in->setChecked(false);
      window->ui.chk_out->setChecked(false);
    }
}


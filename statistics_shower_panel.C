# include <statistics_shower_panel.H>

Statistics_Shower_Panel::Statistics_Shower_Panel(const Statistics_To_Show & s,
                                                 const std::string & name,
                                                 QWidget * parent,
                                                 void * info)
  : QWidget(parent, Qt::Tool), st(s)
{
  ui.setupUi(this);

  statistics = Statistics::get_instance();

  QString window_title;

  switch(st)
    {
    case Nodes:
        {
          window_title = QString("Estadistica de ") + QString(name.c_str());
          spe = statistics->search_node_statistic(name);
          Grid * grid = reinterpret_cast<Grid *>(info);
          Grid::Node * node;
          for (Grid::Node_Iterator it(*grid); it.has_current(); it.next())
            {
              node = it.get_current();
              if (node->to_string() == name)
                break;
            }

          char buffer[1024];

          ui.textEdit->append(QString(name.c_str()) + QString("\n"));
          sprintf(buffer, "Numero de paquetes que pasaron: %ld",
                  node->get_num_nodes_deflected() + node->get_num_nodes_routed());
          ui.textEdit->append(QString(buffer));
          sprintf(buffer, "Numero de paquetes deflectados: %ld",
                  node->get_num_nodes_deflected());
          ui.textEdit->append(QString(buffer));
          sprintf(buffer, "Numero de paquetes enrutados: %ld\n\n",
                  node->get_num_nodes_routed());
          ui.textEdit->append(QString(buffer));
          break;
        }
    case Packages:
        {
          window_title = QString("Estadistica de ") + QString(name.c_str());
          spe = statistics->search_package_statistic(name);
          DynDlist<Package> * packages_list = reinterpret_cast<DynDlist<Package> *>(info);

          Package * p;

          for (DynDlist<Package>::Iterator it(*packages_list); it.has_current(); it.next())
            {
              p = &(it.get_current());
              if (p->to_string() == name)
                break;
            }

          char buffer[1024];

          ui.textEdit->append(QString(name.c_str()) + QString("\n"));
          sprintf(buffer, "Cantidad de iteraciones que estuvo en la red: %ld", p->get_ttl());
          ui.textEdit->append(QString(buffer));
          sprintf(buffer, "Cantidad de iteraciones que estuvo en cola: %ld", p->get_tiq());
          ui.textEdit->append(QString(buffer));
          sprintf(buffer, "Numero de veces deflectado: %ld", p->get_num_deflections());
          ui.textEdit->append(QString(buffer));
          sprintf(buffer, "Numero de veces enrutado correctamente: %ld", p->get_num_routings());
          ui.textEdit->append(QString(buffer));
          break;
        }
    case General:
        {
          Configuration * conf;

          conf = Configuration::get_instance();

          window_title = "Estadisticas Generales";

          ui.textEdit->append(QString("Resultados generales de la Simulacion\n"));
          switch (conf->get_routing_algorithm()){
              case A_Minimum_Deflection: ui.textEdit->append(QString("Algoritmo de enrutamiento: Deflexion Minima\n"));
                                  break;
              case A_Dijkstra_Routing: ui.textEdit->append(QString("Algoritmo de enrutamiento: Dijkstra\n"));
                                  break;

          }          
          char buffer[1024];
          sprintf(buffer, "Numero de iteraciones: %ld", statistics->get_num_iterations());
          ui.textEdit->append(QString(buffer));
          sprintf(buffer, "Numero de paquetes generados: %ld", statistics->get_generated_packages());
          ui.textEdit->append(QString(buffer));
          sprintf(buffer, "Numero de paquetes que quedaron en cola: %ld", statistics->get_num_packages_in_queue());
          ui.textEdit->append(QString(buffer));
          sprintf(buffer, "Numero de paquetes entregados: %ld", statistics->get_deliveried_packages());
          ui.textEdit->append(QString(buffer));
          sprintf(buffer, "Numero de paquetes que quedaron en movimiento: %ld",
                  statistics->get_generated_packages()
                  - statistics->get_deliveried_packages()
                  - statistics->get_num_packages_in_queue());
          ui.textEdit->append(QString(buffer));

          sprintf(buffer, "Tiempo promedio de paquetes en cola: %.2lf iteraciones", statistics->get_tiq_avg());
          ui.textEdit->append(QString(buffer));
          sprintf(buffer, "Tiempo promedio de paquetes en movimiento: %.2lf iteraciones\n", statistics->get_ttl_avg());
          ui.textEdit->append(QString(buffer));

          unsigned long * deflections_counter = statistics->get_deflections_counter();
          const unsigned long & max = statistics->get_max_deflections();

          for (int i = 0; i < max + 1; ++i)
            {
              sprintf(buffer, "Numero de paquetes deflectados %ld  %s: %ld",
                      i, (i == 1 ? " vez" : " veces"), deflections_counter[i]);
              ui.textEdit->append(QString(buffer));
            }

          DynDlist<Statistics::Pair_Of_List> & events_list_per_iteration = statistics->get_events_list_per_iteration();

          ui.textEdit->append(QString("\n\nTraza de la simulacion\n\n"));
          ui.textEdit->append(QString("Resultados por paquete"));
          unsigned long i = 0;
          for (DynDlist<Statistics::Pair_Of_List>::Iterator it(events_list_per_iteration); it.has_current(); it.next(), ++i)
            {
              if (i == statistics->get_num_iterations())
                break;

              if (i > 0)
                sprintf(buffer, "Iteracion numero %ld", i);
               else
                sprintf(buffer, "Estado inicial");
              ui.textEdit->append(buffer);

              DynDlist<Statistics_Per_Element> & statistics_per_package = it.get_current().statistics_per_package;
              for (DynDlist<Statistics_Per_Element>::Iterator it(statistics_per_package); it.has_current(); it.next())
                {
                  Statistics_Per_Element & s = it.get_current();
                  ui.textEdit->append(QString(s.get_element_name()));
                  DynDlist<std::string> & h = s.get_history();
                  for (DynDlist<std::string>::Iterator it(h); it.has_current(); it.next())
                    {
                      std::string & st = it.get_current();
                      sprintf(buffer, "Evento: %s", st.c_str());
                      ui.textEdit->append(QString(buffer));
                    }
                  ui.textEdit->append(QString("\n"));
                }
            }
          break;
        }
    default:
      throw std::domain_error("Invalid statistics selection");
    }
  setWindowTitle(window_title);
  connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(save()));
}

void Statistics_Shower_Panel::closeEvent(QCloseEvent *)
{
  if (not parent())
    return;

  Statistics_Panel * p = static_cast<Statistics_Panel *>(parent());

  switch(st)
    {
    case Nodes:
      p->ui.btn_view_nodes->setEnabled(true);
      break;
    case Packages:
      p->ui.btn_view_packages->setEnabled(true);
      break;
    case General:
      p->ui.btn_general->setEnabled(true);
      break;
    default:
      throw std::domain_error("Invalid statistics selection");
    }
}

void Statistics_Shower_Panel::save()
{
  const QString file_name = QFileDialog::getSaveFileName(this, QString("Guardar archivo como"), QDir::currentPath(), QString("*.*"));
  if (file_name.isEmpty())
    return;

  std::fstream file(file_name.toStdString().c_str(), std::ios::out);
  if (not file)
    QMessageBox::critical(this, QString("Error"), QString("No se puede crear el archivo"));

  file << ui.textEdit->toPlainText().toStdString();

  file.close();
}

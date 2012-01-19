# include <simulator.H>

Simulator::Simulator() : packages_counter(0)
{
  conf = Configuration::get_instance();

  rng = gsl_rng_alloc(gsl_rng_mt19937);
  //gsl_rng_set(rng, time(NULL) % gsl_rng_max(rng));

  statistics = Statistics::get_instance();

  num_iterations = conf->get_num_iterations();
  grid_height = conf->get_grid_height();
  grid_width = conf->get_grid_width();
  routing_algorithm = conf->get_routing_algorithm();

  grid.set_height(grid_height);
  grid.set_width(grid_width);

  grid.build_automatic_grid();
}

Simulator::~Simulator()
{
  gsl_rng_free(rng);
}

void Simulator::add_entrance(Grid::Node * e)
{
  entrances.append(e);
}

void Simulator::remove_entrance(Grid::Node * e)
{
  for (DynDlist<Grid::Node *>::Iterator it(entrances); it.has_current(); it.next())
    {
      Grid::Node *& curr = it.get_current();
      if (curr == e)
        {
          entrances.remove(curr);
          break;
        }
    }
}

void Simulator::add_exit(Grid::Node * e)
{
  exits.append(e);
}

void Simulator::remove_exit(Grid::Node * e)
{
  for (DynDlist<Grid::Node *>::Iterator it(exits); it.has_current(); it.next())
    {
      Grid::Node *& curr = it.get_current();
      if (curr == e)
        {
          exits.remove(curr);
          break;
        }
    }
}

void Simulator::move_packages()
{
  for (DynDlist<Package>::Iterator it(packages); it.has_current(); )
    {
      Package & package = it.get_current();
      it.next();
      if (package.is_in_queue() or package.is_deliveried())
        continue;
      Grid::Node * node = package.get_routed_to();
      if (node == package.get_target_node())
        {
          package.is_deliveried() = true;
          std::string pkn = package.to_string();
          std::string node_name = node->to_string();
          std::string stp = gnu::autosprintf("Paquete entregado en su destino %s", node_name.c_str());
          std::string stn = gnu::autosprintf("Entrego al paquete %s", pkn.c_str());
          statistics->add_statistic_to_package(pkn, stp);
          statistics->add_statistic_to_node(node_name, stn);
          statistics->inc_deliveried_packages();
          continue;
        }
      node->add_client(&package);
    }
}

void Simulator::move_packages_from_queue_to_clients()
{
  //TODO: Recorrer lista de entradas nada mas.
  for (Grid::Node_Iterator it(grid); it.has_current(); it.next())
    {
      Grid_Node * node = it.get_current();
      DynDlist<Package *> & q = node->get_queue();
      if (q.is_empty())
        continue;
      DynDlist<Package *> & c = node->get_clients_list();
      size_t off_set = grid.get_num_arcs(node) - c.size();
      int i = 0;
      while (i++ < off_set and not q.is_empty())
        {
          Package * p = q.remove_first();
          statistics->dec_packages_in_queue();
          p->is_in_queue() = false;
          c.append(p);
        }
     }
}

void Simulator::generate_packages()
{
  int num_packages_to_generate = gsl_ran_poisson(rng, conf->get_mu());

  for (int i = 0; i < num_packages_to_generate; ++i)
    {
      // Elijo el numero de nodo en la lista de entradas aleatoriamente
      int num_node_to_put_package = gsl_rng_uniform_int(rng, entrances.size());
      Grid::Node * selected_entrance = entrances[num_node_to_put_package];

      Grid::Node * selected_exit = NULL;
      do
        {
          int num_exit = gsl_rng_uniform_int(rng, exits.size());
          selected_exit = exits[num_exit];
        }
      while (selected_entrance == selected_exit);

      Package p(packages_counter++, selected_entrance, selected_exit);
      Package & pack = packages.append(p);

      std::string pkn = pack.to_string();
      std::string node_name = selected_entrance->to_string();
      std::string tgt_name = selected_exit->to_string();
      std::string stp = gnu::autosprintf("Se creo el paquete %s en el nodo %s con destino %s",
                                          pkn.c_str(), node_name.c_str(), tgt_name.c_str());
      std::string stn = gnu::autosprintf("Se creo el paquete %s con destino %s",
                                          pkn.c_str(), tgt_name.c_str());
      // Si el nodo ya no tiene capacidad para meter paquetes se mete en cola
      if (selected_entrance->get_clients_list().size() < grid.get_num_arcs(selected_entrance))
        selected_entrance->add_client(&pack);
      else
        {
          selected_entrance->put_in_queue(&pack);
          statistics->inc_packages_in_queue();
          stp += " y se ha colocado en cola de espera";
        }
      statistics->add_statistic_to_package(pkn, stp);
      statistics->add_statistic_to_node(node_name, stn);
      statistics->inc_generated_packages();
    }
}

void Simulator::add_listener(Sim_Listener * l)
{
  listeners.append(l);
}

void Simulator::fire_event(const Event_Type & evt)
{
  for (DynDlist<Sim_Listener *>::Iterator it(listeners); it.has_current(); it.next())
    switch(evt)
      {
      case Step:  it.get_current()->step(); break;
      case Start: it.get_current()->start(); break;
      case Stop:  it.get_current()->stop(); break;
      }
}

void Simulator::inc_tiq_to_packages()
{
  for (DynDlist<Package>::Iterator it(packages); it.has_current(); it.next())
    {
      Package & p = it.get_current();
      if (p.is_in_queue())
        p.inc_tiq();
    }
}

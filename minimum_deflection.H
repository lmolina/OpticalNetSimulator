# ifndef MINIMUM_DEFLECTION_H
# define MINIMUM_DEFLECTION_H

# include <grid.H>
# include <package.H>
# include <statistics.H>

/**
  * \brief Clase que contiene el algoritmo de deflecci&oacute;n m&iacute;nima.
  * @authors Alejandro Mujica, Anna Lezama
  */
class Minimum_Deflection
{
public:
  /**
    * Operaci&oacute;n que ejecuta el enrutamiento por deflecci&oacute;n m&iacute;nima.
    * @param g La malla en la que se est&aaccute; trabajando.
    * @param node El nodo donde se encuentran los paquetes que se desean enrutar.
    */
  void operator()(Grid & g, Grid::Node * node)
  {
    if (node->get_clients_list().is_empty())
      return;

    g.reset_all_arcs();

    Statistics * statistics = Statistics::get_instance();
    std::string node_name = node->to_string();

    DynDlist<Package *> & list = node->get_clients_list();

    int i = 1;

    for (DynDlist<Package *>::Iterator it(list); not list.is_empty(); )
      {
        if (not it.has_current())
          {
            it.reset_first();
            ++i;
          }

        Package *& curr_package = it.get_current();

        if (i > 2)
          {
            Grid::Arc * first_free_link = g.get_first_free_link(node);
            Grid::Node * tgt = g.get_connected_node(first_free_link, node);
            curr_package->rout_to(tgt);
            g.set_arc_busy_for_node(first_free_link, node, curr_package);
            curr_package->inc_ttl();
            it.next();
            curr_package->inc_num_deflections();
            node->inc_num_nodes_deflected();
            statistics->set_max_deflections(curr_package->get_num_deflections());
            list.remove(curr_package);
            std::string pkn = curr_package->to_string();
            std::string tgt_name = tgt->to_string();
            std::string stp = gnu::autosprintf("%s es deflectado de %s a %s",
                                               pkn.c_str(), node_name.c_str(), tgt_name.c_str());
            std::string stn = gnu::autosprintf("%s ha deflectado al paquete %s hacia %s",
                                               node_name.c_str(), pkn.c_str(), tgt_name.c_str());
            statistics->add_statistic_to_package(pkn, stp);
            statistics->add_statistic_to_node(node_name, stn);
            continue;
          }

        if (curr_package->get_num_favorable_directions() != i)
          {
            it.next();
            continue;
          }

        if (i == 1)
          {
            Direction favorable_direction = curr_package->get_h_favorable_direction() != Num_Directions ?
                                            curr_package->get_h_favorable_direction() :
                                            curr_package->get_v_favorable_direction();
            Grid::Arc * link = g.get_arc_by_rout(node, favorable_direction);
            if (not link or g.is_arc_busy_for_node(link, node))
              {
                it.next();
                continue;
              }
            Grid::Node * tgt = g.get_connected_node(link, node);
            curr_package->rout_to(tgt);
            g.set_arc_busy_for_node(link, node, curr_package);
            curr_package->inc_ttl();
            curr_package->inc_num_routings();
            node->inc_num_nodes_routed();
            it.next();
            list.remove(curr_package);
            std::string pkn = curr_package->to_string();
            std::string tgt_name = tgt->to_string();
            std::string stp = gnu::autosprintf("%s es enrutado de %s a %s",
                                               pkn.c_str(), node_name.c_str(), tgt_name.c_str());
            std::string stn = gnu::autosprintf("%s ha enrutado al paquete %s hacia %s",
                                               node_name.c_str(), pkn.c_str(), tgt_name.c_str());
            statistics->add_statistic_to_package(pkn, stp);
            statistics->add_statistic_to_node(node_name, stn);
          }
        else
          {
            Direction favorable_direction = curr_package->get_h_favorable_direction();
            Grid::Arc * link = g.get_arc_by_rout(node, favorable_direction);
            if (link and not g.is_arc_busy_for_node(link, node))
              {
                Grid::Node * tgt = g.get_connected_node(link, node);
                curr_package->rout_to(tgt);
                g.set_arc_busy_for_node(link, node, curr_package);
                curr_package->inc_ttl();
                curr_package->inc_num_routings();
                node->inc_num_nodes_routed();
                it.next();
                list.remove(curr_package);
                std::string pkn = curr_package->to_string();
                std::string tgt_name = tgt->to_string();
                std::string stp = gnu::autosprintf("%s es enrutado de %s a %s", pkn.c_str(),
                                                   node_name.c_str(), tgt_name.c_str());
                std::string stn = gnu::autosprintf("%s ha enrutado al paquete %s hacia %s",
                                                   node_name.c_str(), pkn.c_str(), tgt_name.c_str());
                statistics->add_statistic_to_package(pkn, stp);
                statistics->add_statistic_to_node(node_name, stn);
                continue;
              }
            favorable_direction = curr_package->get_v_favorable_direction();
            link = g.get_arc_by_rout(node, favorable_direction);
            if (not link or g.is_arc_busy_for_node(link, node))
              {
                it.next();
                continue;
              }
            Grid::Node * tgt = g.get_connected_node(link, node);
            curr_package->rout_to(tgt);
            g.set_arc_busy_for_node(link, node, curr_package);
            curr_package->inc_ttl();
            curr_package->inc_num_routings();
            node->inc_num_nodes_routed();
            it.next();
            std::string pkn = curr_package->to_string();
            std::string tgt_name = tgt->to_string();
            std::string stp = gnu::autosprintf("%s es enrutado de %s a %s",
                                               pkn.c_str(), node_name.c_str(), tgt_name.c_str());
            std::string stn = gnu::autosprintf("%s ha enrutado al paquete %s hacia %s",
                                               node_name.c_str(), pkn.c_str(), tgt_name.c_str());
            statistics->add_statistic_to_package(pkn, stp);
            statistics->add_statistic_to_node(node_name, stn);
            list.remove(curr_package);
          }
      }
  }
};

# endif //MINIMUM_DEFLECTION_H


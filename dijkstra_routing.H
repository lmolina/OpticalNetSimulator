# ifndef DIJKSTRA_ROUTING_H
# define DIJKSTRA_ROUTING_H

# include <network.H>
# include <package.H>
# include <statistics.H>
# include <Dijkstra.H>
# include <tpl_dynBinHeap.H>
# include <tpl_graph.H>

struct PathSizeNode
{
    Grid::Node * node;
    size_t size;
};

struct Compare
{
  int operator () (const PathSizeNode & a, const PathSizeNode & b) const
  {
    return a.size < b.size;
  }
};

/**
  * \brief Clase que utiliza el algor&iacute;mo de Dijkstra para seenrutar los
  * paquetes en una red completamente &Oacute;tica
  * @authors Laudin Alessandro Molina, Diego Paez
  */
class DijkstraRouting
{
public:
  /**
    * Operaci&oacute;n que ejecuta el enrutamiento.
    * @param g La malla en la que se est&aaccute; trabajando.
    * @param node El nodo donde se encuentran los paquetes que se desean enrutar.
    */
  void operator()(Grid & g, Grid::Node * node)
  {
      cout << "Dijkstra";
    /*************************************************************************
     * 0. Entrar en el nodo
     * I.1. Calcular el arbol de expansion minima
     * I.2. Varias opciones:
     *    - Atender clientes en el orden en que se encuentran en la lista
     *    - Atender clientes que se encuentran mas cerca de su destino (menor
     *      distancia de Disjkstra)
     *    - Atender clientes que se encuentran mas lejos de su destino (mayor
     *      distancia de Disjkstra)
     *    - Atender los que estan a 1 salto del destino y luego atender en
     *      orden de llegada
     *    - Atender los que estan a 1 salto del destino y luego atender en
     *      orden aleatorio
     *  II.1 Atender los paquetes en el orden en que se encuentran en la lista
     *  II.2 Calcular la ruta minima para cada paquete
     *  II.2.1 Si no es posible utilizar la ruta minima, eliminar el arco
     *         ocupado y computar la ruta minima nuevamente
     *  II.3 Una vez que se tienen una ruta minima posible enviar el paquete al
     *       siguiente nodo en la ruta
     *  III.1 Calcular la ruta minima utilizando Dijkstra desde cada nodo vecino
     *        hacia el nodo destino de cada paquete cliente
     *  III.2 Utilizar la ruta disponible con peso minimo 
     *        (min(ir a vecino i + vecino i a destino)
     *  IV.1 Dado que en el modelo actual los caminos no tienen peso, calcula la
     *       ruta minima utilizando recorrido en amplitud desde cada nodo vecino
     *       hacia el nodo destino de cada paquete cliente
     *  IV.2 Utilizar la ruta disponible con peso minimo 
     *       (min(ir a vecino i + vecino i a destino)
     *************************************************************************/

    // Si no hay clientes en cola no hay nada que hacer
    if (node->get_clients_list().is_empty())
      return;

    // TODO resetear los arcos se debe realizar solo una vez en cada iteracion, mover a simulator
    g.reset_all_arcs();

    Statistics * statistics = Statistics::get_instance();
    std::string node_name = node->to_string();

    // Lista de clientes en espera en el nodo
    DynDlist< Package * > & client_list = node->get_clients_list();

    // Lista de los nodos vecinos
    // DynDlist< Grid::Node * > & neighbors = g.get_neighbors(node);

    // TODO: mover a la clase Grid (al moverse provoca una violacion de
    // segmento, probablemente debido a retorno mal realizado
    DynDlist< Grid::Node * > neighbors;
    for (Grid::Node_Arc_Iterator it(node); it.has_current(); it.next())
    {
        Grid::Arc * arc = it.get_current();
        if (g.get_src_node(arc) == node)
            neighbors.append(g.get_tgt_node(arc));
        else
            neighbors.append(g.get_src_node(arc));
    }

    Path< Grid > min_path(g);

    PathSizeNode path_size;

    // TODO Es simple/eficiente (cambiar por ordenamiento manual?)?
    DynBinHeap< PathSizeNode, Compare > paths_size;

    for (DynDlist< Package * >::Iterator it_clients(client_list);
         it_clients.has_current(); it_clients.next())
    {
      Package *& curr_package = it_clients.get_current();

      // Calcular el tamanio de la ruta hasta el destino desde cada uno de los
      // vecinos del nodo actual
      for (DynDlist< Grid::Node * >::Iterator it_neighbors(neighbors);
           it_neighbors.has_current(); it_neighbors.next())
      {
        Grid::Node * next_node = it_neighbors.get_current();

        min_path.clear_path();

        //dijkstra_min_path< Grid, Distance >(g, next_node,
        //                                    curr_package->get_target_node(), min_path);
        //                                    
        
        //find_path_breadth_first(g, next_node, curr_package->get_target_node(), min_path);

        //std::cout << std::endl << "next_node: " << next_node->get_info() << std::endl;
        //std::cout << "curr_package: " << curr_package->get_target_node()->get_info() << std::endl;
        Find_Path_Breadth_First<Grid>()(g, next_node, curr_package->get_target_node(), min_path);

        path_size.node = next_node;

        // FIXME
        // Actualmente el tamanio de las rutas no es tomado en cuenta,
        // deberia utilizarse la longitud del arco. Para esto la clase Link
        // deberia recibir un LinkInfo en vez de un Empty_Class. En todo caso,
        // con la busqueda en amplitud la longitud de los arcos no tiene sentido
        path_size.size = min_path.size();
        paths_size.insert(path_size);
      }

      // Utilizar el camino mas corto disponible
      while(not paths_size.is_empty())
      {
        Grid::Node * tgt = paths_size.get().node;
        Grid::Arc * link = g.get_link(node, tgt);

        if (link and not g.is_arc_busy_for_node(link, node))
        {
          curr_package->rout_to(tgt);
          g.set_arc_busy_for_node(link, node, curr_package);
          curr_package->inc_ttl();
          curr_package->inc_num_routings();
          node->inc_num_nodes_routed();
          client_list.remove(curr_package);

          std::string pkn = curr_package->to_string();
          std::string tgt_name = tgt->to_string();
          std::string stp = gnu::autosprintf("%s es enrutado de %s a %s", pkn.c_str(),
                                             node_name.c_str(), tgt_name.c_str());
          std::string stn = gnu::autosprintf("%s ha enrutado al paquete %s hacia %s",
                                             node_name.c_str(), pkn.c_str(), tgt_name.c_str());
          statistics->add_statistic_to_package(pkn, stp);

          // Una vez seleccionado el nodo se vacia la lista de nodos
          paths_size.empty();
        }
      }
    }
  }
};

# endif // DIJKSTRA_ROUTING_H


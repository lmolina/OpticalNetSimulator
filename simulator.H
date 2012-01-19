# ifndef SIMULATOR_H
# define SIMULATOR_H

# include <grid.H>
# include <minimum_deflection.H>
# include <dijkstra_routing.H>
# include <sim_listener.H>
# include <configuration.H>
# include <misc.H>

# include <fstream>

# include <gsl/gsl_rng.h>
# include <gsl/gsl_randist.h>

/**
  * \brief Clase que modela al simulador.
  * @authors Alejandro Mujica, Anna Lezama
  */
class Simulator
{
  gsl_rng * rng;

  bool __is_running;

  unsigned long packages_counter;

  unsigned long num_iterations;

  size_t grid_width;

  size_t grid_height;

  DynDlist<Package> packages;

  DynDlist<Grid::Node *> entrances;

  DynDlist<Grid::Node *> exits;

  DynDlist<Sim_Listener *> listeners;

  Grid grid;

  Statistics * statistics;

  Configuration * conf;

  Routing_Algorithm routing_algorithm;

  void fire_event(const Event_Type &);

public:
  Simulator();

  ~Simulator();

  void add_entrance(Grid::Node *);

  void remove_entrance(Grid::Node *);

  void add_exit(Grid::Node *);

  void remove_exit(Grid::Node *);

  void move_packages();

  /**
    * Genera nuevos paquetes para entrar en la red.
    */
  void generate_packages();

  /**
    * Mueve los paquetes desde la cola de entradas a la lista de clientes siempre que se pueda.
    */
  void move_packages_from_queue_to_clients();

  /**
    * Incrementa el tiempo en cola de cada paquete que se encuentre en la cola de entrada de un nodo.
    */
  void inc_tiq_to_packages();

  /**
    * M&eacute;todo plantilla que modela un paso de ejecuta la simulaci&oacute;n.<br />
    * Recibe como par&aacute;metro tipo el algoritmo de enrutamiento que se desea ejecutar el cual debe
    * exportar lo siguiente
    *    <ul>
    *    <li> Routing_Algo::operator()(Grid & g, Grid::Node * node) que retorna void.
    * Donde g es la Malla en la cual se est&aacute; trabajando y node el nodo a que contiene los
    * paquetes que se quieren enrutar.
    *    </ul>
    */
  template <class /* Routing_Algo */>
  void step();

  void add_listener(Sim_Listener *);

  Grid & get_grid() { return grid; }

  bool is_running() const { return __is_running; }

  DynDlist<Package> & get_packages_list() { return packages; }
};

template <class Routing_Algo>
void Simulator::step()
{
  if (entrances.is_empty() or exits.is_empty())
    throw std::logic_error("Debe seleccionar al menos un nodo entrada y una salida");

  if (entrances.is_unitarian() and exits.is_unitarian() and entrances.get_first() == exits.get_first())
    throw std::logic_error("De haber una sola entrada y salida deben ser diferentes");

  statistics->clear();
  statistics->init();
  fire_event(Start);
  for (int i = 0; i < num_iterations; ++i)
    {
      for (Grid::Node_Iterator it(grid); it.has_current(); it.next())
        {
          Grid::Node * curr_node = it.get_current();
          grid.reset_all_arcs();
          Routing_Algo()(grid, curr_node);
        }
      move_packages();
      move_packages_from_queue_to_clients();
      generate_packages();
      inc_tiq_to_packages();
      statistics->inc_iterations();
    }
  statistics->count_deflections(packages);
  fire_event(Stop);
}

# endif // SIMULATOR_H

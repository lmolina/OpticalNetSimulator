# ifndef PACKAGE_H
# define PACKAGE_H

# include <grid.H>
# include <misc.H>

# include <string>

# include <autosprintf.h>

class Grid;

class Grid_Node;

class Link;

/**
  * \brief Modela un paquete que se mueve por la red.
  * @authors Alejandro Mujica, Anna Lezama
  */
class Package
{
  unsigned long id;      // Identificador unico de cada paquete
  unsigned short ttl;    // Tiempo de vida de un paquete, por cuantos enlaces ha pasado
  unsigned short tiq;
  Grid_Node * source;    // Direccion del nodo que envia el paquete
  Grid_Node * target;    // Direccion del destino del paquete
  Grid_Node * routed_to; // Hacia donde se ha enrutado el paquete en un instante de tiempo

  Direction h_favorable_direction; // Direccion favorable horizontal
  Direction v_favorable_direction; // Direccion favorable vertical

  unsigned long num_deflections; // Contador de deflecciones del paquete
  unsigned long num_routings;    // Contador de enrutamientos correctos

  bool __is_in_queue;  // Indica si se encuentra en cola
  bool __is_deliveried; // Indica si ya ha sido entregado

public:
  Package() : id(0), ttl(0), source(NULL), target(NULL), routed_to(NULL),
              h_favorable_direction(Num_Directions), v_favorable_direction(Num_Directions),
              num_deflections(0), num_routings(0), __is_in_queue(false), tiq(0), __is_deliveried(false)
  {
    // Empty
  }

  Package(const unsigned long & _id, Grid_Node * src, Grid_Node * tgt)
    : id(_id), ttl(0), source(src), target(tgt), routed_to(NULL),
      h_favorable_direction(Num_Directions), v_favorable_direction(Num_Directions),
      num_deflections(0), num_routings(0), __is_in_queue(false), tiq(0),  __is_deliveried(false)
  {
    // Empty
  }

  Package(const Package & p) : id(p.id), ttl(p.ttl), source(p.source), target(p.target), routed_to(p.routed_to),
                               h_favorable_direction(p.h_favorable_direction), v_favorable_direction(p.v_favorable_direction),
                               num_deflections(p.num_deflections), num_routings(p.num_routings),
                               __is_in_queue(p.__is_in_queue), tiq(p.tiq),  __is_deliveried(p.__is_deliveried)

  {
    // Empty
  }

  ~Package()
  {
    // Empty
  }

  const unsigned long & get_id() const { return id; }

  /**
    * Retorna el tiempo (cantidad de iteraciones) que ha pasado un nodo movi&eacute;ndose en la red.
    */
  const unsigned short & get_ttl() const { return ttl; }

  /**
    * Retorna el tiempo (cantidad de iteraciones) que ha pasado un nodo movi&eacute;ndose en cola de entrada de un nodo.
    */
  const unsigned short & get_tiq() const { return tiq; }

  /**
    * Incrementa el n&uacute;mero de iteraciones de un paquete en la red.
    */
  void inc_ttl() { ++ttl; }

  /**
    * Incrementa el n&uacute;mero de iteraciones de un paquete en cola de entrada de un nodo.
    */
  void inc_tiq() { ++tiq; }

  /**
    * Incrementa el n&uacute;mero de deflecciones que se le han hecho al paquete.
    */
  void inc_num_deflections() { ++num_deflections; }

  /**
    * Incrementa el n&uacute;mero de enrutamientos favorables que se le han hecho al paquete.
    */
  void inc_num_routings() { ++num_routings; }

  /**
    * Retorna el nodo desde el cual fue enrutado (o deflectado) el paquete en un momento dado.
    */
  Grid_Node * get_source_node() { return source; }

  /**
    * Retorna el nodo hacia el cual se dirige el paquete (destino final en la red).
    */
  Grid_Node * get_target_node() { return target; }

  /**
    * Retorna el nodo al que ha sido enrutado un paquete en un momento dado.
    */
  Grid_Node * get_routed_to() { return routed_to; }

  /**
    * Retorna el n&uacute;mero de deflecciones que se le han hecho al paquete en un momento.
    */
  const unsigned long & get_num_deflections() { return num_deflections; }

  /**
    * Retorna el n&uacute;mero de enrutamientos favorables que se le han hecho al paquete en un momento.
    */
  const unsigned long & get_num_routings() { return num_routings; }

  bool & is_in_queue() { return __is_in_queue; }

  bool & is_deliveried() { return __is_deliveried; }

  /**
    * Retorna el n&uacute;mero de direcciones favorables de un paquete en un momento dado.
    */
  const unsigned short get_num_favorable_directions() const
  {
    int ret_val = 0;
    if (h_favorable_direction != Num_Directions)
      ++ret_val;
    if (v_favorable_direction != Num_Directions)
      ++ret_val;
    return ret_val;
  }

  void reset()
  {
    routed_to = NULL;
    v_favorable_direction = Num_Directions;
    h_favorable_direction = Num_Directions;
  }

  void rout_to(Grid_Node * node)
  {
    routed_to = node;
  }

  /**
    * Retorna la direcci&oacute;n favorable horizontal del nodo (constante).
    */
  const Direction & get_h_favorable_direction() const { return h_favorable_direction; }

  /**
    * Retorna la direcci&oacute;n favorable horizontal del nodo (modificable).
    */
  Direction & get_h_favorable_direction() { return h_favorable_direction; }

  /**
    * Retorna la direcci&oacute;n favorable vertical del nodo (constante).
    */
  const Direction & get_v_favorable_direction() const { return v_favorable_direction; }

  /**
    * Retorna la direcci&oacute;n favorable vertical del nodo (modificable).
    */
  Direction & get_v_favorable_direction() { return v_favorable_direction; }


  /**
    * Retorna una descripci&oacute;n del paquete,
    * en este caso es la palabra Paquete con el n&uacute;mero que lo identifica.
    */
  const std::string to_string()
  {
    return gnu::autosprintf("Paquete %ld", id);
  }
};

# endif

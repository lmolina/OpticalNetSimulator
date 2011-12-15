# ifndef GRID_H
# define GRID_H

# include <tpl_euclidian_graph.H>
# include <package.H>
# include <misc.H>

class Grid;

class Package;

class Op_Node;

class Op_Arc;

/**
  * \brief Modela un nodo de la Malla.
  *
  * La clase se basa en Euclidian_Node<Node_Info> de la biblioteca
  * <a url="http://webdelprofesor.ula.ve/ingenieria/lrleon/aleph/html/index.html" target="_blank">ALEPH-w</a>
  * con par&aacute;metro tipo long.
  * @authors Alejandro Mujica, Anna Lezama
  */
class Grid_Node : public Euclidian_Node<long>
{
  bool __is_in;
  bool __is_out;

  DynDlist<Package *> clients;
  DynDlist<Package *> queue;

  unsigned long num_nodes_deflected;
  unsigned long num_nodes_routed;

public:
  Grid_Node() : Euclidian_Node<long>(),
                __is_in(false), __is_out(false),
                num_nodes_deflected(0), num_nodes_routed(0)
  {
    /* Empty */
  }

  Grid_Node(const long & num)
    : Euclidian_Node<long>(num), __is_in(false), __is_out(false),
    num_nodes_deflected(0), num_nodes_routed(0)
  {
    /* Empty */
  }

  Grid_Node(Grid_Node * node)
    : Euclidian_Node<long>(this), __is_in(node->__is_in), __is_out(node->__is_out),
    num_nodes_deflected(node->num_nodes_deflected), num_nodes_routed(node->num_nodes_routed)
  {
    /* Empty */
  }

  Grid_Node(const Point & point)
    : Euclidian_Node<long>(point),  __is_in(false), __is_out(false)
  {
    /* Empty */
  }

  Grid_Node(const long & num, const Point & point)
    : Euclidian_Node<long>(num, point),  __is_in(false), __is_out(false)
  {
    /* Empty */
  }

  bool & is_in() { return __is_in; }

  bool & is_out() { return __is_out; }

  const unsigned long & get_num_nodes_deflected() const { return num_nodes_deflected; }

  const unsigned long & get_num_nodes_routed() const { return num_nodes_routed; }

  void inc_num_nodes_deflected() { ++num_nodes_deflected; }

  void inc_num_nodes_routed() { ++num_nodes_routed; }

  const bool is_io() const { return __is_in and __is_out; }

  /**
    * Coloca un paquete en la lista de clientes del nodo para ser enrutado posteriormente.
    */
  void add_client(Package *);

  /**
    * Coloca un paquete en la cola de espera del nodo, esto ocurre &uacute;nicamente cuando el paquete
    * intenta entrar en la malla pero el nodo por el que llega tiene su lista de clientes llena.
    */
  void put_in_queue(Package *);

  DynDlist<Package *> & get_clients_list() { return clients; }

  DynDlist<Package *> & get_queue() { return queue; }

  /**
    * Retorna una descripci&oacute;n del nodo, en este caso es la palabra Nodo con el n&uacute;mero que lo identifica.
    */
  const std::string to_string() const { return gnu::autosprintf("Nodo %ld", get_info()); }
};

/**
  * \brief Modela un enlace de la Malla.
  *
  * La clase se basa en Euclidian_Arc<Arc_Info> de la biblioteca
  * <a url="http://webdelprofesor.ula.ve/ingenieria/lrleon/aleph/html/index.html" target="_blank">ALEPH-w</a>
  * sin peso.
  * Euclidian_Arc modela un arco de grafo con nodo fuente y nodo destino y se puede representar de la siguiente
  * manera: src_node -- tgt_node.
  * En un grafo no dirigido, src_node y tgt_node no tienen diferencia ya que cualquiera de los dos nodos pueden ser
  * fuente o destino en cualquier momento.
  * @authors Alejandro Mujica, Anna Lezama
  */
class Link : public Euclidian_Arc<Empty_Class>
{
  Direction dir;

  Package * package_from_src;
  Package * package_from_tgt;

public:
  Link() : Euclidian_Arc<Empty_Class>(), dir(Num_Directions), package_from_src(NULL), package_from_tgt(NULL)
  { 
    /* Empty */
  }

  Link(const Empty_Class & info) : Euclidian_Arc<Empty_Class>(info),
  dir(Num_Directions), package_from_src(NULL), package_from_tgt(NULL)
  {
    /* Empty */
  }

  Link(void * src, void * tgt, const Empty_Class & data) : Euclidian_Arc<Empty_Class>(src, tgt, data),
  dir(Num_Directions), package_from_src(NULL), package_from_tgt(NULL)
  {
    /* Empty */
  }

  Link(void * src, void * tgt) : Euclidian_Arc<Empty_Class>(src, tgt),
  dir(Num_Directions), package_from_src(NULL), package_from_tgt(NULL)
  {
    /* Empty */
  }

  /**
    * Retorna la direcci&oacute;n (Norte, Sur, Este, Oeste) del arco respecto al nodo fuente (modificable).
    */
  Direction & get_direction() { return dir; }

  /**
    * Retorna la direcci&oacute;n (Norte, Sur, Este, Oeste) del arco respecto al nodo fuente (constante).
    */
  const Direction & get_direction() const { return dir; }

  /**
    * Retorna el paquete que est&aacute; pasando por el arco que viene del nodo fuente (src_node).
    */
  Package * get_package_from_src()
  {
    return package_from_src;
  }

  void set_package_from_src(Package * p)
  {
    package_from_src = p;

  }

  /**
    * Paquete que est&aacute; pasando por el arco que viene del nodo destino (tgt_node).
    */
  Package * get_package_from_tgt()
  {
    return package_from_tgt;
  }

  void set_package_from_tgt(Package * p)
  {
    package_from_tgt = p;
  }

  const bool is_busy_for_src() const { return package_from_src != NULL; }

  const bool is_busy_for_tgt() const { return package_from_tgt != NULL; }

  void reset()
  {
    package_from_src = package_from_tgt = NULL;
  }
};


/**
  * \brief Modela una Malla 2D.
  *
  * La clase est&aacute; basada en Euclidian_Graph<__Euclidian_Node, __Euclidian_Arc> de la biblioteca
  * <a url="http://webdelprofesor.ula.ve/ingenieria/lrleon/aleph/html/index.html" target="_blank">ALEPH-w</a>
  * con par&aacute;metros tipo Grid_Node y Link.
  */
class Grid : public Euclidian_Graph<Grid_Node, Link>
{
  size_t width;
  size_t height;
  
public:
  typedef Euclidian_Graph<Grid_Node, Link> Graph;

  Grid()
    : Graph(), width(), height() { /* Empty */ }

  Grid(const size_t & w, const size_t & h)
    : Graph(), width(w), height(h) { /* Empty */ }

  Grid(const Grid & grid)
    : Graph(grid), width(grid.width), height(grid.height)
  { /* Empty */ }

  ~Grid() { /* Empty */ }

  void set_height(const size_t & h) { height = h; }

  const size_t & get_height() const { return height; }

  void set_width(const size_t & w) { width = w; }

  const size_t & get_width() const { return width; }

  void build_automatic_grid()
  {
    Grid_Builder<Grid, Op_Node> ()(*this, width, height);
  }

  Node * insert_grid_node(const int & num, const long & x, const long & y)
  {
    std::auto_ptr<Node> node(new Node(num));
    node->get_position() = Point(x, y);
    return Graph::insert_node(node.release());
  }

  Arc * insert_arc(Node * src, Node * tgt)
  {
    Arc * arc = Graph::insert_arc(src, tgt);
    if (src->get_position().get_x().get_d() < tgt->get_position().get_x().get_d() and
        src->get_position().get_y().get_d() == tgt->get_position().get_y().get_d())
      arc->get_direction() = East;
    else if (src->get_position().get_x().get_d() > tgt->get_position().get_x().get_d() and
             src->get_position().get_y().get_d() == tgt->get_position().get_y().get_d())
      arc->get_direction() = West;
    else if (src->get_position().get_y().get_d() < tgt->get_position().get_y().get_d() and
             src->get_position().get_x().get_d() == tgt->get_position().get_x().get_d())
      arc->get_direction() = South;
    else if (src->get_position().get_y().get_d() > tgt->get_position().get_y().get_d() and
             src->get_position().get_x().get_d() == tgt->get_position().get_x().get_d())
      arc->get_direction() = North;
    return arc;
  }

  /**
    * Retorna la direcci&oacute;n de un enlace respecto a un nodo.
    * @param arc Arco al que se le quiere conocer la direcci&oacute;n.
    * @param node Nodo respecto al cual se desea conocer la direcci&oacute;n del arco.
    */
  Direction get_arc_direction_respect_node(Arc * arc, Node * node)
  {
    return get_src_node(arc) == node ? arc->get_direction() : get_reflect_direction(arc->get_direction());
  }

  /**
    * Retorna el enlace que, euclidianamente, se encuentra a una direcci&oacute;n dada del nodo deseado.
    * @param node Nodo desde donde se desea el enlace
    * @param dir La direcci&oacute;n a la que se quiere acceder.
    */
  Arc * get_arc_by_rout(Node * node, const Direction & dir)
  {
    for (Node_Arc_Iterator it(node); it.has_current(); it.next())
      {
        Arc * a = it.get_current();
        if (get_src_node(a) == node and dir == a->get_direction())
          return a;
        if (get_tgt_node(a) == node and dir == get_reflect_direction(a->get_direction()))
          return a;
      }
    return NULL;
  }

  const bool is_arc_busy_for_node(Arc * arc, Node * node)
  {
    return get_src_node(arc) == node ? arc->is_busy_for_src() : arc->is_busy_for_tgt();
  }

  /**
    * Retorna el primer enlace adyacente a un nodo que no est&eacute; ocupado.
    */
  Arc * get_first_free_link(Node * node)
  {
    for (Node_Arc_Iterator it(node); it.has_current(); it.next())
      {
        Arc * arc = it.get_current();
        if (not is_arc_busy_for_node(arc, node))
          return arc;
      }
    return NULL;
  }

  void set_arc_busy_for_node(Arc * arc, Node * node, Package * p)
  {
    if (get_src_node(arc) == node)
      arc->set_package_from_src(p);
    else
      arc->set_package_from_tgt(p);
  }

  bool is_arc_horizontal(Arc * arc)
  {
    Node * src = get_src_node(arc);
    Node * tgt = get_tgt_node(arc);
    return src->get_position().get_y().get_d() == tgt->get_position().get_y().get_d();
  }

  bool is_arc_vertical(Arc * arc)
  {
    Node * src = get_src_node(arc);
    Node * tgt = get_tgt_node(arc);
    return src->get_position().get_x().get_d() == tgt->get_position().get_x().get_d();
  }

  void reset_all_arcs()
  {
    for (Arc_Iterator it(*this); it.has_current(); it.next())
      {
        Arc * arc = it.get_current();
        arc->reset();
      }
  }
};

/**
  * \brief Operaci&oacute;n que se realiza sobre cada nodo el crear la Malla.
  *
  * Esta esla clase que se le pasa como par&aacute;metro a la clase Grid_Builder de operaci&oacute;n
  * a realizar sobre el nodo cuando lo crea, lo que hace es asignarle las coordenadas euclidianas.
  */
class Op_Node
{
public:
  void operator () (Grid & g, Grid_Node * node, const size_t & i, const size_t & j)
  {
    node->get_info() = g.get_width() * i + j;
    node->get_position() = Point(j, i);
  }
};

# endif

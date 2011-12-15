# ifndef MISC_H
# define MISC_H

# include <tpl_euclidian_graph.H>

# include <cmath>

# define WIDTH_GRID_DRAW_AREA                                     600
# define MINIMUM_X_SCALE                                          100
# define X_RATIO                                                   10
# define X_RATIO_2                                      (X_RATIO / 2)
# define X_2_RATIO                                      (2 * X_RATIO)
# define WIDTH_GRID_PANEL          (WIDTH_GRID_DRAW_AREA + X_2_RATIO)
# define X_PANEL_CENTER                        (WIDTH_GRID_PANEL / 2)
# define HEIGHT_GRID_DRAW_AREA                                    600
# define MINIMUM_Y_SCALE                                          100
# define Y_RATIO                                                   10
# define Y_RATIO_2                                      (Y_RATIO / 2)
# define Y_2_RATIO                                      (2 * Y_RATIO)
# define HEIGHT_GRID_PANEL        (HEIGHT_GRID_DRAW_AREA + Y_2_RATIO)
# define Y_PANEL_CENTER                       (HEIGHT_GRID_PANEL / 2)

/**
  * \brief Enumerado de algoritmos de enrutamiento.
  */
enum Routing_Algorithm
{
  A_Minimum_Deflection,
  Num_Routing_Algorithms
};

/**
  * \brief Direcci&oacute;n que tiene un arco de la malla respecto a su nodo fuente.
  */
enum Direction
{
  North,
  South,
  East,
  West,
  Num_Directions
};

/**
  * Dada una direcci&oacuten; se obtiene la direcci&oacuten; opuesta.
  * @param dir Direcci&oacute;n original.
  * @return La direcci&oacute;n opuesta.
  * @exception domain_error si la direcci&oacute;n pasada como par@aacute;metro no es v@aacute;lida.
  */
inline Direction get_reflect_direction(const Direction & dir)
{
  switch(dir)
    {
    case North: return South;
    case South: return North;
    case East:  return West;
    case West:  return East;
    default: throw std::domain_error("Invalid value of direction");
    }
}

class Geometric
{
  Geometric() { /* Empty */ }
public:
  static bool is_point_inside_ellipse(const Point & point, const Point & center,
                                      const size_t & hr, const size_t & vr)
  {
    return (pow(point.get_x().get_d() - center.get_x().get_d(), 2) / pow(hr, 2)) + (pow(point.get_y().get_d() - center.get_y().get_d(), 2) / pow(vr, 2)) <= 1;
  }
};

template <class GT>
struct Default_Operation_On_Node
{
  void operator () (GT &, typename GT::Node *, const size_t &, const size_t &) { /* Empty */ }
};

template <class GT>
struct Default_Operation_On_Arc
{
  void operator () (GT &, typename GT::Arc *, const size_t &, const size_t &) { /* Empty */ }
};

/**
   * \brief Clase que construye un grafo en forma de Malla 2D con topolog&iacute;a octagonal
   *
   * La clase recibe los siguientes par&aacute;metros tipo:
   * <ol>
   * <li> GT: el tipo de grafo basado en List_Graph
   * <li> Operation_On_Node: La clase de operaci&oacute;n sobre los nodos que debe exportar lo siguiente
   *    <ul>
   *    <li> Operation_On_Node::operator()(GT & g, typename GT::Node * node, const size_t & i, const size_t & j) que retorna void.
   *    </ul>
   *    .
   * <li> Operation_On_Arc: La clase de operaci&oacute;n sobre los arcos que debe exportar lo siguiente
   *    <ul>
   *    <li> Operation_On_Arc::operator()(GT & g, typename GT::Arc * arc, const size_t & i, const size_t & j) que retorna void.
   *    </ul>
   * </ol>
   * @authors Alejandro Mujica, Anna Lezama
   */
template <
          class GT,
          class Operation_On_Node = Default_Operation_On_Node<GT>,
          class Operation_On_Arc = Default_Operation_On_Arc<GT>
         >
class Grid_Builder
{
public:
  void operator () (GT & g, const size_t & width, const size_t & height)
  {
    clear_graph(g);

    if (width < 2 or height < 2)
      throw std::length_error("The minimun size must be 2 x 2");

    typename GT::Node *** map = new typename GT::Node **[height];
    for (size_t i = 0; i < height; ++i)
      {
	try
	  {
	    map[i] = new typename GT::Node *[width];
	    for (size_t j = 0; j < width; ++j)
	      {
		typename GT::Node * n = g.insert_node(typename GT::Node_Type());
                Operation_On_Node()(g, n, i, j);
		map[i][j] = n;
		if (j > 0) //Conecta con el nodo a su izquierda
		  {
		    typename GT::Arc * a = g.insert_arc(n, map[i][j - 1]);
                    Operation_On_Arc()(g, a, i, j);
		  }
		if (i > 0) //Conecta con el nodo que esta arriba
		  {
		    typename GT::Arc * a = g.insert_arc(n, map[i - 1][j]);
                    Operation_On_Arc()(g, a, i, j);
		  }
	      }
	  }
	catch (...)
	  {
            //Si se captura excepcion libero la memoria apartada hasta ahora y la relanzo	
	    for (size_t k = i - 1; k >= 0; --k)
	      delete [] map[k];
	    delete [] map;
	    clear_graph(g);
	    throw;
	  }
      }

    for (size_t i = 0; i < height; ++i)
      delete [] map[i];
    delete [] map;
  }
};

# endif


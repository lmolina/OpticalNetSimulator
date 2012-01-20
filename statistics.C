# include <statistics.H>
# include <configuration.H>

Statistics * Statistics::instance = NULL;

Statistics::Statistics()
  : num_iterations(0), generated_packages(0), deliveried_packages(0),
  package_in_queue(0), max_deflections(0), deflections_counter(NULL)
{
  // Empty
}

Statistics::~Statistics()
{
  if (instance != NULL)
    {
      delete instance;
      if (deflections_counter != NULL)
        delete []deflections_counter;
    }
}

Statistics * Statistics::get_instance()
{
  if (instance == NULL)
    instance = new Statistics();
  return instance;
}

void Statistics::inc_iterations()
{
  ++num_iterations;
  events_list_per_iteration.append(Pair_Of_List());
}

void Statistics::inc_generated_packages()
{
  ++generated_packages;
}

void Statistics::inc_deliveried_packages()
{
  ++deliveried_packages;
}

void Statistics::inc_packages_in_queue()
{
  ++package_in_queue;
}

void Statistics::dec_packages_in_queue()
{
  --package_in_queue;
}

void Statistics::add_statistic_to_node(std::string & node_name, std::string & statistic)
{
  Statistics_Per_Element * s = search_node_statistic(node_name);
  if (s == NULL)
    s = &(events_list_per_iteration.get_last().statistics_per_node.append(Statistics_Per_Element(node_name)));
  s->add_statistic(statistic);
}

void Statistics::add_statistic_to_package(std::string & package_name, std::string & statistic)
{
  Statistics_Per_Element * s = search_package_statistic(package_name);
  if (s == NULL)
    s = &(events_list_per_iteration.get_last().statistics_per_package.append(Statistics_Per_Element(package_name)));
  s->add_statistic(statistic);
}

Statistics_Per_Element * Statistics::search_package_statistic(const std::string & name)
{
  DynDlist<Statistics_Per_Element> & statistics_per_package = events_list_per_iteration.get_last().statistics_per_package;
  for (DynDlist<Statistics_Per_Element>::Iterator it(statistics_per_package); it.has_current(); it.next())
    {
      Statistics_Per_Element & s = it.get_current();
      if (s.get_element_name() == name)
        return &s;
    }
  return NULL;
}

Statistics_Per_Element * Statistics::search_node_statistic(const std::string & name)
{
  DynDlist<Statistics_Per_Element> & statistics_per_node = events_list_per_iteration.get_last().statistics_per_node;
  for (DynDlist<Statistics_Per_Element>::Iterator it(statistics_per_node); it.has_current(); it.next())
    {
      Statistics_Per_Element & s = it.get_current();
      if (s.get_element_name() == name)
        return &s;
    }
  return NULL;
}

# include <iostream>

void Statistics::print()
{
    Configuration * conf;

    conf = Configuration::get_instance();


  std::cout << "Resultados de la Simulacion" << std::endl << std::endl;
  switch (conf->get_routing_algorithm()){
      case A_Minimum_Deflection: std::cout << "Algoritmo de enrutamiento: Deflexion Minima" << std::endl << std::endl;
                          break;
      case A_Dijkstra_Routing: std::cout << "Algoritmo de enrutamiento: Dijkstra" << std::endl << std::endl;
                          break;
      default: std::cout << " Dijkstra" << std::endl << std::endl;
          break;
  }
  std::cout << "Numero de iteraciones 2: " << get_num_iterations() << std::endl;
  std::cout << "Numero de paquetes generados: " <<  get_generated_packages() << std::endl;
  std::cout << "Numero de paquetes en cola: " <<  get_num_packages_in_queue() << std::endl;
  std::cout << "Numero de paquetes entregados: " << get_deliveried_packages() << std::endl;
  std::cout << "Numero de paquetes en movimiento: " << get_generated_packages() - get_deliveried_packages() - get_num_packages_in_queue() << std::endl << std::endl;

  std::cout << "Tiempo promedio de un paquete en cola: " << tiq_avg << " iteraciones" << std::endl;
  std::cout << "Tiempo promedio de un paquete en movimiento: " << ttl_avg << " iteraciones" << std::endl << std::endl;

  std::cout << "Resultados por paquete" << std::endl << std::endl;
  unsigned long i = 0;
  for (DynDlist<Pair_Of_List>::Iterator it(events_list_per_iteration); it.has_current(); it.next(), ++i)
    {
     if (i > 0)
       std::cout << "Iteracion numero " << i << std::endl << std::endl;
     else
       std::cout << "Estado inicial" << std::endl << std::endl;
      DynDlist<Statistics_Per_Element> & statistics_per_package = it.get_current().statistics_per_package;
      for (DynDlist<Statistics_Per_Element>::Iterator it(statistics_per_package); it.has_current(); it.next())
        {
          Statistics_Per_Element & s = it.get_current();
          std::cout << "Paquete: " << s.get_element_name() << std::endl;
          DynDlist<std::string> & h = s.get_history();
          for (DynDlist<std::string>::Iterator it(h); it.has_current(); it.next())
            {
              std::string & st = it.get_current();
              std::cout << "Evento: " << st << std::endl;
            }
          std::cout << std::endl;
        }
    }

  std::cout << "Numero maximo de deflexiones: " << max_deflections << std::endl;

  for (int i = 0; i < max_deflections + 1; ++i)
    cout << "Numero de paquetes deflectados " << i
         << (i == 1 ? " vez: " : " veces: ") << deflections_counter[i] << endl;
}

void Statistics::clear()
{
  num_iterations = generated_packages = deliveried_packages = package_in_queue = 0;
  events_list_per_iteration.empty();
  delete []deflections_counter;
  max_deflections = 0;
}
void Statistics::init()
{
  events_list_per_iteration.append(Pair_Of_List());
}

void Statistics::set_max_deflections(const unsigned long & m)
{
  max_deflections = MAX(m, max_deflections);
}

void Statistics::count_deflections(DynDlist<Package> & packages)
{
  unsigned long long acum_ttl = 0, acum_tiq = 0;

  deflections_counter = new unsigned long[max_deflections + 1];
  for (int i = 0; i < max_deflections + 1; ++i)
    deflections_counter[i] = 0;
  for (DynDlist<Package>::Iterator it(packages); it.has_current(); it.next())
    {
      Package & p = it.get_current();
      ++deflections_counter[p.get_num_deflections()];
      acum_ttl += p.get_ttl();
      acum_tiq += p.get_tiq();
    }

  ttl_avg = double (acum_ttl / packages.size());
  tiq_avg = double (acum_tiq / packages.size());
}

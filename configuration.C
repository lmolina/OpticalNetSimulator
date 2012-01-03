# include <configuration.H>

Configuration * Configuration::instance = 0;

Configuration::Configuration()
  : grid_width(3), grid_height(3), num_iterations(10), mu(5.0)
{
  // Empty
}

Configuration * Configuration::get_instance()
{
  if (instance == 0)
    instance = new Configuration();
  return instance;
}

Configuration::~Configuration()
{
  if (instance != 0)
    delete instance;
}

const unsigned long & Configuration::get_grid_width() const
{
  return grid_width;
}

const unsigned long & Configuration::get_grid_height() const
{
  return grid_height;
}

const unsigned long long & Configuration::get_num_iterations() const
{
  return num_iterations;
}

const double & Configuration::get_mu() const
{
  return mu;
}

const Routing_Algorithm & Configuration::get_routing_algorithm() const
{
    return routing_algorithm;
}

void Configuration::set_grid_width(const unsigned long & w)
{
  grid_width = w;
}

void Configuration::set_grid_height(const unsigned long & h)
{
  grid_height = h;
}

void Configuration::set_num_iterations(const unsigned long long & n)
{
  num_iterations = n;
}

void Configuration::set_mu(const double & m)
{
  mu = m;
}

void Configuration::set_routing_algorithm(const Routing_Algorithm & ra)
{
    routing_algorithm = ra;
}

void Configuration::save()
{
  std::ofstream file("properties.conf", std::ios::out | std::ios::binary);

  if (not file)
    throw std::logic_error("Cannot open file");

  char header[HEADER_SIZE];
  strcpy(header, "***ONS***OPTICALNETSIMULATOR***BY_ALEJANDRO_Y_ANNA***");

  file.write(header, HEADER_SIZE);
  file.write(reinterpret_cast<char *>(&grid_width), sizeof(unsigned long));
  file.write(reinterpret_cast<char *>(&grid_height), sizeof(unsigned long));
  file.write(reinterpret_cast<char *>(&num_iterations), sizeof(unsigned long long));
  file.write(reinterpret_cast<char *>(&mu), sizeof(double));
  file.write(reinterpret_cast<char *>(&routing_algorithm), sizeof(routing_algorithm));

  file.close();
}

void Configuration::load()
{
  std::ifstream file("properties.conf", std::ios::in | std::ios::binary);

  if (not file)
    throw std::logic_error("Cannot open file");

  char header[HEADER_SIZE];
  file.read(header, HEADER_SIZE);
  if (strcmp(header, "***ONS***OPTICALNETSIMULATOR***BY_ALEJANDRO_Y_ANNA***"))
    {
      file.close();
      throw std::logic_error("Invalid file");
    }
  file.read(reinterpret_cast<char *>(&grid_width), sizeof(unsigned long));
  file.read(reinterpret_cast<char *>(&grid_height), sizeof(unsigned long));
  file.read(reinterpret_cast<char *>(&num_iterations), sizeof(unsigned long long));
  file.read(reinterpret_cast<char *>(&mu), sizeof(double));
  file.read(reinterpret_cast<char *>(&routing_algorithm), sizeof(routing_algorithm));

  file.close();
}

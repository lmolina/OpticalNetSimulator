# ifndef CONFIGURATION_H
# define CONFIGURATION_H

# include <stdexcept>
# include <fstream>

# include <cstring>

# define HEADER_SIZE 64

class Configuration
{
  static Configuration * instance;

  unsigned long grid_width;
  unsigned long grid_height;
  unsigned long long num_iterations;
  double mu;

  Configuration();

public:

  static Configuration * get_instance();

  ~Configuration();

  const unsigned long & get_grid_width() const;

  const unsigned long & get_grid_height() const;

  const unsigned long long & get_num_iterations() const;

  const double & get_mu() const;

  void set_grid_width(const unsigned long &);

  void set_grid_height(const unsigned long &);

  void set_num_iterations(const unsigned long long &);

  void set_mu(const double &);

  void save();

  void load();
};

# endif // CONFIGURATION_H


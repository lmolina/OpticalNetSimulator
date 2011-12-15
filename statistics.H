# ifndef STATISTICS_H
# define STATISTICS_H

# include <statistics_per_element.H>
# include <cstdio>
# include <package.H>

class Package;

# ifndef MAX(a, b)
# define MAX(a, b) (a > b ? a : b)
# endif

class Statistics
{
public:

  struct Pair_Of_List
  {
    DynDlist<Statistics_Per_Element> statistics_per_package;
    DynDlist<Statistics_Per_Element> statistics_per_node;
  };

private:

  static Statistics * instance;

  unsigned long num_iterations;
  unsigned long generated_packages;
  unsigned long deliveried_packages;
  unsigned long package_in_queue;

  unsigned long * deflections_counter;
  unsigned long max_deflections;

  double ttl_avg;
  double tiq_avg;

  DynDlist<Pair_Of_List> events_list_per_iteration;

  Statistics();

public:

  Statistics_Per_Element * search_package_statistic(const std::string &);

  Statistics_Per_Element * search_node_statistic(const std::string &);

  ~Statistics();

  DynDlist<Pair_Of_List> & get_events_list_per_iteration() { return events_list_per_iteration; }

  const unsigned long & get_num_iterations() const
  {
    return num_iterations;
  }

  const unsigned long & get_generated_packages() const
  {
    return generated_packages;
  }

  const unsigned long & get_deliveried_packages() const
  {
    return deliveried_packages;
  }

  const unsigned long & get_num_packages_in_queue() const
  {
    return package_in_queue;
  }

  const double & get_ttl_avg() const
  {
    return ttl_avg;
  }

  const double & get_tiq_avg() const
  {
    return tiq_avg;
  }

  unsigned long * get_deflections_counter()
  {
    return deflections_counter;
  }

  const unsigned long & get_max_deflections() const
  {
    return max_deflections;
  }

  static Statistics * get_instance();

  void inc_iterations();

  void inc_generated_packages();

  void inc_deliveried_packages();

  void inc_packages_in_queue();

  void dec_packages_in_queue();

  void set_max_deflections(const unsigned long &);

  void add_statistic_to_node(std::string &, std::string &);

  void add_statistic_to_package(std::string &, std::string &);

  void print();

  void clear();

  void init();

  void count_deflections(DynDlist<Package> &);
};

#endif // STATISTICS_H

# ifndef STATISTICS_PER_ELEMENT_H
# define STATISTICS_PER_ELEMENT_H

# include <string>

# include <tpl_dynDlist.H>

class Statistics_Per_Element
{
  std::string e_name;

  DynDlist<std::string> history;

public:
  Statistics_Per_Element();

  Statistics_Per_Element(const char *);

  Statistics_Per_Element(const std::string &);

  DynDlist<std::string> & get_history() { return history; }

  const char * get_element_name() { return e_name.c_str(); }

  const std::string & get_element_name() const { return e_name; }

  void add_statistic(const std::string &);

  void add_statistic(const char *);
};

# endif // STATISTICS_PER_ELEMENT_H

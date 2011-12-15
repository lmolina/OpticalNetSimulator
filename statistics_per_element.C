# include <statistics_per_element.H>

Statistics_Per_Element::Statistics_Per_Element()
{
  // Empty
}

Statistics_Per_Element::Statistics_Per_Element(const char * _e_name)
  : e_name(_e_name)
{
  // Empty
}

Statistics_Per_Element::Statistics_Per_Element(const std::string & _e_name)
  : e_name(_e_name)
{
  // Empty
}

void Statistics_Per_Element::add_statistic(const std::string & s)
{
  history.append(s);
}

void Statistics_Per_Element::add_statistic(const char * s)
{
  history.append(std::string(s));
}

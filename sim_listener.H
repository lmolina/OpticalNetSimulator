# ifndef SIM_LISTENER_H
# define SIM_LISTENER_H

enum Event_Type
{
  Step,
  Start,
  Stop,
  Num_Events_Type
};

class Sim_Listener
{
public:
  virtual void step() = 0;

  virtual void start() = 0;

  virtual void stop() = 0;
};

# endif // SIM_LISTENER_H

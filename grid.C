# include <grid.H>

void Grid_Node::add_client(Package * p)
{
  p->reset();
  Grid_Node * package_target = p->get_target_node();
  if (package_target->get_position().get_x().get_d() < get_position().get_x().get_d())
    p->get_h_favorable_direction() = West;
  else if (package_target->get_position().get_x().get_d() > get_position().get_x().get_d())
    p->get_h_favorable_direction() = East;
  if (package_target->get_position().get_y().get_d() < get_position().get_y().get_d())
    p->get_v_favorable_direction() = North;
  else if (package_target->get_position().get_y().get_d() > get_position().get_y().get_d())
    p->get_v_favorable_direction() = South;
  clients.append(p);
}

void Grid_Node::put_in_queue(Package * p)
{
  p->is_in_queue() = true;
  queue.append(p);
}

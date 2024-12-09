#include "exgui.h"

bool exgui_base::add_child(exgui_base* p_child)
{
  /* prevent add nullptr */
  if (!p_child)
    return false;

  /* prevent enter infinity loop in any event and stack overflowing */
  if (this == p_child)
    return false;

  /* element with no childs */
  if(!m_elem_flags.has_childs())
    return false;

  m_childs.push_back(p_child);
  return true;
}

void exgui_root::event_dispatcher(exgui_base* p_elem)
{
}

void exgui_root::draw_recursive(exgui_base* p_elem, NVGcontext* p_ctx)
{
  /* is visible? */
  if (p_elem->get_elem_flags().has_visible()) {
    /* add element to draw path container */
    m_draw_list.push_back(p_elem);
    /* element has childs? */
    if (p_elem->get_elem_flags().has_childs()) {
      /* recursive enum childs */
      for (size_t i = 0; i < p_elem->get_num_childs(); i++) {
        /* enter recursive */
        draw_recursive(p_elem->get_child(i), p_ctx);
      }
    }
  }
}

exgui_root::exgui_root()
{
}

exgui_root::~exgui_root()
{
}

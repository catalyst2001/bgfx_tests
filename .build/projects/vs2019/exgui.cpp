#include <algorithm>
#include "exgui.h"

bool exgui_base::add_child(exgui_base* p_child)
{
  /* prevent add nullptr */
  if (!p_child)
    return false;

  /* prevent enter infinity loop in any event and stack overflowing */
  if (this == p_child)
    return false;

  /* if element with no childs */
  if(!m_elem_flags.has_childs())
    return false;

  m_childs.push_back(p_child);
  root_update();
  return true;
}

bool exgui_base::remove_child(exgui_base* p_child)
{
  /* prevent remove nullptr */
  if (!p_child)
    return false;

  /* if element with no childs */
  if (!m_elem_flags.has_childs())
    return false;

  _childs_vec::iterator it = std::find(m_childs.begin(), m_childs.end(), p_child);
  if (it != m_childs.end()) {
    /* child found */
    m_childs.erase(it);
    p_child->on_event(PARENT_CHANGED);
    p_child->set_parent(nullptr);
  }
  return true;
}

void exgui_base::set_parent(exgui_base* p_parent)
{
  m_pparent = p_parent;
  root_update();
}

void exgui_root::event_dispatcher(exgui_base* p_elem)
{
}

void exgui_root::keybd_dispatcher(exgui_base* p_elem, int sc, EXGUI_KEY vk, EXGUI_KEY_STATE state)
{
  /* add element to draw path container */
  p_elem->on_keybd(sc, vk, state);
  /* element has childs? */
  if (p_elem->get_elem_flags().has_childs() && p_elem->get_elem_flags().has_notify_childs()) {
    /* recursive enum childs */
    for (size_t i = 0; i < p_elem->get_num_childs(); i++) {
      /* enter recursively */
      keybd_dispatcher(p_elem->get_child(i), sc, vk, state);
    }
  }
}

void exgui_root::build_draw_cache_recursive(exgui_base* p_elem)
{
  /* is visible? */
  if (p_elem->get_elem_flags().has_visible()) {
    /* add element to draw path container */
    m_draw_cache.push_back(p_elem);
    /* element has childs? */
    if (p_elem->get_elem_flags().has_childs()) {
      /* recursive enum childs */
      for (size_t i = 0; i < p_elem->get_num_childs(); i++) {
        /* enter recursively */
        build_draw_cache_recursive(p_elem->get_child(i));
      }
    }
  }
}

void exgui_root::rebuild_draw_cache()
{
  m_draw_cache.clear();
  build_draw_cache_recursive(this);
}

void exgui_root::draw()
{
  nvgBeginFrame(m_pctx, );
  for (size_t i = 0; i < m_draw_cache.size(); i++) {
    m_draw_cache[i]->on_draw(m_pctx);
  }
  nvgEndFrame(m_pctx);
}

exgui_root::exgui_root() : exgui_base(nullptr, "ui_root_node")
{
  set_root(this);
}

exgui_root::~exgui_root()
{
}

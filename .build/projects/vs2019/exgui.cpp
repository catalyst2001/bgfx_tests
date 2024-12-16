#include <algorithm>
#include "exgui.h"

bool exgui_widget::add_child(exgui_widget* p_child)
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

bool exgui_widget::remove_child(exgui_widget* p_child)
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
    p_child->on_event(PARENT_CHANGED, this);
    p_child->set_parent(nullptr);
  }
  return true;
}

void exgui_widget::set_parent(exgui_widget* p_parent)
{
  m_pparent = p_parent;
  root_update();
}

void exgui_surface::event_dispatcher(exgui_widget* p_elem)
{
}

void exgui_surface::keybd_dispatcher(exgui_widget* p_elem, int sc, EXGUI_KEY vk, EXGUI_KEY_STATE state)
{
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

#if 0
void exgui_surface::text_input_dispatcher(exgui_widget* p_elem, int sym)
{
  p_elem->on_text_input(sym);
  /* element has childs? */
  if (p_elem->get_elem_flags().has_childs() && p_elem->get_elem_flags().has_notify_childs()) {
    /* recursive enum childs */
    for (size_t i = 0; i < p_elem->get_num_childs(); i++) {
      /* enter recursively */
      text_input_dispatcher(p_elem->get_child(i), sym);
    }
  }
}
#endif

void exgui_surface::mouse_dispatcher(exgui_widget* p_elem, EXGUI_MOUSE_EVENT event, EXGUI_KEY vk, EXGUI_KEY_STATE state, exgui_vector2& cursor_pos)
{
  /* cursor is focused? */
  bool b_cursor_inside = p_elem->get_bbox().inside(cursor_pos);
  p_elem->m_elem_flags.toggle_bits(EXGUI_FLAG_HOVERED, b_cursor_inside);

  /* cursor inside of widget? */
  if (b_cursor_inside) {
    /* check mouse events */
    switch (event)
    {
    case EXGUI_MOUSE_EVENT_MOVE:
      /* notify widget */
      p_elem->on_mouse(event, vk, state, cursor_pos);
      break;

    case EXGUI_MOUSE_EVENT_CLICK:
      /* clicked to surface widget? */
      if (p_elem == this) {
        /* not focused */
        m_pfocus = nullptr;
      }
      /* widget is not surface? */
      m_pfocus = p_elem; // set current widget to focused
      break;
    }
  }
  
  /* element has childs? */
  if (p_elem->get_elem_flags().has_childs() && p_elem->get_elem_flags().has_notify_childs()) {
    /* recursive enum childs */
    for (size_t i = 0; i < p_elem->get_num_childs(); i++) {
      /* enter recursively */
      mouse_dispatcher(p_elem->get_child(i), event, vk, state, cursor_pos);
    }
  }
}

void exgui_surface::build_draw_cache_recursive(exgui_widget* p_elem)
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

void exgui_surface::rebuild_draw_cache()
{
  m_draw_cache.clear();
  build_draw_cache_recursive(this);
}

void exgui_surface::draw()
{
  nvgBeginFrame(m_pctx, m_outer_rect.right, m_outer_rect.bottom, 1.f);
  for (size_t i = 0; i < m_draw_cache.size(); i++) {
    m_draw_cache[i]->on_draw(m_pctx);
  }
  nvgEndFrame(m_pctx);
}

void exgui_surface::keybd(int sc, EXGUI_KEY vk, EXGUI_KEY_STATE state)
{
  keybd_dispatcher(this, sc, vk, state);
}

void exgui_surface::textinput(int sym)
{
#if 0
  text_input_dispatcher(this, sym);
#endif
  if (m_pfocus)
    m_pfocus->on_text_input(sym);
}

void exgui_surface::mouse(EXGUI_MOUSE_EVENT event, EXGUI_KEY vk, EXGUI_KEY_STATE state, int x, int y)
{
  exgui_vector2 mouse_pos(x, y);
  mouse_dispatcher(this, event, vk, state, mouse_pos);
}

exgui_surface::exgui_surface(NVGcontext* p_ctx, int width, int height) : exgui_widget(0, 0, width, height, nullptr, "ui_root_node")
{
  set_root(this);
  m_pctx = p_ctx;
  m_pfocus = nullptr;
}

exgui_surface::~exgui_surface()
{
}

void exgui_window::on_draw(NVGcontext* p_ctx)
{
  nvgSave(p_ctx);
  exgui_window_style* p_style = get_style();
  assert(p_style && "exgui_window::on_draw(): window style is not set! Use exgui_window::set_style(exgui_wi1ndow_style *)");
  int b_is_active = (int)(get_elem_flags().is_focused() || get_elem_flags().is_hovered());
  /* draw window background */
  nvgBeginPath(p_ctx);
  nvgFillColor(p_ctx, p_style->get_background_color(b_is_active));
  nvgRoundedRectVarying(p_ctx,
    m_outer_rect.left, m_outer_rect.top, m_outer_rect.right, m_outer_rect.bottom,
    p_style->get_corner_radius(LEFT_TOP), p_style->get_corner_radius(RIGHT_TOP), 0, 0);
  nvgFill(p_ctx);





  nvgFontFaceId(p_ctx, get_font());
  nvgFontSize(p_ctx, p_style->get_font_size());







  nvgRestore(p_ctx);
}

exgui_window::exgui_window(exgui_widget* p_parent, int x, int y, int width, int height, uint32_t flags, uint32_t uflags, void* p_userptr) :
  exgui_widget(x, y, width, height, p_parent, "ui_window", flags, uflags, p_userptr)
{
}

exgui_window::~exgui_window()
{
}

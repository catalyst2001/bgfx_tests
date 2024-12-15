/**
* MIT License
* Copyright (c) 2024. Kirill Deryabin  kd@allalg.org
* 
* Retained Mode GUI (RmGUI)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#pragma once
#include "nanovg/nanovg.h"
#include <vector>
#include <string>
#include <cassert>

/* utils */
#define EXGUI_COUNTOF(x) (sizeof(x) / sizeof(x[0]))
#define EXGUI_UNUSED(x) (void)(x)

class exgui_vector2
{
public:
  union {
    struct { float x, y; };
    float v[2];
  };
  exgui_vector2() : x(0.f), y(0.f) {}
  exgui_vector2(float xx, float yy) : x(xx), y(yy) {}
  exgui_vector2(int xx, int yy) : x((float)xx), y((float)yy) {}
  ~exgui_vector2() {}

  inline exgui_vector2 operator=(exgui_vector2& vec) { return *this = vec; }
  inline exgui_vector2 operator+(exgui_vector2& vec) { return exgui_vector2(x + vec.x, y + vec.y); }
  inline exgui_vector2 operator-(exgui_vector2& vec) { return exgui_vector2(x - vec.x, y - vec.y); }
  inline exgui_vector2 operator*(exgui_vector2& vec) { return exgui_vector2(x * vec.x, y * vec.y); }
  inline exgui_vector2 operator/(exgui_vector2& vec) { return exgui_vector2(x / vec.x, y / vec.y); }
  inline exgui_vector2 operator+(float s) { return exgui_vector2(x + s, y + s); }
  inline exgui_vector2 operator-(float s) { return exgui_vector2(x - s, y - s); }
  inline exgui_vector2 operator*(float s) { return exgui_vector2(x * s, y * s); }
  inline exgui_vector2 operator/(float s) { return exgui_vector2(x / s, y / s); }
  inline exgui_vector2 operator+=(exgui_vector2& vec) { x += vec.x; y += vec.y; return *this; }
  inline exgui_vector2 operator-=(exgui_vector2& vec) { x -= vec.x; y -= vec.y; return *this; }
  inline exgui_vector2 operator*=(exgui_vector2& vec) { x *= vec.x; y *= vec.y; return *this; }
  inline exgui_vector2 operator/=(exgui_vector2& vec) { x /= vec.x; y /= vec.y; return *this; }
  inline exgui_vector2 operator*=(float s) { x *= s; y *= s; return *this; }
  inline exgui_vector2 operator/=(float s) { x /= s; y /= s; return *this; }
  inline exgui_vector2 operator+=(float s) { x += s; y += s; return *this; }
  inline exgui_vector2 operator-=(float s) { x -= s; y -= s; return *this; }

  inline bool compare_strong(exgui_vector2& vec) { return x == vec.x && y == vec.y; }
  inline bool operator==(exgui_vector2& vec) { return fabsf(x - vec.x) < FLT_EPSILON && fabsf(y - vec.y) < FLT_EPSILON; }
  inline bool operator!=(exgui_vector2& vec) { return fabsf(x - vec.x) >= FLT_EPSILON && fabsf(y - vec.y) >= FLT_EPSILON; }
  inline bool operator<(exgui_vector2& vec) { return x < vec.x && y < vec.y; }
  inline bool operator<=(exgui_vector2& vec) { return x <= vec.x && y <= vec.y; }
  inline bool operator>(exgui_vector2& vec) { return x > vec.x && y > vec.y; }
  inline bool operator>=(exgui_vector2& vec) { return x >= vec.x && y >= vec.y; }
  inline float operator[](int idx) { assert(idx < EXGUI_COUNTOF(v) && "index out of bounds"); return v[idx]; }
};

class exgui_rect
{
public:
  union {
    struct { float left, top, right, bottom; };
    struct { float v[4]; };
  };
  exgui_rect() {}
  exgui_rect(exgui_rect& rect) { *this = rect; }
  exgui_rect(float x, float y, float w, float h) : left(x), top(y), right(w), bottom(h) {}
  exgui_rect(int x, int y, int w, int h) : left((float)x), top((float)y), right((float)w), bottom((float)h) {}
  ~exgui_rect() {}

  inline float operator[](int idx) { assert(idx < EXGUI_COUNTOF(v) && "index out of bounds"); return v[idx]; }
};

/* undef min/max if defined macro */
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

class exgui_bbox
{
public:
  exgui_vector2 min, max;
  exgui_bbox() {}
  ~exgui_bbox() {}

  inline bool inside(exgui_vector2& pt) {
    return min <= pt && pt <= max;
  }

  inline void from_rect(exgui_rect& rect) {
    min.x = rect.left;
    min.y = rect.top;
    max.x = min.x + rect.right;
    max.y = min.y + rect.bottom;
  }
};

class exgui_vector3
{
public:
  union {
    struct { float x, y, z; };
    float v[3];
  };
  exgui_vector3() : x(0.f), y(0.f), z(0.f) {}
  exgui_vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
  ~exgui_vector3() {}

  exgui_vector3 operator=(exgui_vector3& vec) { return *this = vec; }
  exgui_vector3 operator+(exgui_vector3& vec) { return exgui_vector3(x + vec.x, y + vec.y, z + vec.z); }
  exgui_vector3 operator-(exgui_vector3& vec) { return exgui_vector3(x - vec.x, y - vec.y, z - vec.z); }
  exgui_vector3 operator*(exgui_vector3& vec) { return exgui_vector3(x * vec.x, y * vec.y, z * vec.z); }
  exgui_vector3 operator/(exgui_vector3& vec) { return exgui_vector3(x / vec.x, y / vec.y, z / vec.z); }
  exgui_vector3 operator+(float s) { return exgui_vector3(x + s, y + s, z + s); }
  exgui_vector3 operator-(float s) { return exgui_vector3(x - s, y - s, z - s); }
  exgui_vector3 operator*(float s) { return exgui_vector3(x * s, y * s, z * s); }
  exgui_vector3 operator/(float s) { return exgui_vector3(x / s, y / s, z / s); }
  exgui_vector3 operator+=(exgui_vector3& vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }
  exgui_vector3 operator-=(exgui_vector3& vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
  exgui_vector3 operator*=(exgui_vector3& vec) { x *= vec.x; y *= vec.y; z *= vec.z; return *this; }
  exgui_vector3 operator/=(exgui_vector3& vec) { x /= vec.x; y /= vec.y; z /= vec.z; return *this; }
  exgui_vector3 operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
  exgui_vector3 operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
  exgui_vector3 operator+=(float s) { x += s; y += s; z += s; return *this; }
  exgui_vector3 operator-=(float s) { x -= s; y -= s; z -= s; return *this; }
  float         operator[](int idx) { assert(idx < EXGUI_COUNTOF(v) && "index out of bounds"); return v[idx]; }
};

/* virtual keys */
enum EXGUI_KEY : uint32_t {

};

/* key state */
enum EXGUI_KEY_STATE : uint32_t {
  DOWN = 0,
  UP,
  REPEAT
};

/* mouse events */
enum EXGUI_MOUSE_EVENT : uint32_t {
  EXGUI_MOUSE_EVENT_MOVE = 0,
  EXGUI_MOUSE_EVENT_CLICK
};

/* node event */
enum EXGUI_EVENT : uint32_t {
  PARENT_CHANGED = 0,
  PARENT_RESIZE,
  PARENT_CHILD_ADDED,
  ROOT_RESIZE
};

/* Node notify and state flags */
#define EXGUI_FLAG_NONE          (0)
#define EXGUI_FLAG_VISIBLE       (1 << 0)
#define EXGUI_FLAG_ACTIVE        (1 << 1)
#define EXGUI_FLAG_NOTIFY_CHILDS (1 << 2)
#define EXGUI_FLAG_HAS_SYM       (1 << 3)
#define EXGUI_FLAG_HAS_KEYBD     (1 << 4)
#define EXGUI_FLAG_HAS_MOUSE     (1 << 5)
#define EXGUI_FLAG_HAS_CHILDS    (1 << 6)

/* special state flags */
#define EXGUI_FLAG_HOWERED       (1 << 7)
#define EXGUI_FLAG_FOCUSED       (1 << 8)
#define EXGUI_FLAG_DRAGGED       (1 << 9)

/* default flags for each widget */
#define EXGUI_FLAG_DEFAULT       (EXGUI_FLAG_VISIBLE|EXGUI_FLAG_ACTIVE|EXGUI_FLAG_NOTIFY_CHILDS|EXGUI_FLAG_HAS_SYM|EXGUI_FLAG_HAS_KEYBD|EXGUI_FLAG_HAS_MOUSE|EXGUI_FLAG_HAS_CHILDS)

/* flags base class */
class exgui_flags
{
  uint32_t m_flags;
protected:
  inline void set_flags(uint32_t f) { m_flags = f; }

public:
  exgui_flags(uint32_t flags) : m_flags(flags) {}
  ~exgui_flags() {}

  inline uint32_t get() { return m_flags; }
  inline void     set(uint32_t f) { m_flags = f; }
  inline bool     is_set(uint32_t flag) { return (m_flags & flag) == flag; }
  inline void     set_bit(uint32_t bit) { m_flags |= bit; }
  inline void     inverse_bit(uint32_t bit) { m_flags &= ~bit; }
  inline void     toggle_bits(uint32_t bit) { m_flags ^= bit; }
  inline void     toggle_bits(uint32_t bits, bool state) { m_flags = state ? (m_flags | bits) : (m_flags & ~bits); }

  operator        uint32_t() { return m_flags; }
  uint32_t        operator=(uint32_t f) { m_flags = f; }
};

/* for elements (access to modify only from class exgui_root) */
class exgui_flags_elem : public exgui_flags
{
  /* allow exgui_root class modify flags */
  friend class exgui_surface;
public:
  exgui_flags_elem() : exgui_flags(EXGUI_FLAG_DEFAULT) {}
  ~exgui_flags_elem() {}

  inline bool has_visible() { return is_set(EXGUI_FLAG_VISIBLE); }
  inline bool has_active() { return is_set(EXGUI_FLAG_ACTIVE); }
  inline bool has_notify_childs() { return is_set(EXGUI_FLAG_NOTIFY_CHILDS); }
  inline bool has_symbols_input() { return is_set(EXGUI_FLAG_HAS_SYM); }
  inline bool has_keybd() { return is_set(EXGUI_FLAG_HAS_KEYBD); }
  inline bool has_mouse() { return is_set(EXGUI_FLAG_HAS_MOUSE); }
  inline bool has_childs() { return is_set(EXGUI_FLAG_HAS_CHILDS); }

  /* state flags */
  inline bool is_howered() { return is_set(EXGUI_FLAG_HOWERED); }
  inline bool is_focused() { return is_set(EXGUI_FLAG_FOCUSED); }

  uint32_t operator=(uint32_t f) {
    exgui_flags::operator=(f);
  }
};

/* clipboard data types */
enum EXGUI_CB_DATA_TYPE : uint32_t {
  EXGUI_CLIPBOARD_DATA_TYPE_NONE = 0,
  EXGUI_CLIPBOARD_DATA_TYPE_BIN,
  EXGUI_CLIPBOARD_DATA_TYPE_TEXT
};

/**
* system dependend operations
*/
class iexgui_sysdf
{
public:
  virtual                   ~iexgui_sysdf() = 0;
  virtual void               get_cursor_pos(int* p_dst_x, int* p_dst_y) = 0;
  virtual void               set_cursor_pos(int x, int h) = 0;
  virtual uint32_t           num_monitors() = 0;
  virtual void               get_monitor_info(uint32_t monitor_idx, uint32_t *p_dst_DPI, uint32_t *p_w, uint32_t *p_h) = 0;
  virtual bool               get_clipboard_data_info(EXGUI_CB_DATA_TYPE &dst_data, size_t &dst_size) = 0;
  virtual const uint8_t     *get_clipboard_data_ex(EXGUI_CB_DATA_TYPE &dst, size_t &size) =0;
  virtual void               set_clipboard_data_ex(const uint8_t* p_src, size_t size) = 0;
};

/**
* GUI element abstract class
*/
class iexgui_widget
{
public:
  virtual     ~iexgui_widget() = 0;
  virtual bool on_event(EXGUI_EVENT event, exgui_widget *p_from) = 0;
  virtual void on_draw(NVGcontext* p_ctx) = 0;
  virtual void on_keybd(int sc, EXGUI_KEY vk, EXGUI_KEY_STATE state) = 0;
  virtual void on_text_input(int sym) = 0;
  virtual void on_mouse(EXGUI_MOUSE_EVENT event, EXGUI_KEY vk, EXGUI_KEY_STATE state, exgui_vector2& cursor_pos) = 0;
};

/**
* exgui_base
* 
* base class for all GUI elements
*/
class exgui_surface;

class exgui_widget : protected iexgui_widget
{
  /* allow exgui_root class to call iexgui_element vmethods */
  friend class exgui_surface;
  inline void set_root(exgui_widget* p_root) { m_proot = p_root; }

protected:
  /* iexgui_element empty impls */
  virtual bool on_event(EXGUI_EVENT event, exgui_widget *p_from) {
    EXGUI_UNUSED(event);
    EXGUI_UNUSED(p_from);
  }
  virtual void on_draw(NVGcontext* p_ctx) {
    EXGUI_UNUSED(p_ctx);
  }
  virtual void on_keybd(int sc, EXGUI_KEY vk, EXGUI_KEY_STATE state) {
    EXGUI_UNUSED(sc);
    EXGUI_UNUSED(vk);
    EXGUI_UNUSED(state);
  }
  virtual void on_text_input(int sym) {
    EXGUI_UNUSED(sym);
  }
  virtual void on_mouse(EXGUI_MOUSE_EVENT event, EXGUI_KEY vk, EXGUI_KEY_STATE state, exgui_vector2& cursor_pos) {
    EXGUI_UNUSED(event);
    EXGUI_UNUSED(vk);
    EXGUI_UNUSED(state);
    EXGUI_UNUSED(cursor_pos);
  }

protected:
  using _childs_vec = std::vector<exgui_widget*>;
  _childs_vec      m_childs;
  exgui_widget    *m_proot;
  exgui_widget    *m_pparent;
  void            *m_puserptr;
  iexgui_sysdf    *m_psysdf;
  exgui_flags_elem m_elem_flags;
  uint32_t         m_user_flags;
  int              m_font_id;
  char             m_szclass[32];
  exgui_bbox       m_bbox;
  exgui_rect       m_outer_rect;
  exgui_rect       m_inner_rect;

  inline exgui_widget* get_root() { return m_proot; }

  ///* exgui_root::rebuild_draw_cache accessor class */
  //class exgui_root_update_acessor : public exgui_root {
  //public:
  //  exgui_root_update_acessor() {}
  //  ~exgui_root_update_acessor() {}
  //  inline void rebuild_draw_cache() {
  //    exgui_root::rebuild_draw_cache();
  //  }
  //};

  /* perform update root draw cache */
  inline void root_update() { ((exgui_surface *)m_proot)->rebuild_draw_cache(); }

public:
  void set_classname(const char* p_clsn) {
    strncpy(m_szclass, p_clsn, sizeof(m_szclass) - 1);
  }

  exgui_widget(int x, int y, int width, int height, exgui_widget *p_parent, const char *p_classname,
    uint32_t flags = EXGUI_FLAG_DEFAULT, 
    uint32_t uflags = 0, void *p_userptr = nullptr) : m_proot(nullptr),
    m_pparent(nullptr), m_puserptr(nullptr), m_psysdf(nullptr) {
    m_pparent = p_parent;
    m_puserptr = p_userptr;
    m_elem_flags = flags;
    m_user_flags = uflags;
    m_outer_rect = exgui_rect(x, y, width, height);
    m_inner_rect = exgui_rect(x, y, width, height);
    m_bbox.from_rect(m_outer_rect);

    /* have parent? */
    if (p_parent)
      /* get sysdf ifaec from parent */ {
      m_psysdf = p_parent->get_sysdf();
      /* get root pointer from parent */
      m_proot = p_parent->get_root();
    }

    /* set font from root */
    if (m_proot)
      set_font(m_proot->get_font());

    set_classname(p_classname);
  }
  ~exgui_widget() {}

  inline const char    *get_classname() { return m_szclass; }
  inline void          *get_userptr() { return m_puserptr; }
  inline void           set_userptr(void* p) { m_puserptr = p; }

  /* rect && bbox */
  inline exgui_bbox& get_bbox() { return m_bbox; }
  inline exgui_rect& get_outer_rect() { return m_outer_rect; }
  inline exgui_rect& get_inner_rect() { return m_inner_rect; }

  /* visual */
  inline bool           is_visible() { return m_elem_flags.has_visible(); }
  inline void           show(bool b_show = true) { m_elem_flags.toggle_bits(EXGUI_FLAG_VISIBLE, b_show); }
  inline void           hide() { show(false); }

  /* childs */
  inline size_t         get_num_childs() { return m_childs.size(); }
  inline exgui_widget*  get_child(size_t idx) { return m_childs[idx]; }
  inline const exgui_widget** get_all_childs() { return m_childs.data(); }
  bool                  add_child(exgui_widget* p_child);
  bool                  remove_child(exgui_widget* p_child);

  /* parent */
  inline exgui_widget  *get_parent() { return m_pparent; }
  void                  set_parent(exgui_widget* p_parent);

  /* flags */
  inline exgui_flags_elem get_elem_flags() { return m_elem_flags; }
  inline uint32_t       get_user_flags() { return m_user_flags; }

  /* system dependend functions interface */
  inline iexgui_sysdf  *get_sysdf() { return m_psysdf; }

  /* font */
  inline void           set_font(int font) { m_font_id = font; }
  inline int            get_font() { return m_font_id; }
};

class exgui_surface : public exgui_widget
{
  using exgui_draw_cache = std::vector<exgui_widget*>;
  exgui_draw_cache m_draw_cache;
  NVGcontext      *m_pctx;
  exgui_widget    *m_pfocus;

  void        build_draw_cache_recursive(exgui_widget *p_elem);

  /* event notifier functions */
  static void event_dispatcher(exgui_widget *p_elem);
  static void keybd_dispatcher(exgui_widget *p_elem, int sc, 
    EXGUI_KEY vk, EXGUI_KEY_STATE state);
#if 0
  static void text_input_dispatcher(exgui_widget *p_elem, int sym);
#endif
  void        mouse_dispatcher(exgui_widget *p_elem, 
    EXGUI_MOUSE_EVENT event, EXGUI_KEY vk, 
    EXGUI_KEY_STATE state, exgui_vector2 &cursor_pos);

  /* access is open for inheritance (exgui_root::rebuild_draw_cache accessor class ) */
//protected:
public:
  inline void rebuild_draw_cache();

public:
  exgui_surface(NVGcontext *p_ctx, int width, int height);
  ~exgui_surface();

  /* main events */
  void draw();
  void keybd(int sc, EXGUI_KEY vk, EXGUI_KEY_STATE state);
  void textinput(int sym);
  void mouse(EXGUI_MOUSE_EVENT event, EXGUI_KEY vk, EXGUI_KEY_STATE state, int x, int y);
};


/*
===========================================================================================
 DEFAULT WIDGETS HERE
===========================================================================================
*/

/**
* style base class
*/
class exgui_style_base
{
public:
  exgui_style_base() {}
  ~exgui_style_base() {}
};

/**
* styled widget base class
*/
template<class _dst_style_type>
class exgui_styled
{
  exgui_style_base* m_pstyle;
public:
  exgui_styled() : m_pstyle(nullptr) {}
  ~exgui_styled() {}

  inline _dst_style_type* get_style() { return reinterpret_cast<_dst_style_type*>(m_pstyle); }
  inline void             set_style(exgui_style_base* p_style) { m_pstyle = p_style; }
};

enum EXGUI_CORNER : uint32_t {
  LEFT_TOP = 0,
  RIGHT_TOP,
  RIGHT_BOTTOM,
  LEFT_BOTTOM,

  EXGUI_NUM_CORNERS
};

class exgui_corners_style
{
protected:
  float m_corner_radius[EXGUI_NUM_CORNERS];
public:
  exgui_corners_style() {
    m_corner_radius[LEFT_TOP] = 0.f;
    m_corner_radius[RIGHT_TOP] = 0.f;
    m_corner_radius[RIGHT_BOTTOM] = 0.f;
    m_corner_radius[LEFT_BOTTOM] = 0.f;
  }
  ~exgui_corners_style() {}

  inline void  set_corner_radius(EXGUI_CORNER corner, float radius) { m_corner_radius[corner] = radius; }
  inline float get_corner_radius(EXGUI_CORNER corner) { return m_corner_radius[corner]; }
};


/**
* Window
*/
class exgui_window_style : public exgui_style_base, public exgui_corners_style
{
protected:
  float    m_title_font_size;
  float    m_title_font_blur_factor;
  NVGcolor m_title_font_color;
  NVGcolor m_title_font_shadow_color;
  union {
    struct {
      NVGcolor m_active_background_color;
      NVGcolor m_inactive_background_color;
    };
    NVGcolor m_background_colors[2];
  };

  NVGcolor m_titlebar_background_color;
  NVGcolor m_titlebar_shadow_color;
  NVGcolor m_titlebar_shadow_alpha_color;
  float    m_titlebar_height;
  NVGcolor m_window_top_gradient;
  NVGcolor m_window_bottom_gradient;
public:
  exgui_window_style() {}
  ~exgui_window_style() {}

  inline float     get_font_size() { return m_title_font_size; }
  inline float     get_font_blur_factor() { return m_title_font_blur_factor; }
  inline NVGcolor &get_font_color() { return m_title_font_color; }
  inline NVGcolor &get_font_shadow_color() { return m_title_font_shadow_color; }
  inline NVGcolor &get_active_background_color() { return m_active_background_color; }
  inline NVGcolor &get_inactive_background_color() { return m_inactive_background_color; }
  inline NVGcolor &get_background_color(int idx) { 
    assert(idx < EXGUI_COUNTOF(m_background_colors) && "index out of bounds");
    return m_background_colors[idx];
  }
  inline NVGcolor &get_titlebar_background_color() { return m_titlebar_background_color; }
  inline NVGcolor &get_titlebar_shadow_color() { return m_titlebar_shadow_color; }
  inline NVGcolor &get_titlebar_shadow_alpha_color() { return m_titlebar_shadow_alpha_color; }
  inline float     get_titlebar_height() { return m_titlebar_height; }
  inline NVGcolor &get_top_gradient_color() { return m_window_top_gradient; }
  inline NVGcolor &get_bottom_gradient_color() { return m_window_bottom_gradient; }
};

class exgui_window : public exgui_widget, public exgui_styled<exgui_window_style>
{
  exgui_window_style        *m_pstyle;
  std::vector<exgui_widget*> m_top_widgets;

  /* paint window background */
  virtual void on_draw(NVGcontext* p_ctx);

public:
  exgui_window(exgui_widget* p_parent, int x, int y, int width, int height, uint32_t flags = EXGUI_FLAG_DEFAULT, uint32_t uflags = 0, void* p_userptr = nullptr);
  ~exgui_window();
};
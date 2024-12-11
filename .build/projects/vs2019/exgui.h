/**
* Copyright (c) 2024. Kirill Deryabin  kd@allalg.org
* 
* Retained Mode GUI (RmGUI)
* 
* 
* 
*/

#pragma once
#include "nanovg/nanovg.h"
#include <vector>
#include <string>

enum EXGUI_KEY : uint32_t {

};

enum EXGUI_KEY_STATE : uint32_t {
  DOWN = 0,
  UP,
  REPEAT
};

enum EXGUI_MOUSE_EVENT : uint32_t {
  EXGUI_MOUSE_EVENT_MOVE = 0,
  EXGUI_MOUSE_EVENT_CLICK
};

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
#define EXGUI_FLAG_HOWERED       (1 << 7)
#define EXGUI_FLAG_FOCUSED       (1 << 8)
#define EXGUI_FLAG_DEFAULT       (EXGUI_FLAG_VISIBLE|EXGUI_FLAG_ACTIVE|EXGUI_FLAG_NOTIFY_CHILDS|EXGUI_FLAG_HAS_SYM|EXGUI_FLAG_HAS_KEYBD|EXGUI_FLAG_HAS_MOUSE|EXGUI_FLAG_HAS_CHILDS)

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

  operator        uint32_t() { return m_flags; }
  uint32_t        operator=(uint32_t f) { m_flags = f; }
};

/* for elements (access to modify only from class exgui_root) */
class exgui_flags_elem : public exgui_flags
{
  /* allow exgui_root class modify flags */
  friend class exgui_root;
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
class iexgui_element
{
public:
  virtual     ~iexgui_element() = 0;
  virtual bool on_event(EXGUI_EVENT event) = 0;
  virtual void on_draw(NVGcontext* p_ctx) = 0;
  virtual void on_keybd(int sc, EXGUI_KEY vk, EXGUI_KEY_STATE state) = 0;
  virtual void on_text_input(int sym) = 0;
  virtual void on_mouse(EXGUI_MOUSE_EVENT event, EXGUI_KEY vk, EXGUI_KEY_STATE state, int x, int y) = 0;
};

/**
* exgui_base
* 
* base class for all GUI elements
*/

class exgui_root;

class exgui_base : protected iexgui_element
{
  /* allow exgui_root class to call iexgui_element vmethods */
  friend class exgui_root;
  inline void set_root(exgui_base* p_root) { m_proot = p_root; }

protected:
  /* iexgui_element empty impls */
  virtual bool on_event(EXGUI_EVENT event) {}
  virtual void on_draw(NVGcontext* p_ctx) {}
  virtual void on_keybd(int sc, EXGUI_KEY vk, EXGUI_KEY_STATE state) {}
  virtual void on_text_input(int sym) {}
  virtual void on_mouse(EXGUI_MOUSE_EVENT event, EXGUI_KEY vk, EXGUI_KEY_STATE state, int x, int y) {}

protected:
  using _childs_vec = std::vector<exgui_base*>;
  _childs_vec      m_childs;
  exgui_base      *m_proot;
  exgui_base      *m_pparent;
  void            *m_puserptr;
  iexgui_sysdf    *m_psysdf;
  exgui_flags_elem m_elem_flags;
  uint32_t         m_user_flags;
  int              m_font_id;
  char             m_szclass[32];

  inline exgui_base* get_root() { return m_proot; }

  /* exgui_root::rebuild_draw_cache accessor class */
  class exgui_root_update_acessor : public exgui_root {
  public:
    exgui_root_update_acessor() {}
    ~exgui_root_update_acessor() {}
    inline void rebuild_draw_cache() {
      exgui_root::rebuild_draw_cache();
    }
  };

  /* perform update root draw cache */
  inline void root_update() { ((exgui_root_update_acessor *)m_proot)->rebuild_draw_cache(); }

public:
  void set_classname(const char* p_clsn) {
    strncpy(m_szclass, p_clsn, sizeof(m_szclass) - 1);
  }

  exgui_base(exgui_base *p_parent, const char *p_classname,
    uint32_t flags = EXGUI_FLAG_DEFAULT, 
    uint32_t uflags = 0, void *p_userptr = nullptr) : m_proot(nullptr),
    m_pparent(nullptr), m_puserptr(nullptr), m_psysdf(nullptr) {
    m_pparent = p_parent;
    m_puserptr = p_userptr;
    m_elem_flags = flags;
    m_user_flags = uflags;

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
  ~exgui_base() {}

  inline const char   *get_classname() { return m_szclass; }
  inline void         *get_userptr() { return m_puserptr; }
  inline void          set_userptr(void* p) { m_puserptr = p; }

  /* childs */
  inline size_t        get_num_childs() { return m_childs.size(); }
  inline exgui_base*   get_child(size_t idx) { return m_childs[idx]; }
  inline exgui_base**  get_all_childs() { return m_childs.data(); }
  bool                 add_child(exgui_base* p_child);
  bool                 remove_child(exgui_base* p_child);

  /* parent */
  inline exgui_base   *get_parent() { return m_pparent; }
  void                 set_parent(exgui_base* p_parent);

  /* flags */
  inline exgui_flags_elem get_elem_flags() { return m_elem_flags; }
  inline uint32_t      get_user_flags() { return m_user_flags; }

  /* system dependend functions interface */
  inline iexgui_sysdf* get_sysdf() { return m_psysdf; }

  /* font */
  inline void          set_font(int font) { m_font_id = font; }
  inline int           get_font() { return m_font_id; }
};

class exgui_root : public exgui_base
{
  using exgui_draw_cache = std::vector<exgui_base*>;
  exgui_draw_cache m_draw_cache;
  NVGcontext      *m_pctx;

  void        build_draw_cache_recursive(exgui_base *p_elem);
  
 
  /* event notifier functions */
  static void event_dispatcher(exgui_base *p_elem);
  static void keybd_dispatcher(exgui_base *p_elem, int sc, 
    EXGUI_KEY vk, EXGUI_KEY_STATE state);
  static void text_input_dispatcher(exgui_base *p_elem, int sym);
  static void mouse_dispatcher(exgui_base *p_elem, 
    EXGUI_MOUSE_EVENT event, EXGUI_KEY vk, 
    EXGUI_KEY_STATE state, int x, int y);

  /* access is open for inheritance (exgui_root::rebuild_draw_cache accessor class ) */
protected:
  inline void rebuild_draw_cache();

public:
  exgui_root();
  ~exgui_root();

  /* main events */
  void draw();



};


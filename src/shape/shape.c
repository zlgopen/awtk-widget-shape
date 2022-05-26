/**
 * File:   shape.c
 * Author: AWTK Develop Team
 * Brief:  用于显示基本的形状。
 *
 * Copyright (c) 2022 - 2022 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2022-05-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "tkc/func_call_parser.h"
#include "tkc/object_array.h"
#include "tkc/color_parser.h"

#include "shape.h"

#define OBJECT_PROP_FUNC "__func__"

static ret_t shape_draw_line(vgcanvas_t* vg, point_t p1, point_t p2, uint32_t on, uint32_t off) {
  float x = p1.x;
  float y = p1.y;
  float ddx_on = 0;
  float ddy_on = 0;
  float ddx_off = 0;
  float ddy_off = 0;
  bool_t done = FALSE;
  int32_t dx = p2.x - p1.x;
  int32_t dy = p2.y - p1.y;
  return_value_if_fail(vg != NULL, RET_BAD_PARAMS);

  if (dx == 0 && dy == 0) {
    return RET_OK;
  }

  if (off == 0) {
    vgcanvas_move_to(vg, p1.x, p1.y);
    vgcanvas_line_to(vg, p2.x, p2.y);

    return RET_OK;
  }

  if (dx == 0) {
    /*垂直*/
    ddx_on = 0;
    ddx_off = 0;
    if (dy < 0) {
      ddy_on = -(int)on;
      ddy_off = -(int)off;
    } else {
      ddy_on = on;
      ddy_off = off;
    }
  } else if (dy == 0) {
    /*水平*/
    ddy_on = 0;
    ddy_off = 0;
    if (dx < 0) {
      ddx_on = -(int)on;
      ddx_off = -(int)off;
    } else {
      ddx_on = on;
      ddx_off = off;
    }
  } else {
    float len = sqrt(dx * dx + dy * dy);
    float scalex = dx / len;
    float scaley = dy / len;
    ddy_on = scaley * on;
    ddy_off = scaley * off;
    ddx_on = scalex * on;
    ddx_off = scalex * off;
  }

  while (!done) {
    if ((dx > 0 && x >= p2.x) || (dx <= 0 && x < p2.x)) {
      break;
    }
    if ((dy > 0 && y >= p2.y) || (dy <= 0 && y < p2.y)) {
      break;
    }
    vgcanvas_move_to(vg, x, y);
    x += ddx_on;
    y += ddy_on;
    if (dx > 0) {
      x = tk_min(x, p2.x);
    } else {
      x = tk_max(x, p2.x);
    }
    if (dy > 0) {
      y = tk_min(y, p2.y);
    } else {
      y = tk_max(y, p2.y);
    }
    vgcanvas_line_to(vg, x, y);
    x += ddx_off;
    y += ddy_off;
  }

  return RET_OK;
}

static inline float_t fix_value(float_t v, float_t max_v) {
  if (v >= 0 && v <= 1) {
    return max_v * v;
  } else {
    return v;
  }
}

static ret_t shape_fill(shape_t* shape, vgcanvas_t* vg, tk_object_t* args) {
  color_t fill = color_init(0xff, 0xff, 0xff, 0xff);
  const char* color = tk_object_get_prop_str(args, "fill");

  if (tk_str_eq(color, "none")) {
    return RET_OK;
  }

  if (color != NULL) {
    fill = color_parse(color);
  }

  if (fill.rgba.a) {
    vgcanvas_set_fill_color(vg, fill);
    vgcanvas_fill(vg);
  }

  return RET_OK;
}

static ret_t shape_stroke(shape_t* shape, vgcanvas_t* vg, tk_object_t* args) {
  color_t stroke = color_init(0, 0, 0, 0xff);
  const char* color = tk_object_get_prop_str(args, "stroke");
  uint32_t line_width = tk_object_get_prop_int(args, "lw", 1);

  if (tk_str_eq(color, "none")) {
    return RET_OK;
  }

  if (color != NULL) {
    stroke = color_parse(color);
  }

  if (stroke.rgba.a && line_width) {
    vgcanvas_set_stroke_color(vg, stroke);
    vgcanvas_set_line_width(vg, line_width);

    vgcanvas_stroke(vg);
  }

  return RET_OK;
}

static ret_t draw_hline(widget_t* widget, canvas_t* canvas, vgcanvas_t* vg, tk_object_t* args) {
  shape_t* shape = SHAPE(widget);
  float_t x = tk_object_get_prop_float(args, "x", 0);
  float_t y = tk_object_get_prop_float(args, "y", 0.5);
  float_t w = tk_object_get_prop_float(args, "w", 1);
  uint32_t on = tk_object_get_prop_uint32(args, "on", 0);
  uint32_t off = tk_object_get_prop_uint32(args, "off", 0);

  x = fix_value(x, widget->w);
  y = fix_value(y, widget->h);
  w = fix_value(w, widget->w);

  shape_draw_line(vg, point_init(x, y), point_init(x + w, y), on, off);
  shape_stroke(shape, vg, args);

  return RET_OK;
}

static ret_t draw_line(widget_t* widget, canvas_t* canvas, vgcanvas_t* vg, tk_object_t* args) {
  shape_t* shape = SHAPE(widget);
  float_t x1 = tk_object_get_prop_float(args, "x1", 0);
  float_t y1 = tk_object_get_prop_float(args, "y1", 0);
  float_t x2 = tk_object_get_prop_float(args, "x2", 1);
  float_t y2 = tk_object_get_prop_float(args, "y2", 1);
  uint32_t on = tk_object_get_prop_uint32(args, "on", 0);
  uint32_t off = tk_object_get_prop_uint32(args, "off", 0);

  x1 = fix_value(x1, widget->w);
  y1 = fix_value(y1, widget->h);
  x2 = fix_value(x2, widget->w);
  y2 = fix_value(y2, widget->h);

  shape_draw_line(vg, point_init(x1, y1), point_init(x2, y2), on, off);
  shape_stroke(shape, vg, args);

  return RET_OK;
}

static ret_t draw_vline(widget_t* widget, canvas_t* canvas, vgcanvas_t* vg, tk_object_t* args) {
  shape_t* shape = SHAPE(widget);
  float_t x = tk_object_get_prop_float(args, "x", 0.5);
  float_t y = tk_object_get_prop_float(args, "y", 0);
  float_t h = tk_object_get_prop_float(args, "h", 1);
  uint32_t on = tk_object_get_prop_uint32(args, "on", 0);
  uint32_t off = tk_object_get_prop_uint32(args, "off", 0);

  x = fix_value(x, widget->w);
  y = fix_value(y, widget->h);
  h = fix_value(h, widget->h);

  shape_draw_line(vg, point_init(x, y), point_init(x, y + h), on, off);
  shape_stroke(shape, vg, args);

  return RET_OK;
}

static ret_t draw_rect(widget_t* widget, canvas_t* canvas, vgcanvas_t* vg, tk_object_t* args) {
  shape_t* shape = SHAPE(widget);
  float_t x = tk_object_get_prop_float(args, "x", 0);
  float_t y = tk_object_get_prop_float(args, "y", 0);
  float_t w = tk_object_get_prop_float(args, "w", 1);
  float_t h = tk_object_get_prop_float(args, "h", 1);
  float_t r = tk_object_get_prop_float(args, "r", 0);

  x = fix_value(x, widget->w);
  y = fix_value(y, widget->h);
  w = fix_value(w, widget->w);
  h = fix_value(h, widget->h);
  r = fix_value(r, tk_min(w, h));

  vgcanvas_rounded_rect(vg, x, y, w, h, r);

  shape_fill(shape, vg, args);
  shape_stroke(shape, vg, args);

  return RET_OK;
}

static ret_t draw_text(widget_t* widget, canvas_t* canvas, vgcanvas_t* vg, tk_object_t* args) {
  rect_t r;
  float_t x = tk_object_get_prop_float(args, "x", 0);
  float_t y = tk_object_get_prop_float(args, "y", 0);
  float_t w = tk_object_get_prop_float(args, "w", 1);
  float_t h = tk_object_get_prop_float(args, "h", 1);
  float_t size = tk_object_get_prop_float(args, "size", 18);
  const char* text = tk_object_get_prop_str(args, "text");
  const char* color = tk_object_get_prop_str(args, "color");
  color_t text_color = color_init(0, 0, 0, 0xff);
  if (color != NULL) {
    text_color = color_parse(color);
  }

  x = fix_value(x, widget->w);
  y = fix_value(y, widget->h);
  w = fix_value(w, widget->w);
  h = fix_value(h, widget->h);
  r = rect_init(x, y, w, h);

  vgcanvas_translate(vg, -canvas->ox, -canvas->oy);
  canvas_set_font(canvas, NULL, size);
  canvas_set_text_color(canvas, text_color);
  canvas_draw_utf8_in_rect(canvas, text, &r);
  vgcanvas_translate(vg, canvas->ox, canvas->oy);

  return RET_OK;
}

static ret_t draw_circle(widget_t* widget, canvas_t* canvas, vgcanvas_t* vg, tk_object_t* args) {
  shape_t* shape = SHAPE(widget);
  float_t x = tk_object_get_prop_float(args, "x", 0.5);
  float_t y = tk_object_get_prop_float(args, "y", 0.5);
  float_t r = tk_object_get_prop_float(args, "r", 0.5);

  x = fix_value(x, widget->w);
  y = fix_value(y, widget->h);
  r = fix_value(r, tk_min(widget->w, widget->h));

  vgcanvas_arc(vg, x, y, r, 0, M_PI * 2, FALSE);

  shape_fill(shape, vg, args);
  shape_stroke(shape, vg, args);

  return RET_OK;
}

static ret_t draw_ellipse(widget_t* widget, canvas_t* canvas, vgcanvas_t* vg, tk_object_t* args) {
  shape_t* shape = SHAPE(widget);
  float_t x = tk_object_get_prop_float(args, "x", 0.5);
  float_t y = tk_object_get_prop_float(args, "y", 0.5);
  float_t rx = tk_object_get_prop_float(args, "rx", 0.5);
  float_t ry = tk_object_get_prop_float(args, "ry", 0.5);

  x = fix_value(x, widget->w);
  y = fix_value(y, widget->h);

  rx = fix_value(rx, widget->w);
  ry = fix_value(ry, widget->h);

  vgcanvas_ellipse(vg, x, y, rx, ry);

  shape_fill(shape, vg, args);
  shape_stroke(shape, vg, args);

  return RET_OK;
}

#include "svg/bsvg_draw.h"
#include "svg/svg_path_parser.h"

static ret_t parser_on_path(void* ctx, const void* data) {
  return bsvg_draw_path((bsvg_draw_ctx_t*)ctx, (svg_path_t*)data);
}

static ret_t draw_path(widget_t* widget, canvas_t* canvas, vgcanvas_t* vg, tk_object_t* args) {
  bsvg_draw_ctx_t ctx;
  float_t scale = 1;
  float_t xscale = 1;
  float_t yscale = 1;
  float_t ox = 0;
  float_t oy = 0;
  shape_t* shape = SHAPE(widget);
  float_t w = tk_object_get_prop_float(args, "w", 1);
  float_t h = tk_object_get_prop_float(args, "h", 1);
  const char* path = tk_object_get_prop_str(args, "data");
  w = fix_value(w, widget->w);
  h = fix_value(h, widget->h);
  xscale = widget->w / w;
  yscale = widget->h / h;
  scale = tk_min_float(xscale, yscale);

  ctx.x = 0;
  ctx.y = 0;
  ctx.bsvg = NULL;
  ctx.shape = NULL;
  ctx.canvas = vg;

  ox = (widget->w - w * scale) / 2;
  oy = (widget->h - h * scale) / 2;
  vgcanvas_translate(vg, ox, oy);
  vgcanvas_scale(vg, scale, scale);

  svg_path_parse(path, &ctx, parser_on_path);

  shape_fill(shape, vg, args);
  shape_stroke(shape, vg, args);

  return RET_OK;
}

typedef struct _name_func_t {
  const char* name;
  shape_draw_func_t draw;
} name_func_t;

static name_func_t s_funcs[] = {
    {"hline", draw_hline},   {"vline", draw_vline},     {"line", draw_line}, {"rect", draw_rect},
    {"circle", draw_circle}, {"ellipse", draw_ellipse}, {"path", draw_path}, {"text", draw_text}};

static shape_draw_func_t find_func_by_name(const char* name) {
  uint32_t i = 0;
  for (i = 0; i < ARRAY_SIZE(s_funcs); i++) {
    const name_func_t* iter = s_funcs + i;
    if (tk_str_eq(iter->name, name)) {
      return iter->draw;
    }
  }

  return NULL;
}

static tk_object_t* shape_parse_one(widget_t* widget, const char* value) {
  tk_object_t* obj = NULL;
  shape_t* shape = SHAPE(widget);
  return_value_if_fail(shape != NULL, NULL);

  obj = func_call_parse(value, tk_strlen(value));
  return_value_if_fail(obj != NULL, NULL);

  shape->draw = find_func_by_name(obj->name);
  if (shape->draw != NULL) {
    tk_object_set_prop_pointer(obj, OBJECT_PROP_FUNC, shape->draw);
  } else {
    TK_OBJECT_UNREF(obj);
  }

  return obj;
}

static ret_t draw_array(widget_t* widget, canvas_t* canvas, vgcanvas_t* vg, tk_object_t* args) {
  value_t v;
  uint32_t i = 0;
  uint32_t n = OBJECT_ARRAY(args)->size;

  for (i = 0; i < n; i++) {
    if (object_array_get(args, i, &v) == RET_OK) {
      tk_object_t* iter = value_object(&v);
      shape_draw_func_t draw =
          (shape_draw_func_t)tk_object_get_prop_pointer(iter, OBJECT_PROP_FUNC);
      ENSURE(draw != NULL);

      vgcanvas_begin_path(vg);
      draw(widget, canvas, vg, iter);
    }
  }

  return RET_OK;
}

static tk_object_t* shape_parse(widget_t* widget, const char* value) {
  shape_t* shape = SHAPE(widget);

  if (strchr(value, ';') != NULL) {
    value_t v;
    tokenizer_t t;
    tk_object_t* arr = object_array_create();
    return_value_if_fail(arr != NULL, NULL);

    tokenizer_init(&t, value, strlen(value), ";");
    while (tokenizer_has_more(&t)) {
      const char* iter = tokenizer_next(&t);
      tk_object_t* obj = shape_parse_one(widget, iter);

      break_if_fail(obj != NULL);

      value_set_object(&v, obj);
      object_array_push(arr, &v);
      TK_OBJECT_UNREF(obj);
    }
    tokenizer_deinit(&t);
    shape->draw = draw_array;

    return arr;
  } else {
    return shape_parse_one(widget, value);
  }
}

ret_t shape_set_value(widget_t* widget, const char* value) {
  shape_t* shape = SHAPE(widget);
  return_value_if_fail(shape != NULL, RET_BAD_PARAMS);

  TK_OBJECT_UNREF(shape->args);
  shape->value = tk_str_copy(shape->value, value);
  if (value != NULL) {
    shape->args = shape_parse(widget, value);
  }

  return RET_OK;
}

static ret_t shape_get_prop(widget_t* widget, const char* name, value_t* v) {
  shape_t* shape = SHAPE(widget);
  return_value_if_fail(shape != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(WIDGET_PROP_VALUE, name)) {
    value_set_str(v, shape->value);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t shape_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(WIDGET_PROP_VALUE, name)) {
    shape_set_value(widget, value_str(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t shape_on_destroy(widget_t* widget) {
  shape_t* shape = SHAPE(widget);
  return_value_if_fail(widget != NULL && shape != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(shape->value);
  TK_OBJECT_UNREF(shape->args);

  return RET_OK;
}

static ret_t shape_on_paint_self(widget_t* widget, canvas_t* c) {
  shape_t* shape = SHAPE(widget);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  shape_draw_func_t draw = NULL;

  if (shape->args == NULL || shape->draw == NULL) {
    return RET_OK;
  }

  draw = shape->draw;

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_begin_path(vg);

  draw(widget, c, vg, shape->args);

  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t shape_on_event(widget_t* widget, event_t* e) {
  return RET_OK;
}

const char* s_shape_properties[] = {WIDGET_PROP_VALUE, NULL};

TK_DECL_VTABLE(shape) = {.size = sizeof(shape_t),
                         .type = WIDGET_TYPE_SHAPE,
                         .clone_properties = s_shape_properties,
                         .persistent_properties = s_shape_properties,
                         .parent = TK_PARENT_VTABLE(widget),
                         .create = shape_create,
                         .on_paint_self = shape_on_paint_self,
                         .set_prop = shape_set_prop,
                         .get_prop = shape_get_prop,
                         .on_event = shape_on_event,
                         .on_destroy = shape_on_destroy};

widget_t* shape_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(shape), x, y, w, h);
  shape_t* shape = SHAPE(widget);
  return_value_if_fail(shape != NULL, NULL);

  shape->value = tk_strdup("hline()");

  return widget;
}

widget_t* shape_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, shape), NULL);

  return widget;
}

/**
 * File:   shape.h
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

#ifndef TK_SHAPE_H
#define TK_SHAPE_H

#include "base/widget.h"

BEGIN_C_DECLS

typedef ret_t (*shape_draw_func_t)(widget_t* widget, canvas_t* canvas, vgcanvas_t* vg,
                                   tk_object_t* args);

/**
 * @class shape_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 用于显示基本的形状。
 * 在xml中使用"shape"标签创建控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <shape x="c" y="50" w="100" h="100" value="hline()"/>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 * 
 * ```xml
 * <!-- style -->
 * <shape>
 *   <style name="default" font_size="32">
 *     <normal text_color="black" />
 *   </style>
 * </shape>
 * ```
 */
typedef struct _shape_t {
  widget_t widget;

  /**
   * @property {char*} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * shape的描述。
   */
  char* value;

  /*private*/
  shape_draw_func_t draw;
  tk_object_t* args;
} shape_t;

/**
 * @method shape_create
 * @annotation ["constructor", "scriptable"]
 * 创建shape对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} shape对象。
 */
widget_t* shape_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method shape_cast
 * 转换为shape对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget shape对象。
 *
 * @return {widget_t*} shape对象。
 */
widget_t* shape_cast(widget_t* widget);

/**
 * @method shape_set_value
 * 设置 shape的描述。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} value shape的描述。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t shape_set_value(widget_t* widget, const char* value);

#define WIDGET_TYPE_SHAPE "shape"

#define SHAPE(widget) ((shape_t*)(shape_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(shape);

END_C_DECLS

#endif /*TK_SHAPE_H*/

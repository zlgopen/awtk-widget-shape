﻿/**
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
#include "shape_register.h"
#include "base/widget_factory.h"
#include "shape/shape.h"

ret_t shape_register(void) {
  return widget_factory_register(widget_factory(), WIDGET_TYPE_SHAPE, shape_create);
}

const char* shape_supported_render_mode(void) {
  return "OpenGL|AGGE-BGR565|AGGE-BGRA8888|AGGE-MONO";
}

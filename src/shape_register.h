/**
 * File:   shape_register.h
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

#ifndef TK_SHAPE_REGISTER_H
#define TK_SHAPE_REGISTER_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @method  shape_register
 * 注册控件。
 *
 * @annotation ["global"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t shape_register(void);

/**
 * @method  shape_supported_render_mode
 * 获取支持的渲染模式。
 *
 * @annotation ["global"]
 *
 * @return {const char*} 返回渲染模式。
 */
const char* shape_supported_render_mode(void);

END_C_DECLS

#endif /*TK_SHAPE_REGISTER_H*/

﻿#include "shape/shape.h"
#include "gtest/gtest.h"

TEST(shape, basic) {
  value_t v;
  widget_t* w = shape_create(NULL, 10, 20, 30, 40);

  widget_destroy(w);
}

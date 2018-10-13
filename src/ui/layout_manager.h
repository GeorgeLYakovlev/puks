// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include <function>
#include <stdint.h>

struct ObjectSize {
  enum {
    ObjectSizeWrap,
    ObjectSizePixels,
    ObjectSizePercent
  } type_;
  uint32_t w;
  uint32_t h;
};

class LayoutObject {
 public:
  /**
   * Defines the layout.
   * width and height in percentage points, -1 if default size is used.
   */
  LayoutObject(int width, int height,
               std::function<GtkWidget *(GtkWidget*)> creator);

  void setParent(LayoutObject *parent);
  LayoutObject& add(std::shared_ptr<LayoutObject> obj);
  void layout();
};

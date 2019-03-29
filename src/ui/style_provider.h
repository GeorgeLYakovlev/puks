// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include <gtkmm.h>
#include <string>

class CustomStyleProvider {
 public:
  CustomStyleProvider(const char *name = "Serif", int size_points = 15,
                      bool bold = false, unsigned color = 0x000000,
                      const char *widget = "GtkLabel");
  ~CustomStyleProvider();

  CustomStyleProvider& set_name(const char* name) {
    name_ = name;
    dirty_ = true;
    return *this;
  }
  CustomStyleProvider& set_size(int size_points) {
    size_points_ = size_points;
    dirty_ = true;
    return *this;
  }
  CustomStyleProvider& set_bold(bool bold) {
    bold_ = bold;
    dirty_ = true;
    return *this;
  }
  CustomStyleProvider& set_color(unsigned color) {
    color_ = color;
    dirty_ = true;
    return *this;
  }
  CustomStyleProvider& set_widget(const char* widget);
  CustomStyleProvider& set_background_color(int background_color) {
    background_color_ = background_color;
    dirty_ = true;
    return *this;
  }

  Glib::RefPtr<Gtk::StyleProvider> provider();

 protected:
  std::string name_;
  int size_points_;
  bool bold_;
  unsigned color_;
  std::string widget_;
  int background_color_;
  bool dirty_;

  Glib::RefPtr<Gtk::StyleProvider> provider_;
};

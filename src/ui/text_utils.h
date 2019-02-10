// Copyright 2019 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include <gtkmm.h>
#include <string>
#include <vector>

class TextMetics {
 public:
  explicit TextMetics(Gtk::Window* w);
  ~TextMetics();

  void set_font(const Pango::FontDescription& fd);

  void get_text_metrics(const char *text, int *w, int *h);

  int find_max_that_fit(const char *text, int w, int h,
                        bool vertically_only = false);
  int find_max_that_fit(const std::vector<std::string>& texts, int w, int h,
                        bool vertically_only = false);

  bool does_fit(const char *text, int w, int h, int font_size);
  bool does_fit(const std::vector<std::string>& texts,
                int w, int h, int font_size);
  bool does_fit_vertically(
    const std::vector<std::string>& texts,
    int h, int font_size);

 private:
  Glib::RefPtr<Pango::Context> pango_context_;
  Glib::RefPtr<Pango::Layout> pango_layout_;
  Pango::FontDescription pango_font_;
};

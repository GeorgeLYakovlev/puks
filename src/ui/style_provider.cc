// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "ui/style_provider.h"

CustomStyleProvider::CustomStyleProvider(const char *name, int size_points,
                                         bool bold, unsigned color,
                                         const char *widget)
    : name_(name), size_points_(size_points), bold_(bold), color_(color),
      widget_(widget), background_color_(-1) {
}

CustomStyleProvider::~CustomStyleProvider() {
}

Glib::RefPtr<Gtk::StyleProvider> CustomStyleProvider::provider() {
  if (!provider_) {
    char data[256];
    char background_color[128];
    if (background_color_ >= 0)
      snprintf(background_color, sizeof(background_color),
               "background-color: #%06x; ", (unsigned)background_color_);
    else
      background_color[0] = 0;
    snprintf(data, sizeof(data), "%s {color: #%06x; %s%sfont: %s %d;}",
             widget_.c_str(), color_, background_color,
             bold_ ? "font-weight: bold; " : "",
             name_.c_str(), size_points_);
    auto css = Gtk::CssProvider::create();
    if (css->load_from_data(data)) {
      provider_ = css;
    }
  }
  return provider_;
}

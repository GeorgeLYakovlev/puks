// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "ui/style_provider.h"
#include <map>
#include <string>

#include "utils/log.h"

namespace {
bool s_new_style;

std::map<std::string, std::string> s_widget_names;

void init_styles_if_necessary() {
  if (!s_widget_names.empty())
    return;
  Log::printf(Log::LINFO, "Gtk version: %u.%u.%u\n",
              gtk_get_major_version(),
              gtk_get_minor_version(),
              gtk_get_micro_version());
  if (gtk_get_major_version() == 3 &&
      gtk_get_minor_version() >= 20) {
    Log::printf(Log::LINFO, "Gtk using new css style (post 3.20)\n");
    s_new_style = true;
  } else {
    Log::printf(Log::LINFO, "Gtk using old css style (pre 3.20)\n");
    s_new_style = false;
  }
  s_widget_names["GtkLabel"] = s_new_style ? "label" : "GtkLabel";
  s_widget_names["GtkEntry"] = s_new_style ? "entry" : "GtkEntry";
  // Add more when necessary
}

}  // namespace


CustomStyleProvider::CustomStyleProvider(const char *name, int size_points,
                                         bool bold, unsigned color,
                                         const char *widget)
    : name_(name), size_points_(size_points), bold_(bold), color_(color),
      widget_(widget), background_color_(-1) {
  init_styles_if_necessary();
  auto it = s_widget_names.find(widget_);
  if (it != s_widget_names.end())
    widget_ = it->second;
}

CustomStyleProvider::~CustomStyleProvider() {
}

Glib::RefPtr<Gtk::StyleProvider> CustomStyleProvider::provider() {
  if (!provider_) {
    char data[256];
    char background_color[128];
    if (background_color_ >= 0) {
      snprintf(background_color, sizeof(background_color),
               "background-color: #%06x; background: #%06x; ",
               (unsigned)background_color_, (unsigned)background_color_);
    } else {
      background_color[0] = 0;
    }
    if (s_new_style) {
      snprintf(data, sizeof(data), "%s {color: #%06x; %s%sfont: %dpx %s;}",
               widget_.c_str(), color_, background_color,
               bold_ ? "font-weight: bold; " : "",
               size_points_, name_.c_str());
    } else {
      snprintf(data, sizeof(data), "%s {color: #%06x; %s%sfont: %s %d;}",
               widget_.c_str(), color_, background_color,
               bold_ ? "font-weight: bold; " : "",
               name_.c_str(), size_points_);
    }
    // Log::printf(Log::LDEBUG, "Widget:%s\nData:%s\n", widget_.c_str(), data);
    auto css = Gtk::CssProvider::create();
    if (css->load_from_data(data)) {
      provider_ = css;
    }
  }
  return provider_;
}

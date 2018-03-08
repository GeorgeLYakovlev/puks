// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "ui/main_window.h"

#include <gdk/gdk.h>
#include <gtkmm.h>

#include "./version.h"
#include "ui/svoyak_window.h"

const int kOffsetWidth = 100;
const int kOffsetHeight = 100;

const char *kResourcesFolder = "resources";

const char *kSvoyakGlade = "svoyak.glade";

std::string get_resource_path(const char *subpath) {
  std::string full(kResourcesFolder);
  full.append("/");
  full.append(subpath);
  return full;
}

void process_ui_events() {
  while (gtk_events_pending())
    gtk_main_iteration();
}

int run_main_window(
  std::map<std::string, std::string>* settings,
  int argc, char *argv[]) {
  auto app = Gtk::Application::create(argc, argv, "com.pacwww.puks");

  Glib::RefPtr<Gtk::Builder> builder =
      Gtk::Builder::create_from_file(get_resource_path(kSvoyakGlade).c_str());

  SvoyakWindow *window = nullptr;

  GdkScreen *screen = gdk_screen_get_default();
  int width = gdk_screen_get_width(screen);
  int height = gdk_screen_get_height(screen);
  printf("Screen w:%d h:%d\n", width, height);

  builder->get_widget_derived("window_svoyak", window);

  window->set_default_size(800, 600);

  return app->run(*window);
}

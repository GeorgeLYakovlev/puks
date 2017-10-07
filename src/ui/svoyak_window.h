// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include <gtkmm.h>

#include "ui/style_provider.h"

class SvoyakWindow : public Gtk::Window {
 public:
  SvoyakWindow(BaseObjectType* cobject,
               const Glib::RefPtr<Gtk::Builder>& ref_glade);
  virtual ~SvoyakWindow();

  void redo_layout();

 protected:
  void on_size_allocate(Gtk::Allocation& allocation) override;

  void update_fonts();

 private:
  Glib::RefPtr<Gtk::Builder> ref_glade_;

  Glib::RefPtr<Gtk::StyleProvider> style_counter_black_;
  Glib::RefPtr<Gtk::StyleProvider> style_counter_green_;
  Glib::RefPtr<Gtk::StyleProvider> style_counter_yellow_;
  Glib::RefPtr<Gtk::StyleProvider> style_counter_red_;

  // Regular buttons
  Glib::RefPtr<Gtk::StyleProvider> button_black_;
  Glib::RefPtr<Gtk::StyleProvider> button_green_;
  Glib::RefPtr<Gtk::StyleProvider> button_red_;

  // TextEdit
  Glib::RefPtr<Gtk::StyleProvider> edit_black_on_gray_;
  Glib::RefPtr<Gtk::StyleProvider> edit_black_on_white_;
  Glib::RefPtr<Gtk::StyleProvider> edit_black_on_green_;

  int last_layout_width_;
  int last_layout_height_;
};

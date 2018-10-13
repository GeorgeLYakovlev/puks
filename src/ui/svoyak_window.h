// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include <gtkmm.h>
#include <string>

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
  void update_children(
      Gtk::Widget *w, Glib::RefPtr<Gtk::StyleProvider> provider);

  void on_button_clicked(std::string name);
  void on_time_clicked();
  void on_cancel_clicked();
  void on_previous_clicked();
  void on_next_clicked();

  void connect_buttons();

  void align_player_controls();

 private:
  Glib::RefPtr<Gtk::Builder> ref_glade_;

  // Timer
  Glib::RefPtr<Gtk::StyleProvider> style_counter_black_;
  Glib::RefPtr<Gtk::StyleProvider> style_counter_green_;
  Glib::RefPtr<Gtk::StyleProvider> style_counter_yellow_;
  Glib::RefPtr<Gtk::StyleProvider> style_counter_red_;

  // Regular buttons
  Glib::RefPtr<Gtk::StyleProvider> button_black_;
  Glib::RefPtr<Gtk::StyleProvider> button_green_;
  Glib::RefPtr<Gtk::StyleProvider> button_red_;

  // Score buttons
  Glib::RefPtr<Gtk::StyleProvider> score_black_;
  Glib::RefPtr<Gtk::StyleProvider> score_blue_;
  Glib::RefPtr<Gtk::StyleProvider> score_red_;

  // TextEdit
  Glib::RefPtr<Gtk::StyleProvider> edit_black_on_gray_;
  Glib::RefPtr<Gtk::StyleProvider> edit_black_on_white_;
  Glib::RefPtr<Gtk::StyleProvider> edit_black_on_green_;

  // All other controls
  Glib::RefPtr<Gtk::StyleProvider> control_black_;

  int last_layout_width_;
  int last_layout_height_;
  int players_;
  float scale_;
};

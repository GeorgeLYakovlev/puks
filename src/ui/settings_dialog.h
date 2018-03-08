// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#pragma once

#include <gtkmm.h>

#include "ui/style_provider.h"

class SettingsDialog : public Gtk::Dialog {
 public:
  SettingsDialog(BaseObjectType* cobject,
                 const Glib::RefPtr<Gtk::Builder>& ref_glade);
  virtual ~SettingsDialog();

 private:
  Glib::RefPtr<Gtk::Builder> ref_glade_;
};

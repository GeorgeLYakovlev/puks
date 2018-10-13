// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "ui/settings_dialog.h"

#include <gtk/gtk.h>

SettingsDialog::SettingsDialog(BaseObjectType* cobject,
                               const Glib::RefPtr<Gtk::Builder>& ref_glade)
    : Gtk::Dialog(cobject),
      ref_glade_(ref_glade) {
}

SettingsDialog::~SettingsDialog() {
}

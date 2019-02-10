// Copyright 2019 Palo Alto What Where When Club
// Author: George Yakovlev

#include "ui/dynamic_window.h"

#include <gdk/gdk.h>
#include <gtkmm.h>

#include "ui/text_utils.h"
#include "utils/config_ids.h"
#include "utils/log.h"

const int kProviderPriority = 800;  // STYLE_PROVIDER_PRIORITY_USER
const int kBorder = 10;

DynamicWindow::DynamicWindow(
    std::map<std::string, std::string>* settings)
    : Gtk::Window(), outer_box_(NULL),
      style_top_label_(),
      players_(4), settings_(settings) {
  last_layout_width_ = 0;
  last_layout_height_ = 0;
}

DynamicWindow::~DynamicWindow() {
}

DynamicWindow* DynamicWindow::CreateWindow(int w, int h,
    std::map<std::string, std::string> *settings) {
  auto win = new DynamicWindow(settings);
  win->set_default_size(w, h);
  win->last_layout_width_ = w;
  win->last_layout_height_ = h;

  win->set_title("GTK Puks. (c) Palo Alto What? Where? When? club 2017-2019.");

  win->redo_layout();
  return win;
}

/**
 * Puks window format:
 *            Tourney Header H:100% V:15%
 * Game decription H:40% V:10% Stage description: H:40% V:10 Round H:20% V:10%
 * Headrers...
 * Players and scores...
 * start stop options                        previous next
 *                    Countdown H:100% V:20%
 */

void DynamicWindow::redo_layout() {
  // sets the border width of the window.
  set_border_width(10);
  update_fonts();

  if (outer_box_) {
    remove();
    delete outer_box_;
  }
  int w, h;
  get_size(w, h);
  Log::printf(Log::LDEBUG, "Rearranging... {%d:%d}\n", w, h);
  outer_box_ = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
  add(*outer_box_);
  CustomStyleProvider sp;

  sp.set_bold(true);
  sp.set_color(0x00FF00);
  // sp.set_background_color(0xFF0000);
  style_top_label_ = sp.provider();
  Gtk::Label* tourney = new Gtk::Label((*settings_)[TOURNEY]);
  tourney->get_style_context()->add_provider(
      style_top_label_, kProviderPriority);
  TextMetics tm(this);
  tm.set_font(tourney->get_style_context()->get_font());
  tourney->set_size_request(w - kBorder * 2, 30);
  Log::printf(Log::LDEBUG, "WH:{%d,%d}\n",
              tourney->get_allocated_width(), tourney->get_allocated_height());
  int updated =
      tm.find_max_that_fit((*settings_)[TOURNEY].c_str(),
                           w - kBorder * 2, 30, true);
  CustomStyleProvider sp1;

  sp1.set_bold(true);
  sp1.set_color(0x00FF00);
  sp1.set_background_color(0xFF0000);
  sp1.set_size(updated + 25);
  tm.does_fit((*settings_)[TOURNEY].c_str(),
              w - kBorder * 2, 30 * 5, updated);
  style_top_label_ = sp1.provider();
  tourney->get_style_context()->add_provider(
      style_top_label_, kProviderPriority);
  outer_box_->pack_start(*Gtk::manage(tourney),
                         false, false, 0);
  // Log::printf(Log::LDEBUG, "{%d:%d}\n", w, h);

  // int w, h;
  /*
  tm.get_text_metrics("Label 2", &w, &h);
  Log::printf(Log::LDEBUG, "String: %s w:%d h:%d\n", "Label 2", w, h);
  Gtk::Label *l = new Gtk::Label("Label 2");
  l->set_size_request(300, 400);
  outer_box_->pack_start(*Gtk::manage(l), false, false, 0);
  int ns = tm.find_max_that_fit("Label 2", 300, 400);
  Log::printf(Log::LDEBUG, "New font size: %d\n", ns);
  outer_box_->pack_start(*Gtk::manage(new Gtk::Button("Button 1")),
  false, false, 0);*/
  show_all();
}

void DynamicWindow::on_size_allocate(Gtk::Allocation& allocation) {
  Gtk::Window::on_size_allocate(allocation);

  if (allocation.get_width() == last_layout_width_ &&
      allocation.get_height() == last_layout_height_)
    return;

  last_layout_height_ = allocation.get_height();
  last_layout_width_ = allocation.get_width();

  Log::printf(Log::LDEBUG, "Resized: w:%d h:%d\n",
              allocation.get_width(), allocation.get_height());

  redo_layout();
}

void DynamicWindow::update_fonts() {
}

void DynamicWindow::on_button_clicked(std::string name) {
}

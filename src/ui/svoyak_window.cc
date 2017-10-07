// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "ui/svoyak_window.h"

#include <gtk/gtk.h>

SvoyakWindow::SvoyakWindow(BaseObjectType* cobject,
                           const Glib::RefPtr<Gtk::Builder>& ref_glade)
    : Gtk::Window(cobject),
      ref_glade_(ref_glade),
      last_layout_width_(-1),
      last_layout_height_(-1) {
}

SvoyakWindow::~SvoyakWindow() {
}

void SvoyakWindow::redo_layout() {
  const int kLabelHeader = 15;  // %
  const int kHeader2 = 10;  // %

  update_fonts();

  auto clip = get_clip();

  int height = clip.get_height();
  int width = clip.get_width();

  // Modify label header
  Gtk::Label *tmp = nullptr;
  int desired_height = (height * kLabelHeader) / 100;
  ref_glade_->get_widget("label_header", tmp);
  tmp->set_size_request(width, desired_height);

  tmp = nullptr;
  desired_height = (height * kHeader2) / 100;
  ref_glade_->get_widget("label_tourney", tmp);
  int desired_width = width / 2;
  tmp->set_size_request(desired_width, desired_height);

  tmp = nullptr;
  ref_glade_->get_widget("label_stage", tmp);
  tmp->set_size_request(desired_width, desired_height);

  Gtk::Grid *score_grid = nullptr;
  ref_glade_->get_widget("grid1", score_grid);
  auto score_allocation = score_grid->get_allocation();
  // printf("Width:%d X:%d\n",
  //        score_grid->get_width(), score_allocation.get_x());
  Gtk::Grid *names_grid = nullptr;
  int desired_delta =
      width - (score_allocation.get_x() + score_allocation.get_width()) - 20;
  ref_glade_->get_widget("grid_names", names_grid);
  desired_width = names_grid->get_width() + desired_delta;
  names_grid->set_size_request(desired_width, names_grid->get_height());

  // Align top headers:
  Gtk::Label *players = nullptr;
  Gtk::Label *total_score = nullptr;
  ref_glade_->get_widget("label_players", players);
  ref_glade_->get_widget("label_total0", total_score);
  desired_height = players->get_height() > total_score->get_height() ?
    players->get_height() : total_score->get_height();
  players->set_size_request(players->get_width(), desired_height);
  total_score->set_size_request(total_score->get_width(), desired_height);

  for (int i = 1; i <= 8; ++i) {
    char buff[128];
    snprintf(buff, sizeof(buff), "entry_name%d", i);
    Gtk::Entry *name_entry = nullptr;
    ref_glade_->get_widget(buff, name_entry);
    name_entry->set_size_request(name_entry->get_width() + desired_delta,
                                 name_entry->get_height());
  }
}

void SvoyakWindow::update_fonts() {
  const int kProviderPriority = 800;  // STYLE_PROVIDER_PRIORITY_USER
  const char *kFont = "Serif";
  // auto clip = get_clip();

  // int height = clip.get_height();
  Gtk::Label *label_header = nullptr;
  ref_glade_->get_widget("label_header", label_header);
  auto context = label_header->get_style_context();
  CustomStyleProvider provider(kFont, 40, true);
  auto style_provider = provider.provider();
  context->add_provider(style_provider, kProviderPriority);

  style_counter_black_ = CustomStyleProvider(kFont, 80, true).provider();
  style_counter_green_ =
      CustomStyleProvider(kFont, 80, true, 0x00A000).provider();
  style_counter_yellow_ =
      CustomStyleProvider(kFont, 80, true, 0xA0A000).provider();
  style_counter_red_ =
      CustomStyleProvider(kFont, 80, true, 0xA00000).provider();
  Gtk::Label *label_time = nullptr;
  ref_glade_->get_widget("label_time", label_time);
  context = label_time->get_style_context();
  context->add_provider(style_counter_green_, kProviderPriority);
}

void SvoyakWindow::on_size_allocate(Gtk::Allocation& allocation) {
  Gtk::Window::on_size_allocate(allocation);

  if (allocation.get_width() == last_layout_width_ &&
      allocation.get_height() == last_layout_height_)
    return;

  last_layout_height_ = allocation.get_height();
  last_layout_width_ = allocation.get_width();

  printf("Resized: w:%d h:%d\n",
         allocation.get_width(), allocation.get_height());
  redo_layout();
}

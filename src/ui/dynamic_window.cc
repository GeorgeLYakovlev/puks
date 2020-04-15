// Copyright 2019 Palo Alto What Where When Club
// Author: George Yakovlev

#include "ui/dynamic_window.h"

#include <gdk/gdk.h>
#include <gtkmm.h>

#include <cstring>

#include "ui/text_utils.h"
#include "utils/config_ids.h"
#include "utils/log.h"

namespace {

const int kProviderPriority = 800;  // STYLE_PROVIDER_PRIORITY_USER
const int kBorder = 10;

std::vector<std::string> kNameChars{
    "abcdefghijklmnopqrstuvwxyz",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "абвгдеёжзийклмнопрстуфхцчшщьыъэюя",
    "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯ",
    "1234567890./\\~!@#$%^&*()_-+={}[]"
};

void ParseControlId(const char *id, int *x1, int *x2) {
  if (x1)
    *x1 = -1;
  if (x2)
    *x2 = -1;
  char *ep = NULL;
  unsigned u = strtoul(id, &ep, 10);
  if (ep != id)
    *x1 = static_cast<int>(u);
  else
    return;
  if (!ep || !*ep)
    return;
  id = ep + 1;
  u = strtoul(id, &ep, 10);
  if (ep != id && x2)
    *x2 = static_cast<int>(u);
}

}  // namespace

DynamicWindow::DynamicWindow(
    std::map<std::string, std::string>* settings)
    : Gtk::Window(), outer_box_(NULL),
      style_top_label_(),
      players_(4), settings_(settings), score_(NULL) {
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
 *            Tourney Header H:100% V:8%
 * Room description: H:40% V:10% Stage description: H:40% V:10 Round H:20% V:4%
 * Headrers...
 * Players and scores...
 * start stop options                        previous next
 *                    Countdown H:100% V:20%
 */
void DynamicWindow::redo_layout() {
  // sets the border width of the window.
  set_border_width(kBorder);
  update_fonts();

  if (outer_box_) {
    Log::printf(Log::LDEBUG, "Layout old:\n");
    if (GTK_IS_CONTAINER(outer_box_->gobj())) {
      std::vector<Gtk::Widget *> widgets =
        static_cast<Gtk::Container *>(outer_box_)->get_children();
      for (unsigned i = 0; i < widgets.size(); ++i) {
        Log::printf(Log::LDEBUG, "    %s w:%d h:%d\n",
                    widgets[i]->get_name().c_str(),
                    widgets[i]->get_width(),
                    widgets[i]->get_height());
      }
    }

    remove();
    delete outer_box_;
  }
  int w, h;
  get_size(w, h);
  w -= kBorder;
  Log::printf(Log::LDEBUG, "Rearranging... {%d:%d}\n", w, h);
  const double kTopLineSize = 8.0;  // %
  const double kSecondLineSize = 4.0;  // %
  const double kSecondLineLabel1 = 40.0;  // %
  const double kSecondLineLabel2 = 40.0;  // %
  const double kSecondLineLabel3 = 20.0;  // %
  const double kBottomSize = 30.0;  // %
  outer_box_ = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
  outer_box_->set_spacing(0);
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

  // Calculate sizes
  int inner_width = w - kBorder * 2;
  int inner_height = h - kBorder * 2;

  int top_line_height = static_cast<int>(inner_height * kTopLineSize / 100.0);
  int second_line_height =
      static_cast<int>(inner_height * kSecondLineSize / 100.0);
  int bottom_height = static_cast<int>(inner_height * kBottomSize / 100.0);
  int score_height = inner_height -
      (top_line_height + second_line_height + bottom_height);

  // Log::printf(Log::LDEBUG, "full w:%d h:%d (%d + %d + %d + %d)\n",
  //            w, h, top_line_height, second_line_height, score_height,
  //            bottom_height);

  // First line.
  tourney->set_size_request(inner_width, top_line_height);
  Log::printf(Log::LDEBUG, "WH:{%d,%d}\n",
              tourney->get_allocated_width(), tourney->get_allocated_height());
  int updated =
      tm.find_max_that_fit(kNameChars,  // (*settings_)[TOURNEY].c_str(),
                           inner_width, top_line_height, true);
  Log::printf(Log::LDEBUG, "Font for Tourney:%d\n", updated);
  CustomStyleProvider sp1;
  sp1.set_bold(true);
  sp1.set_color(0x00FF00);
  // sp1.set_background_color(0xFF0000);
  sp1.set_size(updated);
  style_top_label_ = sp1.provider();
  tourney->get_style_context()->add_provider(
      style_top_label_, kProviderPriority);
  outer_box_->pack_start(*Gtk::manage(tourney),
                         false, false, 0);
  // Second line
  Gtk::Box *second_line = new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);
  second_line->set_spacing(0);
  Gtk::Label* room = new Gtk::Label((*settings_)[ROOM]);
  Gtk::Label* stage = new Gtk::Label((*settings_)[STAGE]);
  round_ = new Gtk::Label((*settings_)[ROUND]);
  room->set_size_request(
      static_cast<int>(inner_width * kSecondLineLabel1 / 100.0),
      second_line_height);
  stage->set_size_request(
      static_cast<int>(inner_width * kSecondLineLabel2 / 100.0),
      second_line_height);
  round_->set_size_request(
      static_cast<int>(inner_width * kSecondLineLabel3 / 100.0),
      second_line_height);

  std::vector<std::string> strings_to_test;
  strings_to_test.push_back((*settings_)[ROOM]);
  strings_to_test.push_back((*settings_)[STAGE]);
  strings_to_test.push_back("0123456789");
  updated =
      tm.find_max_that_fit(strings_to_test,
                           inner_width, second_line_height, true);
  CustomStyleProvider second_line_style;
  second_line_style.set_color(0x0000FF);
  // second_line_style.set_background_color(0x0000FF);
  second_line_style.set_size(updated);
  style_second_label_ = second_line_style.provider();
  outer_box_->pack_start(*Gtk::manage(second_line),
                         false, false, 0);
  second_line->pack_start(*Gtk::manage(room), false, false, 0);
  second_line->pack_start(*Gtk::manage(stage), false, false, 0);
  second_line->pack_start(*Gtk::manage(round_), false, false, 0);
  room->get_style_context()->add_provider(
      style_second_label_, kProviderPriority);
  stage->get_style_context()->add_provider(
      style_second_label_, kProviderPriority);
  round_->get_style_context()->add_provider(
      style_second_label_, kProviderPriority);

  // Score
  Gtk::Grid *score = new Gtk::Grid();
  layout_score(score, inner_width, score_height,
               tourney->get_style_context()->get_font());
  outer_box_->pack_start(*Gtk::manage(score), false, false, 0);

  // Last box: buttons, timer.
  Gtk::Box *bottom = new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);
  bottom->set_size_request(inner_width, bottom_height);
  outer_box_->pack_start(*Gtk::manage(bottom), false, false, 0);

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
  if (GTK_IS_CONTAINER(outer_box_->gobj())) {
    std::vector<Gtk::Widget *> widgets =
        static_cast<Gtk::Container *>(outer_box_)->get_children();
    for (unsigned i = 0; i < widgets.size(); ++i) {
      Log::printf(Log::LDEBUG, "    %s w:%d h:%d\n",
                  widgets[i]->get_name().c_str(),
                  widgets[i]->get_width(),
                  widgets[i]->get_height());
    }
  }

  redo_layout();
}

void DynamicWindow::update_fonts() {
}

void DynamicWindow::on_button_clicked(std::string name) {
  int p, s;
  ParseControlId(name.c_str() + 1, &p, &s);
  Log::printf(Log::LDEBUG, "'%s' (%d:%d) clicked:\n", name.c_str(), p, s);
  /*  if (GTK_IS_CONTAINER(score_->gobj())) {
    std::vector<Gtk::Widget *> widgets =
        static_cast<Gtk::Container *>(score_)->get_children();
    for (unsigned i = 0; i < widgets.size(); ++i) {
      Log::printf(Log::LDEBUG, "    %s w:%d h:%d\n",
                  widgets[i]->get_name().c_str(),
                  widgets[i]->get_width(),
                  widgets[i]->get_height());
    }
    }*/
  /*  if (GTK_IS_CONTAINER(outer_box_->gobj())) {
    std::vector<Gtk::Widget *> widgets =
        static_cast<Gtk::Container *>(outer_box_)->get_children();
    for (unsigned i = 0; i < widgets.size(); ++i) {
      Log::printf(Log::LDEBUG, "    %s w:%d h:%d\n",
                  widgets[i]->get_name().c_str(),
                  widgets[i]->get_width(),
                  widgets[i]->get_height());
    }
    }*/
  if (s == 3 && p == 1) {
    players_--;
    redo_layout();
  }
  if (s == 4 && p == 1) {
    players_++;
    redo_layout();
  }
}

void DynamicWindow::layout_score(Gtk::Grid *score, int w, int h,
                                 const Pango::FontDescription& fd) {
  score->set_size_request(w, h);
  int line_size = h / (players_ + 1);
  // Gtk seems to add two/three vertical pixel space even if it set to 0.
  // line_size -= 3;
  for (int i = 0; i <= players_; i++)
    score->insert_row(0);
  for (int i = 0; i <= 8; i++)
    score->insert_column(0);
  score->set_row_homogeneous(true);
  score->set_column_homogeneous(false);
  score->set_row_spacing(0);
  score->set_column_spacing(0);

  TextMetics tm(this);
  tm.set_font(fd);
  int fs = tm.find_max_that_fit(kNameChars, w, line_size - 4, true);
  CustomStyleProvider sp;
  sp.set_color(0xFF4040);
  sp.set_size(fs);
  score_red_ = sp.provider();
  sp.set_color(0x4040FF);
  score_blue_ = sp.provider();
  sp.set_color(0x0);
  score_black_ = sp.provider();
  score->get_style_context()->add_provider(score_black_, kProviderPriority);
  sp.set_widget("GtkEntry");
  edit_black_on_white_ = sp.provider();
  sp.set_background_color(0x80FF80);
  edit_black_on_green_ = sp.provider();
  sp.set_background_color(0xC0C0C0);
  sp.set_color(0x404040);
  edit_black_on_gray_ = sp.provider();

  // Last 20 adjustment is because the size is slightly bigger
  // Text size calculation lies.
  int edit_size = w - line_size * 8 - 20;

  score_ = score;
  Log::printf(Log::LDEBUG, "Score w:%d h:%d = 8 * %d + %d, %d * %d {%d,%d}\n",
              w, h,
              line_size, edit_size, players_ + 1, line_size,
              8 * line_size + edit_size, (players_ + 1) * line_size);
  int ww[8] = {0};
  char name[128];
  for (int i = 0; i < players_ + 1; ++i) {
    Gtk::Label *label = new Gtk::Label(i ? "" : "#");
    snprintf(name, sizeof(name), "n%d", i);
    label->set_name(name);
    label->set_size_request(line_size, line_size);
    label->get_style_context()->add_provider(score_black_, kProviderPriority);
    score->attach(*Gtk::manage(label), 0, i, 1, 1);
    ww[0] = label->get_allocated_width() > ww[0] ?
        label->get_allocated_width() : ww[0];  // !!!

    if (i) {
      Gtk::Entry *edit = new Gtk::Entry();
      snprintf(name, sizeof(name), "e%d", i);
      edit->set_name(name);
      edit->set_size_request(edit_size, line_size);
      edit->set_text("ABC");
      edit->get_style_context()->add_provider(
          edit_black_on_gray_, kProviderPriority);
      score->attach(*Gtk::manage(edit), 1, i, 1, 1);
      ww[1] = edit->get_allocated_width() > ww[1] ?
          edit->get_allocated_width() : ww[1];  // !!!
    } else {
      label = new Gtk::Label("Игрок");
      label->set_size_request(line_size, line_size);
      label->get_style_context()->add_provider(score_black_, kProviderPriority);
      score->attach(*Gtk::manage(label), 1, 0, 1, 1);
    }

    label = new Gtk::Label(i ? "000" : "Σ");
    snprintf(name, sizeof(name), "score%d", i);
    label->set_name(name);
    label->set_size_request(line_size * 2, line_size);
    label->get_style_context()->add_provider(score_black_, kProviderPriority);
    score->attach(*Gtk::manage(label), 2, i, 1, 1);
    ww[2] = label->get_allocated_width() > ww[2] ?
        label->get_allocated_width() : ww[2];  // !!!
    for (int j = 0; j < 5; ++j) {
      Gtk::Button *button = new Gtk::Button();
      snprintf(name, sizeof(name), "b%d_%d", i, j);
      button->set_name(name);
      button->set_size_request(line_size, line_size);
      if (i > 0) {
        button->set_relief(Gtk::RELIEF_NONE);
        button->set_label("+");
      } else {
        char tmp[32];
        snprintf(tmp, sizeof(tmp), "%2d", (j + 1) * 10);
        button->set_label(tmp);
      }
      button->get_child()->get_style_context()->add_provider(score_black_,
                                                             kProviderPriority);
      button->signal_clicked().connect(
          sigc::bind<std::string>(
              sigc::mem_fun(*this, &DynamicWindow::on_button_clicked),
                            name));
      score->attach(*Gtk::manage(button), 3 + j, i, 1, 1);
      ww[3 + j] = button->get_allocated_width() > ww[3 + j] ?
          button->get_allocated_width() : ww[3 + j];  // !!!
    }
  }
  Log::printf(Log::LDEBUG, "Line %d + %d + %d + %d + %d + %d + %d + %d + "
              "7 * %d = %d\n", ww[0], ww[1], ww[2], ww[3],
              ww[4], ww[5], ww[6], ww[7], score->get_column_spacing(),
              ww[0] + ww[1] + ww[2] + ww[3] + ww[4] + ww[5] + ww[6] + ww[7] +
              score->get_column_spacing() * 7);
}

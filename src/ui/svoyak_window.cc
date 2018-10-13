// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "ui/svoyak_window.h"

#include <gtk/gtk.h>
#include <vector>

#include "ui/main_window.h"
#include "utils/log.h"

const int kProviderPriority = 800;  // STYLE_PROVIDER_PRIORITY_USER
const char *kScoreButtonBeginning = "button_";

SvoyakWindow::SvoyakWindow(BaseObjectType* cobject,
                           const Glib::RefPtr<Gtk::Builder>& ref_glade)
    : Gtk::Window(cobject),
      ref_glade_(ref_glade),
      last_layout_width_(-1),
      last_layout_height_(-1),
      players_(8) {
  connect_buttons();
}

SvoyakWindow::~SvoyakWindow() {
}

void SvoyakWindow::redo_layout() {
  const int kLabelHeader = 15;  // %
  const int kHeader2 = 10;  // %

  auto clip = get_clip();

  int height = clip.get_height();
  int width = clip.get_width();
  char buff[128];

  update_fonts();

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

  align_player_controls();

  Gtk::Button *button = nullptr;
  ref_glade_->get_widget("button_50", button);
  int kRightMargin = 20;
  int desired_delta = width -
      (button->get_allocation().get_x() + button->get_width()) -
      kRightMargin;
  printf("Desired delta %i\n", desired_delta);
  for (int i = 1; i <= 8; ++i) {
    snprintf(buff, sizeof(buff), "entry_name%d", i);
    Gtk::Entry *name_entry = nullptr;
    ref_glade_->get_widget(buff, name_entry);
    printf("Width 2 %d\n", name_entry->get_width());
    name_entry->set_size_request(name_entry->get_width() + desired_delta,
                                 name_entry->get_height());
    printf("Width 3 %d\n", name_entry->get_width());
  }
}

void SvoyakWindow::update_fonts() {
  const char *kFont = "Serif";
  const int kTopLineSize = 32;
  const int kSecondSize = 20;
  const int kTimerSize = 86;
  const int kDefaultSize = 8;

  auto clip = get_clip();

  int height = clip.get_height();
  scale_ = static_cast<float>(height) / 600.0;
  auto scale_func = [=](int sz) -> int {
    return static_cast<int>(scale_ * sz + 0.5);
  };
  CustomStyleProvider all_controls(kFont, scale_func(kDefaultSize), true);
  update_children(this, all_controls.provider());
  Gtk::Label *label_header = nullptr;
  ref_glade_->get_widget("label_header", label_header);
  auto context = label_header->get_style_context();
  CustomStyleProvider provider(kFont, scale_func(kTopLineSize), true);
  auto style_provider = provider.provider();
  context->add_provider(style_provider, kProviderPriority);

  style_provider =
      CustomStyleProvider(kFont, scale_func(kSecondSize), true).provider();
  Gtk::Label *tourney = nullptr;
  ref_glade_->get_widget("label_tourney", tourney);
  tourney->get_style_context()->add_provider(style_provider, kProviderPriority);
  Gtk::Label *stage = nullptr;
  ref_glade_->get_widget("label_stage", stage);
  stage->get_style_context()->add_provider(style_provider, kProviderPriority);

  style_counter_black_ = CustomStyleProvider(kFont,
                                             scale_func(kTimerSize),
                                             true).provider();
  style_counter_green_ =
      CustomStyleProvider(kFont,
                          scale_func(kTimerSize),
                          true,
                          0x00A000).provider();
  style_counter_yellow_ =
      CustomStyleProvider(kFont,
                          scale_func(kTimerSize),
                          true,
                          0xA0A000).provider();
  style_counter_red_ =
      CustomStyleProvider(kFont,
                          scale_func(kTimerSize),
                          true,
                          0xA00000).provider();
  Gtk::Label *label_time = nullptr;
  ref_glade_->get_widget("label_time", label_time);
  context = label_time->get_style_context();
  context->add_provider(style_counter_green_, kProviderPriority);
}

void SvoyakWindow::update_children(
    Gtk::Widget *w, Glib::RefPtr<Gtk::StyleProvider> provider) {
  auto context = w->get_style_context();
  context->add_provider(provider, kProviderPriority);
  if (GTK_IS_CONTAINER(w->gobj())) {
    std::vector<Gtk::Widget *> widgets =
        static_cast<Gtk::Container *>(w)->get_children();
    for (unsigned i = 0; i < widgets.size(); ++i) {
      update_children(widgets[i], provider);
    }
  }
}


void SvoyakWindow::on_button_clicked(std::string name) {
  printf("Button %s is clicked\n", name.c_str());
}


void SvoyakWindow::on_time_clicked() {
  printf("Button time is clicked\n");
}

void SvoyakWindow::on_cancel_clicked() {
  printf("Button cancel is clicked\n");
}

void SvoyakWindow::on_previous_clicked() {
  printf("Button previous is clicked\n");
  if (players_ > 2)
    --players_;
  align_player_controls();
}

void SvoyakWindow::on_next_clicked() {
  printf("Button next is clicked\n");
  if (players_ < 8)
    ++players_;
  redo_layout();  // align_player_controls(players_);
}

void SvoyakWindow::connect_buttons() {
  Gtk::Button *button = nullptr;
  char buff[128];
  // Attach header buttons and scoring buttons.
  for (int i = 1; i <= 5; ++i) {
    snprintf(buff, sizeof(buff), "%s%02d", kScoreButtonBeginning, i * 10);
    ref_glade_->get_widget(buff, button);
    button->signal_clicked().connect(
        sigc::bind<std::string>(
            sigc::mem_fun(*this, &SvoyakWindow::on_button_clicked), buff));
    for (int j = 1; j <= 8; ++j) {
      snprintf(buff, sizeof(buff),
               "%s%02d_%1d", kScoreButtonBeginning, i * 10, j);
      ref_glade_->get_widget(buff, button);
      button->signal_clicked().connect(
          sigc::bind<std::string>(
              sigc::mem_fun(*this, &SvoyakWindow::on_button_clicked),
                            buff));
    }
  }
  ref_glade_->get_widget("button_time", button);
  button->signal_clicked().connect(
      sigc::mem_fun(*this, &SvoyakWindow::on_time_clicked));
  ref_glade_->get_widget("button_cancel", button);
  button->signal_clicked().connect(
      sigc::mem_fun(*this, &SvoyakWindow::on_cancel_clicked));
  ref_glade_->get_widget("button_next", button);
  button->signal_clicked().connect(
      sigc::mem_fun(*this, &SvoyakWindow::on_next_clicked));
  ref_glade_->get_widget("button_previous", button);
  button->signal_clicked().connect(
      sigc::mem_fun(*this, &SvoyakWindow::on_previous_clicked));
}

void SvoyakWindow::align_player_controls() {
  const int kBaseFontSize = 9;
  const char *kButtonFont = "Serif";

  if (players_ < 2)
    players_ = 2;
  if (players_ > 8)
    players_ = 8;
  Gtk::Button *button = nullptr;
  char buff[128];

  int font_size = static_cast<int>(scale_ * kBaseFontSize *
                                 (9.0 / (players_ + 1.0)) +
                                   0.5);

  score_black_ = CustomStyleProvider(kButtonFont, font_size).provider();
  score_blue_ =
      CustomStyleProvider(kButtonFont, font_size, false, 0x20C020).provider();
  score_red_ =
      CustomStyleProvider(kButtonFont, font_size, false, 0xC02020).provider();

  edit_black_on_gray_ =
      CustomStyleProvider(kButtonFont,
                          font_size,
                          false,
                          0x0,
                          "GtkEntry")
        .set_background_color(0x808080)
        .provider();
  edit_black_on_white_ =
      CustomStyleProvider(kButtonFont,
                          font_size,
                          false,
                          0x0,
                          "GtkEntry")
        .set_background_color(0x808080)
        .provider();
  edit_black_on_green_ =
      CustomStyleProvider(kButtonFont,
                          font_size,
                          false,
                          0x0,
                          "GtkEntry")
        .set_background_color(0x408040)
        .provider();


  for (int i = 1; i <= 8; ++i) {
    snprintf(buff, sizeof(buff), "entry_name%d", i);
    Gtk::Entry *name_entry = nullptr;
    ref_glade_->get_widget(buff, name_entry);
    printf("Width 0 %d\n", name_entry->get_width());
    name_entry->set_size_request(-1, -1);
    // name_entry->set_size_request(100, name_entry->get_height());
    process_ui_events();
    printf("Width 1 %d\n", name_entry->get_width());
  }

  // Align top headers:
  Gtk::Label *players = nullptr;
  Gtk::Label *total_score = nullptr;
  ref_glade_->get_widget("label_players", players);
  ref_glade_->get_widget("label_total0", total_score);
  ref_glade_->get_widget("button_time", button);
  int desired_size = (button->get_allocation().get_y() -
                      players->get_allocation().get_y()) /
                     (players_ + 1);
  players->set_size_request(players->get_width(), desired_size);
  total_score->set_size_request(total_score->get_width(), desired_size);
  players
    ->get_style_context()
    ->add_provider(score_black_, kProviderPriority);
  total_score
    ->get_style_context()
    ->add_provider(score_black_, kProviderPriority);
  for (int i = 1; i <= 5; ++i) {
    snprintf(buff, sizeof(buff), "%s%02d", kScoreButtonBeginning, i * 10);
    ref_glade_->get_widget(buff, button);
    // button->hide();
    for (int j = 1; j <= 8; ++j) {
      snprintf(buff, sizeof(buff),
               "%s%02d_%1d", kScoreButtonBeginning, i * 10, j);
      ref_glade_->get_widget(buff, button);
      button->hide();
    }
  }
  for (int i = 1; i <= 8; ++i) {
    snprintf(buff, sizeof(buff), "entry_name%d", i);
    Gtk::Entry *name_entry = nullptr;
    ref_glade_->get_widget(buff, name_entry);
    Gtk::Label *label_total = nullptr;
    snprintf(buff, sizeof(buff), "label_total%d", i);
    ref_glade_->get_widget(buff, label_total);
    Gtk::Label *label_name_select = nullptr;
    snprintf(buff, sizeof(buff), "label_name_select%d", i);
    ref_glade_->get_widget(buff, label_name_select);
    // name_entry->set_size_request(name_entry->get_width() / 2,
    //                             desired_size / 4);
    if (i > players_) {
      name_entry->hide();
      label_total->hide();
      label_name_select->hide();
    } else {
      name_entry->show();
      label_total->show();
      label_name_select->show();
      label_total
          ->get_style_context()
          ->add_provider(score_black_, kProviderPriority);
      label_name_select
          ->get_style_context()
          ->add_provider(score_black_, kProviderPriority);
      name_entry
          ->get_style_context()
          ->add_provider(edit_black_on_green_, kProviderPriority);
    }
    name_entry->set_size_request(name_entry->get_width(), desired_size);
  }
  // Attach header buttons and scoring buttons.
  for (int i = 1; i <= 5; ++i) {
    snprintf(buff, sizeof(buff), "%s%02d", kScoreButtonBeginning, i * 10);
    ref_glade_->get_widget(buff, button);
    button
      ->get_child()
      ->get_style_context()
      ->add_provider(score_black_, kProviderPriority);
    button->set_size_request(desired_size, desired_size);
    for (int j = 1; j <= 8; ++j) {
      snprintf(buff, sizeof(buff),
               "%s%02d_%1d", kScoreButtonBeginning, i * 10, j);
      ref_glade_->get_widget(buff, button);
      if (j > players_) {
        button->hide();
      } else {
        button->show();
        button
          ->get_child()
          ->get_style_context()
          ->add_provider(score_black_, kProviderPriority);
      }
      button->set_size_request(desired_size, desired_size);
    }
  }
}

void SvoyakWindow::on_size_allocate(Gtk::Allocation& allocation) {
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

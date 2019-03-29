// Copyright 2019 Palo Alto What Where When Club
// Author: George Yakovlev

#include "ui/text_utils.h"

#include "utils/log.h"

const int kPangoScale = 1024;  // Pango::PANGO_SCALE;

TextMetics::TextMetics(Gtk::Window* w)
    : pango_context_(w->create_pango_context()),
      pango_layout_(Pango::Layout::create(pango_context_)),
      pango_font_(w->get_style_context()->get_font()) {
}

TextMetics::~TextMetics() {
}

void TextMetics::set_font(const Pango::FontDescription& fd) {
  pango_font_ = fd;
  Log::printf(Log::LDEBUG, "FONT=%s\n", pango_font_.to_string().c_str());
}

void TextMetics::get_text_metrics(const char *text, int *w, int *h) {
  pango_layout_->set_text(text);
  pango_layout_->set_font_description(pango_font_);
  // pango_layout_->get_pixel_size(*w, *h);
  Pango::Rectangle ink_rect, logical_rect;
  pango_layout_->get_extents(ink_rect, logical_rect);
  *w = ink_rect.get_width() / kPangoScale;
  *h = ink_rect.get_height() / kPangoScale;
}

int TextMetics::find_max_that_fit(const char *text, int w, int h,
                                  bool vertically_only) {
  std::vector<std::string> texts;
  texts.push_back(text);
  return find_max_that_fit(texts, w, h, vertically_only);
}

int TextMetics::find_max_that_fit(const std::vector<std::string>& texts,
                                  int w, int h, bool vertically_only) {
  int sz = pango_font_.get_size() / kPangoScale;
  int grow = does_fit(texts, w, h, sz);
  int sz_new = grow ? (sz << 1) : (sz >> 1);
  for (sz_new = grow ? (sz << 1) : (sz >> 1);;
       sz_new = grow ? (sz_new << 1) : (sz_new >> 1)) {
    bool fits = vertically_only ?
        does_fit_vertically(texts, h, sz_new) : does_fit(texts, w, h, sz_new);
    if (grow) {
      if (fits)
        sz = sz_new;
      else
        break;
    } else {
      if (fits)
        break;
      else
        sz = sz_new;
    }
  }
  for (int sz_mid = (sz + sz_new) / 2; ; sz_mid = (sz + sz_new) / 2) {
    bool fits = vertically_only ?
        does_fit_vertically(texts, h, sz_mid) : does_fit(texts, w, h, sz_mid);
    if (grow) {
      if (sz == sz_mid)
        break;
      if (fits)
        sz = sz_mid;
      else
        sz_new = sz_mid;
    } else {
      if (sz_new == sz_mid) {
        sz = sz_mid;
        break;
      }
      if (fits)
        sz_new = sz_mid;
      else
        sz = sz_mid;
    }
  }
  return sz;
}

bool TextMetics::does_fit(const char *text, int w, int h, int font_size) {
  std::vector<std::string> texts;
  texts.push_back(text);
  return does_fit(texts, w, h, font_size);
}

bool TextMetics::does_fit(const std::vector<std::string>& texts,
                          int w, int h, int font_size) {
  int wt, ht;
  int fs = pango_font_.get_size();
  pango_font_.set_size(font_size * kPangoScale);

  bool res = true;
  for (auto it = texts.cbegin(); it != texts.cend(); ++it) {
    get_text_metrics(it->c_str(), &wt, &ht);
    if (wt > w || ht > h) {
      res = false;
      break;
    }
  }
  pango_font_.set_size(fs);
  return res;
}

bool TextMetics::does_fit_vertically(
    const std::vector<std::string>& texts,
    int h, int font_size) {
  int wt, ht;
  int fs = pango_font_.get_size();
  pango_font_.set_size(font_size * kPangoScale);
  bool res = true;
  for (auto it = texts.cbegin(); it != texts.cend(); ++it) {
    get_text_metrics(it->c_str(), &wt, &ht);
    // Log::printf(Log::LDEBUG, "%d TEXT=%s w:%d h:%d\n",
    //            font_size, it->c_str(), wt, ht);
    if (ht > h) {
      res = false;
      break;
    }
  }
  pango_font_.set_size(fs);
  return res;
}

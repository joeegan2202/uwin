#include "Font.hpp"

#include <iostream>

std::vector<Image> FontManager::images;

void FontManager::ensureImages() {
  if(images.size() == '~'-'!'+1) return;

  images.reserve('~'-'!'+1);

  images.emplace_back("font/excl.bmp");
  images.emplace_back("font/dqt.bmp");
  images.emplace_back("font/hash.bmp");
  images.emplace_back("font/dollar.bmp");
  images.emplace_back("font/percent.bmp");
  images.emplace_back("font/amp.bmp");
  images.emplace_back("font/qt.bmp");
  images.emplace_back("font/lparen.bmp");
  images.emplace_back("font/rparen.bmp");
  images.emplace_back("font/star.bmp");
  images.emplace_back("font/plus.bmp");
  images.emplace_back("font/comma.bmp");
  images.emplace_back("font/minus.bmp");
  images.emplace_back("font/period.bmp");
  images.emplace_back("font/slash.bmp");
  images.emplace_back("font/0.bmp");
  images.emplace_back("font/1.bmp");
  images.emplace_back("font/2.bmp");
  images.emplace_back("font/3.bmp");
  images.emplace_back("font/4.bmp");
  images.emplace_back("font/5.bmp");
  images.emplace_back("font/6.bmp");
  images.emplace_back("font/7.bmp");
  images.emplace_back("font/8.bmp");
  images.emplace_back("font/9.bmp");
  images.emplace_back("font/colon.bmp");
  images.emplace_back("font/semicolon.bmp");
  images.emplace_back("font/lt.bmp");
  images.emplace_back("font/equal.bmp");
  images.emplace_back("font/gt.bmp");
  images.emplace_back("font/qst.bmp");
  images.emplace_back("font/at.bmp");
  images.emplace_back("font/A.bmp");
  images.emplace_back("font/B.bmp");
  images.emplace_back("font/C.bmp");
  images.emplace_back("font/D.bmp");
  images.emplace_back("font/E.bmp");
  images.emplace_back("font/F.bmp");
  images.emplace_back("font/G.bmp");
  images.emplace_back("font/H.bmp");
  images.emplace_back("font/I.bmp");
  images.emplace_back("font/J.bmp");
  images.emplace_back("font/K.bmp");
  images.emplace_back("font/L.bmp");
  images.emplace_back("font/M.bmp");
  images.emplace_back("font/N.bmp");
  images.emplace_back("font/O.bmp");
  images.emplace_back("font/P.bmp");
  images.emplace_back("font/Q.bmp");
  images.emplace_back("font/R.bmp");
  images.emplace_back("font/S.bmp");
  images.emplace_back("font/T.bmp");
  images.emplace_back("font/U.bmp");
  images.emplace_back("font/V.bmp");
  images.emplace_back("font/W.bmp");
  images.emplace_back("font/X.bmp");
  images.emplace_back("font/Y.bmp");
  images.emplace_back("font/Z.bmp");
  images.emplace_back("font/lbrk.bmp");
  images.emplace_back("font/bslash.bmp");
  images.emplace_back("font/rbrk.bmp");
  images.emplace_back("font/caret.bmp");
  images.emplace_back("font/under.bmp");
  images.emplace_back("font/tick.bmp");
  images.emplace_back("font/a.bmp");
  images.emplace_back("font/b.bmp");
  images.emplace_back("font/c.bmp");
  images.emplace_back("font/d.bmp");
  images.emplace_back("font/e.bmp");
  images.emplace_back("font/f.bmp");
  images.emplace_back("font/g.bmp");
  images.emplace_back("font/h.bmp");
  images.emplace_back("font/i.bmp");
  images.emplace_back("font/j.bmp");
  images.emplace_back("font/k.bmp");
  images.emplace_back("font/l.bmp");
  images.emplace_back("font/m.bmp");
  images.emplace_back("font/n.bmp");
  images.emplace_back("font/o.bmp");
  images.emplace_back("font/p.bmp");
  images.emplace_back("font/q.bmp");
  images.emplace_back("font/r.bmp");
  images.emplace_back("font/s.bmp");
  images.emplace_back("font/t.bmp");
  images.emplace_back("font/u.bmp");
  images.emplace_back("font/v.bmp");
  images.emplace_back("font/w.bmp");
  images.emplace_back("font/x.bmp");
  images.emplace_back("font/y.bmp");
  images.emplace_back("font/z.bmp");
  images.emplace_back("font/lbrc.bmp");
  images.emplace_back("font/pipe.bmp");
  images.emplace_back("font/rbrc.bmp");
  images.emplace_back("font/tilde.bmp");
  if(image('a').getBpp() != 4) std::cerr << "Loading font, image bitmaps do not have alpha!" << std::endl;
}

inline std::vector<Image>::reference FontManager::image(char c) {
  return images.at(c-'!');
}

Framebuffer FontManager::render(std::string text) {
  ensureImages();

  int text_size = 0;

  for(auto c : text) {
    if(c == ' ') {
      text_size += 5;
    } else if(c-'!' >= 0 || c-'!' < images.size()) {
      text_size += image(c).getXres() + 1;
    } else {
      // Edge case: non-printing character -> replace with hash
      text_size += image('#').getXres() + 1;
    }
  }

  Framebuffer fb(text_size, 11, 4);

  int x_index = 0;
  for(auto c : text) {
    if(c == ' ') {
      x_index += 5;
    } else if(c-'!' >= 0 || c-'!' < images.size()) {
      fb.blit(image(c), x_index, 0);
      x_index += image(c).getXres() + 1;
    } else {
      // Edge case: non-printing character -> replace with hash
      fb.blit(image('#'), x_index, 0);
      x_index += image('#').getXres() + 1;
    }
  }

  return fb;
}

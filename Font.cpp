#include "Font.hpp"

std::vector<Image> FontManager::images;

void FontManager::ensureImages() {
  if(images.size() == '~'-'!'+1) return;

  images.reserve('~'-'!'+1);

  images.push_back(Image("font/excl.bmp"));
  images.push_back(Image("font/dqt.bmp"));
  images.push_back(Image("font/hash.bmp"));
  images.push_back(Image("font/dollar.bmp"));
  images.push_back(Image("font/percent.bmp"));
  images.push_back(Image("font/amp.bmp"));
  images.push_back(Image("font/qt.bmp"));
  images.push_back(Image("font/lparen.bmp"));
  images.push_back(Image("font/rparen.bmp"));
  images.push_back(Image("font/star.bmp"));
  images.push_back(Image("font/plus.bmp"));
  images.push_back(Image("font/comma.bmp"));
  images.push_back(Image("font/minus.bmp"));
  images.push_back(Image("font/period.bmp"));
  images.push_back(Image("font/slash.bmp"));
  images.push_back(Image("font/0.bmp"));
  images.push_back(Image("font/1.bmp"));
  images.push_back(Image("font/2.bmp"));
  images.push_back(Image("font/3.bmp"));
  images.push_back(Image("font/4.bmp"));
  images.push_back(Image("font/5.bmp"));
  images.push_back(Image("font/6.bmp"));
  images.push_back(Image("font/7.bmp"));
  images.push_back(Image("font/8.bmp"));
  images.push_back(Image("font/9.bmp"));
  images.push_back(Image("font/colon.bmp"));
  images.push_back(Image("font/semicolon.bmp"));
  images.push_back(Image("font/lt.bmp"));
  images.push_back(Image("font/equal.bmp"));
  images.push_back(Image("font/gt.bmp"));
  images.push_back(Image("font/qst.bmp"));
  images.push_back(Image("font/at.bmp"));
  images.push_back(Image("font/A.bmp"));
  images.push_back(Image("font/B.bmp"));
  images.push_back(Image("font/C.bmp"));
  images.push_back(Image("font/D.bmp"));
  images.push_back(Image("font/E.bmp"));
  images.push_back(Image("font/F.bmp"));
  images.push_back(Image("font/G.bmp"));
  images.push_back(Image("font/H.bmp"));
  images.push_back(Image("font/I.bmp"));
  images.push_back(Image("font/J.bmp"));
  images.push_back(Image("font/K.bmp"));
  images.push_back(Image("font/L.bmp"));
  images.push_back(Image("font/M.bmp"));
  images.push_back(Image("font/N.bmp"));
  images.push_back(Image("font/O.bmp"));
  images.push_back(Image("font/P.bmp"));
  images.push_back(Image("font/Q.bmp"));
  images.push_back(Image("font/R.bmp"));
  images.push_back(Image("font/S.bmp"));
  images.push_back(Image("font/T.bmp"));
  images.push_back(Image("font/U.bmp"));
  images.push_back(Image("font/V.bmp"));
  images.push_back(Image("font/W.bmp"));
  images.push_back(Image("font/X.bmp"));
  images.push_back(Image("font/Y.bmp"));
  images.push_back(Image("font/Z.bmp"));
  images.push_back(Image("font/lbrk.bmp"));
  images.push_back(Image("font/bslash.bmp"));
  images.push_back(Image("font/rbrk.bmp"));
  images.push_back(Image("font/caret.bmp"));
  images.push_back(Image("font/under.bmp"));
  images.push_back(Image("font/tick.bmp"));
  images.push_back(Image("font/a.bmp"));
  images.push_back(Image("font/b.bmp"));
  images.push_back(Image("font/c.bmp"));
  images.push_back(Image("font/d.bmp"));
  images.push_back(Image("font/e.bmp"));
  images.push_back(Image("font/f.bmp"));
  images.push_back(Image("font/g.bmp"));
  images.push_back(Image("font/h.bmp"));
  images.push_back(Image("font/i.bmp"));
  images.push_back(Image("font/j.bmp"));
  images.push_back(Image("font/k.bmp"));
  images.push_back(Image("font/l.bmp"));
  images.push_back(Image("font/m.bmp"));
  images.push_back(Image("font/n.bmp"));
  images.push_back(Image("font/o.bmp"));
  images.push_back(Image("font/p.bmp"));
  images.push_back(Image("font/q.bmp"));
  images.push_back(Image("font/r.bmp"));
  images.push_back(Image("font/s.bmp"));
  images.push_back(Image("font/t.bmp"));
  images.push_back(Image("font/u.bmp"));
  images.push_back(Image("font/v.bmp"));
  images.push_back(Image("font/w.bmp"));
  images.push_back(Image("font/x.bmp"));
  images.push_back(Image("font/y.bmp"));
  images.push_back(Image("font/z.bmp"));
  images.push_back(Image("font/lbrc.bmp"));
  images.push_back(Image("font/pipe.bmp"));
  images.push_back(Image("font/rbrc.bmp"));
  images.push_back(Image("font/tilde.bmp"));
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

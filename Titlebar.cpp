#include "Titlebar.hpp"
Image Titlebar::topLeft;
Image Titlebar::topRight;
Image Titlebar::topFill;
Image Titlebar::leftFill;
Image Titlebar::rightFill;
Image Titlebar::bottomFill;
Image Titlebar::bottomLeft;
Image Titlebar::bottomRight;

Titlebar::Titlebar(Framebuffer fb, Rectangle outer_bounds) : fb(fb), outer_bounds(outer_bounds) {}

void Titlebar::ensureImages() {
  if(topLeft.memory != nullptr) return;

  topLeft = Image("tbarTopLeft.bmp");
  topRight = Image("tbarTopRight.bmp");
  topFill = Image("tbarTopFill.bmp");
  leftFill = Image("tbarLeftFill.bmp");
  rightFill = Image("tbarRightFill.bmp");
  bottomFill = Image("tbarBottomFill.bmp");
  bottomLeft = Image("tbarBottomLeft.bmp");
  bottomRight = Image("tbarBottomRight.bmp");
}

Titlebar Titlebar::make(std::string name, Rectangle inner_bounds) {
  Titlebar::ensureImages();

  Rectangle outer_bounds = Rectangle::fromTwoPoints({
      inner_bounds.getTopLeft().x - topLeft.getXres(),
      inner_bounds.getTopLeft().y - topLeft.getYres()
      }, {
      inner_bounds.getBottomRight().x + bottomRight.getXres(),
      inner_bounds.getBottomRight().y + bottomRight.getYres()
      });
  
  Framebuffer tbar_fb(outer_bounds.getWidth(), outer_bounds.getHeight(), 4);

  // Top left
  tbar_fb.blit(Titlebar::topLeft, 0, 0);
  // Top fill
  for(int i = Titlebar::topLeft.getXres(); i < inner_bounds.getWidth() + Titlebar::topLeft.getXres(); i++) {
    tbar_fb.blit(Titlebar::topFill, i, 0);
  }
  // Top right
  tbar_fb.blit(Titlebar::topRight, inner_bounds.getWidth() + Titlebar::topLeft.getXres(), 0);
  // Left fill
  for(int i = Titlebar::topLeft.getYres(); i < inner_bounds.getHeight() + Titlebar::topLeft.getYres(); i++) {
    tbar_fb.blit(Titlebar::leftFill, 0, i);
  }
  // Right fill
  for(int i = Titlebar::topRight.getYres(); i < inner_bounds.getHeight() + Titlebar::topRight.getYres(); i++) {
    tbar_fb.blit(Titlebar::rightFill, inner_bounds.getWidth() + Titlebar::topLeft.getXres(), i);
  }
  // Bottom left
  tbar_fb.blit(Titlebar::bottomLeft, 0, inner_bounds.getHeight() + Titlebar::topLeft.getYres());
  // Bottom right
  tbar_fb.blit(Titlebar::bottomRight, inner_bounds.getWidth() + Titlebar::bottomRight.getXres(), inner_bounds.getHeight() + Titlebar::topRight.getYres());
  for(int i = Titlebar::bottomLeft.getXres(); i < inner_bounds.getWidth() + Titlebar::bottomLeft.getXres(); i++) {
    tbar_fb.blit(Titlebar::bottomFill, i, inner_bounds.getHeight() + Titlebar::topFill.getYres());
  }

  return {tbar_fb, outer_bounds};
}

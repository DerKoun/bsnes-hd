auto PPUfast::Line::renderMode7HD(PPUfast::IO::Background& self, uint source) -> void {
  const bool extbg = source == Source::BG2;
  const uint scale = ppufast.hdScale();

  Pixel  pixel;
  Pixel* above = &this->above[-1];
  Pixel* below = &this->below[-1];

  //find the first and last scanline for interpolation
  int y_a = y;
  int y_b = y;
  #define isLineMode7(n) (ppufast.lines[n].io.bg1.tileMode == TileMode::Mode7 && !ppufast.lines[n].io.displayDisable && ( \
    (ppufast.lines[n].io.bg1.aboveEnable || ppufast.lines[n].io.bg1.belowEnable) \
  ))
  if(ppufast.hdPerspective()) {
    while(y_a >   1 && isLineMode7(y_a)) y_a--; y_a++;
    while(y_b < 239 && isLineMode7(y_b)) y_b++; y_b--;
  } else {
    if(y_a >   1 && isLineMode7(y_a)) y_a--;
    if(y_b < 239 && isLineMode7(y_b)) y_b++;
  }
  #undef isLineMode7

  float a_a;
  float b_a;
  float c_a;
  float d_a;
  y_a--;
  do {
    y_a++;
    Line line_a = ppufast.lines[y_a];
    a_a = (int16)line_a.io.mode7.a;
    b_a = (int16)line_a.io.mode7.b;
    c_a = (int16)line_a.io.mode7.c;
    d_a = (int16)line_a.io.mode7.d;
  } while(ppufast.hdPerspective() && y_a < y 
      && b_a == 0 && c_a == 0
      && b_a != (int16)io.mode7.b && c_a != (int16)io.mode7.c);

  float a_b;
  float b_b;
  float c_b;
  float d_b;
  y_b++;
  do {
    y_b--;
    Line line_b = ppufast.lines[y_b];
    a_b = (int16)line_b.io.mode7.a;
    b_b = (int16)line_b.io.mode7.b;
    c_b = (int16)line_b.io.mode7.c;
    d_b = (int16)line_b.io.mode7.d;
  } while(ppufast.hdPerspective() && y_b > y 
      && b_b == 0 && c_b == 0
      && b_b != (int16)io.mode7.b && c_a != (int16)io.mode7.b);

  int hcenter = (int13)io.mode7.x;
  int vcenter = (int13)io.mode7.y;
  int hoffset = (int13)io.mode7.hoffset;
  int voffset = (int13)io.mode7.voffset;

  if(io.mode7.vflip) {
    y_a = 255 - y_a;
    y_b = 255 - y_b;
  }

  array<bool[256]> windowAbove;
  array<bool[256]> windowBelow;
  renderWindow(self.window, self.window.aboveEnable, windowAbove);
  renderWindow(self.window, self.window.belowEnable, windowBelow);

  int pixelYp = INT_MIN;
  for(int ys : range(scale)) {
    float yf = y + ys * 1.0 / scale - 0.5;
    if(io.mode7.vflip) yf = 255 - yf;

    float a = 1.0 / lerp(y_a, 1.0 / a_a, y_b, 1.0 / a_b, yf);
    float b = 1.0 / lerp(y_a, 1.0 / b_a, y_b, 1.0 / b_b, yf);
    float c = 1.0 / lerp(y_a, 1.0 / c_a, y_b, 1.0 / c_b, yf);
    float d = 1.0 / lerp(y_a, 1.0 / d_a, y_b, 1.0 / d_b, yf);

    int ht = (hoffset - hcenter) % 1024;
    float vty = ((voffset - vcenter) % 1024) + yf;
    float originX = (a * ht) + (b * vty) + (hcenter << 8);
    float originY = (c * ht) + (d * vty) + (vcenter << 8);

    int pixelXp = INT_MIN;
    for(int x : range(256+2*ppufast.widescreen())) {
      x -= ppufast.widescreen();
      int wx = x;
      if (wx <   0) wx = 0;
      if (wx > 255) wx = 255;
      bool doAbove = self.aboveEnable && !windowAbove[wx];
      bool doBelow = self.belowEnable && !windowBelow[wx];

      for(int xs : range(scale)) {
        float xf = x + xs * 1.0 / scale - 0.5;
        if(io.mode7.hflip) xf = 255 - xf;

        int pixelX = (originX + a * xf) / 256;
        int pixelY = (originY + c * xf) / 256;

        above++;
        below++;

        //only compute color again when coordinates have changed
        if(pixelX != pixelXp || pixelY != pixelYp) {
          uint tile    = io.mode7.repeat == 3 && ((pixelX | pixelY) & ~1023) ? 0 : ppufast.vram[(pixelY >> 3 & 127) * 128 + (pixelX >> 3 & 127)].byte(0);
          uint palette = io.mode7.repeat == 2 && ((pixelX | pixelY) & ~1023) ? 0 : ppufast.vram[(((pixelY & 7) << 3) + (pixelX & 7)) + (tile << 6)].byte(1);

          uint priority;
          if(!extbg) {
            priority = self.priority[0];
          } else {
            priority = self.priority[palette >> 7];
            palette &= 0x7f;
          }
          if(!palette) continue;

          uint color;
          if(io.col.directColor && !extbg) {
            color = directColor(0, palette);
          } else {
            color = cgram[palette];
          }

          pixel = {source, priority, color};
          pixelXp = pixelX;
          pixelYp = pixelY;
        }

        if(doAbove && (!extbg || pixel.priority > above->priority)) *above = pixel;
        if(doBelow && (!extbg || pixel.priority > below->priority)) *below = pixel;
      }
    }
  }

  if(ppufast.ss()) {
    uint divisor = scale * scale;
    for(uint p : range(256)) {
      uint ab = 0, bb = 0;
      uint ag = 0, bg = 0;
      uint ar = 0, br = 0;
      for(uint y : range(scale)) {
        auto above = &this->above[p * scale];
        auto below = &this->below[p * scale];
        for(uint x : range(scale)) {
          uint a = above[x].color;
          uint b = below[x].color;
          ab += a >>  0 & 31;
          ag += a >>  5 & 31;
          ar += a >> 10 & 31;
          bb += b >>  0 & 31;
          bg += b >>  5 & 31;
          br += b >> 10 & 31;
        }
      }
      uint aboveColor = ab / divisor << 0 | ag / divisor << 5 | ar / divisor << 10;
      uint belowColor = bb / divisor << 0 | bg / divisor << 5 | br / divisor << 10;
      this->above[p] = {source, this->above[p * scale].priority, aboveColor};
      this->below[p] = {source, this->below[p * scale].priority, belowColor};
    }
  }
}

//interpolation and extrapolation
auto PPUfast::Line::lerp(float pa, float va, float pb, float vb, float pr) -> float {
  if(va == vb || pr == pa) return va;
  if(pr == pb) return vb;
  return va + (vb - va) / (pb - pa) * (pr - pa);
}

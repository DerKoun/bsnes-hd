auto PPUfast::Line::renderMode7HD(PPUfast::IO::Background& self, uint source) -> void {
  const bool extbg = source == Source::BG2;
  const uint sampScale = ppufast.hdSupersample();
  const uint scale = ppufast.hdScale() * sampScale;

  int sampSize = sampScale < 2 ? 0 : (256+2*ppufast.widescreen()) * 4 * scale/sampScale;
  uint *sampTmp = new uint[sampSize];
  memory::fill<uint>(sampTmp, sampSize);

  Pixel  pixel;
  Pixel* above = &this->above[0];
  Pixel* below = &this->below[0];

  //find the first and last scanline for interpolation
  int y_a = -1;
  int y_b = -1;
  #define isLineMode7(n) (ppufast.lines[n].io.bg1.tileMode == TileMode::Mode7 && !ppufast.lines[n].io.displayDisable && ( \
    (ppufast.lines[n].io.bg1.aboveEnable || ppufast.lines[n].io.bg1.belowEnable) \
  ))
  if(ppufast.hdPerspective() > 0) {
    for(int i = 0; i < ppufast.ind; i++) {
      if(y >= ppufast.starts[i] && y <= ppufast.ends[i]) {
        y_a = ppufast.startsp[i];
        y_b = ppufast.endsp[i];
        break;
      }
    }
  }
  if(y_a == -1 || y_b == -1) {
    y_a = y;
    y_b = y;
    if(y_a >   1 && isLineMode7(y_a)) y_a--;
    if(y_b < 239 && isLineMode7(y_b)) y_b++;
  }
  #undef isLineMode7

  Line line_a = ppufast.lines[y_a];
  float a_a = (int16)line_a.io.mode7.a;
  float b_a = (int16)line_a.io.mode7.b;
  float c_a = (int16)line_a.io.mode7.c;
  float d_a = (int16)line_a.io.mode7.d;

  Line line_b = ppufast.lines[y_b];
  float a_b = (int16)line_b.io.mode7.a;
  float b_b = (int16)line_b.io.mode7.b;
  float c_b = (int16)line_b.io.mode7.c;
  float d_b = (int16)line_b.io.mode7.d;

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
      bool doAbove = self.aboveEnable && !windowAbove[ppufast.winXad(x, false)];
      bool doBelow = self.belowEnable && !windowBelow[ppufast.winXad(x, true)];

      for(int xs : range(scale)) {
        float xf = x + xs * 1.0 / scale - 0.5;
        if(io.mode7.hflip) xf = 255 - xf;

        int pixelX = (originX + a * xf) / 256;
        int pixelY = (originY + c * xf) / 256;

        bool skip = false;
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
          skip = !palette;

          if(!skip) {
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
        }

        if(sampScale == 1) {
          if(!skip && doAbove && (!extbg || pixel.priority > above->priority)) *above = pixel;
          if(!skip && doBelow && (!extbg || pixel.priority > below->priority)) *below = pixel;
          above++;
          below++;
        } else {
          int p = ((((x+ppufast.widescreen())*(scale/sampScale)) + (xs/sampScale))) * 4;
          sampTmp[p] += pixel.priority;
          sampTmp[p+1] += (pixel.color >> 10) & 31;
          sampTmp[p+2] += (pixel.color >>  5) & 31;
          sampTmp[p+3] += (pixel.color >>  0) & 31;
          if((ys+1) % sampScale == 0 && (xs+1) % sampScale == 0) {
            uint priority = sampTmp[p] / sampScale / sampScale;
            uint color = ((sampTmp[p+1] / sampScale / sampScale) << 10)
                        + ((sampTmp[p+2] / sampScale / sampScale) <<  5)
                        + ((sampTmp[p+3] / sampScale / sampScale) <<  0);
            if(!skip && doAbove && (!extbg || priority > above->priority)) *above = {source, priority, color};
            if(!skip && doBelow && (!extbg || priority > below->priority)) *below = {source, priority, color};
            above++;
            below++;
            sampTmp[p] = 0;
            sampTmp[p+1] = 0;
            sampTmp[p+2] = 0;
            sampTmp[p+3] = 0;
           }
        }
      }
    }
  }
  delete[] sampTmp;
}

//interpolation and extrapolation
auto PPUfast::Line::lerp(float pa, float va, float pb, float vb, float pr) -> float {
  if(va == vb || pr == pa) return va;
  if(pr == pb) return vb;
  return va + (vb - va) / (pb - pa) * (pr - pa);
}

auto PPU::Line::renderMode7(PPU::IO::Background& self, uint source) -> void {
  //#HDmode7>
  if (ppu.hdEnabled()) {
    renderMode7HD(self, source);
    return;
  }
  //#HDmode7<
  
  int Y = this->y - (self.mosaicEnable ? this->y % (1 + io.mosaicSize) : 0);
  int y = !io.mode7.vflip ? Y : 255 - Y;

  int a = (int16)io.mode7.a;
  int b = (int16)io.mode7.b;
  int c = (int16)io.mode7.c;
  int d = (int16)io.mode7.d;
  int hcenter = (int13)io.mode7.x;
  int vcenter = (int13)io.mode7.y;
  int hoffset = (int13)io.mode7.hoffset;
  int voffset = (int13)io.mode7.voffset;

  uint mosaicCounter = 1;
  uint mosaicPalette = 0;
  uint mosaicPriority = 0;
  uint mosaicColor = 0;

  auto clip = [](int n) -> int { return n & 0x2000 ? (n | ~1023) : (n & 1023); };
  int originX = (a * clip(hoffset - hcenter) & ~63) + (b * clip(voffset - vcenter) & ~63) + (b * y & ~63) + (hcenter << 8);
  int originY = (c * clip(hoffset - hcenter) & ~63) + (d * clip(voffset - vcenter) & ~63) + (d * y & ~63) + (vcenter << 8);

  array<bool[256]> windowAbove;
  array<bool[256]> windowBelow;
  renderWindow(self.window, self.window.aboveEnable, windowAbove);
  renderWindow(self.window, self.window.belowEnable, windowBelow);

  if(!configuration.hacks.ppuFast.hiresMode7) {
    for(int X : range(256)) {
      int x = !io.mode7.hflip ? X : 255 - X;
      int pixelX = originX + a * x >> 8;
      int pixelY = originY + c * x >> 8;
      int tileX = pixelX >> 3 & 127;
      int tileY = pixelY >> 3 & 127;
      bool outOfBounds = (pixelX | pixelY) & ~1023;
      uint15 tileAddress = tileY * 128 + tileX;
      uint15 paletteAddress = ((pixelY & 7) << 3) + (pixelX & 7);
      uint8 tile = io.mode7.repeat == 3 && outOfBounds ? 0 : ppu.vram[tileAddress].byte(0);
      uint8 palette = io.mode7.repeat == 2 && outOfBounds ? 0 : ppu.vram[paletteAddress + (tile << 6)].byte(1);

      uint priority;
      if(source == Source::BG1) {
        priority = self.priority[0];
      } else if(source == Source::BG2) {
        priority = self.priority[palette >> 7];
        palette &= 0x7f;
      }

      if(!self.mosaicEnable || --mosaicCounter == 0) {
        mosaicCounter = 1 + io.mosaicSize;
        mosaicPalette = palette;
        mosaicPriority = priority;
        if(io.col.directColor && source == Source::BG1) {
          mosaicColor = directColor(0, palette);
        } else {
          mosaicColor = cgram[palette];
        }
      }
      if(!mosaicPalette) continue;

      if(self.aboveEnable && !windowAbove[X]) plotAbove(X, source, mosaicPriority, mosaicColor);
      if(self.belowEnable && !windowBelow[X]) plotBelow(X, source, mosaicPriority, mosaicColor);
    }
  } else {
    //emulator enhancement option: render 512 pixels instead of 256 pixels of horizontal resolution
    //note: this mode is impossible on real hardware due to needing 512 above+below pixels
    for(int X : range(512)) {
      int x = !io.mode7.hflip ? X : 511 - X;
      int pixelX = 2 * originX + a * x >> 9;
      int pixelY = 2 * originY + c * x >> 9;
      int tileX = pixelX >> 3 & 127;
      int tileY = pixelY >> 3 & 127;
      bool outOfBounds = (pixelX | pixelY) & ~1023;
      uint15 tileAddress = tileY * 128 + tileX;
      uint15 paletteAddress = ((pixelY & 7) << 3) + (pixelX & 7);
      uint8 tile = io.mode7.repeat == 3 && outOfBounds ? 0 : ppu.vram[tileAddress].byte(0);
      uint8 palette = io.mode7.repeat == 2 && outOfBounds ? 0 : ppu.vram[paletteAddress + (tile << 6)].byte(1);

      uint priority;
      if(source == Source::BG1) {
        priority = self.priority[0];
      } else if(source == Source::BG2) {
        priority = self.priority[palette >> 7];
        palette &= 0x7f;
      }

      if(!self.mosaicEnable || !io.mosaicSize || --mosaicCounter == 0) {
        mosaicCounter = (1 + io.mosaicSize) << 1;
        mosaicPalette = palette;
        mosaicPriority = priority;
        if(io.col.directColor && source == Source::BG1) {
          mosaicColor = directColor(0, palette);
        } else {
          mosaicColor = cgram[palette];
        }
      }
      if(!mosaicPalette) continue;

      if(self.aboveEnable && !windowAbove[X >> 1]) plotAbove(X >> 1 | (X & 1 ? 256 : 0), source, mosaicPriority, mosaicColor);
      if(self.belowEnable && !windowBelow[X >> 1]) plotBelow(X >> 1 | (X & 1 ? 256 : 0), source, mosaicPriority, mosaicColor);
    }
  }
}

//#HDmode7>>>>>
auto PPU::Line::renderMode7HD(PPU::IO::Background& self, uint source) -> void {

  int pixelXp = -12345;
  int pixelYp = -12345;
  Pixel pixel;

  bool hp = source == Source::BG1;
  Pixel* abo = &ppu.lines[this->y].above[0];
  Pixel* bel = &ppu.lines[this->y].below[0];
  abo--;
  bel--;

  // get the first and last scan line for interpolation
  int y_a = this->y;
  int y_b = this->y;
  if (configuration.hacks.ppuFast.hdMode7Perspective) {
    while (y_a > 1 && ppu.lines[y_a].io.bg1.tileMode == TileMode::Mode7
        && (ppu.lines[y_a].io.bg1.aboveEnable || ppu.lines[y_a].io.bg1.belowEnable)) {
      y_a--;
    } 
    y_a++;
    while (y_b < 240-1 && ppu.lines[y_b].io.bg1.tileMode == TileMode::Mode7
        && (ppu.lines[y_b].io.bg1.aboveEnable || ppu.lines[y_b].io.bg1.belowEnable)) {
      y_b++;
    }
    y_b -= 8;
  } else {
    if (y_a > 0 && ppu.lines[y_a].io.bg1.tileMode == TileMode::Mode7
        && (ppu.lines[y_a].io.bg1.aboveEnable || ppu.lines[y_a].io.bg1.belowEnable)) {
      y_a--;
    }
    if (y_b < 240-1 && ppu.lines[y_b].io.bg1.tileMode == TileMode::Mode7
        && (ppu.lines[y_b].io.bg1.aboveEnable || ppu.lines[y_b].io.bg1.belowEnable)) {
      y_b++;
    }
  }

  Line line_a = ppu.lines[y_a];
  float a_a = (int16)line_a.io.mode7.a;
  float b_a = (int16)line_a.io.mode7.b;
  float c_a = (int16)line_a.io.mode7.c;
  float d_a = (int16)line_a.io.mode7.d;

  Line line_b = ppu.lines[y_b];
  float a_b = (int16)line_b.io.mode7.a;
  float b_b = (int16)line_b.io.mode7.b;
  float c_b = (int16)line_b.io.mode7.c;
  float d_b = (int16)line_b.io.mode7.d;

  int hcenter = (int13)io.mode7.x;
  int vcenter = (int13)io.mode7.y;
  int hoffset = (int13)io.mode7.hoffset;
  int voffset = (int13)io.mode7.voffset;

  array<bool[256]> windowAbove;
  array<bool[256]> windowBelow;
  renderWindow(self.window, self.window.aboveEnable, windowAbove);
  renderWindow(self.window, self.window.belowEnable, windowBelow);

  int Y = this->y - (self.mosaicEnable ? this->y % (1 + io.mosaicSize) : 0);
  float yt = !io.mode7.vflip ? Y : 255 - Y;

  // additional y positions
  for(int ys = 0; ys < ppu.hdScale() ; ys++) {
    float y = yt + ys * 1.0 / ppu.hdScale() - 0.5;

          // inter-/extra-...
    float a = 1.0/polate(y_a, 1.0/a_a, y_b, 1.0/a_b, y);
    float b = 1.0/polate(y_a, 1.0/b_a, y_b, 1.0/b_b, y);
    float c = 1.0/polate(y_a, 1.0/c_a, y_b, 1.0/c_b, y);
    float d = 1.0/polate(y_a, 1.0/d_a, y_b, 1.0/d_b, y);

    // float version of the integer/bit math
    int ht = (hoffset - hcenter) % 1024;
    float vty = ((voffset - vcenter) % 1024) + y;
    float originX = (a * ht) + (b * vty) + (hcenter << 8);
    float originY = (c * ht) + (d * vty) + (vcenter << 8);

    for(int X : range(256)) {
      float xt = !io.mode7.hflip ? X : 255 - X;

      bool doAbo = self.aboveEnable && !windowAbove[X];
      bool doBel = self.belowEnable && !windowBelow[X];

      // additional x positions
      for(int xs = 0; xs < ppu.hdScale() ; xs++) {
        float x = xt + xs * 1.0 / ppu.hdScale() - 0.5;
        int pixelX = (int)((originX + a * x) / 256);
        int pixelY = (int)((originY + c * x) / 256);
        
        // make sure to increment before any 'if's
        abo++;
        bel++;

        uint mosaicCounter = 1;
        uint mosaicPalette = 0;
        uint mosaicPriority = 0;
        uint mosaicColor = 0;
        // only compute color again when coordinates have changed
        if(pixelX != pixelXp || pixelY != pixelYp) {
          uint8 tile    = io.mode7.repeat == 3 && ((pixelX | pixelY) & ~1023) ? 0 : ppu.vram[(pixelY >> 3 & 127)  * 128  +  (pixelX >> 3 & 127)].byte(0);
          uint8 palette = io.mode7.repeat == 2 && ((pixelX | pixelY) & ~1023) ? 0 : ppu.vram[(((pixelY & 7) << 3) + (pixelX & 7)) + (tile << 6)].byte(1);

          uint priority;
          if(hp) {
            priority = self.priority[0];
          } else if(source == Source::BG2) {
            priority = self.priority[palette >> 7];
            palette &= 0x7f;
          }

          if(!self.mosaicEnable || --mosaicCounter == 0) {
            mosaicCounter = 1 + io.mosaicSize;
            mosaicPalette = palette;
            mosaicPriority = priority;
            if(io.col.directColor && source == Source::BG1) {
              mosaicColor = directColor(0, palette);
            } else {
              mosaicColor = cgram[palette];
            }
          }
          if(!mosaicPalette) continue;
        
          pixelXp = pixelX;
          pixelYp = pixelY;
          pixel = {source, mosaicPriority, mosaicColor};
        }
        if(doAbo && (hp || (mosaicPriority > abo->priority))) *abo = pixel;
        if(doBel && (hp || (mosaicPriority > bel->priority))) *bel = pixel;
      }
    }
  }
}
         // inter-/extra-...
auto PPU::Line::polate(float pa, float va, float pb, float vb, float pr) -> float {
  if (va == vb || pr == pa) {
    return va;
  }
 if (pr == pb) {
    return vb;
  }
  return va + (vb - va) / (pb - pa) * (pr - pa);
}
//#HDmode7<<<<<

uint PPUfast::Line::start = 0;
uint PPUfast::Line::count = 0;

auto PPUfast::Line::flush() -> void {
  uint perspCorMode = ppufast.hdPerspective();
  if(perspCorMode > 0) {
    #define isLineMode7(l) (l.io.bg1.tileMode == TileMode::Mode7 \
        && !l.io.displayDisable && (l.io.bg1.aboveEnable || l.io.bg1.belowEnable))
    ppufast.ind = 0;
    bool state = false;
    uint y;
    int offsPart = 8;
    if(perspCorMode == 2 || perspCorMode == 5) {
      offsPart = 6;
    } else if(perspCorMode == 3 || perspCorMode == 6) {
      offsPart = 4;
    }
    for(y = 0; y < Line::count; y++) {
      if(state != isLineMode7(ppufast.lines[Line::start + y])) {
        state = !state;
        if(state) {
          ppufast.starts[ppufast.ind] = ppufast.lines[Line::start + y].y;
        } else {
          ppufast.ends[ppufast.ind] = ppufast.lines[Line::start + y].y - 1;
          int offs = (ppufast.ends[ppufast.ind] - ppufast.starts[ppufast.ind]) / offsPart;
          ppufast.startsp[ppufast.ind] = ppufast.starts[ppufast.ind] + offs;
          ppufast.endsp[ppufast.ind] = ppufast.ends[ppufast.ind] - offs;
          ppufast.ind++;
        }
      }
    }
    #undef isLineMode7
    if(state) {
      ppufast.ends[ppufast.ind] = ppufast.lines[Line::start + y].y - 1;
      int offs = (ppufast.ends[ppufast.ind] - ppufast.starts[ppufast.ind]) / offsPart;
      ppufast.startsp[ppufast.ind] = ppufast.starts[ppufast.ind] + offs;
      ppufast.endsp[ppufast.ind] = ppufast.ends[ppufast.ind] - offs;
      ppufast.ind++;
    }

    if(perspCorMode < 4) {  
      for(int i = 0; i < ppufast.ind; i++) {
        int la = -1;
        int lb = -1;
        int lc = -1;
        int ld = -1;
        bool abd= false;
        bool bbd= false;
        bool cbd= false;
        bool dbd= false;
        bool ab= false;
        bool bb= false;
        bool cb= false;
        bool db= false;
        for(y = ppufast.startsp[i]; y <= ppufast.endsp[i]; y++) {
          int a = ((int)((int16)(ppufast.lines[y].io.mode7.a)));
          int b = ((int)((int16)(ppufast.lines[y].io.mode7.b)));
          int c = ((int)((int16)(ppufast.lines[y].io.mode7.c)));
          int d = ((int)((int16)(ppufast.lines[y].io.mode7.d)));
          if(la > 0 && a > 0 && a != la) {
            if(!abd) {
              abd = true;
              ab = a > la;
            } else if(ab != a > la) {
              ppufast.startsp[i] = -1;
              ppufast.endsp[i] = -1;
              break;
            }
          }
          if(lb > 0 && b > 0 && b != lb) {
            if(!bbd) {
              bbd = true;
              bb = b > lb;
            } else if(bb != b > lb) {
              ppufast.startsp[i] = -1;
              ppufast.endsp[i] = -1;
              break;
            }
          }
          if(lc > 0 && c > 0 && c != lc) {
            if(!cbd) {
              cbd = true;
              cb = c > lc;
            } else if(cb != c > lc) {
              ppufast.startsp[i] = -1;
              ppufast.endsp[i] = -1;
              break;
            }
          }
          if(ld > 0 && d > 0 && d != ld) {
            if(!dbd) {
              dbd = true;
              db = d > ld;
            } else if(db != d > ld) {
              ppufast.startsp[i] = -1;
              ppufast.endsp[i] = -1;
              break;
            }
          }
          la = a;
          lb = b;
          lc = c;
          ld = d;
        }
      }
    }
  }

  if(Line::count) {
    #pragma omp parallel for if(Line::count >= 8)
    for(uint y = 0; y < Line::count; y++) {
      ppufast.lines[Line::start + y].render();
    }
    Line::start = 0;
    Line::count = 0;
  }
}

auto PPUfast::Line::render() -> void {
  auto hd = ppufast.hd();
  auto ss = ppufast.ss();
  auto scale = ppufast.hdScale();
  auto output = ppufast.output + (!hd
  ? (y * 1024 + (ppufast.interlace() && ppufast.field() ? 512 : 0))
  : (y * (256+2*ppufast.widescreen()) * scale * scale)
  );
  auto width = (!hd
  ? (!ppufast.hires() ? 256 : 512)
  : ((256+2*ppufast.widescreen()) * scale * scale));

  if(io.displayDisable) {
    memory::fill<uint32>(output, width);
    return;
  }

  bool hires = io.pseudoHires || io.bgMode == 5 || io.bgMode == 6;
  auto aboveColor = cgram[0];
  auto belowColor = hires ? cgram[0] : io.col.fixedColor;
  uint xa =  (hd || ss) && ppufast.interlace() && ppufast.field() ? 256 * scale * scale / 2 : 0;
  uint xb = !(hd || ss) ? 256 : ppufast.interlace() && !ppufast.field() ? (256+2*ppufast.widescreen()) * scale * scale / 2 : (256+2*ppufast.widescreen()) * scale * scale;
  for(uint x = xa; x < xb; x++) {
    above[x] = {Source::COL, 0, aboveColor};
    below[x] = {Source::COL, 0, belowColor};
  }

  renderBackground(io.bg1, Source::BG1);
  if(!io.extbg) renderBackground(io.bg2, Source::BG2);
  renderBackground(io.bg3, Source::BG3);
  renderBackground(io.bg4, Source::BG4);
  renderObject(io.obj);
  if(io.extbg) renderBackground(io.bg2, Source::BG2);
  renderWindow(io.col.window, io.col.window.aboveMask, windowAbove);
  renderWindow(io.col.window, io.col.window.belowMask, windowBelow);

  auto luma = io.displayBrightness << 15;
  if(hd) for(uint x : range((256+2*ppufast.widescreen()) * scale * scale)) {
    *output++ = luma | pixel((x / scale % (256+2*ppufast.widescreen()) - ppufast.widescreen()), above[x], below[x]);
  } else if(width == 256) for(uint x : range(256)) {
    *output++ = luma | pixel(x, above[x], below[x]);
  } else if(!hires) for(uint x : range(256)) {
    auto color = luma | pixel(x, above[x], below[x]);
    *output++ = color;
    *output++ = color;
  } else for(uint x : range(256)) {
    *output++ = luma | pixel(x, below[x], above[x]);
    *output++ = luma | pixel(x, above[x], below[x]);
  }
}

auto PPUfast::Line::pixel(uint x, Pixel above, Pixel below) const -> uint15 {
  if(!windowAbove[ppufast.winXad(x, false)]) above.color = 0x0000;
  if(!windowBelow[ppufast.winXad(x, true)]) return above.color;
  if(!io.col.enable[above.source]) return above.color;
  if(!io.col.blendMode) return blend(above.color, io.col.fixedColor, io.col.halve && windowAbove[ppufast.winXad(x, false)]);
  return blend(above.color, below.color, io.col.halve && windowAbove[ppufast.winXad(x, false)] && below.source != Source::COL);
}

auto PPUfast::Line::blend(uint x, uint y, bool halve) const -> uint15 {
  if(!io.col.mathMode) {  //add
    if(!halve) {
      uint sum = x + y;
      uint carry = (sum - ((x ^ y) & 0x0421)) & 0x8420;
      return (sum - carry) | (carry - (carry >> 5));
    } else {
      return (x + y - ((x ^ y) & 0x0421)) >> 1;
    }
  } else {  //sub
    uint diff = x - y + 0x8420;
    uint borrow = (diff - ((x ^ y) & 0x8420)) & 0x8420;
    if(!halve) {
      return   (diff - borrow) & (borrow - (borrow >> 5));
    } else {
      return (((diff - borrow) & (borrow - (borrow >> 5))) & 0x7bde) >> 1;
    }
  }
}

auto PPUfast::Line::directColor(uint paletteIndex, uint paletteColor) const -> uint15 {
  //paletteIndex = bgr
  //paletteColor = BBGGGRRR
  //output       = 0 BBb00 GGGg0 RRRr0
  return (paletteColor << 2 & 0x001c) + (paletteIndex <<  1 & 0x0002)   //R
       + (paletteColor << 4 & 0x0380) + (paletteIndex <<  5 & 0x0040)   //G
       + (paletteColor << 7 & 0x6000) + (paletteIndex << 10 & 0x1000);  //B
}

auto PPUfast::Line::plotAbove(int x, uint source, uint priority, uint color) -> void {
  if(ppufast.hd() || ppufast.ss()) return plotHD(above, x, source, priority, color, false, false);
  if(priority > above[x].priority) above[x] = {source, priority, color};
}

auto PPUfast::Line::plotBelow(int x, uint source, uint priority, uint color) -> void {
  if(ppufast.hd() || ppufast.ss()) return plotHD(below, x, source, priority, color, false, false);
  if(priority > below[x].priority) below[x] = {source, priority, color};
}

//todo: name these variables more clearly ...
auto PPUfast::Line::plotHD(Pixel* pixel, int x, uint source, uint priority, uint color, bool hires, bool subpixel) -> void {
  int scale = ppufast.hdScale();
  int wss = ppufast.widescreen() * scale;
  int xss = hires && subpixel ? scale / 2 : 0;
  int ys = ppufast.interlace() && ppufast.field() ? scale / 2 : 0;
  if(priority > pixel[x * scale + xss + ys * 256 * scale + wss].priority) {
    Pixel p = {source, priority, color};
    int xsm = hires && !subpixel ? scale / 2 : scale;
    int ysm = ppufast.interlace() && !ppufast.field() ? scale / 2 : scale;
    for(int xs = xss; xs < xsm; xs++) {
      pixel[x * scale + xs + ys * 256 * scale + wss] = p;
    }
    int size = sizeof(Pixel) * (xsm - xss);
    Pixel* source = &pixel[x * scale + xss + ys * 256 * scale + wss];
    for(int yst = ys + 1; yst < ysm; yst++) {
      memcpy(&pixel[x * scale + xss + yst * (256+2*ppufast.widescreen()) * scale + wss], source, size);
    }
  }
}

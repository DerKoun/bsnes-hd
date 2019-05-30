uint PPUfast::Line::start = 0;
uint PPUfast::Line::count = 0;

auto PPUfast::Line::flush() -> void {
  if(ppufast.hdPerspective()) {
    #define isLineMode7(l) (l.io.bg1.tileMode == TileMode::Mode7 \
        && !l.io.displayDisable && (l.io.bg1.aboveEnable || l.io.bg1.belowEnable))
    ppufast.ind = 0;
    bool state = false;
    uint y;
    for(y = 0; y < Line::count; y++) {
      if(state != isLineMode7(ppufast.lines[Line::start + y])) {
        state = !state;
        if(state) {
          ppufast.starts[ppufast.ind] = ppufast.lines[Line::start + y].y;
        } else {
          ppufast.ends[ppufast.ind] = ppufast.lines[Line::start + y].y - 1;
          int offs = (ppufast.ends[ppufast.ind] - ppufast.starts[ppufast.ind]) / 8;
          ppufast.startsp[ppufast.ind] = ppufast.starts[ppufast.ind] + offs;
          ppufast.endsp[ppufast.ind] = ppufast.ends[ppufast.ind] - offs;
          ppufast.ind++;
        }
      }
    }
    #undef isLineMode7
    if(state) {
      ppufast.ends[ppufast.ind] = ppufast.lines[Line::start + y].y - 1;
      int offs = (ppufast.ends[ppufast.ind] - ppufast.starts[ppufast.ind]) / 8;
      ppufast.startsp[ppufast.ind] = ppufast.starts[ppufast.ind] + offs;
      ppufast.endsp[ppufast.ind] = ppufast.ends[ppufast.ind] - offs;
      ppufast.ind++;
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

  if(ppufast.renderbg(Source::BG1)) renderBackground(io.bg1, Source::BG1);
  if(ppufast.renderbg(Source::BG2)) renderBackground(io.bg2, Source::BG2);
  if(ppufast.renderbg(Source::BG3)) renderBackground(io.bg3, Source::BG3);
  if(ppufast.renderbg(Source::BG4)) renderBackground(io.bg4, Source::BG4);
  if(ppufast.renderobj()) renderObject(io.obj);
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

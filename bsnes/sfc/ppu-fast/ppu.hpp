//performance-focused, scanline-based, parallelized implementation of PPU

//limitations:
//* mid-scanline effects not support
//* vertical mosaic coordinates are not exact
//* (hardware-mod) 128KB VRAM mode not supported

struct PPUfast : Thread, PPUcounter {
  alwaysinline auto interlace() const -> bool;
  alwaysinline auto overscan() const -> bool;
  alwaysinline auto vdisp() const -> uint;
  alwaysinline auto hires() const -> bool;
  alwaysinline auto hd() const -> bool;
  alwaysinline auto ss() const -> bool;
  alwaysinline auto hdScale() const -> uint;
  alwaysinline auto hdPerspective() const -> bool;
  alwaysinline auto hdSupersample() const -> uint;
  alwaysinline auto hdMosaic() const -> bool;
  alwaysinline auto widescreen() const -> uint;
  alwaysinline auto wsbg(uint bg) const -> uint;
  alwaysinline auto wsobj() const -> bool;
  alwaysinline auto winXad(uint x, bool bel) const -> uint;
  alwaysinline auto renderbg(uint bg) const -> bool;
  alwaysinline auto renderobj() const -> bool;

  //ppu.cpp
  PPUfast();
  ~PPUfast();

  static auto Enter() -> void;
  alwaysinline auto step(uint clocks) -> void;
  auto main() -> void;
  auto scanline() -> void;
  auto refresh() -> void;
  auto load() -> bool;
  auto power(bool reset) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

public:
  struct Source { enum : uint { BG1, BG2, BG3, BG4, OBJ1, OBJ2, COL }; };
  struct TileMode { enum : uint { BPP2, BPP4, BPP8, Mode7, Inactive }; };
  struct ScreenMode { enum : uint { Above, Below }; };

  struct Latch {
    //serialization.cpp
    auto serialize(serializer&) -> void;

     uint1 interlace;
     uint1 overscan;
     uint1 hires;
     uint1 hd;
     uint1 ss;

    uint16 vram;
     uint8 oam;
     uint8 cgram;

    uint10 oamAddress;
     uint8 cgramAddress;

     uint8 mode7;
     uint1 counters;
     uint1 hcounter;  //hdot
     uint1 vcounter;

    struct PPU {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      uint8 mdr;
      uint8 bgofs;
    } ppu1, ppu2;
  };

  struct IO {
    //serialization.cpp
    auto serialize(serializer&) -> void;

     uint1 displayDisable;
     uint4 displayBrightness;
    uint10 oamBaseAddress;
    uint10 oamAddress;
     uint1 oamPriority;
     uint1 bgPriority;
     uint3 bgMode;
     uint4 mosaicSize;
     uint1 vramIncrementMode;
     uint2 vramMapping;
     uint8 vramIncrementSize;
    uint16 vramAddress;
     uint8 cgramAddress;
     uint1 cgramAddressLatch;
     uint9 hcounter;  //hdot
     uint9 vcounter;
     uint1 interlace;
     uint1 overscan;
     uint1 pseudoHires;
     uint1 extbg;

    struct Mode7 {
      //serialization.cpp
      auto serialize(serializer&) -> void;

       uint1 hflip;
       uint1 vflip;
       uint2 repeat;
      uint16 a;
      uint16 b;
      uint16 c;
      uint16 d;
      uint16 x;
      uint16 y;
      uint16 hoffset;
      uint16 voffset;
    } mode7;

    struct Window {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      uint8 oneLeft;
      uint8 oneRight;
      uint8 twoLeft;
      uint8 twoRight;
    } window;

    struct WindowLayer {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      uint1 oneEnable;
      uint1 oneInvert;
      uint1 twoEnable;
      uint1 twoInvert;
      uint2 mask;
      uint1 aboveEnable;
      uint1 belowEnable;
    };

    struct WindowColor {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      uint1 oneEnable;
      uint1 oneInvert;
      uint1 twoEnable;
      uint1 twoInvert;
      uint2 mask;
      uint2 aboveMask;
      uint2 belowMask;
    };

    struct Background {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      WindowLayer window;
       uint1 aboveEnable;
       uint1 belowEnable;
       uint1 mosaicEnable;
      uint15 tiledataAddress;
      uint15 screenAddress;
       uint2 screenSize;
       uint1 tileSize;
      uint16 hoffset;
      uint16 voffset;
       uint3 tileMode;
       uint4 priority[2];
    } bg1, bg2, bg3, bg4;

    struct Object {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      WindowLayer window;
       uint1 aboveEnable;
       uint1 belowEnable;
       uint1 interlace;
       uint3 baseSize;
       uint2 nameselect;
      uint15 tiledataAddress;
       uint7 first;
       uint1 rangeOver;
       uint1 timeOver;
       uint4 priority[4];
    } obj;

    struct Color {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      WindowColor window;
       uint1 enable[7];
       uint1 directColor;
       uint1 blendMode;  //0 = fixed; 1 = pixel
       uint1 halve;
       uint1 mathMode;   //0 = add; 1 = sub
      uint15 fixedColor;
    } col;
  };

  struct Object {
    //serialization.cpp
    auto serialize(serializer&) -> void;

    uint9 x;
    uint8 y;
    uint8 character;
    uint1 nameselect;
    uint1 vflip;
    uint1 hflip;
    uint2 priority;
    uint3 palette;
    uint1 size;
  };

  struct ObjectItem {
    uint1 valid;
    uint7 index;
    uint8 width;
    uint8 height;
  };

  struct ObjectTile {
     uint1 valid;
     uint9 x;
     uint8 y;
     uint2 priority;
     uint8 palette;
     uint1 hflip;
    uint11 number;
  };

  struct Pixel {
     uint8 source;
     uint8 priority;
    uint15 color;
  };

  //io.cpp
  auto latchCounters() -> void;
  alwaysinline auto vramAddress() const -> uint15;
  alwaysinline auto readVRAM() -> uint16;
  alwaysinline auto writeVRAM(uint1 byte, uint8 data) -> void;
  alwaysinline auto updateTiledata(uint15 address) -> void;
  alwaysinline auto readOAM(uint10 address) -> uint8;
  alwaysinline auto writeOAM(uint10 address, uint8 data) -> void;
  alwaysinline auto readCGRAM(uint1 byte, uint8 address) -> uint8;
  alwaysinline auto writeCGRAM(uint8 address, uint15 data) -> void;
  auto readIO(uint24 address, uint8 data) -> uint8;
  auto writeIO(uint24 address, uint8 data) -> void;
  auto updateVideoMode() -> void;

  //object.cpp
  auto oamAddressReset() -> void;
  auto oamSetFirstObject() -> void;
  auto readObject(uint10 address) -> uint8;
  auto writeObject(uint10 address, uint8 data) -> void;

  //[serialized]
  Latch latch;
  IO io;

  uint16 vram[32 * 1024];
  uint15 cgram[256];
  Object objects[128];

  //[unserialized]
  uint32* output;
   uint8* tilecache[3];  //bitplane -> bitmap tiledata
  uint32 ItemLimit;
  uint32 TileLimit;

  struct Line {
    //line.cpp
    static auto flush() -> void;
    auto render() -> void;
    auto pixel(uint x, Pixel above, Pixel below) const -> uint15;
    auto blend(uint x, uint y, bool halve) const -> uint15;
    alwaysinline auto directColor(uint paletteIndex, uint paletteColor) const -> uint15;
    alwaysinline auto plotAbove(int x, uint source, uint priority, uint color) -> void;
    alwaysinline auto plotBelow(int x, uint source, uint priority, uint color) -> void;
    alwaysinline auto plotHD(Pixel*, int x, uint source, uint priority, uint color, bool hires, bool subpixel) -> void;

    //background.cpp
    auto renderBackground(PPUfast::IO::Background&, uint source) -> void;
    auto getTile(PPUfast::IO::Background&, uint hoffset, uint voffset) -> uint;

    //mode7.cpp
    auto renderMode7(PPUfast::IO::Background&, uint source) -> void;

    //mode7hd.cpp
    auto renderMode7HD(PPUfast::IO::Background&, uint source) -> void;
    alwaysinline auto lerp(float pa, float va, float pb, float vb, float pr) -> float;

    //object.cpp
    auto renderObject(PPUfast::IO::Object&) -> void;

    //window.cpp
    auto renderWindow(PPUfast::IO::WindowLayer&, bool, array<bool[256]>&) -> void;
    auto renderWindow(PPUfast::IO::WindowColor&, uint, array<bool[256]>&) -> void;

    //[unserialized]
    uint9 y;  //constant

    IO io;
    array<uint15[256]> cgram;

    array<ObjectItem[128]> items;  //32 on real hardware
    array<ObjectTile[128]> tiles;  //34 on real hardware; 1024 max (128 * 64-width tiles)

    Pixel above[256 * 9 * 9];
    Pixel below[256 * 9 * 9];

    array<bool[256]> windowAbove;
    array<bool[256]> windowBelow;

    //flush()
    static uint start;
    static uint count;
  };
  array<Line[240]> lines;


    array<int[32]> starts;
    array<int[32]> ends;
    int ind = 0;
    array<int[32]> startsp;
    array<int[32]> endsp;

};

extern PPUfast ppufast;

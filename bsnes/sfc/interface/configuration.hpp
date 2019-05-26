struct Configuration {
  auto read() -> string;
  auto read(string) -> string;
  auto write(string) -> bool;
  auto write(string, string) -> bool;

  struct System {
    struct CPU {
      uint version = 2;
    } cpu;
    struct PPU1 {
      uint version = 1;
      struct VRAM {
        uint size = 0x10000;
      } vram;
    } ppu1;
    struct PPU2 {
      uint version = 3;
    } ppu2;
  } system;

  struct Video {
    bool blurEmulation = true;
    bool colorEmulation = true;
  } video;

  struct Hacks {
    struct PPU {
      bool fast = true;
      bool noSpriteLimit = true;
      struct Mode7 {
        uint scale = 2;
        bool perspective = true;
        uint widescreen = 72;
        uint wsbg1 = 1;
        uint wsbg2 = 1;
        uint wsbg3 = 1;
        uint wsbg4 = 1;
        bool wsobj = false;
        bool igwin = false;
        uint supersample = 1;
        bool mosaic = false;
      } mode7;
    } ppu;
    struct DSP {
      bool fast = true;
      bool cubic = false;
    } dsp;
    struct Coprocessors {
      bool delayedSync = true;
      bool hle = true;
    } coprocessors;
  } hacks;

private:
  auto process(Markup::Node document, bool load) -> void;
};

extern Configuration configuration;

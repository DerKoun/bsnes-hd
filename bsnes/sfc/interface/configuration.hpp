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
    struct Serialization {
      string method = "Fast";
    } serialization;
  } system;

  struct Video {
    bool aspectCorrection = false;
    bool overscan = false;
    bool blurEmulation = true;
    bool colorEmulation = true;
    uint saturation = 100;
    uint gamma = 100;
    uint luminance = 100;
  } video;

  struct Hacks {
    bool hotfixes = true;
    string entropy = "Low";
    struct CPU {
      uint overclock = 100;
      bool fastMath = false;
      bool fastJoypadPolling = false;
    } cpu;
    struct PPU {
      bool fast = true;
      bool deinterlace = true;
      bool noSpriteLimit = true;
      bool noVRAMBlocking = false;
      uint renderCycle = 512;
      struct Mode7 {
        uint scale = 2;
        uint perspective = 1;
        uint widescreen = 64;
        uint wsbg1 = 16;
        uint wsbg2 = 16;
        uint wsbg3 = 16;
        uint wsbg4 = 16;
        uint wsobj = 0;
        uint igwin = 1;
        uint igwinx = 128;
        bool strwin = false;
        uint vramExt = 0x7fff;
        uint bgGrad = 4;
        uint windRad = 0;
        uint wsMode = 1;
        uint wsBgCol = 1;
        uint wsMarker = 0;
        uint wsMarkerAlpha = 1;
        uint supersample = 1;
        uint mosaic = 1;
      } mode7;
    } ppu;
    struct DSP {
      bool fast = true;
      bool cubic = false;
      bool echoShadow = false;
    } dsp;
    struct Coprocessor {
      bool delayedSync = true;
      bool preferHLE = false;
    } coprocessor;
    struct SA1 {
      uint overclock = 100;
    } sa1;
    struct SuperFX {
      uint overclock = 100;
    } superfx;
  } hacks;

private:
  auto process(Markup::Node document, bool load) -> void;
};

extern Configuration configuration;

Configuration configuration;

auto Configuration::process(Markup::Node document, bool load) -> void {
  #define bind(type, path, name) \
    if(load) { \
      if(auto node = document[path]) name = node.type(); \
    } else { \
      document(path).setValue(name); \
    } \

  bind(natural, "System/CPU/Version", system.cpu.version);
  bind(natural, "System/PPU1/Version", system.ppu1.version);
  bind(natural, "System/PPU1/VRAM/Size", system.ppu1.vram.size);
  bind(natural, "System/PPU2/Version", system.ppu2.version);

  bind(boolean, "Video/BlurEmulation", video.blurEmulation);
  bind(boolean, "Video/ColorEmulation", video.colorEmulation);

  bind(boolean, "Hacks/PPU/Fast", hacks.ppu.fast);
  bind(boolean, "Hacks/PPU/NoSpriteLimit", hacks.ppu.noSpriteLimit);
  bind(natural, "Hacks/PPU/Mode7/Scale", hacks.ppu.mode7.scale);
  bind(natural, "Hacks/PPU/Mode7/Perspective", hacks.ppu.mode7.perspective);
  bind(natural, "Hacks/PPU/Mode7/Widescreen", hacks.ppu.mode7.widescreen);
  bind(natural, "Hacks/PPU/Mode7/Wsbg1", hacks.ppu.mode7.wsbg1);
  bind(natural, "Hacks/PPU/Mode7/Wsbg2", hacks.ppu.mode7.wsbg2);
  bind(natural, "Hacks/PPU/Mode7/Wsbg3", hacks.ppu.mode7.wsbg3);
  bind(natural, "Hacks/PPU/Mode7/Wsbg4", hacks.ppu.mode7.wsbg4);
  bind(natural, "Hacks/PPU/Mode7/Wsobj", hacks.ppu.mode7.wsobj);
  bind(natural, "Hacks/PPU/Mode7/Igwin", hacks.ppu.mode7.igwin);
  bind(natural, "Hacks/PPU/Mode7/Igwinx", hacks.ppu.mode7.igwinx);
  bind(natural, "Hacks/PPU/Mode7/BgGrad", hacks.ppu.mode7.bgGrad);
  bind(natural, "Hacks/PPU/Mode7/WindRad", hacks.ppu.mode7.windRad);
  bind(natural, "Hacks/PPU/Mode7/WsMode", hacks.ppu.mode7.wsMode);
  bind(natural, "Hacks/PPU/Mode7/WsBgCol", hacks.ppu.mode7.wsBgCol);
  bind(natural, "Hacks/PPU/Mode7/UnintrMode", hacks.ppu.mode7.unintrMode);
  bind(natural, "Hacks/PPU/Mode7/UnintrTop", hacks.ppu.mode7.unintrTop);
  bind(natural, "Hacks/PPU/Mode7/UnintrBottom", hacks.ppu.mode7.unintrBottom);
  bind(natural, "Hacks/PPU/Mode7/UnintrLeft", hacks.ppu.mode7.unintrLeft);
  bind(natural, "Hacks/PPU/Mode7/UnintrRight", hacks.ppu.mode7.unintrRight);
  bind(natural, "Hacks/PPU/Mode7/WsMarker", hacks.ppu.mode7.wsMarker);
  bind(natural, "Hacks/PPU/Mode7/WsMarkerAlpha", hacks.ppu.mode7.wsMarkerAlpha);
  bind(natural, "Hacks/PPU/Mode7/Supersample", hacks.ppu.mode7.supersample);
  bind(natural, "Hacks/PPU/Mode7/Mosaic", hacks.ppu.mode7.mosaic);
  bind(boolean, "Hacks/DSP/Fast", hacks.dsp.fast);
  bind(boolean, "Hacks/DSP/Cubic", hacks.dsp.cubic);
  bind(boolean, "Hacks/Coprocessors/HLE", hacks.coprocessors.hle);
  bind(boolean, "Hacks/Coprocessors/DelayedSync", hacks.coprocessors.delayedSync);

  #undef bind
}

auto Configuration::read() -> string {
  Markup::Node document;
  process(document, false);
  return BML::serialize(document, " ");
}

auto Configuration::read(string name) -> string {
  auto document = BML::unserialize(read());
  return document[name].text();
}

auto Configuration::write(string configuration) -> bool {
  *this = {};

  if(auto document = BML::unserialize(configuration)) {
    return process(document, true), true;
  }

  return false;
}

auto Configuration::write(string name, string value) -> bool {
  if(SuperFamicom::system.loaded() && name.beginsWith("System/")) return false;

  auto document = BML::unserialize(read());
  if(auto node = document[name]) {
    node.setValue(value);
    return process(document, true), true;
  }

  return false;
}

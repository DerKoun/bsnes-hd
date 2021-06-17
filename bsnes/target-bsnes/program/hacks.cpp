auto Program::hackCompatibility() -> void {
  string entropy = settings.emulator.hack.entropy;
  bool fastJoypadPolling = false;
  bool fastPPU = settings.emulator.hack.ppu.fast;
  bool fastPPUNoSpriteLimit = settings.emulator.hack.ppu.noSpriteLimit;
  bool fastDSP = settings.emulator.hack.dsp.fast;
  bool coprocessorDelayedSync = settings.emulator.hack.coprocessor.delayedSync;
  uint renderCycle = 512;

  auto title = superFamicom.title;
  auto region = superFamicom.region;

  //sometimes menu options are skipped over in the main menu with cycle-based joypad polling
  if(title == "Arcades Greatest Hits") fastJoypadPolling = true;

  //the start button doesn't work in this game with cycle-based joypad polling
  if(title == "TAIKYOKU-IGO Goliath") fastJoypadPolling = true;

  //holding up or down on the menu quickly cycles through options instead of stopping after each button press
  if(title == "WORLD MASTERS GOLF") fastJoypadPolling = true;

  //relies on mid-scanline rendering techniques
  if(title == "AIR STRIKE PATROL" || title == "DESERT FIGHTER") fastPPU = false;

  //the dialogue text is blurry due to an issue in the scanline-based renderer's color math support
  if(title == "マーヴェラス") fastPPU = false;

  //stage 2 uses pseudo-hires in a way that's not compatible with the scanline-based renderer
  if(title == "SFC クレヨンシンチャン") fastPPU = false;

  //title screen game select (after choosing a game) changes OAM tiledata address mid-frame
  //this is only supported by the cycle-based PPU renderer
  if(title == "Winter olympics") fastPPU = false;

  //title screen shows remnants of the flag after choosing a language with the scanline-based renderer
  if(title == "WORLD CUP STRIKER") fastPPU = false;

  //relies on cycle-accurate writes to the echo buffer
  if(title == "KOUSHIEN_2") fastDSP = false;

  //will hang immediately
  if(title == "RENDERING RANGER R2") fastDSP = false;

  //will hang sometimes in the "Bach in Time" stage
  if(title == "BUBSY II" && region == "PAL") fastDSP = false;

  //fixes an errant scanline on the title screen due to writing to PPU registers too late
  if(title == "ADVENTURES OF FRANKEN" && region == "PAL") renderCycle = 32;

  //fixes an errant scanline on the title screen due to writing to PPU registers too late
  if(title == "FIREPOWER 2000" || title == "SUPER SWIV") renderCycle = 32;

  //fixes an errant scanline on the title screen due to writing to PPU registers too late
  if(title == "NHL '94" || title == "NHL PROHOCKEY'94") renderCycle = 32;

  //fixes an errant scanline on the title screen due to writing to PPU registers too late
  if(title == "Sugoro Quest++") renderCycle = 128;

  if(settings.emulator.hack.hotfixes) {
    //this game transfers uninitialized memory into video RAM: this can cause a row of invalid tiles
    //to appear in the background of stage 12. this one is a bug in the original game, so only enable
    //it if the hotfixes option has been enabled.
    if(title == "The Hurricanes") entropy = "None";

    //Frisky Tom attract sequence sometimes hangs when WRAM is initialized to pseudo-random patterns
    if(title == "ニチブツ・アーケード・クラシックス") entropy = "None";
  }

  emulator->configure("Hacks/Entropy", entropy);
  emulator->configure("Hacks/CPU/FastJoypadPolling", fastJoypadPolling);
  emulator->configure("Hacks/PPU/Fast", fastPPU);
  emulator->configure("Hacks/PPU/NoSpriteLimit", fastPPUNoSpriteLimit);
  emulator->configure("Hacks/PPU/RenderCycle", renderCycle);
  emulator->configure("Hacks/PPU/Mode7/Scale", settings.emulator.hack.ppu.mode7.scale);	
  emulator->configure("Hacks/PPU/Mode7/Perspective", settings.emulator.hack.ppu.mode7.perspective);	
  emulator->configure("Hacks/PPU/Mode7/Widescreen", settings.emulator.hack.ppu.mode7.widescreen);	
  emulator->configure("Hacks/PPU/Mode7/Wsbg1", settings.emulator.hack.ppu.mode7.wsbg1);	
  emulator->configure("Hacks/PPU/Mode7/Wsbg2", settings.emulator.hack.ppu.mode7.wsbg2);	
  emulator->configure("Hacks/PPU/Mode7/Wsbg3", settings.emulator.hack.ppu.mode7.wsbg3);	
  emulator->configure("Hacks/PPU/Mode7/Wsbg4", settings.emulator.hack.ppu.mode7.wsbg4);	
  emulator->configure("Hacks/PPU/Mode7/Wsobj", settings.emulator.hack.ppu.mode7.wsobj);	
  emulator->configure("Hacks/PPU/Mode7/Igwin", settings.emulator.hack.ppu.mode7.igwin);	
  emulator->configure("Hacks/PPU/Mode7/Igwinx", settings.emulator.hack.ppu.mode7.igwinx);	
  emulator->configure("Hacks/PPU/Mode7/Strwin", false);
  emulator->configure("Hacks/PPU/Mode7/VramExt", 0x7fff);
  emulator->configure("Hacks/PPU/Mode7/BgGrad", settings.emulator.hack.ppu.mode7.bgGrad);	
  emulator->configure("Hacks/PPU/Mode7/WindRad", settings.emulator.hack.ppu.mode7.windRad);	
  emulator->configure("Hacks/PPU/Mode7/WsMode", settings.emulator.hack.ppu.mode7.wsMode);	
  emulator->configure("Hacks/PPU/Mode7/WsBgCol", settings.emulator.hack.ppu.mode7.wsBgCol);	
  emulator->configure("Hacks/PPU/Mode7/WsMarker", settings.emulator.hack.ppu.mode7.wsMarker);	
  emulator->configure("Hacks/PPU/Mode7/WsMarkerAlpha", settings.emulator.hack.ppu.mode7.wsMarkerAlpha);	
  emulator->configure("Hacks/PPU/Mode7/Supersample", settings.emulator.hack.ppu.mode7.supersample);	
  emulator->configure("Hacks/PPU/Mode7/Mosaic", settings.emulator.hack.ppu.mode7.mosaic);
  emulator->configure("Hacks/DSP/Fast", fastDSP);
  emulator->configure("Hacks/DSP/Cubic", settings.emulator.hack.dsp.cubic);
  emulator->configure("Hacks/Coprocessor/DelayedSync", coprocessorDelayedSync);

  if(rso) {
    int i = 0;
    int v = 0;
    int c = -1;
    int n = 0;
    bool e = true;
    while (i < rso.size()) {
      v = rso[i++];
      if (v == '%') {
        e = !e;
      }
      if (!e) {
        continue;
      }
      if ((v >= 'a' && v <= 'z') || (v >= 'A' && v <= 'Z')) {
        c = v;
      } else if (c > -1 && v >= '0' && v <= '9') {
        n = (n * 10) + (v - '0');
        if (i == rso.size() || rso[i] < '0' || rso[i] > '9') {
          switch (c) {
            //  case 'p': //pixelAspectCorrect 0:off 1:on [libretro exclusive]
            //    aspectcorrection = n == 1;
            //    break;
            //  case 'o': //overscan 0:216 1:224 (2:240 3:240f) [libretro exclusive]
            //    overscan = n == 1;
            //    break;
            case 'w': //widescreenMode 0:none 1:on 2:mode7
              emulator->configure("Hacks/PPU/Mode7/WsMode", n == 1 ? 2 : (n == 2 ? 1 : 0));
              break;
            case 'W': //WSaspectRatio int [<=200:wsExt, >200:ar]
              emulator->configure("Hacks/PPU/Mode7/Widescreen",  n);
              break;
            case 's': //WSsprites 0:safe 1:unsafe 2:clip
              emulator->configure("Hacks/PPU/Mode7/Wsobj", n == 1 ? 1 : (n == 2 ? 3 : 0));
              break;
            case 'i': //igwin 0:none 1:outside 2:outside&always 3:all
              emulator->configure("Hacks/PPU/Mode7/Igwin", n > 3 ? 0 : n);
              break;
            case 'I': //igwinx int
              emulator->configure("Hacks/PPU/Mode7/Igwinx", n > 255 ? 128 : n);
              break;
            case 'b': //bg1WS 0:off 1:on 2:auto(h+v)
              emulator->configure("Hacks/PPU/Mode7/Wsbg1", 
                  n >= 1000 ? n : //above/below line (processed later)
                    (n ==  1 ?  1 : //on
                    (n ==  2 ? 16 : //auto H+V
                    (n ==  3 ? 15 : //auto H
                    (n == 10 ? 12 : //crop
                    (n == 11 ? 13 : //crop auto
                    (n == 20 ? 14 : //disable
                  0)))))) //off
               );
              break;
            case 'B': //bg2WS 0:off 1:on 2:auto(h+v)
              emulator->configure("Hacks/PPU/Mode7/Wsbg2", 
                  n >= 1000 ? n : //above/below line (processed later)
                    (n ==  1 ?  1 : //on
                    (n ==  2 ? 16 : //auto H+V
                    (n ==  3 ? 15 : //auto H
                    (n == 10 ? 12 : //crop
                    (n == 11 ? 13 : //crop auto
                    (n == 20 ? 14 : //disable
                  0)))))) //off
               );
              break;
            case 'c': //bg3WS 0:off 1:on 2:auto(h+v)
              emulator->configure("Hacks/PPU/Mode7/Wsbg3", 
                  n >= 1000 ? n : //above/below line (processed later)
                    (n ==  1 ?  1 : //on
                    (n ==  2 ? 16 : //auto H+V
                    (n ==  3 ? 15 : //auto H
                    (n == 10 ? 12 : //crop
                    (n == 11 ? 13 : //crop auto
                    (n == 20 ? 14 : //disable
                  0)))))) //off
               );
              break;
            case 'C': //bg4WS 0:off 1:on 2:auto(h+v)
              emulator->configure("Hacks/PPU/Mode7/Wsbg4", 
                  n >= 1000 ? n : //above/below line (processed later)
                    (n ==  1 ?  1 : //on
                    (n ==  2 ? 16 : //auto H+V
                    (n ==  3 ? 15 : //auto H
                    (n == 10 ? 12 : //crop
                    (n == 11 ? 13 : //crop auto
                    (n == 20 ? 14 : //disable
                  0)))))) //off
               );
              break;
            case 'm': //wsMarker 0:off 1-10:lines 11-20:darken, wsMarkerAlpha 1-10/11-20:opaque-transparent
              emulator->configure("Hacks/PPU/Mode7/WsMarker", n < 1 || n > 20 ? 0 : (n - 1) / 10 + 1);
              emulator->configure("Hacks/PPU/Mode7/WsMarkerAlpha", (n - 1) % 10 + 1);
              break;
            case 'P': //Perspective 0:off 1-3:auto 4-6+:on (wide, medium, narrow)
              emulator->configure("Hacks/PPU/Mode7/Perspective", n < 1 || n > 6 ? 0 : n );
              break;
            case 'O': //Overlock CPU percentage
              emulator->configure("Hacks/CPU/Overclock", n );
              break;
            case 'S': //Stretch Window [for widescreen patches only]
              emulator->configure("Hacks/PPU/Mode7/Strwin", n == 2 );
              break;
            case 'v': //VRAM extension
              emulator->configure("Hacks/PPU/Mode7/VramExt", n > 0 ? 0xffff : 0x7fff );
              break;
            case 'f': //Scale factor 0:disable 1-10:scale
              emulator->configure("Hacks/PPU/Mode7/Scale", n >= 0 && n <= 10 ? n : 2);
              break;
            case 'l': //Disable sprite limit
              emulator->configure("Hacks/PPU/NoSpriteLimit", n == 1);
              break;
          }
          c = -1;
          n = 0;
        }
      }
    }
  }

}

auto Program::hackPatchMemory(vector<uint8_t>& data) -> void {
  auto title = superFamicom.title;

  if(title == "Satellaview BS-X" && data.size() >= 0x100000) {
    //BS-X: Sore wa Namae o Nusumareta Machi no Monogatari (JPN) (1.1)
    //disable limited play check for BS Memory flash cartridges
    //benefit: allow locked out BS Memory flash games to play without manual header patching
    //detriment: BS Memory ROM cartridges will cause the game to hang in the load menu
    if(data[0x4a9b] == 0x10) data[0x4a9b] = 0x80;
    if(data[0x4d6d] == 0x10) data[0x4d6d] = 0x80;
    if(data[0x4ded] == 0x10) data[0x4ded] = 0x80;
    if(data[0x4e9a] == 0x10) data[0x4e9a] = 0x80;
  }
}

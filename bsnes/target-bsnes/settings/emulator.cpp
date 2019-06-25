auto EmulatorSettings::create() -> void {
  setIcon(Icon::Action::Settings);
  setText("Emulator");

  layout.setPadding(5_sx);

  hacksNote.setForegroundColor({224, 0, 0}).setText("Note: some hack setting changes do not take effect until after reloading games or restarting the emulator.");
  optionsLabel.setText("Options").setFont(Font().setBold());
  inputFocusLabel.setText("When focus is lost:");
  pauseEmulation.setText("Pause emulation").onActivate([&] {
    settings.input.defocus = "Pause";
  });
  blockInput.setText("Block input").onActivate([&] {
    settings.input.defocus = "Block";
  });
  allowInput.setText("Allow input").onActivate([&] {
    settings.input.defocus = "Allow";
  });
  if(settings.input.defocus == "Pause") pauseEmulation.setChecked();
  if(settings.input.defocus == "Block") blockInput.setChecked();
  if(settings.input.defocus == "Allow") allowInput.setChecked();
  warnOnUnverifiedGames.setText("Warn when loading games that have not been verified").setChecked(settings.emulator.warnOnUnverifiedGames).onToggle([&] {
    settings.emulator.warnOnUnverifiedGames = warnOnUnverifiedGames.checked();
  });
  autoSaveMemory.setText("Auto-save memory periodically").setChecked(settings.emulator.autoSaveMemory.enable).onToggle([&] {
    settings.emulator.autoSaveMemory.enable = autoSaveMemory.checked();
  });
  autoSaveStateOnUnload.setText("Auto-save undo state when unloading games").setChecked(settings.emulator.autoSaveStateOnUnload).onToggle([&] {
    settings.emulator.autoSaveStateOnUnload = autoSaveStateOnUnload.checked();
    if(!autoSaveStateOnUnload.checked()) {
      autoLoadStateOnLoad.setEnabled(false).setChecked(false).doToggle();
    } else {
      autoLoadStateOnLoad.setEnabled(true);
    }
  }).doToggle();
  autoLoadStateOnLoad.setText("Auto-resume on load").setChecked(settings.emulator.autoLoadStateOnLoad).onToggle([&] {
    settings.emulator.autoLoadStateOnLoad = autoLoadStateOnLoad.checked();
  });
  coprocessorLabel.setText("Coprocessors").setFont(Font().setBold());
  coprocessorsDelayedSyncOption.setText("Fast mode").setChecked(settings.emulator.hack.coprocessors.delayedSync).onToggle([&] {
    settings.emulator.hack.coprocessors.delayedSync = coprocessorsDelayedSyncOption.checked();
  });
  coprocessorsHLEOption.setText("Prefer HLE").setChecked(settings.emulator.hack.coprocessors.hle).onToggle([&] {
    settings.emulator.hack.coprocessors.hle = coprocessorsHLEOption.checked();
  });
  superFXLabel.setText("SuperFX clock speed:");
  superFXValue.setAlignment(0.5);
  superFXClock.setLength(71).setPosition((settings.emulator.hack.fastSuperFX - 100) / 10).onChange([&] {
    settings.emulator.hack.fastSuperFX = superFXClock.position() * 10 + 100;
    superFXValue.setText({settings.emulator.hack.fastSuperFX, "%"});
  }).doChange();
  ppuDspLabel.setText("PPU (video) / DSP (audio)").setFont(Font().setBold());
  fastPPU.setText("Fast PPU mode [required for HD Mode 7 / bsnes-hd]").setChecked(settings.emulator.hack.ppu.fast).onToggle([&] {
    settings.emulator.hack.ppu.fast = fastPPU.checked();
    if(!fastPPU.checked()) {
      noSpriteLimit.setEnabled(false).setChecked(false).doToggle();
    } else {
      noSpriteLimit.setEnabled(true);
    }
  }).doToggle();
  noSpriteLimit.setText("No sprite limit").setChecked(settings.emulator.hack.ppu.noSpriteLimit).onToggle([&] {
    settings.emulator.hack.ppu.noSpriteLimit = noSpriteLimit.checked();
  });
  fastDSP.setText("Fast DSP mode").setChecked(settings.emulator.hack.dsp.fast).onToggle([&] {
    settings.emulator.hack.dsp.fast = fastDSP.checked();
    emulator->configure("Hacks/DSP/Fast", settings.emulator.hack.dsp.fast);
  });
  cubicInterpolation.setText("Cubic interpolation").setChecked(settings.emulator.hack.dsp.cubic).onToggle([&] {
    settings.emulator.hack.dsp.cubic = cubicInterpolation.checked();
    emulator->configure("Hacks/DSP/Cubic", settings.emulator.hack.dsp.cubic);
  });
  mode7Label.setText("HD Mode 7 / bsnes-hd").setFont(Font().setBold());
  mode7ScaleLabel.setText("Scale:");
  mode7Scale.append(ComboButtonItem().setText("disable").setProperty("multiplier", 0));
  mode7Scale.append(ComboButtonItem().setText("1x  224p").setProperty("multiplier", 1));
  mode7Scale.append(ComboButtonItem().setText("2x  448p").setProperty("multiplier", 2));
  mode7Scale.append(ComboButtonItem().setText("3x  672p").setProperty("multiplier", 3));
  mode7Scale.append(ComboButtonItem().setText("4x  896p").setProperty("multiplier", 4));
  mode7Scale.append(ComboButtonItem().setText("5x 1120p").setProperty("multiplier", 5));
  mode7Scale.append(ComboButtonItem().setText("6x 1344p").setProperty("multiplier", 6));
  mode7Scale.append(ComboButtonItem().setText("7x 1568p").setProperty("multiplier", 7));
  mode7Scale.append(ComboButtonItem().setText("8x 1792p").setProperty("multiplier", 8));
  mode7Scale.append(ComboButtonItem().setText("9x 2016p").setProperty("multiplier", 9));
  for(uint n = 0; n <= 9; n++) {
    if(settings.emulator.hack.ppu.mode7.scale == n) mode7Scale.item(n).setSelected();
  }
  mode7Scale.onChange([&] {
    settings.emulator.hack.ppu.mode7.scale = mode7Scale.selected().property("multiplier").natural();
    emulator->configure("Hacks/PPU/Mode7/Scale", settings.emulator.hack.ppu.mode7.scale);
    presentation.resizeViewport();
  });
  mode7PerspectiveLabel.setText("Perspective correction:");
  mode7Perspective.append(ComboButtonItem().setText("off").setProperty("mode", 0));
  mode7Perspective.append(ComboButtonItem().setText("auto/wide").setProperty("mode", 1));
  mode7Perspective.append(ComboButtonItem().setText("auto/medium").setProperty("mode", 2));
  mode7Perspective.append(ComboButtonItem().setText("auto/narrow").setProperty("mode", 3));
  mode7Perspective.append(ComboButtonItem().setText("on/wide").setProperty("mode", 4));
  mode7Perspective.append(ComboButtonItem().setText("on/medium").setProperty("mode", 5));
  mode7Perspective.append(ComboButtonItem().setText("on/narrow").setProperty("mode", 6));
  for(uint n = 0; n < 7; n++) {
    if(mode7Perspective.item(n).property("mode").natural() == settings.emulator.hack.ppu.mode7.perspective)
       mode7Perspective.item(n).setSelected();
  }
  mode7Perspective.onChange([&] {
    settings.emulator.hack.ppu.mode7.perspective = mode7Perspective.selected().property("mode").natural();
    emulator->configure("Hacks/PPU/Mode7/Perspective", settings.emulator.hack.ppu.mode7.perspective);
  });
  mode7SupersampleLabel.setText("Supersampling:");
  mode7Supersample.append(ComboButtonItem().setText("none").setProperty("sss", 1));
  mode7Supersample.append(ComboButtonItem().setText("2x").setProperty("sss", 2));
  mode7Supersample.append(ComboButtonItem().setText("3x").setProperty("sss", 3));
  mode7Supersample.append(ComboButtonItem().setText("4x").setProperty("sss", 4));
  mode7Supersample.append(ComboButtonItem().setText("5x").setProperty("sss", 5));
  mode7Supersample.append(ComboButtonItem().setText("6x").setProperty("sss", 6));
  mode7Supersample.append(ComboButtonItem().setText("7x").setProperty("sss", 7));
  mode7Supersample.append(ComboButtonItem().setText("8x").setProperty("sss", 8));
  for(uint n = 0; n < 8; n++) {
    if(mode7Supersample.item(n).property("sss").natural() == settings.emulator.hack.ppu.mode7.supersample)
       mode7Supersample.item(n).setSelected();
  }
  mode7Supersample.onChange([&] {
    settings.emulator.hack.ppu.mode7.supersample = mode7Supersample.selected().property("sss").natural();
    emulator->configure("Hacks/PPU/Mode7/Supersample", settings.emulator.hack.ppu.mode7.supersample);
  });
  mode7MosaicLabel.setText("Mosaic:");
  mode7Mosaic.append(ComboButtonItem().setText("non-HD").setProperty("mode", 0));
  mode7Mosaic.append(ComboButtonItem().setText("1x scale").setProperty("mode", 1));
  mode7Mosaic.append(ComboButtonItem().setText("ignore").setProperty("mode", 2));
  for(uint n = 0; n <= 2; n++) {
    if(mode7Mosaic.item(n).property("mode").natural() == settings.emulator.hack.ppu.mode7.mosaic)
       mode7Mosaic.item(n).setSelected();
  }
  mode7Mosaic.onChange([&] {
    settings.emulator.hack.ppu.mode7.mosaic = mode7Mosaic.selected().property("mode").natural();
    emulator->configure("Hacks/PPU/Mode7/Mosaic", settings.emulator.hack.ppu.mode7.mosaic);
  });
  
  mode7WidescreenLabel.setText("Aspect ratio:");
  mode7Widescreen.append(ComboButtonItem().setText("4:3").setProperty("adval",  24));
  mode7Widescreen.append(ComboButtonItem().setText("16:10").setProperty("adval",  56));
  mode7Widescreen.append(ComboButtonItem().setText("HD").setProperty("adval",  64));
  mode7Widescreen.append(ComboButtonItem().setText("16:9").setProperty("adval",  72));
  mode7Widescreen.append(ComboButtonItem().setText("21:10").setProperty("adval", 104));
  mode7Widescreen.append(ComboButtonItem().setText("21:9").setProperty("adval", 136));
  mode7Widescreen.append(ComboButtonItem().setText("8:3").setProperty("adval", 168));
  for(uint n = 0; n < 7; n++) {
    if(mode7Widescreen.item(n).property("adval").natural() == settings.emulator.hack.ppu.mode7.widescreen)
       mode7Widescreen.item(n).setSelected();
  }
  mode7Widescreen.onChange([&] {
    settings.emulator.hack.ppu.mode7.widescreen = mode7Widescreen.selected().property("adval").natural();
    emulator->configure("Hacks/PPU/Mode7/Widescreen", settings.emulator.hack.ppu.mode7.widescreen);
    presentation.resizeViewport();
  });
  wsBG1Label.setText("BG1:");
  wsBG1.append(ComboButtonItem().setText("off").setProperty("wsbgmode",  0));
  wsBG1.append(ComboButtonItem().setText("on").setProperty("wsbgmode",  1));
  wsBG1.append(ComboButtonItem().setText("< 40").setProperty("wsbgmode",  2));
  wsBG1.append(ComboButtonItem().setText("> 40").setProperty("wsbgmode",  3));
  wsBG1.append(ComboButtonItem().setText("< 80").setProperty("wsbgmode",  4));
  wsBG1.append(ComboButtonItem().setText("> 80").setProperty("wsbgmode",  5));
  wsBG1.append(ComboButtonItem().setText("<120").setProperty("wsbgmode",  6));
  wsBG1.append(ComboButtonItem().setText(">120").setProperty("wsbgmode",  7));
  wsBG1.append(ComboButtonItem().setText("<160").setProperty("wsbgmode",  8));
  wsBG1.append(ComboButtonItem().setText(">160").setProperty("wsbgmode",  9));
  wsBG1.append(ComboButtonItem().setText("<200").setProperty("wsbgmode", 10));
  wsBG1.append(ComboButtonItem().setText(">200").setProperty("wsbgmode", 11));
  wsBG1.append(ComboButtonItem().setText("autoHor").setProperty("wsbgmode", 15));
  wsBG1.append(ComboButtonItem().setText("autoHor&Ver").setProperty("wsbgmode", 16));
  wsBG1.append(ComboButtonItem().setText("crop").setProperty("wsbgmode", 12));
  wsBG1.append(ComboButtonItem().setText("cropAuto").setProperty("wsbgmode", 13));
  wsBG1.append(ComboButtonItem().setText("disable").setProperty("wsbgmode", 14));
  for(uint n = 0; n <= 16; n++) {
    if(wsBG1.item(n).property("wsbgmode").natural() == settings.emulator.hack.ppu.mode7.wsbg1)
       wsBG1.item(n).setSelected();
  }
  wsBG1.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg1 = wsBG1.selected().property("wsbgmode").natural();
    emulator->configure("Hacks/PPU/Mode7/Wsbg1", settings.emulator.hack.ppu.mode7.wsbg1);
  });
  wsBG2Label.setText("BG2:");
  wsBG2.append(ComboButtonItem().setText("off").setProperty("wsbgmode",  0));
  wsBG2.append(ComboButtonItem().setText("on").setProperty("wsbgmode",  1));
  wsBG2.append(ComboButtonItem().setText("< 40").setProperty("wsbgmode",  2));
  wsBG2.append(ComboButtonItem().setText("> 40").setProperty("wsbgmode",  3));
  wsBG2.append(ComboButtonItem().setText("< 80").setProperty("wsbgmode",  4));
  wsBG2.append(ComboButtonItem().setText("> 80").setProperty("wsbgmode",  5));
  wsBG2.append(ComboButtonItem().setText("<120").setProperty("wsbgmode",  6));
  wsBG2.append(ComboButtonItem().setText(">120").setProperty("wsbgmode",  7));
  wsBG2.append(ComboButtonItem().setText("<160").setProperty("wsbgmode",  8));
  wsBG2.append(ComboButtonItem().setText(">160").setProperty("wsbgmode",  9));
  wsBG2.append(ComboButtonItem().setText("<200").setProperty("wsbgmode", 10));
  wsBG2.append(ComboButtonItem().setText(">200").setProperty("wsbgmode", 11));
  wsBG2.append(ComboButtonItem().setText("autoHor").setProperty("wsbgmode", 15));
  wsBG2.append(ComboButtonItem().setText("autoHor&Ver").setProperty("wsbgmode", 16));
  wsBG2.append(ComboButtonItem().setText("crop").setProperty("wsbgmode", 12));
  wsBG2.append(ComboButtonItem().setText("cropAuto").setProperty("wsbgmode", 13));
  wsBG2.append(ComboButtonItem().setText("disable").setProperty("wsbgmode", 14));
  for(uint n = 0; n <= 16; n++) {
    if(wsBG2.item(n).property("wsbgmode").natural() == settings.emulator.hack.ppu.mode7.wsbg2)
       wsBG2.item(n).setSelected();
  }
  wsBG2.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg2 = wsBG2.selected().property("wsbgmode").natural();
    emulator->configure("Hacks/PPU/Mode7/Wsbg2", settings.emulator.hack.ppu.mode7.wsbg2);
  });
  wsBG3Label.setText("BG3:");
  wsBG3.append(ComboButtonItem().setText("off").setProperty("wsbgmode",  0));
  wsBG3.append(ComboButtonItem().setText("on").setProperty("wsbgmode",  1));
  wsBG3.append(ComboButtonItem().setText("< 40").setProperty("wsbgmode",  2));
  wsBG3.append(ComboButtonItem().setText("> 40").setProperty("wsbgmode",  3));
  wsBG3.append(ComboButtonItem().setText("< 80").setProperty("wsbgmode",  4));
  wsBG3.append(ComboButtonItem().setText("> 80").setProperty("wsbgmode",  5));
  wsBG3.append(ComboButtonItem().setText("<120").setProperty("wsbgmode",  6));
  wsBG3.append(ComboButtonItem().setText(">120").setProperty("wsbgmode",  7));
  wsBG3.append(ComboButtonItem().setText("<160").setProperty("wsbgmode",  8));
  wsBG3.append(ComboButtonItem().setText(">160").setProperty("wsbgmode",  9));
  wsBG3.append(ComboButtonItem().setText("<200").setProperty("wsbgmode", 10));
  wsBG3.append(ComboButtonItem().setText(">200").setProperty("wsbgmode", 11));
  wsBG3.append(ComboButtonItem().setText("autoHor").setProperty("wsbgmode", 15));
  wsBG3.append(ComboButtonItem().setText("autoHor&Ver").setProperty("wsbgmode", 16));
  wsBG3.append(ComboButtonItem().setText("crop").setProperty("wsbgmode", 12));
  wsBG3.append(ComboButtonItem().setText("cropAuto").setProperty("wsbgmode", 13));
  wsBG3.append(ComboButtonItem().setText("disable").setProperty("wsbgmode", 14));
  for(uint n = 0; n <= 16; n++) {
    if(wsBG3.item(n).property("wsbgmode").natural() == settings.emulator.hack.ppu.mode7.wsbg3)
       wsBG3.item(n).setSelected();
  }
  wsBG3.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg3 = wsBG3.selected().property("wsbgmode").natural();
    emulator->configure("Hacks/PPU/Mode7/Wsbg3", settings.emulator.hack.ppu.mode7.wsbg3);
  });
  wsBG4Label.setText("BG4:");
  wsBG4.append(ComboButtonItem().setText("off").setProperty("wsbgmode",  0));
  wsBG4.append(ComboButtonItem().setText("on").setProperty("wsbgmode",  1));
  wsBG4.append(ComboButtonItem().setText("< 40").setProperty("wsbgmode",  2));
  wsBG4.append(ComboButtonItem().setText("> 40").setProperty("wsbgmode",  3));
  wsBG4.append(ComboButtonItem().setText("< 80").setProperty("wsbgmode",  4));
  wsBG4.append(ComboButtonItem().setText("> 80").setProperty("wsbgmode",  5));
  wsBG4.append(ComboButtonItem().setText("<120").setProperty("wsbgmode",  6));
  wsBG4.append(ComboButtonItem().setText(">120").setProperty("wsbgmode",  7));
  wsBG4.append(ComboButtonItem().setText("<160").setProperty("wsbgmode",  8));
  wsBG4.append(ComboButtonItem().setText(">160").setProperty("wsbgmode",  9));
  wsBG4.append(ComboButtonItem().setText("<200").setProperty("wsbgmode", 10));
  wsBG4.append(ComboButtonItem().setText(">200").setProperty("wsbgmode", 11));
  wsBG4.append(ComboButtonItem().setText("autoHor").setProperty("wsbgmode", 15));
  wsBG4.append(ComboButtonItem().setText("autoHor&Ver").setProperty("wsbgmode", 16));
  wsBG4.append(ComboButtonItem().setText("crop").setProperty("wsbgmode", 12));
  wsBG4.append(ComboButtonItem().setText("cropAuto").setProperty("wsbgmode", 13));
  wsBG4.append(ComboButtonItem().setText("disable").setProperty("wsbgmode", 14));
  for(uint n = 0; n <= 16; n++) {
    if(wsBG4.item(n).property("wsbgmode").natural() == settings.emulator.hack.ppu.mode7.wsbg4)
       wsBG4.item(n).setSelected();
  }
  wsBG4.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg4 = wsBG4.selected().property("wsbgmode").natural();
    emulator->configure("Hacks/PPU/Mode7/Wsbg4", settings.emulator.hack.ppu.mode7.wsbg4);
  });
  wsObjLabel.setText("Sprites:");
  wsObj.append(ComboButtonItem().setText("clip").setProperty("mode", 3));
  wsObj.append(ComboButtonItem().setText("safe").setProperty("mode", 0));
  wsObj.append(ComboButtonItem().setText("unsafe").setProperty("mode", 1));
  wsObj.append(ComboButtonItem().setText("disable").setProperty("mode", 2));
  for(uint n = 0; n <= 3; n++) {
    if(wsObj.item(n).property("mode").natural() == settings.emulator.hack.ppu.mode7.wsobj)
       wsObj.item(n).setSelected();
  }
  wsObj.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsobj = wsObj.selected().property("mode").natural();
    emulator->configure("Hacks/PPU/Mode7/Wsobj", settings.emulator.hack.ppu.mode7.wsobj);
  });
  igwinLabel.setText("Ignore window:");
  igwin.append(ComboButtonItem().setText("none").setProperty("mode", 0));
  igwin.append(ComboButtonItem().setText("outside").setProperty("mode", 1));
  igwin.append(ComboButtonItem().setText("outside&always").setProperty("mode", 2));
  igwin.append(ComboButtonItem().setText("all").setProperty("mode", 3));
  for(uint n = 0; n < 4; n++) {
    if(igwin.item(n).property("mode").natural() == settings.emulator.hack.ppu.mode7.igwin)
       igwin.item(n).setSelected();
  }
  igwin.onChange([&] {
    settings.emulator.hack.ppu.mode7.igwin = igwin.selected().property("mode").natural();
    emulator->configure("Hacks/PPU/Mode7/Igwin", settings.emulator.hack.ppu.mode7.igwin);
  });
  igwinxLabel.setText("IW fallback x-coordinate:");
  igwinx.append(ComboButtonItem().setText(" 40").setProperty("col",  40));
  igwinx.append(ComboButtonItem().setText(" 88").setProperty("col",  88));
  igwinx.append(ComboButtonItem().setText("128").setProperty("col", 128));
  igwinx.append(ComboButtonItem().setText("168").setProperty("col", 168));
  igwinx.append(ComboButtonItem().setText("216").setProperty("col", 216));
  for(uint n = 0; n < 5; n++) {
    if(igwinx.item(n).property("col").natural() == settings.emulator.hack.ppu.mode7.igwinx)
       igwinx.item(n).setSelected();
  }
  igwinx.onChange([&] {
    settings.emulator.hack.ppu.mode7.igwinx = igwinx.selected().property("col").natural();
    emulator->configure("Hacks/PPU/Mode7/Igwinx", settings.emulator.hack.ppu.mode7.igwinx);
  });

  wsBgColLabel.setText("WS bg:");
  wsBgCol.append(ComboButtonItem().setText("color").setProperty("mode", 0));
  wsBgCol.append(ComboButtonItem().setText("auto").setProperty("mode", 1));
  wsBgCol.append(ComboButtonItem().setText("black").setProperty("mode", 2));
  for(uint n = 0; n <= 2; n++) {
    if(wsBgCol.item(n).property("mode").natural() == settings.emulator.hack.ppu.mode7.wsBgCol)
       wsBgCol.item(n).setSelected();
  }
  wsBgCol.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsBgCol = wsBgCol.selected().property("mode").natural();
    emulator->configure("Hacks/PPU/Mode7/WsBgCol", settings.emulator.hack.ppu.mode7.wsBgCol);
  });

  wsModeLabel.setText("Widescreen:");
  wsMode.append(ComboButtonItem().setText("none").setProperty("mode", 0));
  wsMode.append(ComboButtonItem().setText("Mode 7").setProperty("mode", 1));
  wsMode.append(ComboButtonItem().setText("all").setProperty("mode", 2));
  for(uint n = 0; n < 3; n++) {
    if(wsMode.item(n).property("mode").natural() == settings.emulator.hack.ppu.mode7.wsMode)
       wsMode.item(n).setSelected();
  }
  wsMode.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsMode = wsMode.selected().property("mode").natural();
    emulator->configure("Hacks/PPU/Mode7/WsMode", settings.emulator.hack.ppu.mode7.wsMode);
    presentation.resizeViewport();
  });

  unintrModeLabel.setText("Soft crop:");
  unintrMode.append(ComboButtonItem().setText(" none ").setProperty("mode", 0));
  unintrMode.append(ComboButtonItem().setText("center").setProperty("mode", 1));
  unintrMode.append(ComboButtonItem().setText(" scale").setProperty("mode", 2));
  for(uint n = 0; n < 3; n++) {
    if(unintrMode.item(n).property("mode").natural() == settings.emulator.hack.ppu.mode7.unintrMode)
       unintrMode.item(n).setSelected();
  }
  unintrMode.onChange([&] {
    settings.emulator.hack.ppu.mode7.unintrMode = unintrMode.selected().property("mode").natural();
    emulator->configure("Hacks/PPU/Mode7/UnintrMode", settings.emulator.hack.ppu.mode7.unintrMode);
    presentation.resizeViewport();
  });

  unintrTopLabel.setText("SC top:");
  unintrTop.append(ComboButtonItem().setText("  0").setProperty("marg",   0));
  unintrTop.append(ComboButtonItem().setText(" 10").setProperty("marg",  10));
  unintrTop.append(ComboButtonItem().setText(" 20").setProperty("marg",  20));
  unintrTop.append(ComboButtonItem().setText(" 30").setProperty("marg",  30));
  unintrTop.append(ComboButtonItem().setText(" 40").setProperty("marg",  40));
  unintrTop.append(ComboButtonItem().setText(" 50").setProperty("marg",  50));
  unintrTop.append(ComboButtonItem().setText(" 60").setProperty("marg",  60));
  unintrTop.append(ComboButtonItem().setText(" 70").setProperty("marg",  70));
  unintrTop.append(ComboButtonItem().setText(" 80").setProperty("marg",  80));
  unintrTop.append(ComboButtonItem().setText(" 90").setProperty("marg",  90));
  unintrTop.append(ComboButtonItem().setText("100").setProperty("marg", 100));
  unintrTop.append(ComboButtonItem().setText("110").setProperty("marg", 110));
  unintrTop.append(ComboButtonItem().setText("120").setProperty("marg", 120));
  unintrTop.append(ComboButtonItem().setText("130").setProperty("marg", 130));
  unintrTop.append(ComboButtonItem().setText("140").setProperty("marg", 140));
  unintrTop.append(ComboButtonItem().setText("150").setProperty("marg", 150));
  for(uint n = 0; n < 17; n++) {
    if(unintrTop.item(n).property("marg").natural() == settings.emulator.hack.ppu.mode7.unintrTop)
       unintrTop.item(n).setSelected();
  }
  unintrTop.onChange([&] {
    settings.emulator.hack.ppu.mode7.unintrTop = unintrTop.selected().property("marg").natural();
    emulator->configure("Hacks/PPU/Mode7/UnintrTop", settings.emulator.hack.ppu.mode7.unintrTop);
    presentation.resizeViewport();
  });

  unintrBottomLabel.setText("SC bottom:");
  unintrBottom.append(ComboButtonItem().setText("  0").setProperty("marg",   0));
  unintrBottom.append(ComboButtonItem().setText(" 10").setProperty("marg",  10));
  unintrBottom.append(ComboButtonItem().setText(" 20").setProperty("marg",  20));
  unintrBottom.append(ComboButtonItem().setText(" 30").setProperty("marg",  30));
  unintrBottom.append(ComboButtonItem().setText(" 40").setProperty("marg",  40));
  unintrBottom.append(ComboButtonItem().setText(" 50").setProperty("marg",  50));
  unintrBottom.append(ComboButtonItem().setText(" 60").setProperty("marg",  60));
  unintrBottom.append(ComboButtonItem().setText(" 70").setProperty("marg",  70));
  unintrBottom.append(ComboButtonItem().setText(" 80").setProperty("marg",  80));
  unintrBottom.append(ComboButtonItem().setText(" 90").setProperty("marg",  90));
  unintrBottom.append(ComboButtonItem().setText("100").setProperty("marg", 100));
  unintrBottom.append(ComboButtonItem().setText("110").setProperty("marg", 110));
  unintrBottom.append(ComboButtonItem().setText("120").setProperty("marg", 120));
  unintrBottom.append(ComboButtonItem().setText("130").setProperty("marg", 130));
  unintrBottom.append(ComboButtonItem().setText("140").setProperty("marg", 140));
  unintrBottom.append(ComboButtonItem().setText("150").setProperty("marg", 150));
  for(uint n = 0; n < 17; n++) {
    if(unintrBottom.item(n).property("marg").natural() == settings.emulator.hack.ppu.mode7.unintrBottom)
       unintrBottom.item(n).setSelected();
  }
  unintrBottom.onChange([&] {
    settings.emulator.hack.ppu.mode7.unintrBottom = unintrBottom.selected().property("marg").natural();
    emulator->configure("Hacks/PPU/Mode7/UnintrBottom", settings.emulator.hack.ppu.mode7.unintrBottom);
    presentation.resizeViewport();
  });

  unintrLeftLabel.setText("SC left:");
  unintrLeft.append(ComboButtonItem().setText("  0").setProperty("marg",   0));
  unintrLeft.append(ComboButtonItem().setText(" 10").setProperty("marg",  10));
  unintrLeft.append(ComboButtonItem().setText(" 20").setProperty("marg",  20));
  unintrLeft.append(ComboButtonItem().setText(" 30").setProperty("marg",  30));
  unintrLeft.append(ComboButtonItem().setText(" 40").setProperty("marg",  40));
  unintrLeft.append(ComboButtonItem().setText(" 50").setProperty("marg",  50));
  unintrLeft.append(ComboButtonItem().setText(" 60").setProperty("marg",  60));
  unintrLeft.append(ComboButtonItem().setText(" 70").setProperty("marg",  70));
  unintrLeft.append(ComboButtonItem().setText(" 80").setProperty("marg",  80));
  unintrLeft.append(ComboButtonItem().setText(" 90").setProperty("marg",  90));
  unintrLeft.append(ComboButtonItem().setText("100").setProperty("marg", 100));
  unintrLeft.append(ComboButtonItem().setText("110").setProperty("marg", 110));
  unintrLeft.append(ComboButtonItem().setText("120").setProperty("marg", 120));
  unintrLeft.append(ComboButtonItem().setText("130").setProperty("marg", 130));
  unintrLeft.append(ComboButtonItem().setText("140").setProperty("marg", 140));
  unintrLeft.append(ComboButtonItem().setText("150").setProperty("marg", 150));
  for(uint n = 0; n < 17; n++) {
    if(unintrLeft.item(n).property("marg").natural() == settings.emulator.hack.ppu.mode7.unintrLeft)
       unintrLeft.item(n).setSelected();
  }
  unintrLeft.onChange([&] {
    settings.emulator.hack.ppu.mode7.unintrLeft = unintrLeft.selected().property("marg").natural();
    emulator->configure("Hacks/PPU/Mode7/UnintrLeft", settings.emulator.hack.ppu.mode7.unintrLeft);
    presentation.resizeViewport();
  });

  unintrRightLabel.setText("SC right:");
  unintrRight.append(ComboButtonItem().setText("  0").setProperty("marg",   0));
  unintrRight.append(ComboButtonItem().setText(" 10").setProperty("marg",  10));
  unintrRight.append(ComboButtonItem().setText(" 20").setProperty("marg",  20));
  unintrRight.append(ComboButtonItem().setText(" 30").setProperty("marg",  30));
  unintrRight.append(ComboButtonItem().setText(" 40").setProperty("marg",  40));
  unintrRight.append(ComboButtonItem().setText(" 50").setProperty("marg",  50));
  unintrRight.append(ComboButtonItem().setText(" 60").setProperty("marg",  60));
  unintrRight.append(ComboButtonItem().setText(" 70").setProperty("marg",  70));
  unintrRight.append(ComboButtonItem().setText(" 80").setProperty("marg",  80));
  unintrRight.append(ComboButtonItem().setText(" 90").setProperty("marg",  90));
  unintrRight.append(ComboButtonItem().setText("100").setProperty("marg", 100));
  unintrRight.append(ComboButtonItem().setText("110").setProperty("marg", 110));
  unintrRight.append(ComboButtonItem().setText("120").setProperty("marg", 120));
  unintrRight.append(ComboButtonItem().setText("130").setProperty("marg", 130));
  unintrRight.append(ComboButtonItem().setText("140").setProperty("marg", 140));
  unintrRight.append(ComboButtonItem().setText("150").setProperty("marg", 150));
  for(uint n = 0; n < 17; n++) {
    if(unintrRight.item(n).property("marg").natural() == settings.emulator.hack.ppu.mode7.unintrRight)
       unintrRight.item(n).setSelected();
  }
  unintrRight.onChange([&] {
    settings.emulator.hack.ppu.mode7.unintrRight = unintrRight.selected().property("marg").natural();
    emulator->configure("Hacks/PPU/Mode7/UnintrRight", settings.emulator.hack.ppu.mode7.unintrRight);
    presentation.resizeViewport();
  });


  wsMarkerLabel.setText("WS marker:");
  wsMarker.append(ComboButtonItem().setText("none").setProperty("mode", 0));
  wsMarker.append(ComboButtonItem().setText("lines").setProperty("mode", 1));
  wsMarker.append(ComboButtonItem().setText("darken").setProperty("mode", 2));
  for(uint n = 0; n <= 2; n++) {
    if(wsMarker.item(n).property("mode").natural() == settings.emulator.hack.ppu.mode7.wsMarker)
       wsMarker.item(n).setSelected();
  }
  wsMarker.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsMarker = wsMarker.selected().property("mode").natural();
    emulator->configure("Hacks/PPU/Mode7/WsMarker", settings.emulator.hack.ppu.mode7.wsMarker);
  });


  wsMarkerAlphaLabel.setText("WSM alpha:");
  wsMarkerAlpha.append(ComboButtonItem().setText("1/1" ).setProperty("alpha",  1));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/2" ).setProperty("alpha",  2));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/3" ).setProperty("alpha",  3));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/4" ).setProperty("alpha",  4));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/5" ).setProperty("alpha",  5));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/6" ).setProperty("alpha",  6));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/7" ).setProperty("alpha",  7));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/8" ).setProperty("alpha",  8));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/9" ).setProperty("alpha",  9));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/10").setProperty("alpha", 10));
  for(uint n = 0; n < 10; n++) {
    if(wsMarkerAlpha.item(n).property("alpha").natural() == settings.emulator.hack.ppu.mode7.wsMarkerAlpha)
       wsMarkerAlpha.item(n).setSelected();
  }
  wsMarkerAlpha.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsMarkerAlpha = wsMarkerAlpha.selected().property("alpha").natural();
    emulator->configure("Hacks/PPU/Mode7/WsMarkerAlpha", settings.emulator.hack.ppu.mode7.wsMarkerAlpha);
  });


}

auto EmulatorSettings::updateConfiguration() -> void {
  emulator->configure("Hacks/PPU/Fast", fastPPU.checked());
  emulator->configure("Hacks/PPU/NoSpriteLimit", noSpriteLimit.checked());
  emulator->configure("Hacks/PPU/Mode7/Scale", mode7Scale.selected().property("multiplier").natural());
  emulator->configure("Hacks/PPU/Mode7/Perspective", mode7Perspective.property("mode").natural());
  emulator->configure("Hacks/PPU/Mode7/Widescreen", mode7Widescreen.property("addval").natural());
  emulator->configure("Hacks/PPU/Mode7/Wsbg1", wsBG1.property("wsbgmode").natural());
  emulator->configure("Hacks/PPU/Mode7/Wsbg2", wsBG2.property("wsbgmode").natural());
  emulator->configure("Hacks/PPU/Mode7/Wsbg3", wsBG3.property("wsbgmode").natural());
  emulator->configure("Hacks/PPU/Mode7/Wsbg4", wsBG4.property("wsbgmode").natural()); 
  emulator->configure("Hacks/PPU/Mode7/Wsobj", wsObj.property("mode").natural());
  emulator->configure("Hacks/PPU/Mode7/Igwin", igwin.property("mode").natural());
  emulator->configure("Hacks/PPU/Mode7/Igwinx", igwin.property("col").natural());
  emulator->configure("Hacks/PPU/Mode7/WsMode", wsMode.property("mode").natural());
  emulator->configure("Hacks/PPU/Mode7/WsBgCol", wsBgCol.property("mode").natural());
  emulator->configure("Hacks/PPU/Mode7/UnintrMode", igwin.property("mode").natural());
  emulator->configure("Hacks/PPU/Mode7/UnintrTop", igwin.property("marg").natural());
  emulator->configure("Hacks/PPU/Mode7/UnintrBottom", igwin.property("marg").natural());
  emulator->configure("Hacks/PPU/Mode7/UnintrLeft", igwin.property("marg").natural());
  emulator->configure("Hacks/PPU/Mode7/WsMarker", wsMarker.property("mode").natural());
  emulator->configure("Hacks/PPU/Mode7/WsMarkerAlpha", wsMarkerAlpha.property("alpha").natural());
  emulator->configure("Hacks/PPU/Mode7/UnintrRight", igwin.property("marg").natural());
  emulator->configure("Hacks/PPU/Mode7/Supersample", mode7Supersample.property("sss").natural());
  emulator->configure("Hacks/PPU/Mode7/Mosaic", mode7Mosaic.property("mode").natural());
  emulator->configure("Hacks/DSP/Fast", fastDSP.checked());
  emulator->configure("Hacks/DSP/Cubic", cubicInterpolation.checked());
  emulator->configure("Hacks/Coprocessor/DelayedSync", coprocessorsDelayedSyncOption.checked());
  emulator->configure("Hacks/Coprocessor/HLE", coprocessorsHLEOption.checked());
}

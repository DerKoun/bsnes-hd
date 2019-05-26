auto EmulatorSettings::create() -> void {
  setIcon(Icon::Action::Settings);
  setText("Emulator");

  layout.setPadding(5_sx);

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
  optionsSpacer.setColor({192, 192, 192});

  ppuLabel.setText("PPU (video)").setFont(Font().setBold());
  fastPPU.setText("Fast mode").setChecked(settings.emulator.hack.ppu.fast).onToggle([&] {
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
  mode7Label.setText("HD Mode 7 (fast PPU only)").setFont(Font().setBold());
  mode7ScaleLabel.setText("Scale:");
  mode7Scale.append(ComboButtonItem().setText( "240p").setProperty("multiplier", 1));
  mode7Scale.append(ComboButtonItem().setText( "480p").setProperty("multiplier", 2));
  mode7Scale.append(ComboButtonItem().setText( "720p").setProperty("multiplier", 3));
  mode7Scale.append(ComboButtonItem().setText( "960p").setProperty("multiplier", 4));
  mode7Scale.append(ComboButtonItem().setText("1200p").setProperty("multiplier", 5));
  mode7Scale.append(ComboButtonItem().setText("1440p").setProperty("multiplier", 6));
  mode7Scale.append(ComboButtonItem().setText("1680p").setProperty("multiplier", 7));
  mode7Scale.append(ComboButtonItem().setText("1920p").setProperty("multiplier", 8));
  mode7Scale.append(ComboButtonItem().setText("2160p").setProperty("multiplier", 9));
  for(uint n = 1; n <= 9; n++) {
    if(settings.emulator.hack.ppu.mode7.scale == n) mode7Scale.item(n - 1).setSelected();
  }
  mode7Scale.onChange([&] {
    settings.emulator.hack.ppu.mode7.scale = mode7Scale.selected().property("multiplier").natural();
    emulator->configure("Hacks/PPU/Mode7/Scale", settings.emulator.hack.ppu.mode7.scale);
    presentation.resizeViewport();
  });
  mode7Perspective.setText("Perspective correction").setChecked(settings.emulator.hack.ppu.mode7.perspective).onToggle([&] {
    settings.emulator.hack.ppu.mode7.perspective = mode7Perspective.checked();
    emulator->configure("Hacks/PPU/Mode7/Perspective", settings.emulator.hack.ppu.mode7.perspective);
  });
  mode7Mosaic.setText("HD->SD Mosaic").setChecked(settings.emulator.hack.ppu.mode7.mosaic).onToggle([&] {
    settings.emulator.hack.ppu.mode7.mosaic = mode7Mosaic.checked();
    emulator->configure("Hacks/PPU/Mode7/Mosaic", settings.emulator.hack.ppu.mode7.mosaic);
  });
  mode7Supersample.setText("Supersample").setChecked(settings.emulator.hack.ppu.mode7.supersample).onToggle([&] {
    settings.emulator.hack.ppu.mode7.supersample = mode7Supersample.checked();
    emulator->configure("Hacks/PPU/Mode7/Supersample", settings.emulator.hack.ppu.mode7.supersample);
  });
  mode7WidescreenLabel.setText("Widescreen:");
  mode7Widescreen.append(ComboButtonItem().setText("none ").setProperty("adval",   0));
  mode7Widescreen.append(ComboButtonItem().setText("16:10").setProperty("adval",  40));
  mode7Widescreen.append(ComboButtonItem().setText("16:9 ").setProperty("adval",  64));
  mode7Widescreen.append(ComboButtonItem().setText(" 1.93").setProperty("adval",  80));
  mode7Widescreen.append(ComboButtonItem().setText(" 2.15").setProperty("adval", 104));
  mode7Widescreen.append(ComboButtonItem().setText("21:9 ").setProperty("adval", 120));
  mode7Widescreen.append(ComboButtonItem().setText(" 2.44").setProperty("adval", 136));
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
  wsBG1.append(ComboButtonItem().setText(" off").setProperty("wsbgmode",  0));
  wsBG1.append(ComboButtonItem().setText(" on ").setProperty("wsbgmode",  1));
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
  for(uint n = 0; n < 12; n++) {
    if(wsBG1.item(n).property("wsbgmode").natural() == settings.emulator.hack.ppu.mode7.wsbg1)
       wsBG1.item(n).setSelected();
  }
  wsBG1.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg1 = wsBG1.selected().property("wsbgmode").natural();
    emulator->configure("Hacks/PPU/Mode7/Wsbg1", settings.emulator.hack.ppu.mode7.wsbg1);
  });
  wsBG2Label.setText("BG2:");
  wsBG2.append(ComboButtonItem().setText(" off").setProperty("wsbgmode",  0));
  wsBG2.append(ComboButtonItem().setText(" on ").setProperty("wsbgmode",  1));
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
  for(uint n = 0; n < 12; n++) {
    if(wsBG2.item(n).property("wsbgmode").natural() == settings.emulator.hack.ppu.mode7.wsbg2)
       wsBG2.item(n).setSelected();
  }
  wsBG2.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg2 = wsBG2.selected().property("wsbgmode").natural();
    emulator->configure("Hacks/PPU/Mode7/Wsbg2", settings.emulator.hack.ppu.mode7.wsbg2);
  });
  wsBG3Label.setText("BG3:");
  wsBG3.append(ComboButtonItem().setText(" off").setProperty("wsbgmode",  0));
  wsBG3.append(ComboButtonItem().setText(" on ").setProperty("wsbgmode",  1));
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
  for(uint n = 0; n < 12; n++) {
    if(wsBG3.item(n).property("wsbgmode").natural() == settings.emulator.hack.ppu.mode7.wsbg3)
       wsBG3.item(n).setSelected();
  }
  wsBG3.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg3 = wsBG3.selected().property("wsbgmode").natural();
    emulator->configure("Hacks/PPU/Mode7/Wsbg3", settings.emulator.hack.ppu.mode7.wsbg3);
  });
  wsBG4Label.setText("BG4:");
  wsBG4.append(ComboButtonItem().setText(" off").setProperty("wsbgmode",  0));
  wsBG4.append(ComboButtonItem().setText(" on ").setProperty("wsbgmode",  1));
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
  for(uint n = 0; n < 12; n++) {
    if(wsBG4.item(n).property("wsbgmode").natural() == settings.emulator.hack.ppu.mode7.wsbg4)
       wsBG4.item(n).setSelected();
  }
  wsBG4.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg4 = wsBG4.selected().property("wsbgmode").natural();
    emulator->configure("Hacks/PPU/Mode7/Wsbg4", settings.emulator.hack.ppu.mode7.wsbg4);
  });
  wsObj.setText("unsafe sprites").setChecked(settings.emulator.hack.ppu.mode7.wsobj).onToggle([&] {
    settings.emulator.hack.ppu.mode7.wsobj = wsObj.checked();
    emulator->configure("Hacks/PPU/Mode7/Wsobj", settings.emulator.hack.ppu.mode7.wsobj);
  });
  dspLabel.setText("DSP (audio)").setFont(Font().setBold());
  fastDSP.setText("Fast mode").setChecked(settings.emulator.hack.dsp.fast).onToggle([&] {
    settings.emulator.hack.dsp.fast = fastDSP.checked();
    emulator->configure("Hacks/DSP/Fast", settings.emulator.hack.dsp.fast);
  });
  cubicInterpolation.setText("Cubic interpolation").setChecked(settings.emulator.hack.dsp.cubic).onToggle([&] {
    settings.emulator.hack.dsp.cubic = cubicInterpolation.checked();
    emulator->configure("Hacks/DSP/Cubic", settings.emulator.hack.dsp.cubic);
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
  hacksNote.setForegroundColor({224, 0, 0}).setText("Note: some hack setting changes do not take effect until after reloading games.");
}

auto EmulatorSettings::updateConfiguration() -> void {
  emulator->configure("Hacks/PPU/Fast", fastPPU.checked());
  emulator->configure("Hacks/PPU/NoSpriteLimit", noSpriteLimit.checked());
  emulator->configure("Hacks/PPU/Mode7/Scale", mode7Scale.selected().property("multiplier").natural());
  emulator->configure("Hacks/PPU/Mode7/Perspective", mode7Perspective.checked());
  emulator->configure("Hacks/PPU/Mode7/Widescreen", mode7Widescreen.property("addval").natural());
  emulator->configure("Hacks/PPU/Mode7/Wsbg1", wsBG1.property("wsbgmode").natural());
  emulator->configure("Hacks/PPU/Mode7/Wsbg2", wsBG2.property("wsbgmode").natural());
  emulator->configure("Hacks/PPU/Mode7/Wsbg3", wsBG3.property("wsbgmode").natural());
  emulator->configure("Hacks/PPU/Mode7/Wsbg4", wsBG4.property("wsbgmode").natural()); 
  emulator->configure("Hacks/PPU/Mode7/Wsobj", wsObj.checked());
  emulator->configure("Hacks/PPU/Mode7/Supersample", mode7Supersample.checked());
  emulator->configure("Hacks/PPU/Mode7/Mosaic", mode7Mosaic.checked());
  emulator->configure("Hacks/DSP/Fast", fastDSP.checked());
  emulator->configure("Hacks/DSP/Cubic", cubicInterpolation.checked());
  emulator->configure("Hacks/Coprocessor/DelayedSync", coprocessorsDelayedSyncOption.checked());
  emulator->configure("Hacks/Coprocessor/HLE", coprocessorsHLEOption.checked());
}

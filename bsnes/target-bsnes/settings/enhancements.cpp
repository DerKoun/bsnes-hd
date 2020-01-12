auto EnhancementSettings::create() -> void {
  setCollapsible();
  setVisible(false);

  runAheadLabel.setText("Run-Ahead").setFont(Font().setBold());
  runAhead0.setText("Disabled").onActivate([&] {
    settings.emulator.runAhead.frames = 0;
  });
  runAhead1.setText("One Frame").onActivate([&] {
    settings.emulator.runAhead.frames = 1;
  });
  runAhead2.setText("Two Frames").onActivate([&] {
    settings.emulator.runAhead.frames = 2;
  });
  runAhead3.setText("Three Frames").onActivate([&] {
    settings.emulator.runAhead.frames = 3;
  });
  runAhead4.setText("Four Frames").onActivate([&] {
    settings.emulator.runAhead.frames = 4;
  });
  if(settings.emulator.runAhead.frames == 0) runAhead0.setChecked();
  if(settings.emulator.runAhead.frames == 1) runAhead1.setChecked();
  if(settings.emulator.runAhead.frames == 2) runAhead2.setChecked();
  if(settings.emulator.runAhead.frames == 3) runAhead3.setChecked();
  if(settings.emulator.runAhead.frames == 4) runAhead4.setChecked();
  runAheadSpacer.setColor({192, 192, 192});

  overclockingLabel.setText("Overclocking").setFont(Font().setBold());
  overclockingLayout.setSize({3, 3});
  overclockingLayout.column(0).setAlignment(1.0);
  overclockingLayout.column(1).setAlignment(0.5);

  cpuLabel.setText("CPU:");
  cpuClock.setLength(301).setPosition((settings.emulator.hack.cpu.overclock - 100)).onChange([&] {
    settings.emulator.hack.cpu.overclock = cpuClock.position() + 100;
    emulator->configure("Hacks/CPU/Overclock", settings.emulator.hack.cpu.overclock);
    cpuValue.setText({settings.emulator.hack.cpu.overclock, "%"});
  }).doChange();

  sa1Label.setText("SA-1:");
  sa1Clock.setLength(301).setPosition((settings.emulator.hack.sa1.overclock - 100)).onChange([&] {
    settings.emulator.hack.sa1.overclock = sa1Clock.position() + 100;
    emulator->configure("Hacks/SA1/Overclock", settings.emulator.hack.sa1.overclock);
    sa1Value.setText({settings.emulator.hack.sa1.overclock, "%"});
  }).doChange();

  sfxLabel.setText("SuperFX:");
  sfxClock.setLength(141).setPosition((settings.emulator.hack.superfx.overclock - 100) / 5).onChange([&] {
    settings.emulator.hack.superfx.overclock = sfxClock.position() * 5 + 100;
    emulator->configure("Hacks/SuperFX/Overclock", settings.emulator.hack.superfx.overclock);
    sfxValue.setText({settings.emulator.hack.superfx.overclock, "%"});
  }).doChange();

  overclockingSpacer.setColor({192, 192, 192});

  ppuLabel.setText("PPU (video)").setFont(Font().setBold());
  fastPPU.setText("Fast mode").setChecked(settings.emulator.hack.ppu.fast).onToggle([&] {
    settings.emulator.hack.ppu.fast = fastPPU.checked();
    if(!fastPPU.checked()) {
      noSpriteLimit.setEnabled(false);
      deinterlace.setEnabled(false);
      //mode7Layout.setEnabled(false);
    } else {
      noSpriteLimit.setEnabled(true);
      deinterlace.setEnabled(true);
      //mode7Layout.setEnabled(true);
    }
  }).doToggle();
  deinterlace.setText("Deinterlace").setChecked(settings.emulator.hack.ppu.deinterlace).onToggle([&] {
    settings.emulator.hack.ppu.deinterlace = deinterlace.checked();
    emulator->configure("Hacks/PPU/Deinterlace", settings.emulator.hack.ppu.deinterlace);
  });
  noSpriteLimit.setText("No sprite limit").setChecked(settings.emulator.hack.ppu.noSpriteLimit).onToggle([&] {
    settings.emulator.hack.ppu.noSpriteLimit = noSpriteLimit.checked();
  });


  mode7Label.setText("HD Mode 7 / bsnes-hd").setFont(Font().setBold());
  mode7ScaleLabel.setText("Scale:");
  mode7Scale.append(ComboButtonItem().setText("disable"));
  mode7Scale.append(ComboButtonItem().setText("1x  224p"));
  mode7Scale.append(ComboButtonItem().setText("2x  448p"));
  mode7Scale.append(ComboButtonItem().setText("3x  672p"));
  mode7Scale.append(ComboButtonItem().setText("4x  896p"));
  mode7Scale.append(ComboButtonItem().setText("5x 1120p"));
  mode7Scale.append(ComboButtonItem().setText("6x 1344p"));
  mode7Scale.append(ComboButtonItem().setText("7x 1568p"));
  mode7Scale.append(ComboButtonItem().setText("8x 1792p"));
  mode7Scale.append(ComboButtonItem().setText("9x 2016p"));
  mode7Scale.append(ComboButtonItem().setText("10x 2240p"));
  for(uint n = 0; n <= 10; n++) {
    if(settings.emulator.hack.ppu.mode7.scale == n) mode7Scale.item(n).setSelected();
  }
  mode7Scale.onChange([&] {
    settings.emulator.hack.ppu.mode7.scale = mode7Scale.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/Scale", settings.emulator.hack.ppu.mode7.scale);
    //.resizeViewport();
  });
  mode7PerspectiveLabel.setText("Perspective correction:");
  mode7Perspective.append(ComboButtonItem().setText("off"));
  mode7Perspective.append(ComboButtonItem().setText("auto/wide"));
  mode7Perspective.append(ComboButtonItem().setText("auto/medium"));
  mode7Perspective.append(ComboButtonItem().setText("auto/narrow"));
  mode7Perspective.append(ComboButtonItem().setText("on/wide"));
  mode7Perspective.append(ComboButtonItem().setText("on/medium"));
  mode7Perspective.append(ComboButtonItem().setText("on/narrow"));
  for(uint n = 0; n < 7; n++) {
    if(mode7Perspective.item(n).offset() == settings.emulator.hack.ppu.mode7.perspective)
       mode7Perspective.item(n).setSelected();
  }
  mode7Perspective.onChange([&] {
    settings.emulator.hack.ppu.mode7.perspective = mode7Perspective.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/Perspective", settings.emulator.hack.ppu.mode7.perspective);
  });
  mode7SupersampleLabel.setText("Supersampling:");
  mode7Supersample.append(ComboButtonItem().setText("none"));
  mode7Supersample.append(ComboButtonItem().setText("2x"));
  mode7Supersample.append(ComboButtonItem().setText("3x"));
  mode7Supersample.append(ComboButtonItem().setText("4x"));
  mode7Supersample.append(ComboButtonItem().setText("5x"));
  mode7Supersample.append(ComboButtonItem().setText("6x"));
  mode7Supersample.append(ComboButtonItem().setText("7x"));
  mode7Supersample.append(ComboButtonItem().setText("8x"));
  mode7Supersample.append(ComboButtonItem().setText("9x"));
  mode7Supersample.append(ComboButtonItem().setText("10x"));
  for(uint n = 0; n < 10; n++) {
    if(mode7Supersample.item(n).offset() + 1 == settings.emulator.hack.ppu.mode7.supersample)
       mode7Supersample.item(n).setSelected();
  }
  mode7Supersample.onChange([&] {
    settings.emulator.hack.ppu.mode7.supersample = mode7Supersample.selected().offset() + 1;
    emulator->configure("Hacks/PPU/Mode7/Supersample", settings.emulator.hack.ppu.mode7.supersample);
  });
  mode7MosaicLabel.setText("Mosaic:");
  mode7Mosaic.append(ComboButtonItem().setText("non-HD"));
  mode7Mosaic.append(ComboButtonItem().setText("1x scale"));
  mode7Mosaic.append(ComboButtonItem().setText("ignore"));
  for(uint n = 0; n <= 2; n++) {
    if(mode7Mosaic.item(n).offset() == settings.emulator.hack.ppu.mode7.mosaic)
       mode7Mosaic.item(n).setSelected();
  }
  mode7Mosaic.onChange([&] {
    settings.emulator.hack.ppu.mode7.mosaic = mode7Mosaic.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/Mosaic", settings.emulator.hack.ppu.mode7.mosaic);
  });
  
  mode7WidescreenLabel.setText("Aspect ratio:");
  mode7Widescreen.append(ComboButtonItem().setText("none"));
  mode7Widescreen.append(ComboButtonItem().setText( "4:3"));
  mode7Widescreen.append(ComboButtonItem().setText("16:10"));
  mode7Widescreen.append(ComboButtonItem().setText("16:9"));
  mode7Widescreen.append(ComboButtonItem().setText( "2:1"));
  mode7Widescreen.append(ComboButtonItem().setText("21:9"));
  if (8 > settings.emulator.hack.ppu.mode7.widescreen)        mode7Widescreen.item(0).setSelected();
  else if ( 28 > settings.emulator.hack.ppu.mode7.widescreen) mode7Widescreen.item(1).setSelected();
  else if ( 52 > settings.emulator.hack.ppu.mode7.widescreen) mode7Widescreen.item(2).setSelected();
  else if ( 76 > settings.emulator.hack.ppu.mode7.widescreen) mode7Widescreen.item(3).setSelected();
  else if (104 > settings.emulator.hack.ppu.mode7.widescreen) mode7Widescreen.item(4).setSelected();
  else if (160 > settings.emulator.hack.ppu.mode7.widescreen) mode7Widescreen.item(5).setSelected();
  mode7Widescreen.onChange([&] {
    if (0 == mode7Widescreen.selected().offset())      settings.emulator.hack.ppu.mode7.widescreen =   0;
    else if (1 == mode7Widescreen.selected().offset()) settings.emulator.hack.ppu.mode7.widescreen =  16;
    else if (2 == mode7Widescreen.selected().offset()) settings.emulator.hack.ppu.mode7.widescreen =  40;
    else if (3 == mode7Widescreen.selected().offset()) settings.emulator.hack.ppu.mode7.widescreen =  64;
    else if (4 == mode7Widescreen.selected().offset()) settings.emulator.hack.ppu.mode7.widescreen =  88;
    else if (5 == mode7Widescreen.selected().offset()) settings.emulator.hack.ppu.mode7.widescreen = 120;
    emulator->configure("Hacks/PPU/Mode7/Widescreen",  settings.emulator.hack.ppu.mode7.widescreen);
  });
  
  wsBG1Label.setText("BG1:");
  wsBG1.append(ComboButtonItem().setText("off"));
  wsBG1.append(ComboButtonItem().setText("on"));
  wsBG1.append(ComboButtonItem().setText("< 40"));
  wsBG1.append(ComboButtonItem().setText("> 40"));
  wsBG1.append(ComboButtonItem().setText("< 80"));
  wsBG1.append(ComboButtonItem().setText("> 80"));
  wsBG1.append(ComboButtonItem().setText("<120"));
  wsBG1.append(ComboButtonItem().setText(">120"));
  wsBG1.append(ComboButtonItem().setText("<160"));
  wsBG1.append(ComboButtonItem().setText(">160"));
  wsBG1.append(ComboButtonItem().setText("<200"));
  wsBG1.append(ComboButtonItem().setText(">200"));
  wsBG1.append(ComboButtonItem().setText("crop"));
  wsBG1.append(ComboButtonItem().setText("cropAuto"));
  wsBG1.append(ComboButtonItem().setText("disable"));
  wsBG1.append(ComboButtonItem().setText("autoHor"));
  wsBG1.append(ComboButtonItem().setText("autoHor&Ver"));
  for(uint n = 0; n <= 16; n++) {
    if(wsBG1.item(n).offset() == settings.emulator.hack.ppu.mode7.wsbg1)
       wsBG1.item(n).setSelected();
  }
  wsBG1.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg1 = wsBG1.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/Wsbg1", settings.emulator.hack.ppu.mode7.wsbg1);
  });
  wsBG2Label.setText("BG2:");
  wsBG2.append(ComboButtonItem().setText("off"));
  wsBG2.append(ComboButtonItem().setText("on"));
  wsBG2.append(ComboButtonItem().setText("< 40"));
  wsBG2.append(ComboButtonItem().setText("> 40"));
  wsBG2.append(ComboButtonItem().setText("< 80"));
  wsBG2.append(ComboButtonItem().setText("> 80"));
  wsBG2.append(ComboButtonItem().setText("<120"));
  wsBG2.append(ComboButtonItem().setText(">120"));
  wsBG2.append(ComboButtonItem().setText("<160"));
  wsBG2.append(ComboButtonItem().setText(">160"));
  wsBG2.append(ComboButtonItem().setText("<200"));
  wsBG2.append(ComboButtonItem().setText(">200"));
  wsBG2.append(ComboButtonItem().setText("crop"));
  wsBG2.append(ComboButtonItem().setText("cropAuto"));
  wsBG2.append(ComboButtonItem().setText("disable"));
  wsBG2.append(ComboButtonItem().setText("autoHor"));
  wsBG2.append(ComboButtonItem().setText("autoHor&Ver"));
  for(uint n = 0; n <= 16; n++) {
    if(wsBG2.item(n).offset() == settings.emulator.hack.ppu.mode7.wsbg2)
       wsBG2.item(n).setSelected();
  }
  wsBG2.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg2 = wsBG2.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/Wsbg2", settings.emulator.hack.ppu.mode7.wsbg2);
  });
  wsBG3Label.setText("BG3:");
  wsBG3.append(ComboButtonItem().setText("off"));
  wsBG3.append(ComboButtonItem().setText("on"));
  wsBG3.append(ComboButtonItem().setText("< 40"));
  wsBG3.append(ComboButtonItem().setText("> 40"));
  wsBG3.append(ComboButtonItem().setText("< 80"));
  wsBG3.append(ComboButtonItem().setText("> 80"));
  wsBG3.append(ComboButtonItem().setText("<120"));
  wsBG3.append(ComboButtonItem().setText(">120"));
  wsBG3.append(ComboButtonItem().setText("<160"));
  wsBG3.append(ComboButtonItem().setText(">160"));
  wsBG3.append(ComboButtonItem().setText("<200"));
  wsBG3.append(ComboButtonItem().setText(">200"));
  wsBG3.append(ComboButtonItem().setText("crop"));
  wsBG3.append(ComboButtonItem().setText("cropAuto"));
  wsBG3.append(ComboButtonItem().setText("disable"));
  wsBG3.append(ComboButtonItem().setText("autoHor"));
  wsBG3.append(ComboButtonItem().setText("autoHor&Ver"));
  for(uint n = 0; n <= 16; n++) {
    if(wsBG3.item(n).offset() == settings.emulator.hack.ppu.mode7.wsbg3)
       wsBG3.item(n).setSelected();
  }
  wsBG3.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg3 = wsBG3.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/Wsbg3", settings.emulator.hack.ppu.mode7.wsbg3);
  });
  wsBG4Label.setText("BG4:");
  wsBG4.append(ComboButtonItem().setText("off"));
  wsBG4.append(ComboButtonItem().setText("on"));
  wsBG4.append(ComboButtonItem().setText("< 40"));
  wsBG4.append(ComboButtonItem().setText("> 40"));
  wsBG4.append(ComboButtonItem().setText("< 80"));
  wsBG4.append(ComboButtonItem().setText("> 80"));
  wsBG4.append(ComboButtonItem().setText("<120"));
  wsBG4.append(ComboButtonItem().setText(">120"));
  wsBG4.append(ComboButtonItem().setText("<160"));
  wsBG4.append(ComboButtonItem().setText(">160"));
  wsBG4.append(ComboButtonItem().setText("<200"));
  wsBG4.append(ComboButtonItem().setText(">200"));
  wsBG4.append(ComboButtonItem().setText("crop"));
  wsBG4.append(ComboButtonItem().setText("cropAuto"));
  wsBG4.append(ComboButtonItem().setText("disable"));
  wsBG4.append(ComboButtonItem().setText("autoHor"));
  wsBG4.append(ComboButtonItem().setText("autoHor&Ver"));
  for(uint n = 0; n <= 16; n++) {
    if(wsBG4.item(n).offset() == settings.emulator.hack.ppu.mode7.wsbg4)
       wsBG4.item(n).setSelected();
  }
  wsBG4.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsbg4 = wsBG4.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/Wsbg4", settings.emulator.hack.ppu.mode7.wsbg4);
  });
  wsObjLabel.setText("Sprites:");
  wsObj.append(ComboButtonItem().setText("safe"));
  wsObj.append(ComboButtonItem().setText("unsafe"));
  wsObj.append(ComboButtonItem().setText("disable"));
  wsObj.append(ComboButtonItem().setText("clip"));
  for(uint n = 0; n <= 3; n++) {
    if(wsObj.item(n).offset() == settings.emulator.hack.ppu.mode7.wsobj)
       wsObj.item(n).setSelected();
  }
  wsObj.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsobj = wsObj.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/Wsobj", settings.emulator.hack.ppu.mode7.wsobj);
  });
  igwinLabel.setText("Ignore window:");
  igwin.append(ComboButtonItem().setText("none"));
  igwin.append(ComboButtonItem().setText("outside"));
  igwin.append(ComboButtonItem().setText("outside&always"));
  igwin.append(ComboButtonItem().setText("all"));
  for(uint n = 0; n < 4; n++) {
    if(igwin.item(n).offset() == settings.emulator.hack.ppu.mode7.igwin)
       igwin.item(n).setSelected();
  }
  igwin.onChange([&] {
    settings.emulator.hack.ppu.mode7.igwin = igwin.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/Igwin", settings.emulator.hack.ppu.mode7.igwin);
  });
  igwinxLabel.setText("IW fallback x-coordinate:");
  igwinx.append(ComboButtonItem().setText(" 40"));//.setProperty("col",  40));
  igwinx.append(ComboButtonItem().setText(" 88"));//.setProperty("col",  88));
  igwinx.append(ComboButtonItem().setText("128"));//.setProperty("col", 128));
  igwinx.append(ComboButtonItem().setText("168"));//.setProperty("col", 168));
  igwinx.append(ComboButtonItem().setText("216"));//.setProperty("col", 216));
  /*for(uint n = 0; n < 5; n++) {
    if(igwinx.item(n).property("col").natural() == settings.emulator.hack.ppu.mode7.igwinx)
       igwinx.item(n).setSelected();
  }*/
  if (40 == settings.emulator.hack.ppu.mode7.igwinx) {
    igwinx.item(0).setSelected();
  } else if (88 == settings.emulator.hack.ppu.mode7.igwinx) {
    igwinx.item(1).setSelected();
  } else if (128 == settings.emulator.hack.ppu.mode7.igwinx) {
    igwinx.item(2).setSelected();
  } else if (168 == settings.emulator.hack.ppu.mode7.igwinx) {
    igwinx.item(3).setSelected();
  } else if (216 == settings.emulator.hack.ppu.mode7.igwinx) {
    igwinx.item(4).setSelected();
  }
  igwinx.onChange([&] {
    //settings.emulator.hack.ppu.mode7.igwinx = igwinx.selected().offset();
    if (0 == mode7Widescreen.selected().offset()) {
      settings.emulator.hack.ppu.mode7.igwinx = 40;
    } else if (1 == mode7Widescreen.selected().offset()) {
      settings.emulator.hack.ppu.mode7.igwinx = 88;
    } else if (2 == mode7Widescreen.selected().offset()) {
      settings.emulator.hack.ppu.mode7.igwinx = 128;
    } else if (3 == mode7Widescreen.selected().offset()) {
      settings.emulator.hack.ppu.mode7.igwinx = 168;
    } else if (4 == mode7Widescreen.selected().offset()) {
      settings.emulator.hack.ppu.mode7.igwinx = 216;
    }
    emulator->configure("Hacks/PPU/Mode7/Igwinx", settings.emulator.hack.ppu.mode7.igwinx);
  });

  wsBgColLabel.setText("WS bg:");
  wsBgCol.append(ComboButtonItem().setText("color"));
  wsBgCol.append(ComboButtonItem().setText("auto"));
  wsBgCol.append(ComboButtonItem().setText("black"));
  for(uint n = 0; n <= 2; n++) {
    if(wsBgCol.item(n).offset() == settings.emulator.hack.ppu.mode7.wsBgCol)
       wsBgCol.item(n).setSelected();
  }
  wsBgCol.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsBgCol = wsBgCol.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/WsBgCol", settings.emulator.hack.ppu.mode7.wsBgCol);
  });

  wsModeLabel.setText("Widescreen:");
  wsMode.append(ComboButtonItem().setText("none"));
  wsMode.append(ComboButtonItem().setText("Mode 7"));
  wsMode.append(ComboButtonItem().setText("all"));
  for(uint n = 0; n < 3; n++) {
    if(wsMode.item(n).offset() == settings.emulator.hack.ppu.mode7.wsMode)
       wsMode.item(n).setSelected();
  }
  wsMode.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsMode = wsMode.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/WsMode", settings.emulator.hack.ppu.mode7.wsMode);
    //presentation.resizeViewport();
  });


  wsMarkerLabel.setText("WS marker:");
  wsMarker.append(ComboButtonItem().setText("none"));
  wsMarker.append(ComboButtonItem().setText("lines"));
  wsMarker.append(ComboButtonItem().setText("darken"));
  for(uint n = 0; n <= 2; n++) {
    if(wsMarker.item(n).offset() == settings.emulator.hack.ppu.mode7.wsMarker)
       wsMarker.item(n).setSelected();
  }
  wsMarker.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsMarker = wsMarker.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/WsMarker", settings.emulator.hack.ppu.mode7.wsMarker);
  });


  wsMarkerAlphaLabel.setText("WSM alpha:");
  wsMarkerAlpha.append(ComboButtonItem().setText("1/1" ));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/2" ));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/3" ));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/4" ));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/5" ));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/6" ));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/7" ));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/8" ));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/9" ));
  wsMarkerAlpha.append(ComboButtonItem().setText("1/10"));
  for(uint n = 0; n < 10; n++) {
    if(wsMarkerAlpha.item(n).offset() + 1 == settings.emulator.hack.ppu.mode7.wsMarkerAlpha)
       wsMarkerAlpha.item(n).setSelected();
  }
  wsMarkerAlpha.onChange([&] {
    settings.emulator.hack.ppu.mode7.wsMarkerAlpha = wsMarkerAlpha.selected().offset() + 1;
    emulator->configure("Hacks/PPU/Mode7/WsMarkerAlpha", settings.emulator.hack.ppu.mode7.wsMarkerAlpha);
  });


  bgGradLabel.setText("Line color HD:");
  bgGrad.append(ComboButtonItem().setText("0"));
  bgGrad.append(ComboButtonItem().setText("1"));
  bgGrad.append(ComboButtonItem().setText("2"));
  bgGrad.append(ComboButtonItem().setText("3"));
  bgGrad.append(ComboButtonItem().setText("4"));
  bgGrad.append(ComboButtonItem().setText("5"));
  bgGrad.append(ComboButtonItem().setText("6"));
  bgGrad.append(ComboButtonItem().setText("7"));
  bgGrad.append(ComboButtonItem().setText("8"));
  for(uint n = 0; n <= 8; n++) {
    if(bgGrad.item(n).offset() == settings.emulator.hack.ppu.mode7.bgGrad)
       bgGrad.item(n).setSelected();
  }
  bgGrad.onChange([&] {
    settings.emulator.hack.ppu.mode7.bgGrad = bgGrad.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/BgGrad", settings.emulator.hack.ppu.mode7.bgGrad);
  });

  windRadLabel.setText("Window HD:");
  windRad.append(ComboButtonItem().setText("0"));
  windRad.append(ComboButtonItem().setText("1"));
  windRad.append(ComboButtonItem().setText("2"));
  windRad.append(ComboButtonItem().setText("3"));
  windRad.append(ComboButtonItem().setText("4"));
  windRad.append(ComboButtonItem().setText("5"));
  windRad.append(ComboButtonItem().setText("6"));
  windRad.append(ComboButtonItem().setText("7"));
  windRad.append(ComboButtonItem().setText("8"));
  for(uint n = 0; n <= 8; n++) {
    if(windRad.item(n).offset() == settings.emulator.hack.ppu.mode7.windRad)
       windRad.item(n).setSelected();
  }
  windRad.onChange([&] {
    settings.emulator.hack.ppu.mode7.windRad = windRad.selected().offset();
    emulator->configure("Hacks/PPU/Mode7/WindRad", settings.emulator.hack.ppu.mode7.windRad);
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
  coprocessorDelayedSyncOption.setText("Fast mode").setChecked(settings.emulator.hack.coprocessor.delayedSync).onToggle([&] {
    settings.emulator.hack.coprocessor.delayedSync = coprocessorDelayedSyncOption.checked();
  });
  coprocessorPreferHLEOption.setText("Prefer HLE").setChecked(settings.emulator.hack.coprocessor.preferHLE).setToolTip(
    "When checked, less accurate HLE emulation will always be used when available.\n"
    "When unchecked, HLE will only be used when LLE firmware is missing."
  ).onToggle([&] {
    settings.emulator.hack.coprocessor.preferHLE = coprocessorPreferHLEOption.checked();
  });

  gameLabel.setText("Game Enhancements").setFont(Font().setBold());
  hotfixes.setText("Hotfixes").setToolTip({
    "Even commercially licensed and officially released software sometimes shipped with bugs.\n"
    "This option will correct certain issues that occurred even on real hardware."
  }).setChecked(settings.emulator.hack.hotfixes).onToggle([&] {
    settings.emulator.hack.hotfixes = hotfixes.checked();
  });

  note.setText("Note: some settings do not take effect until after reloading games.");
}

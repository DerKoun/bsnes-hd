auto Program::updateVideoDriver(Window parent) -> void {
  auto changed = (bool)video;
  video.create(settings.video.driver);
  video.setContext(presentation.viewport.handle());
  if(changed) {
    settings.video.format = video.format();
  }
  updateVideoExclusive();
  updateVideoBlocking();
  updateVideoFlush();
  updateVideoMonitor();
  updateVideoFormat();
  updateVideoShader();

  if(video.ready()) {
    video.clear();
    updateVideoShader();
  }

  video.onUpdate([&](uint width, uint height) {
    if(!emulator->loaded()) video.clear();
  });

  if(!video.ready()) {
    MessageDialog({
      "Error: failed to initialize [", settings.video.driver, "] video driver."
    }).setAlignment(parent).error();
    settings.video.driver = "None";
    return updateVideoDriver(parent);
  }

  presentation.updateShaders();
}

auto Program::updateVideoExclusive() -> void {
  video.setExclusive(settings.video.exclusive);
}

auto Program::updateVideoBlocking() -> void {
  video.setBlocking(settings.video.blocking);
}

auto Program::updateVideoFlush() -> void {
  video.setFlush(settings.video.flush);
}

auto Program::updateVideoMonitor() -> void {
  if(!video.hasMonitor(settings.video.monitor)) {
    settings.video.monitor = video.monitor();
  }
  video.setMonitor(settings.video.monitor);
}

auto Program::updateVideoFormat() -> void {
  if(!video.hasFormat(settings.video.format)) {
    settings.video.format = video.format();
  }
  video.setFormat(settings.video.format);
}

auto Program::updateVideoShader() -> void {
  video.setShader(settings.video.shader);
}

auto Program::updateVideoEffects() -> void {
  emulator->configure("Video/BlurEmulation", settings.video.blur);
  // Widescreen AR relevant settings:
  emulator->configure("Video/AspectCorrection", settings.video.aspectCorrection);
  emulator->configure("Video/Overscan", settings.video.overscan);
}

auto Program::toggleVideoFullScreen() -> void {
  if(!video.hasFullScreen()) return;
  if(presentation.fullScreen()) return;

  video.clear();

  if(!video.fullScreen()) {
    video.setFullScreen(true);
    if(!input.acquired() && (video.exclusive() || video.hasMonitors().size() == 1)) input.acquire();
  } else {
    if(input.acquired()) input.release();
    video.setFullScreen(false);
    presentation.viewport.setFocused();
  }
}

auto Program::toggleVideoPseudoFullScreen() -> void {
  if(video.fullScreen()) return;

  if(!presentation.fullScreen()) {
    presentation.setFullScreen(true);
    presentation.menuBar.setVisible(false);
    if(!input.acquired() && video.hasMonitors().size() == 1) input.acquire();
  } else {
    if(input.acquired()) input.release();
    presentation.menuBar.setVisible(true);
    presentation.setFullScreen(false);
  }
}

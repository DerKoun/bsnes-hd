auto PPU::Line::renderWindow(PPU::IO::WindowLayer& self, bool enable,
                             bool output[1024], uint ws, bool stretch) -> void {
  uint width = 256 + 2 * ws;
  if(!enable || (!self.oneEnable && !self.twoEnable)) {
    memory::fill<bool>(output, width, 0);
    return;
  }

  int oneLeft  = io.window.oneLeft;
  int oneRight = io.window.oneRight;
  if (stretch) {
    oneLeft  = (oneLeft  << 1) - 128 + ws;
    oneRight = (oneRight << 1) - 128 + ws;
  } else {
    if (oneLeft == 0) {
      oneLeft = -1;
    } else if (oneLeft == 255) {
      oneLeft = width;
    } else  {
      oneLeft += ws;
    }
    if (oneRight == 0) {
      oneRight = -1;
    } else if (oneRight == 255) {
      oneRight = width;
    } else  {
      oneRight += ws;
    } 
  }

  if(self.oneEnable && !self.twoEnable) {
    bool set = 1 ^ self.oneInvert, clear = !set;
    for(int x : range(width)) {
      output[x] = x >= oneLeft && x <= oneRight ? set : clear;
    }
    return;
  }

  int twoLeft  = io.window.twoLeft;
  int twoRight = io.window.twoRight;
  if (stretch) {
    twoLeft  = (twoLeft  << 1) - 128 + ws;
    twoRight = (twoRight << 1) - 128 + ws;
  } else { // extend edges
    if (twoLeft == 0) {
      twoLeft = -1;
    } else if (twoLeft == 255) {
      twoLeft = width;
    } else  {
      twoLeft += ws;
    }
    if (twoRight == 0) {
      twoRight = -1;
    } else if (twoRight == 255) {
      twoRight = width;
    } else  {
      twoRight += ws;
    } 
  }

  if(self.twoEnable && !self.oneEnable) {
    bool set = 1 ^ self.twoInvert, clear = !set;
    for(int x : range(width)) {
      output[x] = x >= twoLeft && x <= twoRight ? set : clear;
    }
    return;
  }

  for(int x : range(width)) {
    bool oneMask = (x >= oneLeft && x <= oneRight) ^ self.oneInvert;
    bool twoMask = (x >= twoLeft && x <= twoRight) ^ self.twoInvert;
    switch(self.mask) {
    case 0: output[x] = (oneMask | twoMask) == 1; break;
    case 1: output[x] = (oneMask & twoMask) == 1; break;
    case 2: output[x] = (oneMask ^ twoMask) == 1; break;
    case 3: output[x] = (oneMask ^ twoMask) == 0; break;
    }
  }
}

auto PPU::Line::renderWindow(PPU::IO::WindowColor& self, uint mask, bool *output,
                             int oneLeft, int oneRight, int twoLeft, int twoRight,
                             uint scale, uint offset, uint ws) -> void {
  uint width = 256 + 2 * ws;
  bool set, clear;
  switch(mask) {
  case 0: memory::fill<bool>(output, width * scale * scale, 1); return;  //always
  case 1: set = 1, clear = 0; break;  //inside
  case 2: set = 0, clear = 1; break;  //outside
  case 3: memory::fill<bool>(output, width * scale * scale, 0); return;  //never
  }

  if(!self.oneEnable && !self.twoEnable) {
    memory::fill<bool>(output, width * scale * scale, clear);
    return;
  }

  if(self.oneEnable && !self.twoEnable) {
    if(self.oneInvert) set ^= 1, clear ^= 1;
    for(int x : range(width * scale)) {
      output[x+offset] = x >= oneLeft && x <= oneRight ? set : clear;
    }
    return;
  }

  if(self.twoEnable && !self.oneEnable) {
    if(self.twoInvert) set ^= 1, clear ^= 1;
    for(int x : range(width * scale)) {
      output[x+offset] = x >= twoLeft && x <= twoRight ? set : clear;
    }
    return;
  }

  for(int x : range(width * scale)) {
    bool oneMask = (x >= oneLeft && x <= oneRight) ^ self.oneInvert;
    bool twoMask = (x >= twoLeft && x <= twoRight) ^ self.twoInvert;
    switch(self.mask) {
    case 0: output[x+offset] = (oneMask | twoMask) == 1 ? set : clear; break;
    case 1: output[x+offset] = (oneMask & twoMask) == 1 ? set : clear; break;
    case 2: output[x+offset] = (oneMask ^ twoMask) == 1 ? set : clear; break;
    case 3: output[x+offset] = (oneMask ^ twoMask) == 0 ? set : clear; break;
    }
  }
}

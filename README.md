# bsnes-hd



## What is it?

bsnes-hd (formally "*HD Mode 7 mod, for bsnes*") is a fork of bsnes (currently 107r3) that adds HD video features, such as:

### HD Mode 7

Rendering the rotated, scaled or pseudo perspective backgrounds at higher resolutions. This does not involve new custom imagery or upscaling algorithms. It is a higher resolution version of the process the SNES uses.

### Widescreen

Mostly for Mode 7 scenes, but also works for some other scenes/games here and there. Some setting tweaking required...

### more to come

For example higher color depth and interpolation for the colors blending that help the sense of depth.

### plus some none-HD related features

like the ability to disable background layers, sprites and window effects for screenshots for wallpapers or soft crop to zoom in, leaving maps or static art off the sides of the screen.



## Settings

all in Settings / Emulator

### Scale

The resolution Mode 7 is rendered at. Higher setting drastically increase CPU usage. "*disable*" uses the classic Mode 7 code, disabling some of the following features.

### Perspective correction

Whether and how pseudo 3D perspective are optimized, avoiding limitations of SNES integer math.
- **off**: disable correction
- **on**: enable correction
- **auto**(default): enable correction, but try to detect when it breaks the image and automatically disable it
- **wide**(default)/**medium**/**narrow**: the distance between the lines used for the interpolation (and also for the detection, if enabled). Use "*wide*" unless it causes issues. Only currently known game that requires this is "*Tales of Phantasia*", requiring "*narrow*".

### Supersampling

Allows basing every resulting pixel on the average of multiple pixels. At 1x scale it allows using the higher precision at the original resolution, e.g. for use with CRT-shaders. At higher resolutions it is basically expensive anti-aliasing. Keep in mind that to estimate the performance impact you have to multiply(!) this value with the "*scale*" value.

### Keep Mosaics non-HD

When unchecked mosaic effects are ignored on Mode 7 backgrounds. When checked the affected lines fall back to classic Mode 7.

### Widescreen

Enables experimental widescreen (works best for Mode 7 scenes, rest is hit and miss) and sets its AR. The actual AR are slightly wider than the selection, use some "*soft crop*" "*left*" and "*right*" for the ideal fullscreen experience. "*HDcrop*" at 5x scale results in a width of 1920, which (combined with "*soft crop*" "*top*" and "*bottom*" at 10, output/center and shader/none) results in perfect 1080p fullscreen.

### BG1/BG2/BG2/BG4

Settings for the background layers:
- **off**: no widescreen (e.g. for HUDs)
- **on**: widescreen enabled
- **< xy**/**> xy**: widescreen enabled above/below a certain scanline (for backgrounds that contain HUDs and backgrounds).
- **crop**: do not draw the left- and right-most 8 pixels (next to the widescreen areas) of the background (for backgrounds blanking some edge pixels, leaving black lines in widescreen, e.g. "*Top Gear 2*").
- **cAuto**: same as "*crop*", except for 2 differences. Only black pixel are not rendered. And lines of black pixel starting in the left crop area are removed entirely, e.g. for "*Super Mario Kart*".
- **dis.**: Disable the background entirely, e.g. to remove HUDs for screenshots for wallpapers.

### Sprites

- **normal**: Sprites are rendered if they are at least partially in the classic content area. So they can only partially reach into the widescreen areas.
- **unsafe**: Sprites are rendered, even if they are entirely in a widescreen area. Works fine for a view games, like "*Super Mario World*", causes artifacts in many.
- **disab.**: Disable sprites entirely, e.g. to remove characters or HUD elements for screenshots for wallpapers.

### Ignore window

- **none**: Apply window effects normally. Widescreen pixels are treated like the nearest non-widescreen pixel.
- **outside**: "*outside*" window effects are ignored, i.e. all pixels are treated like they are at the "*fallback x-coordinate*" for this purpose. This allows Mode 7 widescreen in "*Final Fantasy III*"
- **outside&alw**: Same as outside, but also for "*always*" window effects.
- **all**: Same as outside, but for all window effects. Can for example be used to remove the shadow and meter from "*F-Zero*" for screenshots for wallpapers (requires a "*fallback x-coordinate*" other than 128).

### Fallback x-coordinate

The x-coordinate used as fallback for "*ignore window*"

### Soft crop

Allows widths at the four edges of the image to be declared uninteresting. So when sizing and placing the image, especially in fullscreen, they can overflow the screen, allowing the significant parts of the image to be larger. So to focus on the top part of "*Super Mario Kart*" set "*bottom*" to 130 and "*top*" to 0. Further tweaking based on your monitor AR and liking may be required.
- **none**: disable soft crop
- **center**: enable soft crop for output/center
- **scale**: enable soft crop for output/center and output/scale

### Top/Bottom/Left/Right

The soft crop widths for all four edges.



## Help Wanted

### Converting C++ Rendering Code to Shaders for Optimization

Currently all the HD rendering is running on the CPU cores. This will have to be moved to the GPU to achieve usable performance. I'd really appreciate any help with that, even if it's just to get started. Please open an issue if you want to contribute, or contact me on Reddit.

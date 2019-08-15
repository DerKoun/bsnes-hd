# bsnes-hd

- [github project](https://github.com/DerKoun/bsnes-hd)
- [downloads](https://github.com/DerKoun/bsnes-hd/releases) for the latest betas
- [posts](https://www.reddit.com/r/emulation/search/?q=hd%20mode%207&restrict_sr=1&sort=new) and discussions for every beta on Reddit *r/emulation*
- [bsneshd-dev](https://discord.gg/3bVaNcK) at *Emulator Nexus* Discord (not just for developers, feel free to post suggestions, issues, game specific settings, recommended games, broken games, videos, screenshots, ...)



## What is it?

bsnes-hd (called "*HD Mode 7 mod, for bsnes*" in early betas) is a fork of bsnes (great SNES emulator by *byuu*, currently based on version 107r3) that adds HD video features, such as:

### HD Mode 7

Rendering the rotated, scaled or pseudo perspective backgrounds at higher resolutions. This does not involve new custom imagery or upscaling algorithms. It is a higher resolution version of the process the SNES uses.
- [image comparison](http://www.framecompare.com/image-compare/screenshotcomparison/EB9MNNNU) (framecompare)
- [video comparison](https://www.youtube.com/watch?v=6VrzJ6Y1kjQ) by *reznoire* (youtube)

### Widescreen

Extending the scenes to the left and right, without distorting them. Works for most Mode 7 scenes, but also for some other scenes/games, after some setting tweaking.
- **Demo video**: *Chrono Trigger* ending credits: [Beyond Time](https://www.youtube.com/watch?v=YE1gp6BWilg) by *retroprez* (youtube)
- **Demo video**: *Chrono Trigger* ending credits: [Float Away](https://www.youtube.com/watch?v=Ss49vVbULOQ) by *retroprez* (youtube)

### True color

Color calculation are done at true color instead of the SNES color depth (3*8 instead of 3*5 bit). With the optional line color smoothing color "steps" turn into actual gradients (without influencing the sharpness of the artwork).

### Other high resolution features

Smother color gradients and window effect, not limited by the original resolution or integer precision.

### plus some non-HD related features

like the ability to disable background layers, sprites and window effects for screenshots for wallpapers or soft crop to zoom in, leaving maps or static art off the sides of the screen.

### more to come

Feel free to suggest features. Please remember that this fork focuses on HD and visual output.


## Help Wanted

### Converting C++ Rendering Code to Shaders for Optimization

Currently all the HD rendering is running on the CPU cores. This will have to be moved to the GPU to achieve usable performance. I'd really appreciate any help with that, even if it's just to get started. Please open an issue if you want to contribute, or contact me on Reddit.



## Settings

all in Settings / Emulator

### Scale

The resolution Mode 7 is rendered at. Higher setting drastically increase CPU usage. "*disable*" uses the classic Mode 7 code, disabling some of the following features. (defaults to 2x, to show the effect with the smallest possible CPU impact)

### Perspective correction

Whether and how pseudo 3D perspective are optimized, avoiding limitations of SNES integer math.
- **off**: disable correction
- **on**: enable correction
- **auto**(default): enable correction, but try to detect when it breaks the image and automatically disable it.
- **wide**(default)/**medium**/**narrow**: the distance between the lines used for the interpolation (and also for the detection, if enabled). Use "*wide*" unless it causes issues. Only currently known game that requires this is "*Tales of Phantasia*", requiring "*narrow*".

### Mosaic

How to handle Mosaic effects on Mode 7 backgrounds.
- **non-HD**: use 'classic' Mode 7 with Mosaic applied. Looks as intended, but disables HD and widescreen.
- **1x scale**(default): instead of the set *Scale* use "*1x*". Provides a good compromise between the other two options.
- **ignore**: ignore the Mosaic effect completely.

### Supersampling

Allows basing every resulting pixel on the average of multiple pixels. At 1x scale it allows using the higher precision at the original resolution, e.g. for use with CRT-shaders. At higher resolutions it is basically expensive anti-aliasing. (defaults to "*none*") *Keep in mind that to estimate the performance impact you have to multiply(!) this value with the "*scale*" value.*

### Widescreen

Enables experimental widescreen functionality
- **none**: do not enable widescreen
- **Mode 7**(default): enable widescreen for Mode 7 scenes (works for most games)
- **all**: enable widescreen for all scenes (non-Mode 7 scenes only work acceptable in some games, but many have mostly artifacts)

### Aspect ratio

Specifies the widescreen aspect ratio. The actual AR are slightly wider than the selection, use some "*soft crop*" "*left*" and "*right*" for the ideal fullscreen experience. "*HD*" at 5x scale results in a width of 1920, which (combined with "*soft crop*" "*top*" and "*bottom*" at 10, output/center and shader/none) results in perfect 1080p fullscreen. (defaults to 16:9)

### WS bg

Sets how the backgrounds of the widescreen areas are filled
- **color**: always fill the widescreen areas with the scanline background colors.
- **auto**(default): fill the widescreen areas with the scanline background colors, except when the "*Widescreen:Mode 7*" setting disables widescreen for the scene, then use black. 
- **black**: the widescreen areas always have a black background, ignoring the background color, even when the scene is widescreen. (do not use unless a a game specifically requires it, none known for now)

### WS marker

Allows highlighting the edges of the widescreen areas in some ways.
- **none**(default): no highlighting
- **lines**: shows a dashed lined at the edges of each widescreen area
- **darken**: darkens the widescreen areas

### WSM alpha

Specifies how translucent the widescreen markers are. (defaults to 1/1)

### BG1/BG2/BG2/BG4

Settings for the background layers.
- **off**: no widescreen (e.g. for HUDs)
- **on**: widescreen enabled
- **< xy**/**> xy**: widescreen enabled above/below a certain scanline (for backgrounds that contain HUDs and backgrounds).
- **autoHor**: Disables widescreen for this background when it is as wide as the screen and has a horizontal position of 0.
- **autoHor&Ver**(default): Disables widescreen for this background when it is as wide as the screen and has a horizontal and vertical position of 0.
- **crop**: do not draw the left- and right-most 8 pixels (next to the widescreen areas) of the background (for backgrounds blanking some edge pixels, leaving black lines in widescreen, e.g. "*Top Gear 2*").
- **cropAuto**: same as "*crop*", except for 2 differences. Only black pixel are not rendered. And lines of black pixel starting in the left crop area are removed entirely, e.g. for "*Super Mario Kart*".
- **disable**: Disable the background entirely, e.g. to remove HUDs for screenshots for wallpapers.

### Sprites

Settings for sprites/objects.
- **clip**: Sprites are clipped at the edges of the widescreen areas.
- **safe**(default): Sprites are rendered if they are at least partially in the classic content area. So they can only partially reach into the widescreen areas.
- **unsafe**: Sprites are rendered, even if they are entirely in a widescreen area. Works fine for a view games, like "*Super Mario World*", causes artifacts in many.
- **disable**: Disable sprites entirely, e.g. to remove characters or HUD elements for screenshots for wallpapers.

### Ignore window

- **none**: Apply window effects normally. Widescreen pixels are treated like the nearest non-widescreen pixel.
- **outside**(default): "*outside*" window effects are ignored, i.e. all pixels are treated like they are at the "*fallback x-coordinate*" for this purpose. This allows Mode 7 widescreen in "*Final Fantasy III*"
- **outside&always**: Same as outside, but also for "*always*" window effects.
- **all**: Same as outside, but for all window effects. Can for example be used to remove the shadow and meter from "*F-Zero*" for screenshots for wallpapers (requires changing "*fallback x-coordinate*" from 128 to something like 40).

### Fallback x-coordinate

The x-coordinate used as fallback for "*ignore window*". (defaults to 128 (the center))

### Line color HD

The amount of neighboring lines used to smooth color gradients that are applied to the frame, e.g. to improve perspective Mode 7 effects, by fading to dark for the "far away" top. "*0*" disables this smoothing. The default is "*4*".

### Window HD

The amount of neighboring lines used to smooth Window effects, like iris transitions, shadows or spell effects. "*0*" disables smoothing and is the default. (*This feature is considered a preview, as the lines at the top and bottom of effects are currently not entirely HD and it, in general, is not that well tested. Please let me know about any games/scenes/effects that work noticeable badly or well*)

### Soft crop

Allows widths at the four edges of the image to be declared uninteresting. So when sizing and placing the image, especially in fullscreen, they can overflow the screen, allowing the significant parts of the image to be larger. So to focus on the top part of "*Super Mario Kart*" set "*bottom*" to 130 and "*top*" to 0. Further tweaking based on your monitor AR and liking may be required.
- **none**: disable soft crop
- **center**(default): enable soft crop for output/center
- **scale**: enable soft crop for output/center and output/scale

### SC Top/Bottom/Left/Right

The soft crop widths for all four edges. (defaults to 10, 10, 20, 20)



## More documentation to come...

# bsnes-hd *beta 10.5*

- [downloads](https://github.com/DerKoun/bsnes-hd/releases) for the latest betas (there are only beta) / also on the libretro auto-updater
- [GitHub project](https://github.com/DerKoun/bsnes-hd) for source code, issues, feature requests, ...
- [Reddit](https://www.reddit.com/r/emulation/search/?q=bsnes-hd&restrict_sr=1&sort=new) for announcements and discussions on *r/emulation*
- [Discord](https://discord.gg/7ahAzCV) if you prefer chatting (Thanks to everyone who set it up and keeps it running)

1. [What is it?](#what-is-it)
2. [Help wanted](#help-wanted)
3. [Settings](#settings)
4. [Differences in setting/options from bsnes](#differences-in-settingoptions-from-bsnes)
5. [Widescreen technical](#widescreen-technical)


## What is it?

bsnes-hd (called "*HD Mode 7 mod, for bsnes*" in early betas) is a fork of bsnes (great SNES emulator by *Near*) that adds HD video features, such as:

### HD Mode 7

Rendering the rotated, scaled or pseudo perspective backgrounds at higher resolutions. This does not involve new custom imagery or upscaling algorithms. It is a higher resolution version of the process the SNES uses.
- [image comparison](http://www.framecompare.com/image-compare/screenshotcomparison/EB9MNNNU) (framecompare)
- [video comparison](https://www.youtube.com/watch?v=6VrzJ6Y1kjQ) by *reznoire* (youtube)

### Widescreen

Extending the scenes to the left and right, without distorting them. Works for most Mode 7 scenes, but also for some other scenes/games, after some setting tweaking.
- **Demo video**: *Chrono Trigger* ending credits: [Beyond Time](https://www.youtube.com/watch?v=YE1gp6BWilg) by *retroprez* (youtube)
- **Demo video**: *Chrono Trigger* ending credits: [Float Away](https://www.youtube.com/watch?v=Ss49vVbULOQ) by *retroprez* (youtube)

Objects/Sprites will not be visible correctly in the widescreen areas. Fixing this will require ROM-hacks. Some technical information is available below.

### True color

Color calculation are done at true color instead of the SNES color depth (3x8 instead of 3x5 bit). With the optional line color smoothing color "steps" turn into actual gradients (without influencing the sharpness of the artwork).

### Other high resolution features

Smother color gradients and window effect, not limited by the original resolution or integer precision.

### plus some non-HD related features

like the ability to disable background layers, sprites and window effects for screenshots for wallpapers.

### more to come

Feel free to suggest features. Please remember that this fork focuses on HD and visual output.


## Help Wanted

### Performance on devices

It would be helpful to know how the framerates of different scale levels is on various devices, especially handhelds and consoles, Android and others.

### OpenGL/Vulkan

If anyone with OpenGL/Vulkan programming and/or shader knowledge would assit me, we could work on a fork that improves both quality and performance using the GPU. This is a long-term goal that I won't start on without help.

### Logo

I have a view ideas for a banner and an icon, but not the skills to design and render them. I hope someone can help me out with that.

## Settings

all in *Settings* / *Enhancements* / "*HD Mode 7 / bsnes-hd*"

names in parentheses are for the options of the libretro core

### Scale

The resolution Mode 7 is rendered at. Higher setting drastically increase CPU usage. "*disable*" uses the classic Mode 7 code, disabling some of the following features. (defaults to 2x, to show the effect with the smallest possible CPU impact)

### Perspective correction

Whether and how pseudo 3D perspective are optimized, avoiding limitations of SNES integer math.
- **off**: disable correction
- **on**: enable correction
- **auto**(default): enable correction, but try to detect when it breaks the image and automatically disable it.
- **wide**(default)/**medium**/**narrow**: the distance between the lines used for the interpolation (and also for the detection, if enabled). Use "*wide*" unless it causes issues. Only currently known game that requires this is "*Tales of Phantasia*", requiring "*narrow*".

### Mosaic (HD->SD Mosaic)

How to handle Mosaic effects on Mode 7 backgrounds.
- **non-HD**: use 'classic' Mode 7 with Mosaic applied. Looks as intended, but disables HD and widescreen.
- **1x scale**(default): instead of the set *Scale* use "*1x*". Provides a good compromise between the other two options.
- **ignore**: ignore the Mosaic effect completely.

### Supersampling

Allows basing every resulting pixel on the average of multiple pixels. At 1x scale it allows using the higher precision at the original resolution, e.g. for use with CRT-shaders. At higher resolutions it is basically expensive anti-aliasing. (defaults to "*none*") *Keep in mind that to estimate the performance impact you have to multiply(!) this value with the "*scale*" value.*

### Widescreen (WideScreen Mode)

Enables experimental widescreen functionality
- **none**: do not enable widescreen
- **Mode 7**(default): enable widescreen for Mode 7 scenes (works for most games)
- **all**: enable widescreen for all scenes (non-Mode 7 scenes only work acceptable in some games, but many have mostly artifacts)

### Aspect ratio

Specifies the widescreen aspect ratio. 16:9 at 5x scale results in a width of 1920, which (with the default *overscan* setting *off*, see below) results in exactly *1080 HD*. (defaults to 16:9)

### WS bg (WideScreen Area Background Color)

Sets how the backgrounds of the widescreen areas are filled
- **color**: always fill the widescreen areas with the scanline background colors.
- **auto**(default): fill the widescreen areas with the scanline background colors, except when the "*Widescreen:Mode 7*" setting disables widescreen for the scene, then use black. 
- **black**: the widescreen areas always have a black background, ignoring the background color, even when the scene is widescreen. (do not use unless a a game specifically requires it, none known for now)

### WS marker (WideScreen Marker)

Allows highlighting the edges of the widescreen areas in some ways.
- **none**(default): no highlighting
- **lines**: shows a dashed lined at the edges of each widescreen area
- **darken**: darkens the widescreen areas

### WSM alpha (WideScreen Marker Alpha)

Specifies how translucent the widescreen markers are. (defaults to 1/1)

### BG1/BG2/BG2/BG4 (WideScreen Background 1/.../WideScreen Background 4)

Settings for the background layers.
- **off**: no widescreen (e.g. for HUDs)
- **on**: widescreen enabled
- **< xy**/**> xy**: widescreen enabled above/below a certain scanline (for backgrounds that contain HUDs and backgrounds), e.g. for "*HyperZone*" at "*BG2: >80*".
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

### Ignore window (WideScreen Ignore Window)

- **none**: Apply window effects normally. Widescreen pixels are treated like the nearest non-widescreen pixel.
- **outside**(default): "*outside*" window effects are ignored, i.e. all pixels are treated like they are at the "*fallback x-coordinate*" for this purpose. This allows Mode 7 widescreen in "*Final Fantasy III*"
- **outside&always**: Same as outside, but also for "*always*" window effects.
- **all**: Same as outside, but for all window effects. Can for example be used to remove the shadow and meter from "*F-Zero*" for screenshots for wallpapers (requires changing "*fallback x-coordinate*" from 128 to something like 40).

### Fallback x-coordinate (WideScreen Ig Win Coordinate)

The x-coordinate used as fallback for "*ignore window*". (defaults to 128 (the center))

### Line color HD (HD Background Color Radius)

The amount of neighboring lines used to smooth color gradients that are applied to the frame, e.g. to improve perspective Mode 7 effects, by fading to dark for the "far away" top. "*0*" disables this smoothing. The default is "*4*".

### Window HD (HD Windowing (experimental))

The amount of neighboring lines used to smooth Window effects, like iris transitions, shadows or spell effects. "*0*" disables smoothing and is the default. (*This feature is considered a preview, as the lines at the top and bottom of effects are currently not entirely HD and it still has noticeable issues. Please let me know about any games/scenes/effects that work noticeable badly or well*)

## Differences in Setting/Options from bsnes

### Settings / Output / Show Overscan Area (Show Overscan)

In *bsnes* the overscan setting allows switching between cropping 8 and 0 lines form top and bottom, which are unused due to the way TVs in the time of the SNES worked. In *bsnes-hd* it switches between 12 and 8 lines, defaulting to 12 (*off*). This cuts of 4 lines on both sides that technically contain content, but should not cut important information as these lines are still in an area that wasn't safe to use (12 lines is 5%). The reason to do is that the resulting height of 216 is exactly a 5th of 1080, so you can integer scale to HD and 4K resolutions, e.g. *5x* at *16:9* is exactly *1080 HD* with every Mode 7 pixel rendered specifically.

### Settings / Output / (Pixel) Aspect Correction

Renamed Aspect Correction to Pixel Aspect Correction to avoid confusion with the widescreen aspect ratio.

## Widescreen Technical

### Dimensions

The amount of pixel columns added to both sides for the various aspect ratios are, depending on some settings:
- overscan *off*, pixel aspect correction *off*: (4:3, 16), (16:10, 44), (16:9, 64), (2:1, 88), (21:9, 124).
- overscan *on*, pixel aspect correction *off*: (4:3, 20), (16:10, 52), (16:9, 72), (2:1, 96), (21:9, 132).
- overscan *on*, pixel aspect correction *on*: (4:3, 0), (16:10, 24), (16:9, 44), (2:1, 64), (21:9, 96).

### Maximum width for objects/sprites

The maximum width for widescreen areas that still can have places objects in them is *96* (exactly 2:1 AR with overscan *on* and pixel aspect correction *off* or 21:9 with overscan *on* and pixel aspect correction *on*). 

### Object/sprite wrap-around

At that maximum width *352* is the only coordinate that places a large object (width 64) entirely off screen. Smaller value make it reach into the screen from the right, larger ones from the left.

### Setting override files

Along with (or instead of) widescreen patches you can override certain settings via a file with the same name as the ROM and the extension ".bso". It is searched in the same way as patches. The easiest way is to place it in the same folder as the ROM.

The file must contain alternating letters and numbers, each pair overriding a setting.

#### Settings

| Description                         | Letter  | Values                                          |
| ----------------------------------- | ------- | ----------------------------------------------- |
| widescreen mode                     | w       | 0:off    1:on(always)           2:on(mode7)     |
| widescreen sprites                  | s       | 0:safe   1:unsafe(widescreen)   2:clip          |
| widescreen aspect ratio             | W       | 0-200:widescreen-extension 201+:AR(*see below*) |
| widescreen background 1/2/3/4       | b/B/c/C | 0+:WS 10+:crop 20:disab 1000+:line(*see below*) |
| widescreen marker                   | m       | 0:off    1+:line    11+:darken    (*see below*) |
| mode 7 perspective correction       | P       | 0:off    1-3:auto    4-6+:on      (*see below*) |
| ignore window                       | i       | 0:none   1:outside   2:outside&always   3:all   |
| ignore window fallback x-coordinate | I       | 0-255:x-coordinate                              |
| overclock CPU                       | O       | 100+:percentage(100 is normal)                  |
| stretch windowing                   | S       | (*for widescreen patches only*,    *see below*) |

#### Widescreen Aspect Ratio values

Values of 200 and less specify the widescreen extension on each side in pixel columns. The value is rounded to a multiple of 8.

Values larger than 200 specify the aspect ratio as (horizontal*100+vertical), e.g. 16:10, 16:9, 2:1 and 21:9 as 1610, 1609, 201 and 2109, respectively. From this AR the widescreen extension is computed in the same way as for ARs specified in the settings dialog, except that arbitrary ARs can be specified here. The resulting additional columns are rounded to a multiple of 8.

#### Widescreen Background

0:off, 1:on, 2:auto(horizanotal+vertical), 2:auto(horizanotal), 10:crop, 11:crop(auto), 20:disable. 
To enable widescreen for the background only above or below a certain line the value must be the line number plus 1000 or 2000 respectively.

#### Widescreen Marker 

Values from 1-10 and 11-20 enable lines and darkening respectively. The values in the respective ranges vary the opacity.

#### Perspective Correction

1-3 and 4-6 trigger auto an on respectively. In both cases the 3 settings are in the order: wide, medium, narrow.

#### Stretch Windowing

A value of 2 causes all windowing coordinates to be stretched, i.e. their distance from 128 is doubled.
This allows widescreen patches to extend windowing effects into the widescreen areas at the cost of precision.
But it also requires the patch to adapt all windowing coordinates in the ROM.

#### Sample

To force enable widescreen, including for sprites and setting a widescreen extension of 64 the file can simply be: 
```
w1s1W64
```
Any character that is not a letter or digit is ignored. So you can also do:
```
w :  1
s :  1
W : 64
```
The percent character ("%") disabled and enables interpretation, allowing comments:
```
  %  HyperZone  %  
B : 2100    % bg 2 includes dashboard (non-ws) and far away bg (ws):
              enable widescreen for line 100 and below %
s :    1    % unlimited sprites (not perfect, but worth trying) %
m :    0    % no ws markers %
```

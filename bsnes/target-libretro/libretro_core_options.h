#ifndef LIBRETRO_CORE_OPTIONS_H__
#define LIBRETRO_CORE_OPTIONS_H__

#include <stdlib.h>
#include <string.h>

#include "libretro.h"
#include "libretro-common/include/retro_inline.h"

#ifndef HAVE_NO_LANGEXTRA
#include "libretro_core_options_intl.h"
#endif

/*
 ********************************
 * VERSION: 1.3
 ********************************
 *
 * - 1.3: Move translations to libretro_core_options_intl.h
 *        - libretro_core_options_intl.h includes BOM and utf-8
 *          fix for MSVC 2010-2013
 *        - Added HAVE_NO_LANGEXTRA flag to disable translations
 *          on platforms/compilers without BOM support
 * - 1.2: Use core options v1 interface when
 *        RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION is >= 1
 *        (previously required RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION == 1)
 * - 1.1: Support generation of core options v0 retro_core_option_value
 *        arrays containing options with a single value
 * - 1.0: First commit
*/

#ifdef __cplusplus
extern "C" {
#endif

/*
 ********************************
 * Core Option Definitions
 ********************************
*/

/* RETRO_LANGUAGE_ENGLISH */

/* Default language:
 * - All other languages must include the same keys and values
 * - Will be used as a fallback in the event that frontend language
 *   is not available
 * - Will be used as a fallback for any missing entries in
 *   frontend language definition */

struct retro_core_option_definition option_defs_us[] = {
   {
      "bsnes_video_luminance",
      "Color Adjustment - Luminance",
      "Adjusts the luminance level of the video output.",
      {
         { "0",   NULL },
         { "10",  NULL },
         { "20",  NULL },
         { "30",  NULL },
         { "40",  NULL },
         { "50",  NULL },
         { "60",  NULL },
         { "70",  NULL },
         { "80",  NULL },
         { "90",  NULL },
         { "100", NULL },
      },
      "100"
   },
   {
      "bsnes_video_saturation",
      "Color Adjustment - Saturation",
      "Adjusts the saturation level of the video output.",
      {
         { "0",   NULL },
         { "10",  NULL },
         { "20",  NULL },
         { "30",  NULL },
         { "40",  NULL },
         { "50",  NULL },
         { "60",  NULL },
         { "70",  NULL },
         { "80",  NULL },
         { "90",  NULL },
         { "100", NULL },
         { "110", NULL },
         { "120", NULL },
         { "130", NULL },
         { "140", NULL },
         { "150", NULL },
         { "160", NULL },
         { "170", NULL },
         { "180", NULL },
         { "190", NULL },
         { "200", NULL },
      },
      "100"
   },
   {
      "bsnes_video_gamma",
      "Color Adjustment - Gamma",
      "Adjusts the gamma level of the video output.",
      {
         { "100", NULL },
         { "110", NULL },
         { "120", NULL },
         { "130", NULL },
         { "140", NULL },
         { "150", NULL },
         { "160", NULL },
         { "170", NULL },
         { "180", NULL },
         { "190", NULL },
         { "200", NULL },
      },
      "100"
   },
   {
      "bsnes_video_aspectcorrection",
      "Pixel Aspect Correction",
      "Determines whether the internal pixel aspect should be corrected using 4:3 as the basis, or preserved as-is with the original 8:7 proportion. This specifically affects the ratio of the pixels and should not be confused with the widescreen-related options.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "OFF"
   },
   {
      "bsnes_ppu_show_overscan",
      "Crop Overscan",
      "Crops the borders at the top and bottom of the screen, typically unused by games and hidden by the bezel of a standard-definition television, by the specified amount of pixels.",
      {
         { "OFF",     "12 pixels" },
         { "ON",      "8 pixels"  },
         { NULL, NULL },
      },
      "OFF"
   },
   {
      "bsnes_blur_emulation",
      "Blur Emulation",
      "Simulates the limited horizontal resolution of SDTVs by blurring together horizontally-adjacent pixels. Some games depend on this to emulate a transparency effect.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "OFF"
   },
   {
      "bsnes_hotfixes",
      "Hotfixes",
      "Even commercially licensed and officially released software sometimes shipped with bugs. This option will correct certain issues that occurred even on real hardware.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "OFF"
   },
   {
      "bsnes_entropy",
      "Entropy (randomization)",
      "Determines the level of randomization of the memory and registers. If set to None, all memory and registers are initialized to constant values at startup. Low randomization provides the most accurate representation of a real system. High randomizes as much as possible.",
      {
         { "Low",  NULL },
         { "High", NULL },
         { "None", NULL },
         { NULL, NULL },
      },
      "Low"
   },
   {
      "bsnes_cpu_overclock",
      "Overclocking - CPU",
      "Overclocks or downclocks the CPU. Setting this value above 100% may help reduce loading times and remove slowdown. Use with caution as it may also cause certain games to crash or exhibit issues.",
      {
         { "10",  "10%"  },
         { "20",  "20%"  },
         { "30",  "30%"  },
         { "40",  "40%"  },
         { "50",  "50%"  },
         { "60",  "60%"  },
         { "70",  "70%"  },
         { "80",  "80%"  },
         { "90",  "90%"  },
         { "100", "100%" },
         { "110", "110%" },
         { "120", "120%" },
         { "130", "130%" },
         { "140", "140%" },
         { "150", "150%" },
         { "160", "160%" },
         { "170", "170%" },
         { "180", "180%" },
         { "190", "190%" },
         { "200", "200%" },
         { "210", "210%" },
         { "220", "220%" },
         { "230", "230%" },
         { "240", "240%" },
         { "250", "250%" },
         { "260", "260%" },
         { "270", "270%" },
         { "280", "280%" },
         { "290", "290%" },
         { "300", "300%" },
         { "310", "310%" },
         { "320", "320%" },
         { "330", "330%" },
         { "340", "340%" },
         { "350", "350%" },
         { "360", "360%" },
         { "370", "370%" },
         { "380", "380%" },
         { "390", "390%" },
         { "400", "400%" },
         { NULL, NULL },
      },
      "100"
   },
   {
      "bsnes_cpu_fastmath",
      "CPU Fast Math",
      "CPU multiplication and division take time to complete on a real SNES. Older emulators did not simulate these delays and provided results immediately. Some older ROM hacks do not wait for math operations to complete and need this hack.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "OFF"
   },
   {
      "bsnes_cpu_sa1_overclock",
      "Overclocking - SA-1 Coprocessor",
      "Overclocks or downclocks the Super Accelerator 1 (SA-1) chip. Setting this value above 100% may help obtain higher performance in games that support the SA-1 chip. Use with caution as it may also cause certain games to crash or exhibit issues.",
      {
         { "10",  "10%"  },
         { "20",  "20%"  },
         { "30",  "30%"  },
         { "40",  "40%"  },
         { "50",  "50%"  },
         { "60",  "60%"  },
         { "70",  "70%"  },
         { "80",  "80%"  },
         { "90",  "90%"  },
         { "100", "100%" },
         { "110", "110%" },
         { "120", "120%" },
         { "130", "130%" },
         { "140", "140%" },
         { "150", "150%" },
         { "160", "160%" },
         { "170", "170%" },
         { "180", "180%" },
         { "190", "190%" },
         { "200", "200%" },
         { "210", "210%" },
         { "220", "220%" },
         { "230", "230%" },
         { "240", "240%" },
         { "250", "250%" },
         { "260", "260%" },
         { "270", "270%" },
         { "280", "280%" },
         { "290", "290%" },
         { "300", "300%" },
         { "310", "310%" },
         { "320", "320%" },
         { "330", "330%" },
         { "340", "340%" },
         { "350", "350%" },
         { "360", "360%" },
         { "370", "370%" },
         { "380", "380%" },
         { "390", "390%" },
         { "400", "400%" },
         { NULL, NULL },
      },
      "100"
   },
   {
      "bsnes_cpu_sfx_overclock",
      "Overclocking - SuperFX Coprocessor",
      "Overclocks or downclocks the SuperFX coprocessor. Setting this value above 100% may help obtain higher performance in games that support the SuperFX. Use with caution as it may also cause certain games to crash or exhibit issues.",
      {
         { "10",  "10%"  },
         { "20",  "20%"  },
         { "30",  "30%"  },
         { "40",  "40%"  },
         { "50",  "50%"  },
         { "60",  "60%"  },
         { "70",  "70%"  },
         { "80",  "80%"  },
         { "90",  "90%"  },
         { "100", "100%" },
         { "110", "110%" },
         { "120", "120%" },
         { "130", "130%" },
         { "140", "140%" },
         { "150", "150%" },
         { "160", "160%" },
         { "170", "170%" },
         { "180", "180%" },
         { "190", "190%" },
         { "200", "200%" },
         { "210", "210%" },
         { "220", "220%" },
         { "230", "230%" },
         { "240", "240%" },
         { "250", "250%" },
         { "260", "260%" },
         { "270", "270%" },
         { "280", "280%" },
         { "290", "290%" },
         { "300", "300%" },
         { "310", "310%" },
         { "320", "320%" },
         { "330", "330%" },
         { "340", "340%" },
         { "350", "350%" },
         { "360", "360%" },
         { "370", "370%" },
         { "380", "380%" },
         { "390", "390%" },
         { "400", "400%" },
         { "410", "410%" },
         { "420", "420%" },
         { "430", "430%" },
         { "440", "440%" },
         { "450", "450%" },
         { "460", "460%" },
         { "470", "470%" },
         { "480", "480%" },
         { "490", "490%" },
         { "500", "500%" },
         { "510", "510%" },
         { "520", "520%" },
         { "530", "530%" },
         { "540", "540%" },
         { "550", "550%" },
         { "560", "560%" },
         { "570", "570%" },
         { "580", "580%" },
         { "590", "590%" },
         { "600", "600%" },
         { "610", "610%" },
         { "620", "620%" },
         { "630", "630%" },
         { "640", "640%" },
         { "650", "650%" },
         { "660", "660%" },
         { "670", "670%" },
         { "680", "680%" },
         { "690", "690%" },
         { "700", "700%" },
         { "710", "710%" },
         { "720", "720%" },
         { "730", "730%" },
         { "740", "740%" },
         { "750", "750%" },
         { "760", "760%" },
         { "770", "770%" },
         { "780", "780%" },
         { "790", "790%" },
         { "800", "800%" },
         { NULL, NULL },
      },
      "100"
   },
   {
      "bsnes_ppu_fast",
      "PPU (Video) - Fast Mode",
      "Enables faster emulation of the PPU at the cost of a minor reduction of accuracy. It is recommended to leave this active.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "ON"
   },
   {
      "bsnes_ppu_deinterlace",
      "PPU (Video) - Deinterlace",
      "Deinterlaces all games by rendering internally at 480p. Performance penalty is almost non-existent, so it is recommended to leave this active.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "ON"
   },
   {
      "bsnes_ppu_no_sprite_limit",
      "PPU (Video) - No Sprite Limit",
      "Removes any limit to the number of sprites that can be drawn simultaneously on screen. May cause issues with certain games.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "ON"
   },
   {
      "bsnes_ppu_no_vram_blocking",
      "PPU (Video) - No VRAM Blocking",
      "Emulates a bug in older releases of ZSNES and Snes9x, where VRAM blocking was not emulated. A few older ROM hacks relied on this behavior and will render graphics incorrectly if not enabled. This option is extremely inaccurate and hurts PPU speed, so it is recommended to leave it disabled unless you need to play a game that is otherwise incompatible with this core.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "OFF"
   },
   {
      "bsnes_dsp_fast",
      "DSP (Audio) - Fast Mode",
      "Enables faster emulation of the DSP at the cost of a minor reduction of accuracy. It is recommended to leave this active.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "ON"
   },
   {
      "bsnes_dsp_cubic",
      "DSP (Audio) - Cubic Interpolation",
      "Applies cubic interpolation to the sound, preserving more of the high range.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "OFF"
   },
   {
      "bsnes_dsp_echo_shadow",
      "DSP (Audio) - Echo Shadow RAM",
      "Emulates a bug in ZSNES where echo RAM was treated as separate from APU RAM. Many older ROM hacks for Super Mario World relied on this behavior and will crash without enabling this. This option is extremely inaccurate and should not be enabled unless required.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "OFF"
   },
   {
      "bsnes_coprocessor_delayed_sync",
      "Coprocessors - Fast Mode",
      "Enables faster emulation of the coprocessors at the cost of a minor reduction of accuracy. It is recommended to leave this active.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "ON"
   },
   {
      "bsnes_coprocessor_prefer_hle",
      "Coprocessors - Prefer HLE",
      "When this option is enabled, less accurate HLE emulation will always be used when available. If disabled, HLE will only be used when LLE firmware is missing.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
         { NULL, NULL },
      },
      "ON"
   },
   {
      "bsnes_sgb_bios",
      "Preferred Super Game Boy BIOS (Requires Restart)",
      "Allows to choose the preferred Super Game Boy BIOS to be used with compatible titles. Requires a restart to take effect.",
      {
         { "SGB1.sfc", "Super Game Boy (SGB1.sfc)"   },
         { "SGB2.sfc", "Super Game Boy 2 (SGB2.sfc)" },
         { NULL, NULL },
      },
      "SGB1.sfc"
   },
   {
      "bsnes_run_ahead_frames",
      "Internal Run-Ahead",
      "Simulates the system ahead of time and rolls back to reduce input latency. Has very high system requirements.",
      {
         { "OFF", "disabled" },
         { "1",   "1 frame"  },
         { "2",   "2 frames" },
         { "3",   "3 frames" },
         { "4",   "4 frames" },
         { NULL, NULL },
      },
      "OFF"
   },
   {
      "bsnes_ips_headered",
      "Expect Headered ROMs for IPS Patches",
      "Defines whether the core should always expect headered ROMs when using IPS patches.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
      },
      "OFF"
   },
   {
      "bsnes_mode7_scale",
      "HD Mode 7 - Scale",
      "Allows to increase the horizontal and vertical resolution of the Mode 7 graphics used in certain games. If disabled, the original unaltered Mode 7 graphics will be used.",
      {
         { "disable", "disabled"    },
         { "1x",      "240p (1x)"   },
         { "2x",      "480p (2x)"   },
         { "3x",      "720p (3x)"   },
         { "4x",      "960p (4x)"   },
         { "5x",      "1200p (5x)"  },
         { "6x",      "1440p (6x)"  },
         { "7x",      "1680p (7x)"  },
         { "8x",      "1920p (8x)"  },
         { "9x",      "2160p (9x)"  },
         { "10x",     "2400p (10x)" },
         { NULL, NULL },
      },
      "2x"
   },
   {
      "bsnes_mode7_perspective",
      "HD Mode 7 - Perspective Correction",
      "Corrects the perspective of the Mode 7 graphics used in certain games by working around some limitations of the integer math used by the SNES.",
      {
         { "auto (wide)",   "Auto (Wide)"   },
         { "auto (medium)", "Auto (Medium)" },
         { "auto (narrow)", "Auto (Narrow)" },
         { "on (wide)",     "ON (Wide)"     },
         { "on (medium)",   "ON (Medium)"   },
         { "on (narrow)",   "ON (Narrow)"   },
         { "off",           "disabled"      },
         { NULL, NULL },
      },
      "auto (wide)"
   },
   {
      "bsnes_mode7_supersample",
      "HD Mode 7 - Supersampling",
      "Allows to supersample the Mode 7 graphics used in certain games by the specified amount. Combined with higher Mode 7 scale factors, it produces an effect similar to anti-aliasing.",
      {
         { "none", "disabled" },
         { "2x",  NULL },
         { "3x",  NULL },
         { "4x",  NULL },
         { "5x",  NULL },
         { "6x",  NULL },
         { "7x",  NULL },
         { "8x",  NULL },
         { "9x",  NULL },
         { "10x", NULL },
         { NULL, NULL },
      },
      "none"
   },
   {
      "bsnes_mode7_mosaic",
      "HD Mode 7 - HD->SD Mosaic",
      "Determines whether and how the mosaic effect should be shown with Mode 7 graphics. 'Non-HD' uses classic Mode 7 with the mosaic effect applied, but disables HD and widescreen. 'Ignore' disregards the mosaic effect entirely. '1x Scale' provides a good compromise between the other two options.",
      {
         { "non-HD",   "Non-HD"   },
         { "ignore",   "Ignore"   },
         { "1x scale", "1x Scale" },
      },
      "1x scale"
   },
   {
      "bsnes_mode7_bgGrad",
      "HD Background Color Radius",
      "Defines the amount of neighboring lines used to smooth color gradients that are applied to the frame.",
      {
         { "1", NULL       },
         { "2", NULL       },
         { "3", NULL       },
         { "4", NULL       },
         { "5", NULL       },
         { "6", NULL       },
         { "7", NULL       },
         { "8", NULL       },
         { "0", "disabled" },
         { NULL, NULL },
      },
      "4"
   },
   {
      "bsnes_mode7_windRad",
      "HD Windowing (Experimental)",
      "Defines the amount of neighboring lines used to smooth window effects, such as iris transitions, shadows or spell effects.",
      {
         { "1", NULL       },
         { "2", NULL       },
         { "3", NULL       },
         { "4", NULL       },
         { "5", NULL       },
         { "6", NULL       },
         { "7", NULL       },
         { "8", NULL       },
         { "0", "disabled" },
         { NULL, NULL },
      },
      "0"
   },
   {
      "bsnes_mode7_wsMode",
      "Widescreen Mode",
      "Enables experimental widescreen functionality.",
      {
         { "Mode 7", "Only for Mode 7 scenes" },
         { "all",    "Enable for all scenes"  },
         { "none",   "disabled"               },
         { NULL, NULL },
      },
      "Mode 7"
   },
   {
      "bsnes_mode7_widescreen",
      "Widescreen - Aspect Ratio",
      "Defines the content aspect ratio used when Widescreen Mode is enabled.",
      {
         { "16:9",  NULL },
         { "16:10", NULL },
         { "21:9",  NULL },
         { "2:1",   NULL },
         { "4:3",   NULL },
         { "none",  "disabled" },
         { NULL, NULL },
      },
      "16:9"
   },
   {
      "bsnes_mode7_wsbg1",
      "Widescreen - Background Layer 1",
      "Determines how Widescreen Mode should be applied to Background Layer 1.",
      {
         { "on",                 "enabled"                                    },
         { "auto horizontal",    "Enable if screen-wide and X position is 0"  },
         { "auto horz and vert", "Enable if screen-wide and XY position is 0" },
         { "above line 40",      "Enable above line 40"                       },
         { "below line 40",      "Enable below line 40"                       },
         { "above line 80",      "Enable above line 80"                       },
         { "below line 80",      "Enable below line 80"                       },
         { "above line 120",     "Enable above line 120"                      },
         { "below line 120",     "Enable below line 120"                      },
         { "above line 160",     "Enable above line 160"                      },
         { "below line 160",     "Enable below line 160"                      },
         { "above line 200",     "Enable above line 200"                      },
         { "below line 200",     "Enable below line 200"                      },
         { "crop edges",         "Hide lateral 8 pixels"                      },
         { "auto crop edges",    "Hide lateral 8 black pixels"                },
         { "off",                "Disable widescreen for this layer"          },
         { "disable entirely",   "Hide layer entirely"                        },
         { NULL, NULL },
      },
      "auto horz and vert"
   },
   {
      "bsnes_mode7_wsbg2",
      "Widescreen - Background Layer 2",
      "Determines how Widescreen Mode should be applied to Background Layer 2.",
      {
         { "on",                 "enabled"                                    },
         { "auto horizontal",    "Enable if screen-wide and X position is 0"  },
         { "auto horz and vert", "Enable if screen-wide and XY position is 0" },
         { "above line 40",      "Enable above line 40"                       },
         { "below line 40",      "Enable below line 40"                       },
         { "above line 80",      "Enable above line 80"                       },
         { "below line 80",      "Enable below line 80"                       },
         { "above line 120",     "Enable above line 120"                      },
         { "below line 120",     "Enable below line 120"                      },
         { "above line 160",     "Enable above line 160"                      },
         { "below line 160",     "Enable below line 160"                      },
         { "above line 200",     "Enable above line 200"                      },
         { "below line 200",     "Enable below line 200"                      },
         { "crop edges",         "Hide lateral 8 pixels"                      },
         { "auto crop edges",    "Hide lateral 8 black pixels"                },
         { "off",                "Disable widescreen for this layer"          },
         { "disable entirely",   "Hide layer entirely"                        },
         { NULL, NULL },
      },
      "auto horz and vert"
   },
   {
      "bsnes_mode7_wsbg3",
      "Widescreen - Background Layer 3",
      "Determines how Widescreen Mode should be applied to Background Layer 3.",
      {
         { "on",                 "enabled"                                    },
         { "auto horizontal",    "Enable if screen-wide and X position is 0"  },
         { "auto horz and vert", "Enable if screen-wide and XY position is 0" },
         { "above line 40",      "Enable above line 40"                       },
         { "below line 40",      "Enable below line 40"                       },
         { "above line 80",      "Enable above line 80"                       },
         { "below line 80",      "Enable below line 80"                       },
         { "above line 120",     "Enable above line 120"                      },
         { "below line 120",     "Enable below line 120"                      },
         { "above line 160",     "Enable above line 160"                      },
         { "below line 160",     "Enable below line 160"                      },
         { "above line 200",     "Enable above line 200"                      },
         { "below line 200",     "Enable below line 200"                      },
         { "crop edges",         "Hide lateral 8 pixels"                      },
         { "auto crop edges",    "Hide lateral 8 black pixels"                },
         { "off",                "Disable widescreen for this layer"          },
         { "disable entirely",   "Hide layer entirely"                        },
         { NULL, NULL },
      },
      "auto horz and vert"
   },
   {
      "bsnes_mode7_wsbg4",
      "Widescreen - Background Layer 4",
      "Determines how Widescreen Mode should be applied to Background Layer 4.",
      {
         { "on",                 "enabled"                                    },
         { "auto horizontal",    "Enable if screen-wide and X position is 0"  },
         { "auto horz and vert", "Enable if screen-wide and XY position is 0" },
         { "above line 40",      "Enable above line 40"                       },
         { "below line 40",      "Enable below line 40"                       },
         { "above line 80",      "Enable above line 80"                       },
         { "below line 80",      "Enable below line 80"                       },
         { "above line 120",     "Enable above line 120"                      },
         { "below line 120",     "Enable below line 120"                      },
         { "above line 160",     "Enable above line 160"                      },
         { "below line 160",     "Enable below line 160"                      },
         { "above line 200",     "Enable above line 200"                      },
         { "below line 200",     "Enable below line 200"                      },
         { "crop edges",         "Hide lateral 8 pixels"                      },
         { "auto crop edges",    "Hide lateral 8 black pixels"                },
         { "off",                "Disable widescreen for this layer"          },
         { "disable entirely",   "Hide layer entirely"                        },
         { NULL, NULL },
      },
      "auto horz and vert"
   },
   {
      "bsnes_mode7_wsobj",
      "Widescreen - Sprites",
      "Adjusts how sprites and objects should be rendered when using Widescreen Mode.",
      {
         { "safe",             "Render when in safe area" },
         { "unsafe",           "Render anywhere (unsafe)" },
         { "clip",             "Clip at widescreen edges" },
         { "disable entirely", "Hide sprites entirely"    },
         { NULL, NULL },
      },
      "safe"
   },
   {
      "bsnes_mode7_wsBgCol",
      "Widescreen - Area Background Color",
      "Specifies when to fill the backgrounds of the widescreen areas with the scanline background colors.",
      {
         { "auto",  "Fill if scene is widescreen" },
         { "color", "Always fill"                 },
         { "black", "Use black background"        },
         { NULL, NULL },
      },
      "auto"
   },
   {
      "bsnes_mode7_igwin",
      "Widescreen - Ignore Window Effects",
      "Defines which window effects should be ignored when using Widescreen Mode.",
      {
         { "outside",            "Only 'outside' effects"              },
         { "outside and always", "Only 'outside' and 'always' effects" },
         { "all",                "All window effects"                  },
         { "none",               "disabled"                            },
         { NULL, NULL },
      },
      "outside"
   },
   {
      "bsnes_mode7_igwinx",
      "Widescreen - Fallback X-Coordinate",
      "Defines which fallback x-coordinate should be used when ignoring window effects in Widescreen Mode.",
      {
         { "40",  NULL           },
         { "88",  NULL           },
         { "128", "128 (Center)" },
         { "168",  NULL          },
         { "216",  NULL          },
         { NULL, NULL },
      },
      "128"
   },
   {
      "bsnes_mode7_wsMarker",
      "Widescreen - Marker",
      "Allows to highlight the edges of the widescreen areas in some ways.",
      {
         { "lines",  "Show lines at edges" },
         { "darken", "Darken areas"        },
         { "none",   "disabled"            },
         { NULL, NULL },
      },
      "none"
   },
   {
      "bsnes_mode7_wsMarkerAlpha",
      "Widescreen - Marker Alpha",
      "If widescreen markers are enabled, this option allows to specify their translucency.",
      {
         { "1/1",  NULL },
         { "1/2",  NULL },
         { "1/3",  NULL },
         { "1/4",  NULL },
         { "1/5",  NULL },
         { "1/6",  NULL },
         { "1/7",  NULL },
         { "1/8",  NULL },
         { "1/9",  NULL },
         { "1/10", NULL },
         { NULL, NULL },
      },
      "1/1"
   },
   {
      "bsnes_mode7_strWin",
      "Widescreen - Stretch Window",
      "Extends windowing effects into the widescreen areas at the cost of precision. It is recommended to keep this option disabled, unless needed for widescreen hacks.",
      {
         { "ON",  "enabled"  },
         { "OFF", "disabled" },
      },
      "OFF"
   },

   { NULL, NULL, NULL, {{0}}, NULL },
};

/*
 ********************************
 * Language Mapping
 ********************************
*/

#ifndef HAVE_NO_LANGEXTRA
struct retro_core_option_definition *option_defs_intl[RETRO_LANGUAGE_LAST] = {
   option_defs_us, /* RETRO_LANGUAGE_ENGLISH */
   NULL,           /* RETRO_LANGUAGE_JAPANESE */
   NULL,           /* RETRO_LANGUAGE_FRENCH */
   NULL,           /* RETRO_LANGUAGE_SPANISH */
   NULL,           /* RETRO_LANGUAGE_GERMAN */
   NULL,           /* RETRO_LANGUAGE_ITALIAN */
   NULL,           /* RETRO_LANGUAGE_DUTCH */
   NULL,           /* RETRO_LANGUAGE_PORTUGUESE_BRAZIL */
   NULL,           /* RETRO_LANGUAGE_PORTUGUESE_PORTUGAL */
   NULL,           /* RETRO_LANGUAGE_RUSSIAN */
   NULL,           /* RETRO_LANGUAGE_KOREAN */
   NULL,           /* RETRO_LANGUAGE_CHINESE_TRADITIONAL */
   NULL,           /* RETRO_LANGUAGE_CHINESE_SIMPLIFIED */
   NULL,           /* RETRO_LANGUAGE_ESPERANTO */
   NULL,           /* RETRO_LANGUAGE_POLISH */
   NULL,           /* RETRO_LANGUAGE_VIETNAMESE */
   NULL,           /* RETRO_LANGUAGE_ARABIC */
   NULL,           /* RETRO_LANGUAGE_GREEK */
   NULL,           /* RETRO_LANGUAGE_TURKISH */
   NULL,           /* RETRO_LANGUAGE_SLOVAK */
   NULL,           /* RETRO_LANGUAGE_PERSIAN */
   NULL,           /* RETRO_LANGUAGE_HEBREW */
   NULL,           /* RETRO_LANGUAGE_ASTURIAN */
   NULL,           /* RETRO_LANGUAGE_FINNISH */

};
#endif

/*
 ********************************
 * Functions
 ********************************
*/

/* Handles configuration/setting of core options.
 * Should be called as early as possible - ideally inside
 * retro_set_environment(), and no later than retro_load_game()
 * > We place the function body in the header to avoid the
 *   necessity of adding more .c files (i.e. want this to
 *   be as painless as possible for core devs)
 */

static INLINE void libretro_set_core_options(retro_environment_t environ_cb)
{
   unsigned version = 0;

   if (!environ_cb)
      return;

   if (environ_cb(RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION, &version) && (version >= 1))
   {
#ifndef HAVE_NO_LANGEXTRA
      struct retro_core_options_intl core_options_intl;
      unsigned language = 0;

      core_options_intl.us    = option_defs_us;
      core_options_intl.local = NULL;

      if (environ_cb(RETRO_ENVIRONMENT_GET_LANGUAGE, &language) &&
          (language < RETRO_LANGUAGE_LAST) && (language != RETRO_LANGUAGE_ENGLISH))
         core_options_intl.local = option_defs_intl[language];

      environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL, &core_options_intl);
#else
      environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS, &option_defs_us);
#endif
   }
   else
   {
      size_t i;
      size_t num_options               = 0;
      struct retro_variable *variables = NULL;
      char **values_buf                = NULL;

      /* Determine number of options */
      for (;;)
      {
         if (!option_defs_us[num_options].key)
            break;
         num_options++;
      }

      /* Allocate arrays */
      variables  = (struct retro_variable *)calloc(num_options + 1, sizeof(struct retro_variable));
      values_buf = (char **)calloc(num_options, sizeof(char *));

      if (!variables || !values_buf)
         goto error;

      /* Copy parameters from option_defs_us array */
      for (i = 0; i < num_options; i++)
      {
         const char *key                        = option_defs_us[i].key;
         const char *desc                       = option_defs_us[i].desc;
         const char *default_value              = option_defs_us[i].default_value;
         struct retro_core_option_value *values = option_defs_us[i].values;
         size_t buf_len                         = 3;
         size_t default_index                   = 0;

         values_buf[i] = NULL;

         if (desc)
         {
            size_t num_values = 0;

            /* Determine number of values */
            for (;;)
            {
               if (!values[num_values].value)
                  break;

               /* Check if this is the default value */
               if (default_value)
                  if (strcmp(values[num_values].value, default_value) == 0)
                     default_index = num_values;

               buf_len += strlen(values[num_values].value);
               num_values++;
            }

            /* Build values string */
            if (num_values > 0)
            {
               size_t j;

               buf_len += num_values - 1;
               buf_len += strlen(desc);

               values_buf[i] = (char *)calloc(buf_len, sizeof(char));
               if (!values_buf[i])
                  goto error;

               strcpy(values_buf[i], desc);
               strcat(values_buf[i], "; ");

               /* Default value goes first */
               strcat(values_buf[i], values[default_index].value);

               /* Add remaining values */
               for (j = 0; j < num_values; j++)
               {
                  if (j != default_index)
                  {
                     strcat(values_buf[i], "|");
                     strcat(values_buf[i], values[j].value);
                  }
               }
            }
         }

         variables[i].key   = key;
         variables[i].value = values_buf[i];
      }

      /* Set variables */
      environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);

error:

      /* Clean up */
      if (values_buf)
      {
         for (i = 0; i < num_options; i++)
         {
            if (values_buf[i])
            {
               free(values_buf[i]);
               values_buf[i] = NULL;
            }
         }

         free(values_buf);
         values_buf = NULL;
      }

      if (variables)
      {
         free(variables);
         variables = NULL;
      }
   }
}

#ifdef __cplusplus
}
#endif

#endif

#include <emulator/emulator.hpp>
#include <sfc/interface/interface.hpp>
#include <filter/filter.hpp>
#include <lzma/lzma.hpp>
#include <nall/directory.hpp>
#include <nall/instance.hpp>
#include <nall/decode/rle.hpp>
#include <nall/decode/zip.hpp>
#include <nall/encode/rle.hpp>
#include <nall/encode/zip.hpp>
#include <nall/hash/crc16.hpp>
#include <nall/beat/single/apply.hpp>
using namespace nall;

#include <heuristics/heuristics.hpp>
#include <heuristics/heuristics.cpp>
#include <heuristics/super-famicom.cpp>
#include <heuristics/game-boy.cpp>
#include <heuristics/bs-memory.cpp>

#include "resources.hpp"

static Emulator::Interface *emulator;

struct Program : Emulator::Platform
{
	Program();
	~Program();
	
	auto open(uint id, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
	auto load(uint id, string name, string type, vector<string> options = {}) -> Emulator::Platform::Load override;
	auto videoFrame(const uint32* data, uint pitch, uint width, uint height, uint scale) -> void override;
	auto audioFrame(const double* samples, uint channels) -> void override;
	auto inputPoll(uint port, uint device, uint input) -> int16 override;
	auto inputRumble(uint port, uint device, uint input, bool enable) -> void override;
	
	auto load() -> void;
	auto loadFile(string location) -> vector<uint8_t>;
	auto loadSuperFamicom(string location) -> bool;
	auto loadGameBoy(string location) -> bool;
	auto loadBSMemory(string location) -> bool;

	auto save() -> void;
	
	auto openRomSuperFamicom(string name, vfs::file::mode mode) -> shared_pointer<vfs::file>;
	auto openRomGameBoy(string name, vfs::file::mode mode) -> shared_pointer<vfs::file>;
	auto openRomBSMemory(string name, vfs::file::mode mode) -> shared_pointer<vfs::file>;
	
	auto hackPatchMemory(vector<uint8_t>& data) -> void;

	auto applyPatchBPS(vector<uint8_t>& data, string location) -> bool;
	vector<uint8_t> rso;
	
	string base_name;

	bool overscan = false;
	bool aspectcorrection = false;
	uint ws = 0;
	uint scale = 1;


public:	
	struct Game {
		explicit operator bool() const { return (bool)location; }
		
		string option;
		string location;
		string manifest;
		Markup::Node document;
		boolean patched;
		boolean verified;
	};

	struct SuperFamicom : Game {
		string title;
		string region;
		vector<uint8_t> program;
		vector<uint8_t> data;
		vector<uint8_t> expansion;
		vector<uint8_t> firmware;
	} superFamicom;

	struct GameBoy : Game {
		vector<uint8_t> program;
	} gameBoy;
	
	struct BSMemory : Game {
		vector<uint8_t> program;
	} bsMemory;
};

static Program *program = nullptr;

Program::Program()
{
	Emulator::platform = this;
}

Program::~Program()
{
	delete emulator;
}

auto Program::save() -> void
{
	if(!emulator->loaded()) return;
	emulator->save();
}

auto Program::open(uint id, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file>
{
	shared_pointer<vfs::file> result;

	if (name == "ipl.rom" && mode == vfs::file::mode::read) {
		result = vfs::memory::file::open(iplrom, sizeof(iplrom));
	}

	if (name == "boards.bml" && mode == vfs::file::mode::read) {
		result = vfs::memory::file::open(Boards, sizeof(Boards));
	}

	if (id == 1) { //Super Famicom
		if (name == "manifest.bml" && mode == vfs::file::mode::read) {
			result = vfs::memory::file::open(superFamicom.manifest.data<uint8_t>(), superFamicom.manifest.size());
		}
		else if (name == "program.rom" && mode == vfs::file::mode::read) {
			result = vfs::memory::file::open(superFamicom.program.data(), superFamicom.program.size());
		}
		else if (name == "data.rom" && mode == vfs::file::mode::read) {
			result = vfs::memory::file::open(superFamicom.data.data(), superFamicom.data.size());
		}
		else if (name == "expansion.rom" && mode == vfs::file::mode::read) {
			result = vfs::memory::file::open(superFamicom.expansion.data(), superFamicom.expansion.size());
		}
		else {
			result = openRomSuperFamicom(name, mode);
		}
	}
	else if (id == 2) { //Game Boy
		if (name == "manifest.bml" && mode == vfs::file::mode::read) {
			result = vfs::memory::file::open(gameBoy.manifest.data<uint8_t>(), gameBoy.manifest.size());
		}
		else if (name == "program.rom" && mode == vfs::file::mode::read) {
			result = vfs::memory::file::open(gameBoy.program.data(), gameBoy.program.size());
		}
		else {
			result = openRomGameBoy(name, mode);
		}
	}
	else if (id == 3) {  //BS Memory
		if (name == "manifest.bml" && mode == vfs::file::mode::read) {
			result = vfs::memory::file::open(bsMemory.manifest.data<uint8_t>(), bsMemory.manifest.size());
		}
		else if (name == "program.rom" && mode == vfs::file::mode::read) {
			result = vfs::memory::file::open(bsMemory.program.data(), bsMemory.program.size());
		}
		else if(name == "program.flash") {
			//writes are not flushed to disk in bsnes
			result = vfs::memory::file::open(bsMemory.program.data(), bsMemory.program.size());
		}
		else {
			result = openRomBSMemory(name, mode);
		}
	}
	return result;
}

auto Program::load() -> void {
	emulator->unload();
	emulator->load();

	// per-game hack overrides
	auto title = superFamicom.title;
	auto region = superFamicom.region;

	//sometimes menu options are skipped over in the main menu with cycle-based joypad polling
	if(title == "Arcades Greatest Hits") emulator->configure("Hacks/CPU/FastJoypadPolling", true);

	//the start button doesn't work in this game with cycle-based joypad polling
	if(title == "TAIKYOKU-IGO Goliath") emulator->configure("Hacks/CPU/FastJoypadPolling", true);

	//holding up or down on the menu quickly cycles through options instead of stopping after each button press
	if(title == "WORLD MASTERS GOLF") emulator->configure("Hacks/CPU/FastJoypadPolling", true);

	//relies on mid-scanline rendering techniques
	if(title == "AIR STRIKE PATROL" || title == "DESERT FIGHTER") emulator->configure("Hacks/PPU/Fast", false);

	//the dialogue text is blurry due to an issue in the scanline-based renderer's color math support
	if(title == "マーヴェラス") emulator->configure("Hacks/PPU/Fast", false);

	//stage 2 uses pseudo-hires in a way that's not compatible with the scanline-based renderer
	if(title == "SFC クレヨンシンチャン") emulator->configure("Hacks/PPU/Fast", false);

	//title screen game select (after choosing a game) changes OAM tiledata address mid-frame
	//this is only supported by the cycle-based PPU renderer
	if(title == "Winter olympics") emulator->configure("Hacks/PPU/Fast", false);

	//title screen shows remnants of the flag after choosing a language with the scanline-based renderer
	if(title == "WORLD CUP STRIKER") emulator->configure("Hacks/PPU/Fast", false);

	//relies on cycle-accurate writes to the echo buffer
	if(title == "KOUSHIEN_2") emulator->configure("Hacks/DSP/Fast", false);

	//will hang immediately
	if(title == "RENDERING RANGER R2") emulator->configure("Hacks/DSP/Fast", false);

	//will hang sometimes in the "Bach in Time" stage
	if(title == "BUBSY II" && region == "PAL") emulator->configure("Hacks/DSP/Fast", false);

	//fixes an errant scanline on the title screen due to writing to PPU registers too late
	if(title == "ADVENTURES OF FRANKEN" && region == "PAL") emulator->configure("Hacks/PPU/RenderCycle", 32);

	//fixes an errant scanline on the title screen due to writing to PPU registers too late
	if(title == "FIREPOWER 2000" || title == "SUPER SWIV") emulator->configure("Hacks/PPU/RenderCycle", 32);

	//fixes an errant scanline on the title screen due to writing to PPU registers too late
	if(title == "NHL '94" || title == "NHL PROHOCKEY'94") emulator->configure("Hacks/PPU/RenderCycle", 32);

	//fixes an errant scanline on the title screen due to writing to PPU registers too late
	if(title == "Sugoro Quest++") emulator->configure("Hacks/PPU/RenderCycle", 128);

	if (emulator->configuration("Hacks/Hotfixes")) {
		//this game transfers uninitialized memory into video RAM: this can cause a row of invalid tiles
		//to appear in the background of stage 12. this one is a bug in the original game, so only enable
		//it if the hotfixes option has been enabled.
		if(title == "The Hurricanes") emulator->configure("Hacks/Entropy", "None");

		//Frisky Tom attract sequence sometimes hangs when WRAM is initialized to pseudo-random patterns
		if (title == "ニチブツ・アーケード・クラシックス") emulator->configure("Hacks/Entropy", "None");
	}

  // setting override processing (copied from standalone target)
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
          //  case 'p': //pixelAspectCorrect 0:off 1:on
          //    emulator->configure("Video/AspectCorrection", n == 1);
          //    break;
          //  case 'o': //overscan 0:216 1:224 (2:240 3:240f)
          //    emulator->configure("Video/Overscan", n == 1);
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
          }
          c = -1;
          n = 0;
        }
      }
    }
  }
  // END OF setting override processing (copied from standalone target)

	emulator->power();
}

auto Program::load(uint id, string name, string type, vector<string> options) -> Emulator::Platform::Load {
	if (id == 1)
	{
		if (loadSuperFamicom(superFamicom.location))
		{
			return {id, superFamicom.region};
		}
	}
	else if (id == 2)
	{
		if (loadGameBoy(gameBoy.location))
		{
			return { id, NULL };
		}
	}
	else if (id == 3) {
		if (loadBSMemory(bsMemory.location)) {
			return { id, NULL };
		}
	}
	return { id, options(0) };
}

auto Program::videoFrame(const uint32* data, uint pitch, uint width, uint height, uint scale) -> void {

	uint offset = overscan ? 8 : 12;
	uint multiplier = height / 215;
	data   += offset * (pitch >> 2) * multiplier;
	height -= offset * 2 * multiplier;

	video_cb(data, width, height, pitch);
}

// Double the fun!
static int16_t d2i16(double v)
{
	v *= 0x8000;
	if (v > 0x7fff)
		v = 0x7fff;
	else if (v < -0x8000)
		v = -0x8000;
	return int16_t(floor(v + 0.5));
}

auto Program::audioFrame(const double* samples, uint channels) -> void
{
	int16_t left = d2i16(samples[0]);
	int16_t right = d2i16(samples[1]);
	//audio_cb(left, right);
	audio_queue(left, right);
}

auto pollInputDevices(uint port, uint device, uint input) -> int16
{
	// TODO: This will need to be remapped on a per-system basis.
	unsigned libretro_port;
	unsigned libretro_id;
	unsigned libretro_device;
	unsigned libretro_index = 0;

	static const unsigned joypad_mapping[] = {
		RETRO_DEVICE_ID_JOYPAD_UP,
		RETRO_DEVICE_ID_JOYPAD_DOWN,
		RETRO_DEVICE_ID_JOYPAD_LEFT,
		RETRO_DEVICE_ID_JOYPAD_RIGHT,
		RETRO_DEVICE_ID_JOYPAD_B,
		RETRO_DEVICE_ID_JOYPAD_A,
		RETRO_DEVICE_ID_JOYPAD_Y,
		RETRO_DEVICE_ID_JOYPAD_X,
		RETRO_DEVICE_ID_JOYPAD_L,
		RETRO_DEVICE_ID_JOYPAD_R,
		RETRO_DEVICE_ID_JOYPAD_SELECT,
		RETRO_DEVICE_ID_JOYPAD_START,
	};

	static const unsigned mouse_mapping[] = {
		RETRO_DEVICE_ID_MOUSE_X,
		RETRO_DEVICE_ID_MOUSE_Y,
		RETRO_DEVICE_ID_MOUSE_LEFT,
		RETRO_DEVICE_ID_MOUSE_RIGHT,
	};

	switch (port)
	{
		case SuperFamicom::ID::Port::Controller1:
			libretro_port = 0;
			break;
		case SuperFamicom::ID::Port::Controller2:
			libretro_port = 1;
			break;

		default:
			return 0;
	}

	switch (device)
	{
		case SuperFamicom::ID::Device::Gamepad:
			libretro_device = RETRO_DEVICE_JOYPAD;
			libretro_id = joypad_mapping[input];
			break;

		case SuperFamicom::ID::Device::Mouse:
			libretro_device = RETRO_DEVICE_MOUSE;
			libretro_id = mouse_mapping[input];
			break;

		case SuperFamicom::ID::Device::SuperMultitap:
			libretro_device = RETRO_DEVICE_JOYPAD; // Maps to player [2, 5].
			libretro_port += input / 12;
			libretro_id = joypad_mapping[input % 12];
			break;

		// TODO: SuperScope/Justifiers.
		// Do we care? The v94 port hasn't hooked them up. :)

		default:
			return 0;
	}

	return input_state(libretro_port, libretro_device, libretro_index, libretro_id);
}

auto Program::inputPoll(uint port, uint device, uint input) -> int16
{
	return pollInputDevices(port, device, input);
}

auto Program::inputRumble(uint port, uint device, uint input, bool enable) -> void
{
}

auto Program::openRomSuperFamicom(string name, vfs::file::mode mode) -> shared_pointer<vfs::file>
{
	if(name == "program.rom" && mode == vfs::file::mode::read)
	{
		return vfs::memory::file::open(superFamicom.program.data(), superFamicom.program.size());
	}

	if(name == "data.rom" && mode == vfs::file::mode::read)
	{
		return vfs::memory::file::open(superFamicom.data.data(), superFamicom.data.size());
	}

	if(name == "expansion.rom" && mode == vfs::file::mode::read)
	{
		return vfs::memory::file::open(superFamicom.expansion.data(), superFamicom.expansion.size());
	}

	if(name == "msu1/data.rom")
	{
		return vfs::fs::file::open({Location::notsuffix(superFamicom.location), ".msu"}, mode);
	}

	if(name.match("msu1/track*.pcm"))
	{
		name.trimLeft("msu1/track", 1L);
		return vfs::fs::file::open({Location::notsuffix(superFamicom.location), name}, mode);
	}

	if(name == "save.ram")
	{
		string save_path;

		auto suffix = Location::suffix(base_name);
		auto base = Location::base(base_name.transform("\\", "/"));

		const char *save = nullptr;
		if (environ_cb && environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &save) && save)
			save_path = { string(save).transform("\\", "/"), "/", base.trimRight(suffix, 1L), ".srm" };
		else
			save_path = { base_name.trimRight(suffix, 1L), ".srm" };

		return vfs::fs::file::open(save_path, mode);
	}

	return {};
}

auto Program::openRomGameBoy(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
	if(name == "program.rom" && mode == vfs::file::mode::read)
	{
		return vfs::memory::file::open(gameBoy.program.data(), gameBoy.program.size());
	}

	if(name == "save.ram")
	{
		string save_path;

		auto suffix = Location::suffix(base_name);
		auto base = Location::base(base_name.transform("\\", "/"));

		const char *save = nullptr;
		if (environ_cb && environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &save) && save)
			save_path = { string(save).transform("\\", "/"), "/", base.trimRight(suffix, 1L), ".srm" };
		else
			save_path = { base_name.trimRight(suffix, 1L), ".srm" };

		return vfs::fs::file::open(save_path, mode);
	}

	if(name == "time.rtc")
	{
		string save_path;

		auto suffix = Location::suffix(base_name);
		auto base = Location::base(base_name.transform("\\", "/"));

		const char *save = nullptr;
		if (environ_cb && environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &save) && save)
			save_path = { string(save).transform("\\", "/"), "/", base.trimRight(suffix, 1L), ".rtc" };
		else
			save_path = { base_name.trimRight(suffix, 1L), ".rtc" };

		return vfs::fs::file::open(save_path, mode);
	}

	return {};
}

auto Program::openRomBSMemory(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
	if (name == "program.rom" && mode == vfs::file::mode::read)
	{
		return vfs::memory::file::open(bsMemory.program.data(), bsMemory.program.size());
	}

	if (name == "program.flash")
	{
		//writes are not flushed to disk
		return vfs::memory::file::open(bsMemory.program.data(), bsMemory.program.size());
	}

	return {};
}

auto Program::loadFile(string location) -> vector<uint8_t>
{
	if(Location::suffix(location).downcase() == ".zip") {
		Decode::ZIP archive;
		if(archive.open(location)) {
			for(auto& file : archive.file) {
				auto type = Location::suffix(file.name).downcase();
				if(type == ".sfc" || type == ".smc" || type == ".gb" || type == ".gbc" || type == ".bs" || type == ".st") {
					return archive.extract(file);
				}
			}
		}
	return {};
	}
	else if(Location::suffix(location).downcase() == ".7z") {
		return LZMA::extract(location);
	}
	else {
		return file::read(location);
	}
}

auto Program::loadSuperFamicom(string location) -> bool
{
	vector<uint8_t> rom;
	rom = loadFile(location);

	if(rom.size() < 0x8000) return false;

	// soft patching (copied from standalone target)
	// note: soft patching should be done via the libretro frontend
	//       so this is only a workaround until that is possible
    if(!superFamicom.patched) superFamicom.patched = applyPatchBPS(rom, location);
	// END OF soft patching (copied from standalone target)

	// setting override loading (copied from standalone target)
	if(location.endsWith("/")) {
		rso = file::read({location, "gamesettings.bso"});
	} else if(location.iendsWith(".zip")) {
		Decode::ZIP archive;
		if(archive.open(location)) {
			for(auto& file : archive.file) {
				if(file.name.iendsWith(".bso")) {
				rso = archive.extract(file);
				break;
				}
			}
		}
		if(!rso) rso = file::read({Location::path(location),
                  Location::prefix(Location::file(location)), ".bso"});
	} else {
		rso = file::read({Location::path(location),
         Location::prefix(Location::file(location)), ".bso"});
	}
	// END OF setting override loading (copied from standalone target)

	if((rom.size() & 0x7fff) == 512) {
		//remove copier header
		memory::move(&rom[0], &rom[512], rom.size() - 512);
		rom.resize(rom.size() - 512);
	}

	auto heuristics = Heuristics::SuperFamicom(rom, location);
	auto sha256 = Hash::SHA256(rom).digest();

	superFamicom.title = heuristics.title();
	superFamicom.region = heuristics.videoRegion();
	superFamicom.manifest = heuristics.manifest();

	hackPatchMemory(rom);
	superFamicom.document = BML::unserialize(superFamicom.manifest);
	superFamicom.location = location;

	uint offset = 0;
	if(auto size = heuristics.programRomSize()) {
		superFamicom.program.resize(size);
		memory::copy(&superFamicom.program[0], &rom[offset], size);
		offset += size;
	}
	if(auto size = heuristics.dataRomSize()) {
		superFamicom.data.resize(size);
		memory::copy(&superFamicom.data[0], &rom[offset], size);
		offset += size;
	}
	if(auto size = heuristics.expansionRomSize()) {
		superFamicom.expansion.resize(size);
		memory::copy(&superFamicom.expansion[0], &rom[offset], size);
		offset += size;
	}
	if(auto size = heuristics.firmwareRomSize()) {
		superFamicom.firmware.resize(size);
		memory::copy(&superFamicom.firmware[0], &rom[offset], size);
		offset += size;
	}
	return true;
}

auto Program::loadGameBoy(string location) -> bool {
	vector<uint8_t> rom;
	rom = loadFile(location);

	if (rom.size() < 0x4000) return false;

	auto heuristics = Heuristics::GameBoy(rom, location);
	auto sha256 = Hash::SHA256(rom).digest();

	gameBoy.manifest = heuristics.manifest();
	gameBoy.document = BML::unserialize(gameBoy.manifest);
	gameBoy.location = location;
	gameBoy.program = rom;

	return true;
}

auto Program::loadBSMemory(string location) -> bool {
	vector<uint8_t> rom;
	rom = loadFile(location);

	if (rom.size() < 0x8000) return false;

	auto heuristics = Heuristics::BSMemory(rom, location);
	auto sha256 = Hash::SHA256(rom).digest();

	bsMemory.manifest = heuristics.manifest();
	bsMemory.document = BML::unserialize(bsMemory.manifest);
	bsMemory.location = location;

	bsMemory.program = rom;
	return true;
}

auto Program::hackPatchMemory(vector<uint8_t>& data) -> void
{
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

auto decodeSNES(string& code) -> bool {
  //Game Genie
  if(code.size() == 9 && code[4u] == '-') {
    //strip '-'
    code = {code.slice(0, 4), code.slice(5, 4)};
    //validate
    for(uint n : code) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //decode
    code.transform("df4709156bc8a23e", "0123456789abcdef");
    uint32_t r = toHex(code);
    //abcd efgh ijkl mnop qrst uvwx
    //ijkl qrst opab cduv wxef ghmn
    uint address =
      (!!(r & 0x002000) << 23) | (!!(r & 0x001000) << 22)
    | (!!(r & 0x000800) << 21) | (!!(r & 0x000400) << 20)
    | (!!(r & 0x000020) << 19) | (!!(r & 0x000010) << 18)
    | (!!(r & 0x000008) << 17) | (!!(r & 0x000004) << 16)
    | (!!(r & 0x800000) << 15) | (!!(r & 0x400000) << 14)
    | (!!(r & 0x200000) << 13) | (!!(r & 0x100000) << 12)
    | (!!(r & 0x000002) << 11) | (!!(r & 0x000001) << 10)
    | (!!(r & 0x008000) <<  9) | (!!(r & 0x004000) <<  8)
    | (!!(r & 0x080000) <<  7) | (!!(r & 0x040000) <<  6)
    | (!!(r & 0x020000) <<  5) | (!!(r & 0x010000) <<  4)
    | (!!(r & 0x000200) <<  3) | (!!(r & 0x000100) <<  2)
    | (!!(r & 0x000080) <<  1) | (!!(r & 0x000040) <<  0);
    uint data = r >> 24;
    code = {hex(address, 6L), "=", hex(data, 2L)};
    return true;
  }

  //Pro Action Replay
  if(code.size() == 8) {
    //validate
    for(uint n : code) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //decode
    uint32_t r = toHex(code);
    uint address = r >> 8;
    uint data = r & 0xff;
    code = {hex(address, 6L), "=", hex(data, 2L)};
    return true;
  }

  //higan: address=data
  if(code.size() == 9 && code[6u] == '=') {
    string nibbles = {code.slice(0, 6), code.slice(7, 2)};
    //validate
    for(uint n : nibbles) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //already in decoded form
    return true;
  }

  //higan: address=compare?data
  if(code.size() == 12 && code[6u] == '=' && code[9u] == '?') {
    string nibbles = {code.slice(0, 6), code.slice(7, 2), code.slice(10, 2)};
    //validate
    for(uint n : nibbles) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //already in decoded form
    return true;
  }

  //unrecognized code format
  return false;
}

auto decodeGB(string& code) -> bool {
  auto nibble = [&](const string& s, uint index) -> uint {
    if(index >= s.size()) return 0;
    if(s[index] >= '0' && s[index] <= '9') return s[index] - '0';
    return s[index] - 'a' + 10;
  };

  //Game Genie
  if(code.size() == 7 && code[3u] == '-') {
    code = {code.slice(0, 3), code.slice(4, 3)};
    //validate
    for(uint n : code) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    uint data = nibble(code, 0) << 4 | nibble(code, 1) << 0;
    uint address = (nibble(code, 5) ^ 15) << 12 | nibble(code, 2) << 8 | nibble(code, 3) << 4 | nibble(code, 4) << 0;
    code = {hex(address, 4L), "=", hex(data, 2L)};
    return true;
  }

  //Game Genie
  if(code.size() == 11 && code[3u] == '-' && code[7u] == '-') {
    code = {code.slice(0, 3), code.slice(4, 3), code.slice(8, 3)};
    //validate
    for(uint n : code) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    uint data = nibble(code, 0) << 4 | nibble(code, 1) << 0;
    uint address = (nibble(code, 5) ^ 15) << 12 | nibble(code, 2) << 8 | nibble(code, 3) << 4 | nibble(code, 4) << 0;
    uint8_t t = nibble(code, 6) << 4 | nibble(code, 8) << 0;
    t = t >> 2 | t << 6;
    uint compare = t ^ 0xba;
    code = {hex(address, 4L), "=", hex(compare, 2L), "?", hex(data, 2L)};
    return true;
  }

  //GameShark
  if(code.size() == 8) {
    //validate
    for(uint n : code) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //first two characters are the code type / VRAM bank, which is almost always 01.
    //other values are presumably supported, but I have no info on them, so they're not supported.
    if(code[0u] != '0') return false;
    if(code[1u] != '1') return false;
    uint data = toHex(code.slice(2, 2));
    uint16_t address = toHex(code.slice(4, 4));
    address = address >> 8 | address << 8;
    code = {hex(address, 4L), "=", hex(data, 2L)};
    return true;
  }

  //higan: address=data
  if(code.size() == 7 && code[4u] == '=') {
    string nibbles = {code.slice(0, 4), code.slice(5, 2)};
    //validate
    for(uint n : nibbles) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //already in decoded form
    return true;
  }

  //higan: address=compare?data
  if(code.size() == 10 && code[4u] == '=' && code[7u] == '?') {
    string nibbles = {code.slice(0, 4), code.slice(5, 2), code.slice(8, 2)};
    //validate
    for(uint n : nibbles) {
      if(n >= '0' && n <= '9') continue;
      if(n >= 'a' && n <= 'f') continue;
      return false;
    }
    //already in decoded form
    return true;
  }

  //unrecognized code format
  return false;
}

// soft patching (copied from standalone target), note: soft patching should be done via the libretro frontend, so this is only a workaround until that is possible
auto Program::applyPatchBPS(vector<uint8_t>& input, string location) -> bool {
  vector<uint8_t> patch;

  if(location.endsWith("/")) {
    patch = file::read({location, "patch.bps"});
  } else if(location.iendsWith(".zip")) {
    Decode::ZIP archive;
    if(archive.open(location)) {
      for(auto& file : archive.file) {
        if(file.name.iendsWith(".bps")) {
          patch = archive.extract(file);
          break;
        }
      }
    }
    if(!patch) patch = file::read({Location::path(location),
                  Location::prefix(Location::file(location)), ".bps"});
  } else {
    patch = file::read({Location::path(location),
       Location::prefix(Location::file(location)), ".bps"});
  }

  if(!patch) return false;

  string manifest;
  string error;
  if(auto output = Beat::Single::apply(input, patch, manifest, error)) {
    if(!error) {
      input = move(*output);
      return true;
    }
  }

  return false;
}
// END OF soft patching (copied from standalone target)
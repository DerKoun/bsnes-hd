#pragma once

#include <nall/vfs/vfs.hpp>
#include "libretro.h"

extern struct retro_vfs_interface *libretro_vfs_interface;

namespace nall::vfs::libretro {

struct file : nall::vfs::file {
  static auto openTyped(string location, nall::vfs::file::mode mode) -> shared_pointer<file> {
    if(!libretro_vfs_interface || !libretro_vfs_interface->open) return {};

    unsigned access = RETRO_VFS_FILE_ACCESS_READ;
    switch(mode) {
    case nall::vfs::file::mode::read:
      access = RETRO_VFS_FILE_ACCESS_READ;
      break;
    case nall::vfs::file::mode::write:
      access = RETRO_VFS_FILE_ACCESS_WRITE;
      break;
    case nall::vfs::file::mode::modify:
      access = RETRO_VFS_FILE_ACCESS_READ_WRITE | RETRO_VFS_FILE_ACCESS_UPDATE_EXISTING;
      break;
    case nall::vfs::file::mode::create:
      access = RETRO_VFS_FILE_ACCESS_READ_WRITE;
      break;
    }

    auto handle = libretro_vfs_interface->open((const char*)location, access, RETRO_VFS_FILE_ACCESS_HINT_NONE);
    if(!handle) return {};

    auto instance = shared_pointer<file>{new file};
    instance->_handle = handle;
    return instance;
  }

  static auto open(string location, nall::vfs::file::mode mode) -> shared_pointer<nall::vfs::file> {
    if(auto instance = openTyped(location, mode)) return instance;
    return nall::vfs::fs::file::open(location, mode);
  }

  ~file() {
    if(_handle && libretro_vfs_interface && libretro_vfs_interface->close) {
      libretro_vfs_interface->close(_handle);
    }
  }

  auto size() const -> uintmax override {
    if(!_handle || !libretro_vfs_interface->size) return 0;
    auto result = libretro_vfs_interface->size(_handle);
    return result < 0 ? 0 : (uintmax)result;
  }

  auto offset() const -> uintmax override {
    if(!_handle || !libretro_vfs_interface->tell) return 0;
    auto result = libretro_vfs_interface->tell(_handle);
    return result < 0 ? 0 : (uintmax)result;
  }

  auto seek(intmax offset_, nall::vfs::file::index index_) -> void override {
    if(!_handle || !libretro_vfs_interface->seek) return;
    int origin = RETRO_VFS_SEEK_POSITION_START;
    if(index_ == nall::vfs::file::index::relative) origin = RETRO_VFS_SEEK_POSITION_CURRENT;
    libretro_vfs_interface->seek(_handle, (int64_t)offset_, origin);
  }

  auto read() -> uint8_t override {
    uint8_t data = 0;
    if(_handle && libretro_vfs_interface->read) {
      libretro_vfs_interface->read(_handle, &data, 1);
    }
    return data;
  }

  auto write(uint8_t data_) -> void override {
    if(_handle && libretro_vfs_interface->write) {
      libretro_vfs_interface->write(_handle, &data_, 1);
    }
  }

  auto flush() -> void override {
    if(_handle && libretro_vfs_interface->flush) {
      libretro_vfs_interface->flush(_handle);
    }
  }

  auto readAll() -> vector<uint8_t> {
    vector<uint8_t> data;
    if(!_handle) return data;
    auto length = size();
    if(!length) return data;
    data.resize(length);
    seek(0, nall::vfs::file::index::absolute);
    if(libretro_vfs_interface->read) {
      auto got = libretro_vfs_interface->read(_handle, data.data(), length);
      if(got < 0 || (uintmax)got != length) data.resize(got > 0 ? (uintmax)got : 0);
    }
    return data;
  }

private:
  file() = default;
  file(const file&) = delete;
  auto operator=(const file&) -> file& = delete;

  struct retro_vfs_file_handle* _handle = nullptr;
};

}

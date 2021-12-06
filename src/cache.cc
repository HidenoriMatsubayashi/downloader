#include "cache.h"

Cache::~Cache() {
  Close();
}

bool Cache::Open(std::string& filename) {
  ofs_ = std::make_unique<std::ofstream>(filename);
  if (!ofs_ || ofs_->bad()) {
    std::cerr << "Failed to open " << filename << std::endl;
    return false;
  }
  return true;
}

bool Cache::Close() {
  if (ofs_) {
    ofs_->close();
    ofs_ = nullptr;
  }
  return true;
}

bool Cache::Write(char* buffer, size_t size) {
  if (!ofs_) {
    std::cerr << "The output file isn't opened." << std::endl;
    return false;
  }
  ofs_->write(buffer, size);
  ofs_->flush();
  return true;
}

int32_t Cache::UpdateProgress(curl_off_t dltotal, curl_off_t dlnow) {
  auto progress = static_cast<int>(100.0 * dlnow / dltotal);
  return progress;
}

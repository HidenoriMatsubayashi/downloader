#ifndef DOWNLOADER_CACHE_H_
#define DOWNLOADER_CACHE_H_

#include <curl/curl.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

class Cache {
 public:
  Cache() = default;
  ~Cache();

  bool Open(std::string& filename);
  bool Close();
  bool Write(char* buffer, size_t size);
  int32_t UpdateProgress(curl_off_t dltotal, curl_off_t dlnow);

 private:
  std::unique_ptr<std::ofstream> ofs_ = nullptr;
};

#endif  // DOWNLOADER_CACHE_H_

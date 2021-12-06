#ifndef DOWNLOADER_DOWNLOADER_H_
#define DOWNLOADER_DOWNLOADER_H_

#include <curl/curl.h>

#include <memory>
#include <string>

#include "cache.h"
#include "downloader_stream_handler.h"

class Downloader {
 public:
  Downloader();
  ~Downloader();

  void SetHandler(std::unique_ptr<DownloaderStreamHandler> handler);
  bool StartDownload(std::string src_uri, std::string dest_filename);
  bool CancelDownload();
  bool DeleteDownloadFile();

 private:
  static int CurlProgressCallback(void* clientp,
                                  curl_off_t dltotal,
                                  curl_off_t dlnow,
                                  curl_off_t ultotal,
                                  curl_off_t ulnow);
  static size_t CurlWriteBufferCallback(char* ptr,
                                        size_t size,
                                        size_t nmemb,
                                        void* userdata);

  CURL* curl_;
  std::unique_ptr<Cache> cache_;
  std::unique_ptr<DownloaderStreamHandler> handler_;
  int32_t progress_;
};

#endif  // DOWNLOADER_DOWNLOADER_H_

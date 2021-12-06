#include "downloader.h"

#include <iostream>

Downloader::Downloader() {
  // init libcurl
  curl_global_init(CURL_GLOBAL_ALL);

  // init the curl session
  curl_ = curl_easy_init();
  if (!curl_) {
    std::cerr << "Failed to init libcurl." << std::endl;
  }
}

Downloader::~Downloader() {
  CancelDownload();
  curl_global_cleanup();
}

void Downloader::SetHandler(std::unique_ptr<DownloaderStreamHandler> handler) {
  handler_ = std::move(handler);
}

bool Downloader::StartDownload(std::string src_uri, std::string dest_filename) {
  progress_ = -1;
  cache_ = std::make_unique<Cache>();
  if (!cache_->Open(dest_filename)) {
    if (handler_) {
      handler_->OnError();
    }
    return false;
  }

  curl_easy_setopt(curl_, CURLOPT_URL, src_uri.c_str());
  curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0);

  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, CurlWriteBufferCallback);
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, this);

  // enable progress meter
  curl_easy_setopt(curl_, CURLOPT_NOPROGRESS, 0);
  curl_easy_setopt(curl_, CURLOPT_XFERINFOFUNCTION, CurlProgressCallback);
  curl_easy_setopt(curl_, CURLOPT_XFERINFODATA, this);

  auto res = curl_easy_perform(curl_);
  if (res != CURLE_OK) {
    std::cerr << "curl_easy_perform failed: " << curl_easy_strerror(res)
              << std::endl;
    curl_easy_cleanup(curl_);
    if (handler_) {
      handler_->OnError();
    }
    return false;
  }
  curl_easy_cleanup(curl_);

  return true;
}

bool Downloader::CancelDownload() {
  return true;
}

bool Downloader::DeleteDownloadFile() {
  return true;
}

// static
int Downloader::CurlProgressCallback(void* clientp,
                                     curl_off_t dltotal,
                                     curl_off_t dlnow,
                                     curl_off_t ultotal,
                                     curl_off_t ulnow) {
  auto self = reinterpret_cast<Downloader*>(clientp);
  if (!self || dltotal == 0) {
    return 0;
  }

  auto progress = self->cache_->UpdateProgress(dltotal, dlnow);
  if (self->progress_ != progress) {
    self->progress_ = progress;
    if (self->handler_) {
      self->handler_->OnUpdateProgress(progress);
      if (progress == 100) {
        self->handler_->OnComplete();
      }
    }
  }
  return 0;
}

// static
size_t Downloader::CurlWriteBufferCallback(char* buffer,
                                           size_t size,
                                           size_t nmemb,
                                           void* userdata) {
  if (!buffer) {
    std::cerr << "DownloadWriteData::buffer is null" << std::endl;
    return 0;
  }

  if (!userdata) {
    std::cerr << "DownloadWriteData::userdata is null" << std::endl;
    return 0;
  }

  auto self = reinterpret_cast<Downloader*>(userdata);
  if (!self || !self->cache_->Write(buffer, size * nmemb)) {
    std::cerr << "Failed to write the data." << std::endl;
    return 0;
  }

  return size * nmemb;
}

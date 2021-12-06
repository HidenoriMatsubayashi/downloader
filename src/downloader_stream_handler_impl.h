#ifndef DOWNLOADER_DOWNLOADER_STREAM_HANDLER_IMPL_H_
#define DOWNLOADER_DOWNLOADER_STREAM_HANDLER_IMPL_H_

#include <functional>

#include "downloader_stream_handler.h"

using DonloaderStreamHandlerOnUpdateProgress =
    std::function<void(int32_t progress)>;
using DonloaderStreamHandlerOnComplete = std::function<void(void)>;
using DonloaderStreamHandlerOnError = std::function<void(void)>;

class DownloaderStreamHandlerImpl : public DownloaderStreamHandler {
 public:
  DownloaderStreamHandlerImpl(
      DonloaderStreamHandlerOnUpdateProgress on_update_progress,
      DonloaderStreamHandlerOnComplete on_complete,
      DonloaderStreamHandlerOnError on_error)
      : on_update_progress_(on_update_progress),
        on_complete_(on_complete),
        on_error_(on_error) {}
  ~DownloaderStreamHandlerImpl() = default;

 protected:
  void OnUpdateProgressInternal(int32_t progress) override {
    if (on_update_progress_) {
      on_update_progress_(progress);
    }
  }

  void OnCompleteInternal() override {
    if (on_complete_) {
      on_complete_();
    }
  }

  void OnErrorInternal() override {
    if (on_error_) {
      on_error_();
    }
  }

 private:
  DonloaderStreamHandlerOnUpdateProgress on_update_progress_;
  DonloaderStreamHandlerOnComplete on_complete_;
  DonloaderStreamHandlerOnError on_error_;
};

#endif  // DOWNLOADER_DOWNLOADER_STREAM_HANDLER_IMPL_H_

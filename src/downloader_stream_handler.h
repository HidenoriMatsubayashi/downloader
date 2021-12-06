#ifndef DOWNLOADER_DOWNLOADER_STREAM_HANDLER_H_
#define DOWNLOADER_DOWNLOADER_STREAM_HANDLER_H_

#include <iostream>

class DownloaderStreamHandler {
 public:
  DownloaderStreamHandler() = default;
  virtual ~DownloaderStreamHandler() = default;

  void OnUpdateProgress(int progress) { OnUpdateProgressInternal(progress); }
  void OnComplete() { OnCompleteInternal(); }
  void OnError() { OnErrorInternal(); }

 protected:
  virtual void OnUpdateProgressInternal(int32_t progress) = 0;
  virtual void OnCompleteInternal() = 0;
  virtual void OnErrorInternal() = 0;
};

#endif  // DOWNLOADER_DOWNLOADER_STREAM_HANDLER_H_

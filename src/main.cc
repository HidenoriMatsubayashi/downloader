#include <iostream>
#include <memory>

#include "downloader.h"
#include "downloader_stream_handler_impl.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0]
              << " <path_to_download_file> <path_to_output_file>" << std::endl;
    return 0;
  }

  auto downloader = std::make_unique<Downloader>();
  auto handler = std::make_unique<DownloaderStreamHandlerImpl>(
      [](int32_t progress) {
        std::cout << "Download progress: " << progress << "%" << std::endl;
      },
      []() { std::cout << "File download completed." << std::endl; },
      []() { std::cerr << "Error happened." << std::endl; });
  downloader->SetHandler(std::move(handler));
  downloader->StartDownload(argv[1], argv[2]);
  return 0;
}

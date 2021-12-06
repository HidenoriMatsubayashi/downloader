# What's downloader?
An file downloader example using `libcurl` for learning how to use `libcurl`.

## How to build

```shell
$ sudo apt install libcurl4-openssl-dev
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

## How to run

```shell
$ ./downloader \
    https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_cropped_multilingual.webm \
    ./sintel_cropped_multilingual.webm
```

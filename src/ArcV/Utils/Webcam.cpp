#include <cstring>
#include <fcntl.h>
#include <iostream>

#ifdef _WIN32
#include <Dshow.h>
#elif __gnu_linux__
#include <libv4l2.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
//#elif __ANDROID__
#endif

#include "ArcV/Utils/Webcam.hpp"

namespace Arcv {

Webcam::Webcam(unsigned int width, unsigned int height, uint8_t videoIndex) {
#ifdef __gnu_linux__
  if ((index = v4l2_open(("/dev/video" + std::to_string(videoIndex)).c_str(), O_RDWR | O_NONBLOCK, 0)) < 0)
    throw std::invalid_argument("Error: Failed to open webcam; does /dev/video" + std::to_string(videoIndex) + " exist?");

  v4l2_format format;
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  format.fmt.pix.width = width;
  format.fmt.pix.height = height;
  format.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
  format.fmt.pix.field = V4L2_FIELD_INTERLACED;

  checkIoCtl(VIDIOC_S_FMT, &format);

  if (format.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24)
    throw std::runtime_error("Error: v4l2 didn't accept RGB24 format");

  if (format.fmt.pix.width != width || format.fmt.pix.height != height)
    std::cerr << "Warning: Width and/or height ignored, frame size will be "
              << format.fmt.pix.width << 'x' << format.fmt.pix.height << std::endl;

  frameWidth = format.fmt.pix.width;
  frameHeight = format.fmt.pix.height;

  v4l2_requestbuffers req;
  req.count = 1;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  checkIoCtl(VIDIOC_REQBUFS, &req);

  v4l2_buffer frameBuffer;
  frameBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  frameBuffer.memory = V4L2_MEMORY_MMAP;
  frameBuffer.index = 0;

  checkIoCtl(VIDIOC_QUERYBUF, &frameBuffer);

  buffer.length = frameBuffer.length;
  buffer.start = v4l2_mmap(nullptr, frameBuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, index, frameBuffer.m.offset);

  if (buffer.start == MAP_FAILED)
    throw std::runtime_error("Error: Failed memory mapping on webcam instanciation");

  checkIoCtl(VIDIOC_QBUF, &frameBuffer);
#endif
}

Matrix<> Webcam::captureImage() const {
  Matrix<> res(frameWidth, frameHeight, 3, 8, ARCV_COLORSPACE_RGB);

#ifdef __gnu_linux__
  v4l2_buf_type bufferType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  checkIoCtl(VIDIOC_STREAMON, &bufferType);

  int selectRes = 0;
  do {
    fd_set set;

    FD_ZERO(&set);
    FD_SET(index, &set);

    selectRes = select(index + 1, &set, nullptr, nullptr, nullptr);
  } while (selectRes == -1 && (errno = EINTR));

  if (selectRes == -1)
    throw std::runtime_error("Error: Select failed on webcam instanciation");

  v4l2_buffer frameBuffer;
  frameBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  frameBuffer.memory = V4L2_MEMORY_MMAP;

  checkIoCtl(VIDIOC_DQBUF, &frameBuffer);

  std::copy(static_cast<char*>(buffer.start), static_cast<char*>(buffer.start) + buffer.length, res.getData().begin());

  checkIoCtl(VIDIOC_QBUF, &frameBuffer);
  checkIoCtl(VIDIOC_STREAMOFF, &bufferType);
#endif

  return res;
}

Webcam::~Webcam() {
#ifdef __gnu_linux__
  v4l2_munmap(buffer.start, buffer.length);
  v4l2_close(index);
#endif
}

#ifdef __gnu_linux__
void Webcam::checkIoCtl(unsigned long request, void* attribute) const {
  if (v4l2_ioctl(index, request, attribute) == -1)
    throw std::runtime_error("Error: " + std::string(std::strerror(errno)));
}
#endif

} // namespace Arcv

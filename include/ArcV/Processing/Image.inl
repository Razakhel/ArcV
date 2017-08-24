#include <numeric>
#include <cassert>

namespace Arcv {

template <typename T>
Matrix<T> Image::threshold(Matrix<T> mat, T firstLowerBound, T firstUpperBound,
                                          T secondLowerBound, T secondUpperBound,
                                          T thirdLowerBound, T thirdUpperBound) {
  assert(("Error: Input matrix should have 3 or 4 channels", mat.getChannelCount() == 3 || mat.getChannelCount() == 4));

  Matrix<T> res(mat.getWidth(), mat.getHeight(), 1, mat.getImgBitDepth(), ARCV_COLORSPACE_GRAY);
  std::size_t resIndex = 0;

  for (auto elt = mat.getData().begin(); elt != mat.getData().end(); elt += mat.getChannelCount(), ++resIndex)
    res[resIndex] = (*elt <= firstUpperBound && *elt >= firstLowerBound
                     && *(elt + 1) <= secondUpperBound && *(elt + 1) >= secondLowerBound
                     && *(elt + 2) <= thirdUpperBound && *(elt + 2) >= thirdLowerBound ? 255 : 0);

  return res;
}

template <typename T>
Matrix <T> Image::rotateLeft(const Matrix <T>& mat) {
  Matrix<T> res(mat.getHeight(), mat.getWidth(), mat.getChannelCount(), mat.getImgBitDepth(), mat.getColorspace());

  for (std::size_t hIndex = 0, resWIndex = 0; hIndex < mat.getHeight(), resWIndex < res.getWidth(); ++hIndex, ++resWIndex) {
    for (std::size_t wIndex = 0, resHIndex = res.getHeight() - 1; wIndex < mat.getWidth(), resHIndex > 0; ++wIndex, --resHIndex) {
      const std::size_t matIndex = (hIndex * mat.getWidth() + wIndex) * mat.getChannelCount();
      const std::size_t resIndex = (resHIndex * res.getWidth() + resWIndex) * res.getChannelCount();

      for (uint8_t chan = 0; chan < mat.getChannelCount(); ++chan)
        res[resIndex + chan] = mat[matIndex + chan];
    }
  }

  return res;
}

template <typename T>
Matrix <T> Image::rotateRight(const Matrix <T>& mat) {
  Matrix<T> res(mat.getHeight(), mat.getWidth(), mat.getChannelCount(), mat.getImgBitDepth(), mat.getColorspace());

  for (std::size_t hIndex = 0, resWIndex = res.getWidth(); hIndex < mat.getHeight(), resWIndex > 0; ++hIndex, --resWIndex) {
    for (std::size_t wIndex = 0, resHIndex = 0; wIndex < mat.getWidth(), resHIndex < res.getHeight(); ++wIndex, ++resHIndex) {
      const std::size_t matIndex = (hIndex * mat.getWidth() + wIndex) * mat.getChannelCount();
      const std::size_t resIndex = (resHIndex * res.getWidth() + resWIndex) * res.getChannelCount();

      for (uint8_t chan = 0; chan < mat.getChannelCount(); ++chan)
        res[resIndex + chan] = mat[matIndex + chan];
    }
  }

  return res;
}

template <typename T>
Matrix <T> Image::reverse(const Matrix <T>& mat) {
  Matrix<T> res(mat.getWidth(), mat.getHeight(), mat.getChannelCount(), mat.getImgBitDepth(), mat.getColorspace());

  for (std::size_t hIndex = 0, resHIndex = res.getHeight() - 1; hIndex < mat.getHeight(), resHIndex > 0; ++hIndex, --resHIndex) {
    for (std::size_t wIndex = 0, resWIndex = res.getWidth(); wIndex < mat.getWidth(), resWIndex > 0; ++wIndex, --resWIndex) {
      const std::size_t matIndex = (hIndex * mat.getWidth() + wIndex) * mat.getChannelCount();
      const std::size_t resIndex = (resHIndex * res.getWidth() + resWIndex) * res.getChannelCount();

      for (uint8_t chan = 0; chan < mat.getChannelCount(); ++chan)
        res[resIndex + chan] = mat[matIndex + chan];
    }
  }

  return res;
}

template <typename T>
Matrix<T> Image::horizontalFlip(const Matrix<T>& mat) {
  Matrix<T> res(mat.getWidth(), mat.getHeight(), mat.getChannelCount(), mat.getImgBitDepth(), mat.getColorspace());

  for (std::size_t hIndex = 0, resHIndex = 0; hIndex < mat.getHeight(), resHIndex < res.getHeight(); ++hIndex, ++resHIndex) {
    for (std::size_t wIndex = 0, resWIndex = res.getWidth() - 1; wIndex < mat.getWidth(), resWIndex > 0; ++wIndex, --resWIndex) {
      const std::size_t matIndex = (hIndex * mat.getWidth() + wIndex) * mat.getChannelCount();
      const std::size_t resIndex = (resHIndex * res.getWidth() + resWIndex) * res.getChannelCount();

      for (uint8_t chan = 0; chan < mat.getChannelCount(); ++chan)
        res[resIndex + chan] = mat[matIndex + chan];
    }
  }

  return res;
}

template <typename T>
Matrix<T> Image::verticalFlip(const Matrix<T>& mat) {
  Matrix<T> res(mat.getWidth(), mat.getHeight(), mat.getChannelCount(), mat.getImgBitDepth(), mat.getColorspace());

  for (std::size_t hIndex = 0, resHIndex = res.getHeight() - 1; hIndex < mat.getHeight(), resHIndex > 0; ++hIndex, --resHIndex) {
    for (std::size_t wIndex = 0, resWIndex = 0; wIndex < mat.getWidth(), resWIndex < res.getWidth(); ++wIndex, ++resWIndex) {
      const std::size_t matIndex = (hIndex * mat.getWidth() + wIndex) * mat.getChannelCount();
      const std::size_t resIndex = (resHIndex * res.getWidth() + resWIndex) * res.getChannelCount();

      for (uint8_t chan = 0; chan < mat.getChannelCount(); ++chan)
        res[resIndex + chan] = mat[matIndex + chan];
    }
  }

  return res;
}

template <typename T>
Matrix<T> Image::region(const Matrix<T>& mat, std::size_t widthBegin, std::size_t widthEnd,
                                              std::size_t heightBegin, std::size_t heightEnd) {
  assert(("Error: Beginning boundaries must be lower than ending ones", widthBegin < widthEnd && heightBegin < heightEnd));

  Matrix<T> res(widthEnd - widthBegin, heightEnd - heightBegin, mat.getChannelCount(), mat.getImgBitDepth(), mat.getColorspace());

  for (std::size_t hIndex = heightBegin, resHIndex = 0; hIndex < heightEnd, resHIndex < res.getHeight(); ++hIndex, ++resHIndex) {
    for (std::size_t wIndex = widthBegin, resWIndex = 0; wIndex < heightEnd, resWIndex < res.getWidth(); ++wIndex, ++resWIndex) {
      const std::size_t matIndex = (hIndex * mat.getWidth() + wIndex) * mat.getChannelCount();
      const std::size_t resIndex = (resHIndex * res.getWidth() + resWIndex) * res.getChannelCount();

      for (uint8_t chan = 0; chan < mat.getChannelCount(); ++chan)
        res[resIndex + chan] = mat[matIndex + chan];
    }
  }

  return res;
}

} // namespace Arcv

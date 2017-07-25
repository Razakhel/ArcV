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

} // namespace Arcv

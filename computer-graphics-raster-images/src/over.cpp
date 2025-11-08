#include "over.h"

void over(
  const std::vector<unsigned char> & A,
  const std::vector<unsigned char> & B,
  const int & width,
  const int & height,
  std::vector<unsigned char> & C)
{
  C.resize(A.size());
  ////////////////////////////////////////////////////////////////////////////
  for (int i = 0; i < width * height; ++i) {
    double alpha_a = A[i * 4 + 3] / 255.0;
    double alpha_b = B[i * 4 + 3] / 255.0;

    double alpha_c = alpha_a + alpha_b * (1.0 - alpha_a);

    if (alpha_c > 1e-9) {
        for (int j = 0; j < 3; ++j) {
            double color_a = A[i * 4 + j] / 255.0;
            double color_b = B[i * 4 + j] / 255.0;
            C[i * 4 + j] = ((color_a * alpha_a + color_b * alpha_b * (1.0 - alpha_a)) / alpha_c) * 255;
        }
    } else {
        for (int j = 0; j < 3; ++j) {
            C[i * 4 + j] = 0;
        }
    }
    C[i * 4 + 3] = alpha_c * 255;
  }
  ////////////////////////////////////////////////////////////////////////////
}

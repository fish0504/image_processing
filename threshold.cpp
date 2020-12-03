#include "simd_threshold.hpp"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <numeric>
#include <omp.h>
#include <type_traits>

namespace SIMD
{
namespace
{
constexpr int START = 0 * 800;
constexpr int STOP = 600 * 800;
constexpr std::size_t INTERVAL256 = sizeof(__m256) / sizeof(float);  // 8
constexpr std::size_t INTERVAL512 = sizeof(__m512) / sizeof(float);  // 16

union SIMD256 {
  __m256 f;
  __m256i i;
  __m256d d;
};
}  // namespace

inline void coreThreshold2Uchar(float* const float_ptr, __m256i* m256i_ptr, __m256 threshold, int i);


// 最強の二値化(less than AVX2)
void threshold2Uchar(const cv::Mat& float_image, cv::Mat& uchar_image, float val)
{
  float* float_ptr = reinterpret_cast<float*>(float_image.data);
  __m256i* m256i_ptr = reinterpret_cast<__m256i*>(uchar_image.data);
  __m256 threshold = _mm256_set1_ps(val);

  static constexpr int N = 8;
  for (int i = START; i < STOP / 32; i += N) {
    coreThreshold2Uchar(float_ptr, m256i_ptr, threshold, i + 0);
    coreThreshold2Uchar(float_ptr, m256i_ptr, threshold, i + 1);
    coreThreshold2Uchar(float_ptr, m256i_ptr, threshold, i + 2);
    coreThreshold2Uchar(float_ptr, m256i_ptr, threshold, i + 3);
    coreThreshold2Uchar(float_ptr, m256i_ptr, threshold, i + 4);
    coreThreshold2Uchar(float_ptr, m256i_ptr, threshold, i + 4);
    coreThreshold2Uchar(float_ptr, m256i_ptr, threshold, i + 5);
    coreThreshold2Uchar(float_ptr, m256i_ptr, threshold, i + 6);
    coreThreshold2Uchar(float_ptr, m256i_ptr, threshold, i + 7);
  }
}

inline void coreThreshold2Uchar(float* const float_ptr, __m256i* m256i_ptr, __m256 threshold, int i)
{
  __m256 tmp[4];
  tmp[0] = _mm256_load_ps(&float_ptr[32 * i + 8 * 0]);
  tmp[1] = _mm256_load_ps(&float_ptr[32 * i + 8 * 1]);
  tmp[2] = _mm256_load_ps(&float_ptr[32 * i + 8 * 2]);
  tmp[3] = _mm256_load_ps(&float_ptr[32 * i + 8 * 3]);

  __m256 flag_f[4];
  flag_f[0] = _mm256_cmp_ps(tmp[0], threshold, _CMP_GE_OQ);
  flag_f[1] = _mm256_cmp_ps(tmp[1], threshold, _CMP_GE_OQ);
  flag_f[2] = _mm256_cmp_ps(tmp[2], threshold, _CMP_GE_OQ);
  flag_f[3] = _mm256_cmp_ps(tmp[3], threshold, _CMP_GE_OQ);

  __m256i flag_i[4];
  static SIMD256 t;
  for (int j = 0; j < 4; ++j) {
    t.f = flag_f[j];
    flag_i[j] = t.i;
  }

  __m256i pre_merged[2];
  pre_merged[0] = _mm256_packs_epi32(flag_i[0], flag_i[1]);
  pre_merged[1] = _mm256_packs_epi32(flag_i[2], flag_i[3]);
  __m256i merged = _mm256_packs_epi16(pre_merged[0], pre_merged[1]);

  long for_swap[4];
  for (int j = 0; j < 4; ++j) {
    for_swap[j] = _mm256_extract_epi64(merged, j);
  }
  __m256i out = _mm256_set_epi64x(for_swap[3], for_swap[1], for_swap[2], for_swap[0]);
  out = _mm256_shuffle_epi32(out, _MM_SHUFFLE(3, 1, 2, 0));

  _mm256_store_si256(&m256i_ptr[i], out);
}

}  // namespace SIMD

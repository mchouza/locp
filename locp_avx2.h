// Copyright (c) 2016 Mariano M. Chouza

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#ifndef LOCP_AVX2_H
#define LOCP_AVX2_H

#include <stdio.h>
#include <x86intrin.h>

namespace locp
{
// Basic configuration with no quoting.
template <uint8_t col_delimiter_, uint8_t row_delimiter_> struct no_quote_policy
{
    // Column delimiter constant.
    static const uint8_t col_delimiter = col_delimiter_;

    // Row delimiter constant.
    static const uint8_t row_delimiter = row_delimiter_;

    // Finds the delimiters in a buffer and records its positions.
    static size_t find_delimiters(uint16_t *delimiter_pos,
                                  size_t delimiter_pos_size,
                                  const uint8_t *buffer, size_t buffer_len)
    {
        // input index
        size_t i = 0;
        // output index
        size_t j = 0;

        // initial section processing loop
        for (; (uintptr_t)&buffer[i] % 64 != 0; i++)
            if (buffer[i] == col_delimiter || buffer[i] == row_delimiter)
                delimiter_pos[j++] = i;

        // creates the masks
        __m256i col_mask = _mm256_set1_epi8(col_delimiter);
        __m256i row_mask = _mm256_set1_epi8(row_delimiter);

        // processes 64 bytes at a time
        for (; i + 64 < buffer_len; i += 64)
        {
            __m256i *b = (__m256i *)&buffer[i];
            __m256i col_matches_lo = _mm256_cmpeq_epi8(b[0], col_mask);
            __m256i row_matches_lo = _mm256_cmpeq_epi8(b[0], row_mask);
            __m256i all_matches_lo = _mm256_or_si256(col_matches_lo, row_matches_lo);
            uint32_t all_matches_lo_bits = _mm256_movemask_epi8(all_matches_lo);
            size_t all_matches_lo_count = __builtin_popcount(all_matches_lo_bits);
            for (size_t k = 0; k < all_matches_lo_count; k++)
            {
                delimiter_pos[j + k] = i + __builtin_ctz(all_matches_lo_bits);
                all_matches_lo_bits &= all_matches_lo_bits - 1;
            }
            __m256i col_matches_hi = _mm256_cmpeq_epi8(b[1], col_mask);
            __m256i row_matches_hi = _mm256_cmpeq_epi8(b[1], row_mask);
            __m256i all_matches_hi = _mm256_or_si256(col_matches_hi, row_matches_hi);
            uint32_t all_matches_hi_bits = _mm256_movemask_epi8(all_matches_hi);
            size_t all_matches_hi_count = __builtin_popcount(all_matches_hi_bits);
            for (size_t k = 0; k < all_matches_hi_count; k++)
            {
                delimiter_pos[j + all_matches_lo_count + k] = i + 32 + __builtin_ctz(all_matches_hi_bits);
                all_matches_hi_bits &= all_matches_hi_bits - 1;
            }
            j += all_matches_lo_count + all_matches_hi_count;
        }

        // final section processing loop
        for (; i < buffer_len; i++)
            if (buffer[i] == col_delimiter || buffer[i] == row_delimiter)
                delimiter_pos[j++] = i;
        delimiter_pos[j++] = buffer_len;

        // returns the number of delimiters
        return j;
    }
};
}

#endif

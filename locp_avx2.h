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
        for (; (uintptr_t)&buffer[i] % 32 != 0; i++)
            if (buffer[i] == col_delimiter || buffer[i] == row_delimiter)
                delimiter_pos[j++] = i;

        // creates the masks
        __m256i col_mask = _mm256_set1_epi8(col_delimiter);
        __m256i row_mask = _mm256_set1_epi8(row_delimiter);

        // processes 32 bytes at a time
        for (; i + 32 < buffer_len; i += 32)
        {
            __m256i *b = (__m256i *)&buffer[i];
            __m256i col_matches = _mm256_cmpeq_epi8(*b, col_mask);
            __m256i row_matches = _mm256_cmpeq_epi8(*b, row_mask);
            __m256i all_matches = _mm256_or_si256(col_matches, row_matches);
            int all_matches_bits = _mm256_movemask_epi8(all_matches);
            while (all_matches_bits != 0)
            {
                delimiter_pos[j++] = i + __builtin_ctzll(all_matches_bits);
                all_matches_bits &= all_matches_bits - 1;
            }
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

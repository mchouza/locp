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

#ifndef LOCP_BASE_H
#define LOCP_BASE_H

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

        // masks
        constexpr uint64_t mask_0x01 = ~(uint64_t)0 / 0xff;
        constexpr uint64_t mask_0x7f = 0x7f * mask_0x01;
        constexpr uint64_t mask_0x80 = 0x80 * mask_0x01;
        uint64_t col_delimiter_mask = col_delimiter * mask_0x01;
        uint64_t row_delimiter_mask = row_delimiter * mask_0x01;

        // initial section processing loop
        for (; (uintptr_t)&buffer[i] % 8 != 0; i++)
            if (buffer[i] == col_delimiter || buffer[i] == row_delimiter)
                delimiter_pos[j++] = i;

        // processes 8 bytes at a time
        for (; i + 7 < buffer_len; i += 8)
        {
            // based on
            // https://graphics.stanford.edu/~seander/bithacks.html#HasLessInWord

            // accesses the buffer as a 64 bit integer
            const uint64_t *b = (const uint64_t *)&buffer[i];

            // gets the matches as null bytes
            uint64_t col_matches_as_0x00 = *b ^ col_delimiter_mask;
            uint64_t row_matches_as_0x00 = *b ^ row_delimiter_mask;

            // leaves 0x80 in those positions
            uint64_t col_matches_as_0x80 =
                (mask_0x80 - (col_matches_as_0x00 & mask_0x7f)) &
                ~col_matches_as_0x00 & mask_0x80;
            uint64_t row_matches_as_0x80 =
                (mask_0x80 - (row_matches_as_0x00 & mask_0x7f)) &
                ~row_matches_as_0x00 & mask_0x80;
            uint64_t matches_as_0x80 =
                col_matches_as_0x80 | row_matches_as_0x80;

            // goes over the matches
            while (matches_as_0x80 != 0)
            {
                // stores the match position
                delimiter_pos[j++] = i + __builtin_ctzll(matches_as_0x80) / 8;

                // clears it
                matches_as_0x80 &= matches_as_0x80 - 1;
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

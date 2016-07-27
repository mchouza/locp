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

#ifndef LOCP_H
#define LOCP_H

#include <climits>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

#if __AVX2__
#include "locp_avx2.h"
#elif __SSE2__
#include "locp_sse2.h"
#else
#include "locp_base.h"
#endif

namespace locp
{
namespace
{
// Does its best to fill a buffer.
size_t read_all_(uint8_t *buffer, size_t buffer_size, int fd)
{
    if (buffer_size > SSIZE_MAX)
        throw std::out_of_range("Buffer size out of range.");
    uint8_t *p = buffer;
    uint8_t *end = buffer + buffer_size;
    while (p < end)
    {
        ssize_t read_bytes = ::read(fd, p, end - p);
        if (read_bytes < 0)
            throw std::runtime_error("Unexpected error when reading file.");
        if (read_bytes == 0)
            break;
        p += read_bytes;
    }
    return p - buffer;
}
}

// CSV parser class.
template <typename policy = no_quote_policy<',', '\n'>> class CSVParser
{
    // Buffer.
    uint8_t buffer_[(1 << 16) - 1];

    // Number of valid elements in the buffer.
    size_t buffer_len_;

    // Position of the delimiters inside the buffer.
    uint16_t delimiter_pos_[1 << 16];

    // Number of valid elements in the delimiter positions array.
    size_t delimiter_pos_len_;

    // Current field number inside the buffer.
    size_t field_num_;

    // More data available flag.
    bool more_data_available_;

    // File descriptor.
    int fd_;

    // Fills the internal buffer and finds the delimiters.
    void fill_and_delimit_()
    {
        size_t pos = field_num_ == 0 ? 0 : delimiter_pos_[field_num_ - 1] + 1;
        size_t num_remaining_bytes = buffer_len_ - pos;
        ::memmove(buffer_, buffer_ + pos, num_remaining_bytes);
        size_t read_bytes =
            read_all_(buffer_ + num_remaining_bytes,
                      sizeof(buffer_) - num_remaining_bytes, fd_);
        buffer_len_ = num_remaining_bytes + read_bytes;
        delimiter_pos_len_ = policy::find_delimiters(
            delimiter_pos_, sizeof(delimiter_pos_), buffer_, buffer_len_);
        field_num_ = 0;
        more_data_available_ = (buffer_len_ == sizeof(buffer_));
    }

  public:
    // Disables copying.
    CSVParser(const CSVParser &) = delete;
    CSVParser &operator=(const CSVParser &) = delete;

    // Constructs the CSV parser from a CSV file path.
    explicit CSVParser(const char *csv_path)
        : buffer_len_(0), delimiter_pos_len_(0), field_num_(0),
          more_data_available_(true), fd_(-1)
    {
        fd_ = ::open(csv_path, O_RDONLY);
        if (fd_ < 0)
            throw std::runtime_error("Could not open the input file.");
        fill_and_delimit_();
    }

    // Destructs the CSV parser.
    ~CSVParser() {}

    // Gets the next field delimiters, following the usual [start, end)
    // convention and returning false if there are no more fields.
    bool get_next_field(const uint8_t *&field_start, const uint8_t *&field_end)
    {
        // if the next field starts at the end of the buffer, there are no more
        // fields
        if (field_num_ == delimiter_pos_len_)
            return false;

        // if the next field ends delimited by the end of the buffer, there is
        // no more data, the field has length 0 and the last delimite is a row
        // delimiter, there are no more fields
        if (field_num_ + 1 == delimiter_pos_len_ && !more_data_available_ &&
            delimiter_pos_[field_num_ - 1] + 1 == delimiter_pos_[field_num_] &&
            buffer_[delimiter_pos_[field_num_ - 1]] == policy::row_delimiter)
            return false;

        // if the next field ends delimited by the end of the buffer, we refill
        // the buffer if there is more data
        if (field_num_ + 1 == delimiter_pos_len_ && more_data_available_)
            fill_and_delimit_();

        // gets start & end
        field_start = field_num_ == 0
                          ? &buffer_[0]
                          : &buffer_[delimiter_pos_[field_num_ - 1] + 1];
        field_end = &buffer_[delimiter_pos_[field_num_]];

        // we successfully read a new field
        field_num_++;
        return true;
    }
};
}

#endif

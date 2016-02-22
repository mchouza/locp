#include "third-party/fast-cpp-csv-parser/csv.h"
#include <cstdio>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Syntax: %s <csv-file>\n", argv[0]);
        return 1;
    }
    try
    {
        io::CSVReader<6,io::trim_chars<>> r(argv[1]);
        const char *p1, *p2, *p3, *p4, *p5, *p6;
        size_t field_count = 0;
        while (r.read_row(p1, p2, p3, p4, p5, p6))
            field_count += 6;
        printf("%zu\n", field_count);
    }
    catch (const std::exception &e)
    {
        fprintf(stderr, "Error: %s\n", e.what());
        return 1;
    }
    return 0;
}

#include "Matrix.h"
#include <cstddef>
#include <string>
#include <utility>

struct PGMSpecification {
    std::string signature;
    std::string comment;
   std::size_t max_gray_scale;
   Matrix data;

   void ToPGM(const char* file_name) {
        FILE* file_ptr;

        file_ptr = std::fopen(file_name, "w");
        fprintf(file_ptr, "%s\n", signature.c_str());
        fprintf(file_ptr, "%s\n", comment.c_str());
        fprintf(file_ptr, "%d %d\n", data.cols, data.rows);
        fprintf(file_ptr, "%lu\n", max_gray_scale);

        int num_rows = 0; 
        for (int i = 0; i < data.cols * data.rows; i++)
        {
            fprintf(file_ptr, "%.10g\t", data.data[i]);
            num_rows += 1;

            if (num_rows == data.rows) {
                fprintf(file_ptr, "\n");
                num_rows = 0;
            }

        }

   };
};

inline auto MakePGM(std::string signature, std::string comment, std::size_t max_gray_scale, Matrix data) -> PGMSpecification {
    return PGMSpecification {
        std::move(signature),
        std::move(comment),
        max_gray_scale,
        std::move(data)
    };
}

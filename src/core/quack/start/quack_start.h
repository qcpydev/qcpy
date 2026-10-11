#include <complex>
#include <iostream>
#include <string>
#include <vector>
#pragma once

namespace QuackStart {

class QuackBootArgs {
public:
  QuackBootArgs(const QuackBootArgs &) = delete;
  QuackBootArgs &operator=(const QuackBootArgs &) = delete;

  static const QuackBootArgs &get(char **argv = nullptr) {
    static QuackBootArgs bootargs{argv};
    return bootargs;
  }

  const int &get_export_migrate_memfd() const { return export_migrate_memfd; }

private:
  const int export_migrate_memfd;

  explicit QuackBootArgs(char **argv)
      : export_migrate_memfd{
            std::move(std::stoi(argv[_export_migrate_memfd_idx]))} {}

  static constexpr int _export_migrate_memfd_idx = 1;
};

class QuackLinearAlgebra {
public:
  QuackLinearAlgebra(size_t row_size, size_t col_size)
      : matrix(row_size * col_size) {};

  void dot_product(QuackLinearAlgebra &matrix_one,
                   const QuackLinearAlgebra &matrix_two);

  void kronecker_product(QuackLinearAlgebra &matrix_one,
                         const QuackLinearAlgebra &matrix_two);

protected:
  mutable std::vector<std::complex<float>> matrix;

private:
  static constexpr size_t to_expand_matrix_by = 2;
};

class QuackUnitaryMatrix : public QuackLinearAlgebra {
public:
  QuackUnitaryMatrix() : QuackLinearAlgebra{base_size, base_size} {};
  QuackUnitaryMatrix operator*(const QuackUnitaryMatrix &to_dot_product);

private:
  static constexpr size_t base_size = 2;
};

class QuackStateVector : public QuackLinearAlgebra {
public:
  QuackStateVector()
      : QuackLinearAlgebra(vector_start_size, vector_col_size) {};

  QuackUnitaryMatrix operator*(const QuackStateVector &to_dot_product);

private:
  static constexpr size_t vector_start_size = 2;
  static constexpr size_t vector_col_size = 1;
};

}; // namespace QuackStart

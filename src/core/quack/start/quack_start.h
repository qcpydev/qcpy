#include <complex>
#include <qlog_entry.h>
#include <string>
#include <vector>

#ifndef QUACK_START_H
#define QUACK_START_H
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
      : matrix(row_size * col_size, 0.0) {};

  virtual QuackLinearAlgebra &
  operator*=(const QuackLinearAlgebra &to_multiply) {
    return *this;
  };

  virtual void kronecker_product(QuackLinearAlgebra &to_kronecker) = 0;

protected:
  std::vector<std::complex<float>> matrix;

private:
  static constexpr size_t to_expand_matrix_by = 2;
};

class QuackUnitaryMatrix : public QuackLinearAlgebra {
public:
  QuackUnitaryMatrix() : QuackLinearAlgebra{base_size, base_size} {
    matrix.at(0) = std::complex<float>(1, 0.0f);
    matrix.at(1) = std::complex<float>(0, 0.0f);
    matrix.at(2) = std::complex<float>(0, 0.0f);
    matrix.at(3) = std::complex<float>(1, 0.0f);
  };
  QuackUnitaryMatrix(qlog_entry_t &qlog_entry)
      : QuackLinearAlgebra{base_size, base_size} {
    if (qlog_entry.gate_name == GATE_HADAMARD) {
      float scale = 1.0f / std::sqrt(2.0f);
      matrix.at(0) = std::complex<float>(scale, 0.0f);
      matrix.at(1) = std::complex<float>(scale, 0.0f);
      matrix.at(2) = std::complex<float>(scale, 0.0f);
      matrix.at(3) = std::complex<float>(-scale, 0.0f);
    }
  };

  QuackUnitaryMatrix &operator*=(const QuackUnitaryMatrix &to_dot_product) {
    size_t rows{this->matrix.size() / 2};
    size_t cols{to_dot_product.matrix.size() / 2};

    assert(rows == cols);
    std::vector<std::complex<float>> dot_product_result(rows * cols, 0);

    for (int i = 0; i < rows; i++)
      for (int j = 0; j < rows; j++) {
        for (int k = 0; k < rows; k++)
          dot_product_result[i * rows + j] +=
              (this->matrix[i * rows + k] *
               to_dot_product.matrix[k * rows + j]);
      }

    this->matrix = dot_product_result;

    return *this;
  }

  void kronecker_product(QuackLinearAlgebra &to_kronecker) {}

private:
  static constexpr size_t base_size = 2;
};

class QuackStateVector : public QuackLinearAlgebra {
public:
  QuackStateVector()
      : QuackLinearAlgebra(vector_start_size, vector_col_size) {};

  QuackStateVector &operator*=(const QuackUnitaryMatrix &to_dot_product) {
    return *this;
  }

  void kronecker_product(QuackLinearAlgebra &to_kronecker) {}

private:
  static constexpr size_t vector_start_size = 2;
  static constexpr size_t vector_col_size = 1;
};

}; // namespace QuackStart
#endif

#include <stdexcept>
#include "matrix.h"

template<class FLOAT, size_t N>
SquareMatrix<FLOAT, N>::SquareMatrix() {
    for(size_t i = 0; i < N; i++) {
        matrix[i] = {{}};
    }
}

template<class FLOAT, size_t N>
SquareMatrix<FLOAT, N>::SquareMatrix(std::initializer_list<Vector<FLOAT, N>> values) {
    size_t i = 0;
    for (const auto& value : values) {
        matrix[i++] = value;
    }
}

template <class FLOAT, size_t N>
Vector<FLOAT, N>& SquareMatrix<FLOAT, N>::operator[](size_t i) {
    if (i >= N) {
        throw std::out_of_range("Index out of range");
    }
    return matrix[i];
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> SquareMatrix<FLOAT, N>::operator[](size_t i) const {
    if (i >= N) {
        throw std::out_of_range("Index out of range");
    }
    return matrix[i];
}

template <class FLOAT, size_t N>
FLOAT & SquareMatrix<FLOAT, N>::at(size_t row, size_t column) {
    if (row >= N || column >= N) {
        throw std::out_of_range("Index out of range");
    }

    return (matrix[column])[row];
}

template <class FLOAT, size_t N>
FLOAT SquareMatrix<FLOAT, N>::at(size_t row, size_t column) const {
    if (row >= N || column >= N) {
        throw std::out_of_range("Index out of range");
    }

    return (matrix[column])[row];
}

template <class FLOAT, size_t N>
Vector<FLOAT,N> SquareMatrix<FLOAT, N>::operator*(const Vector<FLOAT, N> vector) {
    Vector<FLOAT, N> result = {{}};
    for (size_t col = 0; col < N; col++) {
        for (size_t row = 0; row < N; row++) {
            result[col] += matrix[row][col] * vector[row];
        }
    }
    return result;
}

template <class FLOAT, size_t N>
SquareMatrix<FLOAT, N> operator*(const SquareMatrix<FLOAT, N> factor1, const SquareMatrix<FLOAT, N> factor2) {
    SquareMatrix<FLOAT, N> result;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            FLOAT sum = 0.0;
            for (size_t k = 0; k < N; ++k) {
                sum += factor1[k][i] * factor2[j][k];
            }
            result[j][i] = sum;
        }
    }
    return result;
}
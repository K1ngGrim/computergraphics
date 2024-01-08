#include <cassert>
#include "cmath"

template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N>::Vector() {

}


template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N>::Vector( std::initializer_list<FLOAT_TYPE> values ) {
    auto iterator = values.begin();
    for (size_t i = 0u; i < N; i++) {
        if ( iterator != values.end()) {
            vector[i] = *iterator++;
        } else {
            vector[i] = (i > 0 ? vector[i - 1] : 0.0);
        }
    }
}

template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N>::Vector(FLOAT_TYPE angle ) {
    *this = { static_cast<FLOAT_TYPE>( cos(angle) ), static_cast<FLOAT_TYPE>(sin(angle)) };
}

template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N> & Vector<FLOAT_TYPE, N>::operator+=(const Vector<FLOAT_TYPE, N> addend) {
    for (size_t i = 0u; i < N; i++) {
        vector[i] += addend.vector[i];
    }
    return *this;
}

template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N> & Vector<FLOAT_TYPE, N>::operator-=(const Vector<FLOAT_TYPE, N> minuend) {
    for (size_t i = 0u; i < N; i++) {
        vector[i] -= minuend.vector[i];
    }
    return *this;
}

template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N> & Vector<FLOAT_TYPE, N>::operator*=(const FLOAT_TYPE factor) {
    for (size_t i = 0u; i < N; i++) {
        vector[i] *= factor;
    }
    return *this;
}

template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N> & Vector<FLOAT_TYPE, N>::operator/=(const FLOAT_TYPE factor) {
    for (size_t i = 0u; i < N; i++) {
        vector[i] /= factor;
    }
    return *this;
}


template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N> operator*(FLOAT_TYPE scalar, Vector<FLOAT_TYPE, N> value) {
    Vector<FLOAT_TYPE, N> scalar_product = value;

    scalar_product *= scalar;

    return scalar_product;
}

template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N> operator+(const Vector<FLOAT_TYPE, N> value, const Vector<FLOAT_TYPE, N> addend) {
    Vector<FLOAT_TYPE, N> sum = value;
    sum += addend;
    return sum;
}

template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N> operator-(const Vector<FLOAT_TYPE, N> value, const Vector<FLOAT_TYPE, N> minuend) {
    Vector<FLOAT_TYPE, N> difference = value;
    difference -= minuend;
    return difference;
}

template <class FLOAT_TYPE, size_t N>
FLOAT_TYPE & Vector<FLOAT_TYPE, N>::operator[](std::size_t i) {
    return vector[i];
}

template <class FLOAT_TYPE, size_t N>
FLOAT_TYPE Vector<FLOAT_TYPE, N>::operator[](std::size_t i) const {
    return vector[i];
}

template<class FLOAT_TYPE, size_t N>
FLOAT_TYPE Vector<FLOAT_TYPE, N>::length() const {
    float sum = 0;
    for (size_t i = 0u; i < N; i++) {
        sum += pow(this->vector[i], 2);
    }

    return sqrtf(sum);
}

template<class FLOAT_TYPE, size_t N>
FLOAT_TYPE Vector<FLOAT_TYPE, N>::square_of_length() const {
    float length = this->length();
    return length * length;
}

template<class FLOAT_TYPE, size_t N>
FLOAT_TYPE operator*(Vector<FLOAT_TYPE, N> vector1, const Vector<FLOAT_TYPE, N> vector2) {
    float skalar = 0;

    for (size_t i = 0u; i < N; ++i) {
        skalar += vector1[i]*vector2[i];
    }
    return skalar;
}

template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, 3u> Vector<FLOAT_TYPE, N>::cross_product(const Vector<FLOAT_TYPE, 3u> v) const {
    assert(N >= 3u);
    return {this->vector[1] * v.vector[2] - this->vector[2] * v.vector[1],
            this->vector[0] * v.vector[2] - this->vector[2] * v.vector[0],
            this->vector[0] * v.vector[1] - this->vector[1] * v.vector[0] };
}

template <class FLOAT_TYPE, size_t N>
void Vector<FLOAT_TYPE, N>::normalize() {
    *this /= length(); //  +/- INFINITY if length is (near to) zero
}

template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N> Vector<FLOAT_TYPE, N>::get_reflective(Vector<FLOAT_TYPE, N> normal) const {
    assert(0.99999 < normal.square_of_length() && normal.square_of_length()  < 1.000001);
    return *this - static_cast<FLOAT_TYPE>(2.0) * (*this * normal ) * normal;
}

template <class FLOAT_TYPE, size_t N>
FLOAT_TYPE Vector<FLOAT_TYPE, N>::angle(size_t axis_1, size_t axis_2) const {
    Vector<FLOAT_TYPE, N> normalized = (1.0f / length()) * *this;
    return atan2( normalized[axis_2], normalized[axis_1] );
}
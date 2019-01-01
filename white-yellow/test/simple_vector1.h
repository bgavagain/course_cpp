#pragma once

#include <cstdlib>

template <typename T>
class SimpleVector {
public:
    SimpleVector() { data = nullptr; size_ = 0; capacity_ = 0; }
    explicit SimpleVector(size_t size) : size_(size), capacity_(size) { data = new T[size]; }
    ~SimpleVector() { delete[] data; }

    T& operator[](size_t index) { return data[index]; }

    T* begin() { return data; }
    T* end() { return data + size_; }

    size_t Size() const { return size_; }
    size_t Capacity() const { return capacity_; }
    void PushBack(const T& value) {
        if (size_ == capacity_) {
            capacity_ = capacity_ == 0 ? 1 : capacity_ * 2;
            T* tmp = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                tmp[i] = data[i];
            }
            if (data) {
                delete[] data;
            }
            data = tmp;
        }
        data[size_] = value;
        ++size_;
    }

private:
    size_t size_;
    size_t capacity_;
    T* data;
};
#pragma once

template<typename T>
class MyVector {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
public:
    MyVector() noexcept : data_(nullptr), size_(0), capacity_(0) {}

    explicit MyVector(std::size_t count) : data_(count ? new T[count] : nullptr), size_(count), capacity_(count) {}

    ~MyVector() {
        delete[] data_;
    }

    MyVector(const MyVector& other) : data_(other.capacity_ ? new T[other.capacity_] : nullptr), size_(other.size_), capacity_(other.capacity_) {
        for (std::size_t i = 0; i < size_; ++i) {
            *(data_ + i) = *(other.data_ + i);
        }
    }

    MyVector& operator=(const MyVector& other) {
        if (this == &other) {
            return *this;
        }

        T* new_data = other.capacity_ ? new T[other.capacity_] : nullptr;
        for (std::size_t i = 0; i < other.size_; ++i) {
            new_data[i] = other.data_[i];
        }

        delete[] data_;
        data_ = new_data;
        size_ = other.size_;
        capacity_ = other.capacity_;
        return *this;
    }

    MyVector(MyVector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        return *this;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void push_back(const T&& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = std::move(value);
    }

    void clear() noexcept {
        size_ = 0;
    }

    void reserve(std::size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }

        T* new_data = new T[new_capacity];
        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }

        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    std::size_t size() const noexcept {
        return size_;
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    T& operator[](std::size_t index) {
        return data_[index];
    }

    const T& operator[](const std::size_t index) const {
        return data_[index];
    }

};
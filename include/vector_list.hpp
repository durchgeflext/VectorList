/*
Created by FlyingLeek in 15/10/2025.
    Copyright (c) 2025 FlyingLeek


    GNU GENERAL PUBLIC LICENSE Version 3
*/


#pragma once

#include <ranges>
#include <vector>

template <class T>
class vector_list {

    struct block {
        private:
        std::vector<T> data;
        size_t m_size = 0;

        public:
        explicit block() = delete;

        explicit block(size_t size) {
            data.reserve(size);
        }

        [[nodiscard]] size_t size() const {
            return this->m_size;
        }

        [[nodiscard]] size_t capacity() const {
            return data.capacity();
        }
    };

    std::vector<block> vectorList;
    std::vector<size_t> blockOffsets;
    size_t m_size = 0;
    size_t m_capacity = 0;

    public:

    //TODO: All the types

    //=============================================================
    //    Member functions
    //=============================================================

    vector_list() {
        this->vectorList.emplace_back(4);
        this->blockOffsets.push_back(0);
        this->m_size       = 0;
        this->m_capacity   = 4;
    }

    vector_list(vector_list &vecL) = default;

    vector_list(vector_list &&vecL) = default;

    explicit vector_list(size_t size) {
        this->vectorList.emplace_back(size);
        this->blockOffsets.push_back(0);
        this->m_size       = size;
        this->m_capacity   = size;
    }

    vector_list(size_t size, const T& value) {
        this->vectorList.emplace_back(size, value);
        this->blockOffsets.push_back(0);
        this->m_size       = size;
        this->m_capacity   = size;
    }

    vector_list(size_t size, size_t capacity) {
        this->vectorList.emplace_back(capacity);
        this->blockOffsets.push_back(0);
        this->m_size = size;
        this->m_capacity = capacity;
    }

    vector_list& operator = (const vector_list &other) {
        //TODO:
    }

    vector_list& operator = (const vector_list &&other)  noexcept {
        //TODO:
    }

    vector_list& operator = (std::initializer_list<T> ilist) {
        //TODO:
    }

    void assign(size_t count, const T& value) {
        //TODO:
    }

    template <class InputIt>
    void assign(InputIt begin, InputIt end) {
        //TODO:
    }

    void assign(std::initializer_list<T> ilist) {
        //TODO:
    }

    template <std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_reference_t<R>, T>
    constexpr void assign_range(R && r) {
        //TODO:
    }

    //=============================================================
    //    Element Access
    //=============================================================

    T& at(size_t pos) {
        //TODO:
    }

    const T& at(size_t pos) const {
        //TODO:
    }

    T& operator[](size_t pos) {
        //TODO:
    }

    const T& operator[](size_t pos) const {
        //TODO:
    }

    T& front() {
        //TODO:
    }

    const T& front() const {
        //TODO:
    }

    T& back() {
        //TODO:
    }

    const T& back() const {
        //TODO:
    }

    T* data() {
        //TODO:
    }

    const T* data() const {
        //TODO:
    }

    //=============================================================
    //    Iterators
    //=============================================================

    //TODO: Iterators

    //=============================================================
    //    Capacity
    //=============================================================

    bool empty() const {
        //TODO:
    }

    size_t size() const {
        //TODO:
    }

    size_t max_size() const {
        //TODO:
    }

    void reserve(size_t new_cap) {
        //TODO:
    }

    constexpr size_t capacity() const noexcept {
        //TODO:
    }

    constexpr void shrink_to_fit() {
        //TODO:
    }

    void flatten() {
        //TODO:
    }

    //=============================================================
    //    Modifiers
    //=============================================================

    constexpr void clear() noexcept {
        //TODO:
    }

    //TODO: Insert and iterator stuff
    //TODO: insert_range and interator stuff
    //TODO: emplace
    //TODO: erase

    constexpr void push_back(const T& value) noexcept {
        //TODO:
    }

    constexpr void push_back(T&& value) noexcept {
        //TODO:
    }

    template<class... Args>
    constexpr T& emplace_back(Args&&... args) {
        //TODO:
    }

    template<std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_reference_t<R>, T>
    constexpr void append_range(R&& rg) {
        //TODO:
    }

    constexpr void pop_back() {
        //TODO:
    }

    constexpr void resize(size_t count) {
        //TODO:
    }

    constexpr void resize(size_t, const T& value) {
        //TODO:
    }

    constexpr void swap(vector_list& other) noexcept {
        //TODO:
    }
};

template<class T>
constexpr bool operator==(const vector_list<T>& lhs, const vector_list<T>& rhs) {
    //TODO:
}

template<class T>
constexpr bool operator<=>(const vector_list<T>& lhs, const vector_list<T>& rhs) {
    //TODO:
}

namespace std {
    template<class T, class Alloc>
    constexpr void swap(vector_list<T>& lhs, vector_list<T>& rhs) noexcept {
        //TODO:
    }

    template<class T, class Alloc, class U>
    constexpr size_t erase(vector_list<T>& vec, const T& value) {
        //TODO:
    }

    template<class T, class Alloc, class Pred>
    constexpr size_t erase_if(vector_list<T>& vec, Pred pred) {
        //TODO:
    }

}

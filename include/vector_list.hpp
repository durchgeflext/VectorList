/*
Created by FlyingLeek in 15/10/2025.
    Copyright (c) 2025 FlyingLeek


    GNU GENERAL PUBLIC LICENSE Version 3
*/


#pragma once

#include <cassert>
#include <ranges>
#include <vector>

template <class T, class Allocator = std::allocator<T>>
class vector_list {

    public:
    class iterator {
        vector_list* parent;
        size_t pos;

        public:
        constexpr iterator() noexcept : parent(nullptr), pos(0) {

        }

        constexpr iterator(vector_list* parent, const size_t pos) noexcept : parent(parent), pos(pos) {

        }


        constexpr T& operator*() const {
            return *parent->internal_at_ptr(pos);
        }

        constexpr T* operator->() const {
           return parent->internal_at_ptr(pos);
        }


        constexpr iterator& operator++() noexcept {
            ++pos;
            return *this;
        }

        constexpr iterator operator++(int) noexcept {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }


        constexpr iterator& operator--() noexcept {
            --pos;
            return *this;
        }

        constexpr iterator operator--(int) noexcept {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        constexpr iterator& operator+=(const ptrdiff_t n) noexcept {
            pos += n;
            return *this;
        }

        constexpr iterator& operator-=(const ptrdiff_t n) noexcept {
            pos -= n;
            return *this;
        }

        constexpr iterator operator+(const ptrdiff_t n) const noexcept {
            return iterator(parent, pos + n);
        }

        constexpr iterator operator-(const ptrdiff_t n) const noexcept {
            return iterator(parent, pos - n);
        }

        constexpr ptrdiff_t operator-(const iterator& other) const noexcept {
            return static_cast<ptrdiff_t>(pos) - static_cast<ptrdiff_t>(other.pos);
        }


        constexpr T& operator[](ptrdiff_t n) const {
            return *(*this + n);
        }


        constexpr bool operator==(const iterator& o) const noexcept {
            return parent == o.parent && pos == o.pos;
        }

        constexpr std::strong_ordering operator<=>(const iterator& o) const noexcept {
            return parent->internal_at_ptr(0) + pos <=> o.parent->internal_at_ptr(0) + o.pos;
        }
    };

    class const_iterator {
        const vector_list* parent;
        size_t pos;

        public:
        constexpr const_iterator() noexcept : parent(nullptr), pos(0) {

        }

        constexpr const_iterator(const vector_list* parent, const size_t pos) noexcept : parent(parent), pos(pos) {

        }

        constexpr explicit const_iterator(const iterator& it) noexcept :parent(it.parent), pos(it.pos) {

        }


        constexpr const T& operator*() const {
            return *(parent->internal_at_ptr(pos));
        }

        constexpr const T* operator->() const {
            return parent->internal_at_ptr(pos);
        }


        constexpr const_iterator& operator++() noexcept {
            ++pos;
            return *this;
        }

        constexpr const_iterator operator++(int) noexcept {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }


        constexpr const_iterator& operator--() noexcept {
            --pos;
            return *this;
        }

        constexpr const_iterator operator--(int) noexcept {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        constexpr const_iterator& operator+=(const ptrdiff_t n) noexcept {
            pos += n;
            return *this;
        }

        constexpr const_iterator& operator-=(ptrdiff_t n) noexcept {
            pos -= n;
            return *this;
        }

        constexpr const_iterator operator+(ptrdiff_t n) const noexcept {
            return iterator(parent, pos + n);
        }

        constexpr const_iterator operator-(ptrdiff_t n) const noexcept {
            return iterator(parent, pos - n);
        }

        constexpr ptrdiff_t operator-(const const_iterator& other) const noexcept {
            return static_cast<ptrdiff_t>(pos) - static_cast<ptrdiff_t>(other.pos);
        }


        constexpr const T& operator[](ptrdiff_t n) const {
            return *(*this + n);
        }


        constexpr bool operator==(const iterator& o) const noexcept {
            return parent == o.parent && pos == o.pos;
        }

        constexpr std::strong_ordering operator<=>(const iterator& o) const noexcept {
            return parent->internal_at_ptr(0) + pos <=> o.parent->internal_at_ptr(0) + o.pos;
        }
    };

    private:
    struct data_block {
        private:
        T* m_start;
        T* m_data_end;
        T* m_block_end;
        Allocator m_alloc;

        public:
        explicit data_block() = delete;

        explicit data_block(size_t size, Allocator& alloc = std::allocator<T>()) {
            m_alloc = alloc;
            m_start = m_alloc.allocate(size);
            m_data_end = m_start;
            m_block_end = m_start + size;
        }

        ~data_block() {
            m_alloc.deallocate(m_start);
        }

        [[nodiscard]] size_t size() const {
            return m_data_end - m_start;
        }

        [[nodiscard]] size_t capacity() const {
            return m_block_end - m_start;
        }

        constexpr T* ptr_at(size_t pos) noexcept {
            return m_start + pos;
        }
    };

    std::vector<data_block> m_vectorList;
    std::vector<size_t> m_blockOffsets;

    size_t m_size = 0;
    size_t m_capacity = 0;

    Allocator m_alloc;

    constexpr T* internal_at_ptr(size_t pos) noexcept {
        assert(pos < m_size);
        ptrdiff_t blockIdx = std::lower_bound(m_blockOffsets.begin(), m_blockOffsets.end(), pos) - m_blockOffsets.begin();
        return m_vectorList[blockIdx].ptr_at(pos);
    }

    constexpr const T* internal_at_ptr(size_t pos) const noexcept {
        assert(pos < m_size);
        ptrdiff_t blockIdx = std::lower_bound(m_blockOffsets.begin(), m_blockOffsets.end(), pos) - m_blockOffsets.begin();
        return m_vectorList[blockIdx].ptr_at(pos);
    }

    public:
    //=============================================================
    //    Member functions
    //=============================================================

    vector_list() {
        this->m_vectorList.emplace_back(4);
        this->m_blockOffsets.push_back(0);
        this->m_size       = 0;
        this->m_capacity   = 4;
    }

    vector_list(vector_list &vecL) = default;

    vector_list(vector_list &&vecL) = default;

    explicit vector_list(size_t size) {
        this->m_vectorList.emplace_back(size);
        this->m_blockOffsets.push_back(0);
        this->m_size       = size;
        this->m_capacity   = size;
    }

    vector_list(size_t size, const T& value) {
        this->m_vectorList.emplace_back(size, value);
        this->m_blockOffsets.push_back(0);
        this->m_size       = size;
        this->m_capacity   = size;
    }

    vector_list(size_t size, size_t capacity) {
        this->m_vectorList.emplace_back(capacity);
        this->m_blockOffsets.push_back(0);
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

    Allocator get_allocator() const {
        return m_alloc;
    }

    //=============================================================
    //    Element Access
    //=============================================================

    T& at(size_t pos) {
        if (pos >= this->m_size) {
            throw std::out_of_range("vector_list::at  access, which is " + std::to_string(pos) +
                                        " is greater or equal to the size, which is " + std::to_string(this->m_size));
        }
        return *internal_at_ptr(pos);
    }

    const T& at(size_t pos) const {
        if (pos >= this->m_size) {
            throw std::out_of_range("vector_list::at  access, which is " + std::to_string(pos) +
                                        " is greater or equal to the size, which is " + std::to_string(this->m_size));
        }
        return *internal_at_ptr(pos);
    }

    T& operator[](size_t pos) {
        return *internal_at_ptr(pos);
    }

    const T& operator[](size_t pos) const {
        return *internal_at_ptr(pos);
    }

    T& front() {
        return *internal_at_ptr(0);
    }

    const T& front() const {
        return *internal_at_ptr(0);
    }

    T& back() {
        return *internal_at_ptr(this->m_size - 1);
    }

    const T& back() const {
        return *internal_at_ptr(this->m_size - 1);
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

    [[nodiscard]] bool empty() const {
        return this->m_size == 0;
    }

    [[nodiscard]] size_t size() const {
        return this->m_size;
    }

    size_t max_size() const {
        //TODO:
    }

    void reserve(size_t new_cap) {
        //TODO:
    }

    [[nodiscard]] constexpr size_t capacity() const noexcept {
        return this->m_capacity;
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

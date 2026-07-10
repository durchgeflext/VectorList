/*
Created by FlyingLeek in 15/10/2025.
    Copyright (c) 2025 FlyingLeek


    GNU GENERAL PUBLIC LICENSE Version 3
*/


#pragma once

#include <cassert>
#include <memory>
#include <ranges>
#include <vector>

template <class T, class Allocator = std::allocator<T>>
class vector_list {

    public:
    //=============================================================
    //    Type Definitions
    //=============================================================
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;

    class iterator {
        vector_list* parent;
        size_type pos;

        public:
        constexpr iterator() noexcept : parent(nullptr), pos(0) {

        }

        constexpr iterator(vector_list* parent, const size_type pos) noexcept : parent(parent), pos(pos) {

        }


        constexpr reference operator*() const {
            return *parent->internal_at_ptr(pos);
        }

        constexpr value_type* operator->() const {
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

        constexpr iterator& operator+=(const difference_type n) noexcept {
            pos += n;
            return *this;
        }

        constexpr iterator& operator-=(const difference_type n) noexcept {
            pos -= n;
            return *this;
        }

        constexpr iterator operator+(const difference_type n) const noexcept {
            return iterator(parent, pos + n);
        }

        constexpr iterator operator-(const difference_type n) const noexcept {
            return iterator(parent, pos - n);
        }

        constexpr difference_type operator-(const iterator& other) const noexcept {
            return static_cast<difference_type>(pos) - static_cast<difference_type>(other.pos);
        }


        constexpr reference operator[](difference_type n) const {
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
        size_type pos;

        public:
        constexpr const_iterator() noexcept : parent(nullptr), pos(0) {

        }

        constexpr const_iterator(const vector_list* parent, const size_type pos) noexcept : parent(parent), pos(pos) {

        }

        constexpr explicit const_iterator(const iterator& it) noexcept :parent(it.parent), pos(it.pos) {

        }


        constexpr const_reference operator*() const {
            return *(parent->internal_at_ptr(pos));
        }

        constexpr const value_type* operator->() const {
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

        constexpr const_iterator& operator+=(const difference_type n) noexcept {
            pos += n;
            return *this;
        }

        constexpr const_iterator& operator-=(difference_type n) noexcept {
            pos -= n;
            return *this;
        }

        constexpr const_iterator operator+(difference_type n) const noexcept {
            return iterator(parent, pos + n);
        }

        constexpr const_iterator operator-(difference_type n) const noexcept {
            return iterator(parent, pos - n);
        }

        constexpr difference_type operator-(const const_iterator& other) const noexcept {
            return static_cast<difference_type>(pos) - static_cast<difference_type>(other.pos);
        }


        constexpr const_reference operator[](difference_type n) const {
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
        value_type* m_start;
        value_type* m_data_end;
        value_type* m_block_end;
        allocator_type m_alloc;

        public:
        explicit data_block() = delete;

        explicit data_block(size_type size, allocator_type& alloc = std::allocator<value_type>()) {
            m_alloc = alloc;
            m_start = m_alloc.allocate(size);
            m_data_end = m_start;
            m_block_end = m_start + size;
        }

        ~data_block() {
            m_alloc.deallocate(m_start, m_block_end - m_start);
        }

        [[nodiscard]] size_type size() const {
            return m_data_end - m_start;
        }

        [[nodiscard]] size_type capacity() const {
            return m_block_end - m_start;
        }

        constexpr value_type* ptr_at(size_type pos) noexcept {
            return m_start + pos;
        }

        constexpr void refit() noexcept {
            m_alloc.deallocate(m_data_end, m_block_end - m_data_end);
            m_block_end = m_data_end;
        }

        constexpr void swap(data_block& other) noexcept {
            std::swap(m_start, other.m_start);
            std::swap(m_data_end, other.m_data_end);
            std::swap(m_block_end,other.m_block_end);
            std::swap(m_alloc, other.m_alloc);
        }
    };

    std::vector<data_block> m_vectorList;
    std::vector<size_type> m_blockOffsets;

    size_type m_size = 0;
    size_type m_capacity = 0;

    allocator_type m_alloc;

    //TODO: Check if those functions compute the correct blockIndex
    constexpr value_type* internal_at_ptr(const size_type pos) noexcept {
        assert(pos < m_size);
        difference_type blockIdx = std::lower_bound(m_blockOffsets.begin(), m_blockOffsets.end(), pos) - m_blockOffsets.begin();
        return m_vectorList[blockIdx].ptr_at(pos - m_blockOffsets[blockIdx]);
    }

    constexpr const value_type* internal_at_ptr(const size_type pos) const noexcept {
        assert(pos < m_size);
        difference_type blockIdx = std::lower_bound(m_blockOffsets.begin(), m_blockOffsets.end(), pos) - m_blockOffsets.begin();
        return m_vectorList[blockIdx].ptr_at(pos - m_blockOffsets[blockIdx]);
    }

    constexpr void new_block() {
        this->m_blockOffsets.push_back(m_capacity);
        this->m_vectorList.emplace_back(m_capacity);
        this->m_capacity *= 2;
    }


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

    explicit vector_list(size_type size) {
        this->m_vectorList.emplace_back(size);
        this->m_blockOffsets.push_back(0);
        this->m_size       = size;
        this->m_capacity   = size;
    }

    vector_list(size_type size, const_reference value) {
        this->m_vectorList.emplace_back(size, value);
        this->m_blockOffsets.push_back(0);
        this->m_size       = size;
        this->m_capacity   = size;
    }

    vector_list(size_type size, size_type capacity) {
        this->m_vectorList.emplace_back(capacity);
        this->m_blockOffsets.push_back(0);
        this->m_size = size;
        this->m_capacity = capacity;
    }

    vector_list& operator = (const vector_list &other) {
        //TODO: Implement allocator behaviour
        this->m_vectorList = other.m_vectorList;
        this->m_blockOffsets = other.m_blockOffsets;
        this->m_capacity = other.m_capacity;
        this->m_size = other.m_size;
        return *this;
    }

    vector_list& operator = (vector_list &&other)  noexcept {
        //TODO: Implement allocator behaviour
        this->m_vectorList = std::move(other.m_vectorList);
        this->m_blockOffsets = std::move(other.m_blockOffsets);
        this->m_capacity = other.m_capacity;
        this->m_size = other.m_size;
        return *this;
    }

    vector_list& operator = (std::initializer_list<value_type> ilist) {
        //TODO: What happens to the allocator here?
        this->m_blockOffsets.clear();
        this->m_blockOffsets.push_back(0);

        this->m_vectorList.clear();
        this->m_vectorList.emplace_back(ilist.size());

        this->m_size = ilist.size();
        this->m_capacity = ilist.size();

        for (size_t i = 0; i < ilist.size(); i++) {
            this->internal_at_ptr(i) = ilist[i];
        }

        return *this;
    }

    void assign(size_type count, const_reference value) {
        resize(count);
        for (size_t i = 0; i < count; i++) {
            internal_at_ptr(i) = value;
        }
    }

    template <class InputIt>
    void assign(InputIt begin, InputIt end) {
        const auto count = static_cast<size_type>(end - begin);
        resize(count);
        for (size_t i = 0; i < count; i++) {
            internal_at_ptr(i) = *begin;
            std::advance(begin, 1);
        }
    }

    void assign(std::initializer_list<value_type> ilist) {
        const size_t count = ilist.size();
        for (size_t i = 0; i < count; i++) {
            internal_at_ptr(i) = ilist[i];
        }
    }

    template <std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    constexpr void assign_range(R && rg) {
        auto& beginIt = std::ranges::begin(rg);
        auto& endIt = std::ranges::end(rg);
        const auto count = static_cast<size_type>(endIt - beginIt);
        for (size_t i = 0; i < count; i++) {
            internal_at_ptr(i) = *beginIt;
            std::advance(beginIt, 1);
        }
    }

    allocator_type get_allocator() const {
        return m_alloc;
    }

    //=============================================================
    //    Element Access
    //=============================================================

    reference at(size_type pos) {
        if (pos >= this->m_size) {
            throw std::out_of_range("vector_list::at  access, which is " + std::to_string(pos) +
                                        " is greater or equal to the size, which is " + std::to_string(this->m_size));
        }
        return *internal_at_ptr(pos);
    }

    const_reference at(size_type pos) const {
        if (pos >= this->m_size) {
            throw std::out_of_range("vector_list::at  access, which is " + std::to_string(pos) +
                                        " is greater or equal to the size, which is " + std::to_string(this->m_size));
        }
        return *internal_at_ptr(pos);
    }

    reference operator[](size_type pos) {
        return *internal_at_ptr(pos);
    }

    const_reference operator[](size_type pos) const {
        return *internal_at_ptr(pos);
    }

    reference front() {
        return *internal_at_ptr(0);
    }

    const_reference front() const {
        return *internal_at_ptr(0);
    }

    reference back() {
        return *internal_at_ptr(this->m_size - 1);
    }

    const_reference back() const {
        return *internal_at_ptr(this->m_size - 1);
    }

    value_type* data() {
        // Returns a pointer to the first data_block (if not flattened before, this may produce UB)
#warning "Do not use before calling vector_list::flatten"
        return m_vectorList[0].ptr_at(0);
    }

    const value_type* data() const {
        // Returns a pointer to the first data_block (if not flattened before, this may produce UB)
#warning "Do not use before calling vector_list::flatten"
        return m_vectorList[0].ptr_at(0);
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

    [[nodiscard]] size_type size() const {
        return this->m_size;
    }

    [[nodiscard]] constexpr size_type max_size() const {
        return std::numeric_limits<difference_type>::max() / sizeof(value_type);
    }

    void reserve(size_type new_cap) {
        while (new_cap > m_capacity) {
            new_block();
        }
    }

    [[nodiscard]] constexpr size_type capacity() const noexcept {
        return this->m_capacity;
    }

    constexpr void shrink_to_fit() {
        if (m_size == m_capacity) return;

        while (m_vectorList.back().size() == 0) {
            m_vectorList.erase(m_vectorList.end());
        }

        m_vectorList.back().refit();
        m_capacity = m_size;
    }

    void flatten() {
        data_block flat(m_capacity);
        for (size_t i = 0; i < m_size; i++) {
            *flat.ptr_at(i) = *internal_at_ptr(i);
        }
        m_vectorList[0].swap(flat);
        m_vectorList.resize(1);
    }

    //=============================================================
    //    Modifiers
    //=============================================================

    constexpr void clear() noexcept {
        for (size_t i = 0; i < m_size; i++) {
            std::allocator_traits<allocator_type>::destroy(m_alloc, internal_at_ptr(i));
        }
        m_size = 0;
    }

    constexpr iterator insert(const_iterator pos, const_reference value) {
        //TODO: implement
        //std::vector does reallocation.
        //Would it make sense to just allocate a data block inbetween pos-1 and pos?
    }

    constexpr iterator insert(const_iterator pos, value_type&& value) {
        //TODO: implement
    }

    constexpr iterator insert(const_iterator pos, size_type count, const_reference value ) {
        //TODO: implement
    }

    template< class InputIt >
    constexpr iterator insert(const_iterator pos, InputIt first, InputIt last ) {
        //TODO: implement
    }

    constexpr iterator insert( const_iterator pos, std::initializer_list<T> ilist) {
        //TODO: implement
    }

    template <std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    constexpr iterator insert_range( const_iterator pos, R&& rg ) {
        //TODO: implement
    }


    template< class... Args >
    constexpr iterator emplace( const_iterator pos, Args&&... args) {
        //TODO: implement
    }

    iterator erase(iterator pos) {
        //TODO: implement
    }

    constexpr iterator erase(const_iterator pos) {
        //TODO: implement
    }

    iterator erase(iterator first, iterator last) {
        //TODO: implement
    }

    constexpr iterator erase(const_iterator first, const_iterator last) {
        //TODO: implement
    }

    constexpr void push_back(const_reference value) noexcept {
        if (m_size == m_capacity) new_block();
        *internal_at_ptr(++m_size - 1) = value;
    }

    constexpr void push_back(reference& value) noexcept {
        if (m_size == m_capacity) new_block();
        *internal_at_ptr(++m_size - 1) = value;
    }

    template<class... Args>
    constexpr reference emplace_back(Args&&... args) {
        //TODO:
    }

    template<std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    constexpr void append_range(R&& rg) {
        //TODO:
    }

    constexpr void pop_back() {
        std::allocator_traits<allocator_type>::destroy(m_alloc, internal_at_ptr(m_size - 1));
        m_size--;
    }

    constexpr void resize(size_type count) {
        if constexpr (count < m_size) {
            m_size = count;
        } else if (count > m_size) {
            while (count + m_size > m_capacity) {
                new_block();
            }
            for (size_t i = 0; i < count; i++) {
                internal_at_ptr(m_size++) = value_type{};
            }
        }
    }

    constexpr void resize(size_type count, const_reference value) {
        if constexpr (count < m_size) {
            m_size = count;
        } else if (count > m_size) {
            while (count + m_size > m_capacity) {
                new_block();
            }
            for (size_t i = 0; i < count; i++) {
                internal_at_ptr(m_size++) = value;
            }
        }
    }

    constexpr void swap(vector_list& other) noexcept {
        std::swap(this->m_vectorList, other.m_vectorList);
        std::swap(this->m_blockOffsets, other.m_blockOffsets);

        std::swap(this->m_size, other.m_size);
        std::swap(this->m_capacity, other.m_capacity);

        if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(this->m_alloc, other.m_alloc);
        }
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
        lhs.swap(rhs);
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

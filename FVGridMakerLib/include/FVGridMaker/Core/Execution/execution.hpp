// ----------------------------------------------------------------------------
// File: execution.hpp
// Project: FVGridMaker
// Version: 1.0
// Description: Modo de execução (serial vs paralelo) e utilitários para loops
//              indexados compatíveis com algoritmos paralelos (<execution>).
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVMGridMaker 
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>

// ----------------------------------------------------------------------------
// includes C++  
// ----------------------------------------------------------------------------
#include <cstddef>
#include <cstdint>
#include <execution>
#include <iterator>
#include <type_traits>

FVGRIDMAKER_NAMESPACE_OPEN
CORE_NAMESPACE_OPEN

enum class ExecMode : std::uint8_t {
    Serial   = 0,
    Parallel = 1
};

[[nodiscard]] constexpr bool is_parallel(ExecMode mode) noexcept {
    return mode == ExecMode::Parallel;
}

DETAIL_NAMESPACE_OPEN

// Iterador de contagem random-access (evita alocar vetor de índices)
template <typename IndexT>
class CountingIterator {
public:
    using value_type        = IndexT;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
    using pointer           = void;
    using reference         = value_type;

    constexpr explicit CountingIterator(IndexT v = 0) noexcept : v_(v) {}

    constexpr reference operator*()  const noexcept { return v_; }
    constexpr CountingIterator& operator++() noexcept { ++v_; return *this; }
    constexpr CountingIterator  operator++(int) noexcept { auto t=*this; ++(*this); return t; }
    constexpr CountingIterator& operator--() noexcept { --v_; return *this; }
    constexpr CountingIterator  operator--(int) noexcept { auto t=*this; --(*this); return t; }

    constexpr CountingIterator& operator+=(difference_type n) noexcept { v_ += static_cast<IndexT>(n); return *this; }
    constexpr CountingIterator& operator-=(difference_type n) noexcept { v_ -= static_cast<IndexT>(n); return *this; }

    friend constexpr CountingIterator operator+(CountingIterator it, difference_type n) noexcept { it += n; return it; }
    friend constexpr CountingIterator operator-(CountingIterator it, difference_type n) noexcept { it -= n; return it; }
    friend constexpr difference_type operator-(CountingIterator a, CountingIterator b) noexcept { return static_cast<difference_type>(a.v_ - b.v_); }

    friend constexpr bool operator==(CountingIterator a, CountingIterator b) noexcept { return a.v_ == b.v_; }
    friend constexpr bool operator!=(CountingIterator a, CountingIterator b) noexcept { return !(a==b); }
    friend constexpr bool operator< (CountingIterator a, CountingIterator b) noexcept { return a.v_ < b.v_; }
    friend constexpr bool operator> (CountingIterator a, CountingIterator b) noexcept { return b < a; }
    friend constexpr bool operator<=(CountingIterator a, CountingIterator b) noexcept { return !(b < a); }
    friend constexpr bool operator>=(CountingIterator a, CountingIterator b) noexcept { return !(a < b); }

private:
    IndexT v_{};
};

DETAIL_NAMESPACE_CLOSE

template <typename Fn>
inline void for_index(ExecMode mode, std::size_t n, Fn&& fn) {
    if (mode == ExecMode::Serial) {
        for (std::size_t i = 0; i < n; ++i) fn(i);
        return;
    }
    using It = detail::CountingIterator<std::size_t>;
    std::for_each(std::execution::par, It{0}, It{n}, std::forward<Fn>(fn));
}

CORE_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE

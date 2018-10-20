#ifndef CIGNAL_HPP_INCLUDED
#define CIGNAL_HPP_INCLUDED

#include <algorithm>
#include <array>
#include <cstdint>
#include <utility>
#include <vector>
namespace cignal {

template <typename T, std::size_t N> class n_vector : private std::vector<T> {
public:
  using value_type = T;
  using base_type = std::vector<T>;
  using size_type = typename base_type::size_type;
  using difference_type = typename base_type::difference_type;
  using pointer = typename base_type::pointer;
  using const_pointer = typename base_type::const_pointer;
  using reference = typename base_type::reference;
  using const_reference = typename base_type::const_reference;
  using iterator = typename base_type::iterator;
  using const_iterator = typename base_type::const_iterator;
  using reverse_iterator = typename base_type::reverse_iterator;
  using const_reverse_iterator = typename base_type::const_reverse_iterator;
  using n_difference_type = std::array<size_type, N>;
  using n_size_type = std::array<size_type, N>;
  using n_index_type = n_size_type;
  const_reference operator[](n_size_type const &indics) const {
    size_type index = std::accumulate(indics.begin(), indics.end(), 1,
                                      std::multiplies<size_type>{});
    return (*this)[index];
  }
  reference operator[](n_size_type const &indics) {
    size_type index = std::accumulate(indics.begin(), indics.end(), 1,
                                      std::multiplies<size_type>{});
    return (*this)[index];
  }
  n_vector(const n_vector &) = default;
  n_vector(n_vector &&) = default;
  n_vector(size_type size, const_reference value = value_type())
      : base_type(size, value), m_n_size() {
    m_n_size.fill(1);
    if (N > 0)
      m_n_size[N - 1] = size;
  }
  n_vector(n_size_type n_size, const_reference value = value_type())
      : base_type(std::accumulate(n_size.begin(), n_size.end(), 1,
                                  std::multiplies<size_type>{}),
                  value),
        m_n_size(n_size) {}
  // template <typename... ArgTs>
  // n_vector(ArgTs &&... args) : std::vector<T>(std::forward<ArgTs>(args)...) {
  //   m_n_size.fill(1);
  //   if (N > 0) {
  //     m_n_size[N - 1] = this->std::vector<T>::size();
  //   }
  // }
  n_vector &operator=(const n_vector &) = default;
  n_vector &operator=(n_vector &&) = default;

  using base_type::assign;
  using base_type::at;
  using base_type::operator[];
  using base_type::back;
  using base_type::begin;
  using base_type::capacity;
  using base_type::cbegin;
  using base_type::cend;
  using base_type::crbegin;
  using base_type::crend;
  using base_type::data;
  using base_type::empty;
  using base_type::end;
  using base_type::front;
  using base_type::max_size;
  using base_type::rbegin;
  using base_type::rend;
  using base_type::reserve;
  using base_type::size;
  n_size_type n_size() const noexcept { return m_n_size; }

  void resize(size_type size) {
    this->base_type::resize(size);
    m_n_size.fill(1);
    if (N > 0)
      m_n_size[N - 1] = size;
  }
  void swap(n_vector &other) noexcept(
      noexcept(base_type::swap(static_cast<base_type &>(other))) &&
      noexcept(n_vector::m_n_size.swap(other.m_n_size))) {
    base_type::swap(static_cast<base_type &>(other));
    m_n_size.swap(other.m_n_size);
  }
  void fill(const value_type &value) { std::fill(begin(), end(), value); }
  void clear() noexcept(noexcept(base_type::clear())) {
    base_type::clear();
    m_n_size = {};
  }
  void resize(n_size_type n_size) {
    size_type size = std::accumulate(n_size.begin(), n_size.end(), 1,
                                     std::multiplies<size_type>{});
    base_type::resize(size);
    m_n_size = n_size;
  }
  n_size_type n_index_base() {
    // given n_size = {3, 6, 7}
    // result = {6*7*1, 7*1, 1}
    n_size_type result;
    size_type n = m_n_size.size();
    if (n == 0)
      return;
    result[n - 1] = 1;
    // j goes from n-1
    // j = n-2, n-3, n-4, ..., 0
    for (size_type i = 1; i < n; ++i) {
      size_type j = n - 1 - i;
      result[j] = result[j + 1] * m_n_size[j + 1];
    }
  }
  size_type index_from_n_index(n_size_type n_index) {
    size_type index;
    n_size_type base = n_index_base();
    index =
        std::accumulate(base.begin(), base.end(), 0, std::plus<size_type>{});
    return index;
  }
  size_type n_index_from_index(size_type index) {
    n_size_type n_index;
    n_size_type base = n_index_base();
    size_type n = n_index.size();
    for (size_type i = 0; i < n; ++i) {
      n_index[i] = index / base[i];
    }
    return n_index;
  }
  size_type index_of(iterator i) noexcept { return i - base_type::begin(); }
  size_type index_of(const_iterator i) const noexcept {
    return i - base_type::cbegin();
  }
  n_size_type n_index_of(iterator i) noexcept {
    n_size_type n_index;
    size_type index = i - base_type::begin();
    n_index = n_index_from_index(index);
    return n_index;
  }
  n_size_type n_index_of(const_iterator i) const noexcept {
    n_size_type n_index;
    size_type index = i - base_type::cbegin();
    n_index = n_index_from_index(index);
    return n_index;
  }
  reference at(n_size_type n_index) {
    size_type index = index_from_n_index(n_index);
    return base_type::at(index);
  }
  const_reference at(n_size_type n_index) const {
    size_type index = index_from_n_index(n_index);
    return base_type::at(index);
  }
  reference operator[](n_size_type n_index) noexcept {
    size_type index = index_from_n_index(n_index);
    return base_type::operator[](index);
  }
  const_reference operator[](n_size_type n_index) const noexcept {
    size_type index = index_from_n_index(n_index);
    return base_type::operator[](index);
  }
  // friend bool operator==(const n_vector &, const n_vector &);
  // friend bool operator!=(const n_vector &, const n_vector &);
  // friend bool operator<(const n_vector &, const n_vector &);
  // friend bool operator>(const n_vector &, const n_vector &);
  // friend bool operator<=(const n_vector &, const n_vector &);
  // friend bool operator>=(const n_vector &, const n_vector &);
  // friend void swap(n_vector &, n_vector &);
private:
  n_size_type m_n_size;
};

template <typename T, std::size_t N> class n_vector_mutable_view {
public:
  using n_vector_type = n_vector<T, N>;
  using value_type = T;
  using size_type = typename n_vector_type::size_type;
  using difference_type = typename n_vector_type::difference_type;
  using pointer = typename n_vector_type::pointer;
  using const_pointer = typename n_vector_type::const_pointer;
  using reference = typename n_vector_type::reference;
  using const_reference = typename n_vector_type::const_reference;
  using iterator = typename n_vector_type::iterator;
  using const_iterator = typename n_vector_type::const_iterator;
  using reverse_iterator = typename n_vector_type::reverse_iterator;
  using const_reverse_iterator = typename n_vector_type::const_reverse_iterator;
  using n_difference_type = typename n_vector_type::n_difference_type;
  using n_size_type = typename n_vector_type::n_size_type;
  n_vector_mutable_view()
      : m_n_vector(nullptr), m_n_offset({}), m_n_size({}) {}
  n_vector_mutable_view(const n_vector_mutable_view &view)
      : m_n_vector(view.m_n_vector), m_n_offset({}), m_n_size({}) {}
  n_vector_mutable_view(n_vector_mutable_view &view, size_type n_offset,
                        size_type n_size)
      : m_n_vector(view.m_n_vector), m_n_offset(n_offset), m_n_size(n_size) {}
  n_vector_mutable_view(n_vector_type &n_vector, size_type n_offset,
                        size_type n_size)
      : m_n_vector(&n_vector), m_n_offset(n_offset), m_n_size(n_size) {}
private:
  n_vector_type *m_n_vector;
  n_size_type m_n_offset;
  n_size_type m_n_size;
};
} // namespace cignal

#endif // CIGNAL_HPP_INCLUDED

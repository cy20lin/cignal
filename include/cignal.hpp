#ifndef CIGNAL_HPP_INCLUDED
#define CIGNAL_HPP_INCLUDED

#include <cstdint>
#include <vector>
#include <array>
#include <algorithm>
#include <utility>
namespace cignal {

template <typename T, std::size_t N>
class n_vector : std::vector<T>
{
public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using iterator = T &;
    using const_iterator = const T &;
    using size_type = std::size_t;
    using n_size_type = std::array<size_type,N>;
    const_reference operator[](n_size_type const & indics) const {
        size_type index = std::accumulate(indics.begin(), indics.end(), 1, std::multiplies<size_type>{});
        return (*this)[index];
    }
    reference operator[](n_size_type const & indics) {
        size_type index = std::accumulate(indics.begin(), indics.end(), 1, std::multiplies<size_type>{});
        return (*this)[index];
    }
    n_vector(std::array<std::size_t, N> n_size)
        : std::vector<T>(std::accumulate(n_size.begin(), n_size.end(), 1, std::multiplies<size_type>{})),
        m_n_size(n_size) {}
    template <typename ...ArgTs>
    n_vector(ArgTs && ...args) : std::vector<T>(std::forward<ArgTs>(args)...) {
        m_n_size.fill(1);
        m_n_size[0] = this->std::vector<T>::size();
    }
    n_size_type cumulative_n_size() {
        n_size_type result;
        size_type n = m_n_size.size();
        if (n == 0)
            return;
        result[n-1] = 1;
        // j goes from n-1
        for (size_type i = 1; i < n; ++i) {
            size_type j = n-i;
            result[j-1] = result[j]*m_n_size[j];
        }
    }
private:
    n_size_type m_n_size;
};
} // namespace cignal

#endif // CIGNAL_HPP_INCLUDED

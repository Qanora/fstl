#include <type_traits>

namespace detail {

template <class T, class Type, class T1, class... Args>
constexpr decltype(auto) INVOKE(Type T::*f, T1&& t1, Args&&... args) {
  if constexpr (std::is_member_function_pointer_v<decltype(f)>) {
    if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)
      return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
    else
      return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
  } else {
    static_assert(std::is_member_object_pointer_v<decltype(f)>);
    static_assert(sizeof...(args) == 0);
    if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)
      return std::forward<T1>(t1).*f;
    else
      return (*std::forward<T1>(t1)).*f;
  }
}

template <class F, class... Args>
constexpr decltype(auto) INVOKE(F&& f, Args&&... args) {
  return std::forward<F>(f)(std::forward<Args>(args)...);
}

template <typename F, typename... Args>
struct invoke_result {
  using type = decltype(INVOKE(std::declval<F>(), std::declval<Args>()...));
};
};  // namespace detail

namespace fstl {
template <class F, class... Args>
struct invoke_result : detail::invoke_result<F, Args...> {};

template <class F, class... Args>
using invoke_result_t = typename invoke_result<F, Args...>::type;

template <class F, class... Args>
constexpr invoke_result_t<F, Args...> invoke(F&& f, Args&&... args) {
  return detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
}

};  // namespace fstl
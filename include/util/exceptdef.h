#pragma once

namespace fstl {

#define FSTL_DEBUG(expr) assert(expr)

#define THROW_LENGTH_ERROR_IF(expr, what)                                      \
  if ((expr))                                                                  \
  throw std::length_error(what)

#define THROW_OUT_OF_RANGE_IF(expr, what)                                      \
  if ((expr))                                                                  \
  throw std::out_of_range(what)

#define THROW_RUNTIME_ERRORIF(expr, what)                                      \
  if ((expr))                                                                  \
  throw std::run_time_error(what)
}; // namespace fstl
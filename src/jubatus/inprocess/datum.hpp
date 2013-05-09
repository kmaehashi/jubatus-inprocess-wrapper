/*
 * Jubatus In-Process Wrapper
 * Copyright (C) 2013 Kenichi Maehashi
 */

#ifndef JUBATUS_INPROCESS_DATUM_HPP_
#define JUBATUS_INPROCESS_DATUM_HPP_

#include <jubatus/fv_converter/datum.hpp>

#include "common.h"

namespace jubatus {
namespace inprocess {

jubatus::fv_converter::datum unpack_datum(const struct datum& d) {
  jubatus::fv_converter::datum d2;
  for (uint64_t i = 0; i < d.string_values_size; ++i) {
    pair_sv p = d.string_values[i];
    d2.string_values_.push_back(std::make_pair(p.key, p.value));
  }
  for (uint64_t i = 0; i < d.num_values_size; ++i) {
    pair_nv p = d.num_values[i];
    d2.num_values_.push_back(std::make_pair(p.key, p.value));
  }
  return d2;
}

}  // namespace inprocess
}  // namespace jubatus

#endif  // JUBATUS_INPROCESS_DATUM_HPP_

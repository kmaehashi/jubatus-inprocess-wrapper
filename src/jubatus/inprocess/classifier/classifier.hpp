/*
 * Jubatus In-Process Wrapper
 * Copyright (C) 2013 Kenichi Maehashi
 */

#ifndef JUBATUS_INPROCESS_CLASSIFIER_CLASSIFIER_HPP_
#define JUBATUS_INPROCESS_CLASSIFIER_CLASSIFIER_HPP_

#include <jubatus/core/driver/classifier.hpp>
#include <jubatus/core/classifier/classifier_type.hpp>

#include <jubatus/core/fv_converter/datum.hpp>
#include <jubatus/core/fv_converter/datum_to_fv_converter.hpp>

#include <pficommon/lang/shared_ptr.h>
#include <pficommon/text/json.h>

namespace jubatus {
namespace inprocess {
namespace classifier {

struct server_config {
  std::string method;
  pfi::data::optional<pfi::text::json::json> parameter;
  pfi::text::json::json converter;

  template<typename Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(method) & MEMBER(parameter) & MEMBER(converter);
  }
};

class classifier {
 public:
  classifier(const std::string& config);
  void train(
      const std::pair<std::string, jubatus::core::fv_converter::datum>& data);
  jubatus::core::classifier::classify_result classify(
      const jubatus::core::fv_converter::datum& d);

 private:
  pfi::lang::shared_ptr<jubatus::core::driver::classifier> driver_;
};

}  // namespace classifier
}  // namespace inprocess
}  // namespace jubatus

#endif  // JUBATUS_INPROCESS_CLASSIFIER_CLASSIFIER_HPP_

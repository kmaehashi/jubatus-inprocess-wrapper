/*
 * Jubatus In-Process Wrapper
 * Copyright (C) 2013 Kenichi Maehashi
 */

#include "classifier.hpp"

#include <jubatus/core/driver/classifier.hpp>
#include <jubatus/core/storage/storage_factory.hpp>
#include <jubatus/core/storage/storage_base.hpp>
#include <jubatus/core/classifier/classifier_base.hpp>
#include <jubatus/core/classifier/classifier_factory.hpp>
#include <jubatus/core/classifier/classifier_config.hpp>
#include <jubatus/core/classifier/classifier_type.hpp>
#include <jubatus/core/fv_converter/datum.hpp>
#include <jubatus/core/fv_converter/converter_config.hpp>
#include <jubatus/core/fv_converter/datum_to_fv_converter.hpp>
#include <jubatus/core/common/type.hpp>
#include <jubatus/core/common/jsonconfig/cast.hpp>
#include <jubatus/core/common/jsonconfig/config.hpp>

#include <pficommon/lang/cast.h>
#include <pficommon/text/json.h>

namespace jubatus {
namespace inprocess {
namespace classifier {

classifier::classifier(const std::string& config) {
  server_config conf =
      jubatus::core::common::jsonconfig::config_cast_check<server_config>(
          jubatus::core::common::jsonconfig::config(
              pfi::lang::lexical_cast<pfi::text::json::json>(config)));

  jubatus::core::common::jsonconfig::config param;
  if (conf.parameter) {
    param = jubatus::core::common::jsonconfig::config(*conf.parameter);
  }

  jubatus::core::storage::storage_base* storage =
      jubatus::core::storage::storage_factory::create_storage("local");
  driver_.reset(new jubatus::core::driver::classifier(
      storage,
      jubatus::core::classifier::classifier_factory::create_classifier(
          conf.method,
          param,
          storage),
      jubatus::core::fv_converter::make_fv_converter(conf.converter)));
}

void classifier::train(
    const std::pair<std::string, jubatus::core::fv_converter::datum>& data) {
  driver_->train(data);
}

jubatus::core::classifier::classify_result classifier::classify(
    const jubatus::core::fv_converter::datum& d) {
  return driver_->classify(d);
}

void classifier::save(std::ostream& os) {
  jubatus::core::framework::mixable_holder::mixable_list mixables =
      driver_->get_mixable_holder()->get_mixables();
  for (size_t i = 0; i < mixables.size(); ++i) {
    mixables[i]->save(os);
  }
}

void classifier::load(std::istream& is) {
  jubatus::core::framework::mixable_holder::mixable_list mixables =
      driver_->get_mixable_holder()->get_mixables();
  for (size_t i = 0; i < mixables.size(); ++i) {
    mixables[i]->clear();
    mixables[i]->load(is);
  }
}

void classifier::clear() {
  driver_->clear();
}

}  // namespace classifier
}  // namespace inprocess
}  // namespace jubatus

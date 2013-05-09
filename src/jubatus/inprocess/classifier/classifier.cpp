/*
 * Jubatus In-Process Wrapper
 * Copyright (C) 2013 Kenichi Maehashi
 */

#include "classifier.hpp"

#include <jubatus/driver/classifier.hpp>
#include <jubatus/storage/storage_factory.hpp>
#include <jubatus/storage/storage_base.hpp>
#include <jubatus/classifier/classifier_base.hpp>
#include <jubatus/classifier/classifier_factory.hpp>
#include <jubatus/classifier/classifier_config.hpp>
#include <jubatus/classifier/classifier_type.hpp>
#include <jubatus/framework/mixer/mixer.hpp>
#include <jubatus/framework/mixer/dummy_mixer.hpp>
#include <jubatus/fv_converter/datum.hpp>
#include <jubatus/fv_converter/converter_config.hpp>
#include <jubatus/fv_converter/datum_to_fv_converter.hpp>
#include <jubatus/common/type.hpp>
#include <jubatus/common/jsonconfig/cast.hpp>
#include <jubatus/common/jsonconfig/config.hpp>

#include <pficommon/lang/cast.h>
#include <pficommon/text/json.h>

namespace jubatus {
namespace inprocess {
namespace classifier {

classifier::classifier(const std::string& config) {
  server_config conf = jubatus::jsonconfig::config_cast_check<server_config>(
      jsonconfig::config(
          pfi::lang::lexical_cast<pfi::text::json::json>(config)));

  jsonconfig::config param;
  if (conf.parameter) {
    param = jsonconfig::config(*conf.parameter);
  }

  jubatus::storage::storage_base* storage =
      jubatus::storage::storage_factory::create_storage("local");
  mixer_.reset(new jubatus::framework::mixer::dummy_mixer);
  driver_.reset(new jubatus::driver::classifier(
      storage,
      jubatus::classifier::classifier_factory::create_classifier(
          conf.method,
          param,
          storage),
      mixer_,
      jubatus::fv_converter::make_fv_converter(conf.converter)));
}

void classifier::train(
    const std::pair<std::string, jubatus::fv_converter::datum>& data) {
  driver_->train(data);
}

classify_result classifier::classify(const jubatus::fv_converter::datum& d) {
  return driver_->classify(d);
}

}  // namespace classifier
}  // namespace inprocess
}  // namespace jubatus

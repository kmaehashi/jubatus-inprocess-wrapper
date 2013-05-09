/*
 * Jubatus In-Process Wrapper
 * Copyright (C) 2013 Kenichi Maehashi
 */

#include <jubatus/classifier/classifier_type.hpp>

#include "classifier.hpp"
#include "jubatus/inprocess/datum.hpp"

typedef jubatus::inprocess::classifier::classifier jubaclassifier;

extern "C" {

#include "classifier.h"
#include "jubatus/inprocess/common.h"

const struct classify_result pack_classify_result(
    const jubatus::classify_result& r) {
  struct classify_result r2;
  int count = r.size();

  r2.size = count;
  r2.results = (classify_result_elem*) malloc(
      sizeof(classify_result_elem) * count);
  for (int i = 0; i < count; ++i) {
    r2.results[i].label = r[i].label.c_str();
    r2.results[i].score = r[i].score;
  }
  return r2;
}

JUBATUS_HANDLE create_classifier(const char* config) {
  return new jubaclassifier(config);
}

void dispose_classifier(JUBATUS_HANDLE classifier) {
  delete static_cast<jubaclassifier*>(classifier);
}

void train(
    JUBATUS_HANDLE classifier,
    const char* label,
    const struct datum* d) {
  static_cast<jubaclassifier*>(classifier)->train(
      std::make_pair(label, jubatus::inprocess::unpack_datum(*d)));
}

struct classify_result classify(
    JUBATUS_HANDLE classifier,
    const struct datum* d) {
  return pack_classify_result(
      static_cast<jubaclassifier*>(classifier)->classify(
          jubatus::inprocess::unpack_datum(*d)));
}

void dispose_classify_result(struct classify_result result) {
  free(result.results);
}

}  // extern "C"

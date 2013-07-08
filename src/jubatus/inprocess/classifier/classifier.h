/*
 * Jubatus In-Process Wrapper
 * Copyright (C) 2013 Kenichi Maehashi
 */

#ifndef JUBATUS_INPROCESS_CLASSIFIER_CLASSIFIER_H_
#define JUBATUS_INPROCESS_CLASSIFIER_CLASSIFIER_H_

#include <stdint.h>

#include "../common.h"

struct classify_result_elem {
  const char* label;
  float score;
};

struct classify_result {
  uint64_t size;
  struct classify_result_elem* results;
};

JUBATUS_HANDLE create_classifier(const char* config);
void dispose_classifier(JUBATUS_HANDLE classifier);
void train(
    JUBATUS_HANDLE classifier,
    const char* label,
    const struct datum* d);
struct classify_result classify(
    JUBATUS_HANDLE classifier,
    const struct datum* d);
void dispose_classify_result(struct classify_result* result);

#endif  // JUBATUS_INPROCESS_CLASSIFIER_CLASSIFIER_H_

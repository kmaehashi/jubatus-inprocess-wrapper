/*
 * Jubatus In-Process Wrapper
 * Copyright (C) 2013 Kenichi Maehashi
 */

#ifndef JUBATUS_INPROCESS_COMMON_H_
#define JUBATUS_INPROCESS_COMMON_H_

#include <stdint.h>

typedef void* JUBATUS_HANDLE;

struct pair_sv {
  const char* key;
  const char* value;
};

struct pair_nv {
  const char* key;
  float value;
};

struct datum {
  uint64_t string_values_size;
  struct pair_sv* string_values;

  uint64_t num_values_size;
  struct pair_nv* num_values;
};

#endif  // JUBATUS_INPROCESS_COMMON_H_

/*
 * Example of Jubatus In-Process Wrapper (C)
 * Copyright (C) 2013 Kenichi Maehashi
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "jubatus/inprocess/classifier/classifier.h"

struct datum make_datum(
    const char* hair,
    const char* top,
    const char* bottom,
    double height) {
  struct datum d = {0};

  d.string_values_size = 3;
  d.string_values =
      (struct pair_sv*) malloc(sizeof(struct pair_sv) * d.string_values_size);
  d.string_values[0].key = "hair";
  d.string_values[0].value = hair;
  d.string_values[1].key = "top";
  d.string_values[1].value = top;
  d.string_values[2].key = "bottom";
  d.string_values[2].value = bottom;

  d.num_values_size = 1;
  d.num_values =
      (struct pair_nv*) malloc(sizeof(struct pair_nv) * d.num_values_size);
  d.num_values[0].key = "height";
  d.num_values[0].value = height;

  return d;
}

void free_datum(struct datum* d) {
  free(d->string_values);
  free(d->num_values);
}

void show_result(struct classify_result result) {
  uint64_t i;
  for (i = 0; i < result.size; ++i) {
    printf(
        "Label: %s (Score: %f)\n",
        result.results[i].label,
        result.results[i].score);
  }
  dispose_classify_result(&result);
}

void do_train(
    JUBATUS_HANDLE classifier,
    const char* label,
    struct datum d) {
  train(classifier, label, &d);
  free_datum(&d);
}

struct classify_result do_classify(
    JUBATUS_HANDLE classifier,
    struct datum d) {
  struct classify_result result = classify(classifier, &d);
  free_datum(&d);
  return result;
}

int run() {
  const int MAXLEN = 1024;
  char buf[MAXLEN];

  FILE* fp = fopen("config.json", "r");
  if (!fp) {
    perror("fopen config.json");
    return 1;
  }
  fread(buf, MAXLEN, 1, fp);
  fclose(fp);

  printf("=== Config ===\n");
  printf("%s\n", buf);
  printf("==============\n");

  JUBATUS_HANDLE c = create_classifier(buf);

  // train
  printf("train\n");
  do_train(c, "male", make_datum("short", "sweater", "jeans", 1.70));
  do_train(c, "female", make_datum("long", "shirt", "skirt", 1.56));
  do_train(c, "male", make_datum("short", "jacket", "chino", 1.65));
  do_train(c, "female", make_datum("short", "T shirt", "jeans", 1.72));
  do_train(c, "male", make_datum("long", "T shirt", "jeans", 1.82));
  do_train(c, "female", make_datum("long", "jacket", "skirt", 1.43));

  // save
  const char* model;
  size_t length;
  save(c, &model, &length);
  printf("model size: %zd\n", length);
  free((void *) model);

  // clear
  clear(c);

  // load
  load(c, model, length);

  // classify
  printf("classify 1\n");
  show_result(do_classify(c, make_datum("short", "T shirt", "jeans", 1.81)));
  printf("classify 2\n");
  show_result(do_classify(c, make_datum("long", "shirt", "skirt", 1.50)));

  dispose_classifier(c);

  return 0;
}

int main(int argc, char** argv) {
  run();
}

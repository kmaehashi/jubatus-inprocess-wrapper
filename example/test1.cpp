/*
 * Example of Jubatus In-Process Wrapper (C++)
 * Copyright (C) 2013 Kenichi Maehashi
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include <jubatus/core/fv_converter/datum.hpp>
#include <jubatus/core/classifier/classifier_type.hpp>

#include "jubatus/inprocess/classifier/classifier.hpp"

using std::string;
using std::make_pair;
using std::vector;
using std::pair;

using jubatus::core::fv_converter::datum;
using jubatus::core::classifier::classify_result_elem;
using jubatus::core::classifier::classify_result;

datum make_datum(
    const string& hair,
    const string& top,
    const string& bottom,
    double height) {
  datum d;
  d.string_values_.push_back(make_pair("hair", hair));
  d.string_values_.push_back(make_pair("top", top));
  d.string_values_.push_back(make_pair("bottom", bottom));

  d.num_values_.push_back(make_pair("height", height));
  return d;
}

void show_result(const classify_result result) {
  for (classify_result::const_iterator it = result.begin();
      it != result.end();
      ++it) {
    std::cout << "Label: " << it->label <<
        " (Score: " << it->score << ")" << std::endl;
  }
}

int run() {
  std::ifstream is("config.json");
  if (!is) {
    std::cerr << "failed to open config.json" << std::endl;
    exit(1);
  }

  std::stringstream ss;
  ss << is.rdbuf();
  is.close();

  std::cout << "=== Config ===" << std::endl;
  std::cout << ss.str() << std::endl;
  std::cout << "==============" << std::endl;

  jubatus::inprocess::classifier::classifier c(ss.str());

  // train
  std::cout << "train" << std::endl;
  c.train(make_pair("male", make_datum("short", "sweater", "jeans", 1.70)));
  c.train(make_pair("female", make_datum("long", "shirt", "skirt", 1.56)));
  c.train(make_pair("male", make_datum("short", "jacket", "chino", 1.65)));
  c.train(make_pair("female", make_datum("short", "T shirt", "jeans", 1.72)));
  c.train(make_pair("male", make_datum("long", "T shirt", "jeans", 1.82)));
  c.train(make_pair("female", make_datum("long", "jacket", "skirt", 1.43)));

  // classify
  std::cout << "classify 1" << std::endl;
  show_result(c.classify(make_datum("short", "T shirt", "jeans", 1.81)));
  std::cout << "classify 2" << std::endl;
  show_result(c.classify(make_datum("long", "shirt", "skirt", 1.50)));

  return 0;
}

int main(int argc, char** argv) {
  run();
}

#pragma once
#include <inttypes.h>

template <typename T> class Timer {
  T time_ = 0;
  T target_ = 0;
  bool alarm_ = false;
public:
  Timer() = delete;

  void add(T time) {
    time_ += time;
    alarm_ = time_ > target_;
  }

  void sub(T time) {
    time_ -= time;
    alarm_ = time_ > target_;
  }

  void subTarget() {
    time_ -= target_;
    alarm_ = time_ > target_;
  }

  void reset() {
    time_ = 0;
  }

  void setTarget(T target) {
    target_ = target;
  }

  bool alarm() {
    return alarm;
  }
};
/* Copyright (c) 2018. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include <exception>
#include <mutex>

#include <xbt/ex.hpp>
#include <xbt/log.hpp>

#include "simgrid/barrier.h"
#include "simgrid/s4u/Barrier.hpp"
#include "simgrid/simix.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(s4u_barrier, "S4U barrier");

namespace simgrid {
namespace s4u {

Barrier::Barrier(unsigned int expected_processes) : mutex_(Mutex::create()), cond_(ConditionVariable::create()), expected_processes_(expected_processes)
{
}

/**
 * Wait functions
 */
int Barrier::wait()
{
  mutex_->lock();
  arrived_processes_++;
  XBT_DEBUG("waiting %p %u/%u", this, arrived_processes_, expected_processes_);
  if (arrived_processes_ == expected_processes_) {
    cond_->notify_all();
    mutex_->unlock();
    arrived_processes_ = 0;
    return -1;
  }

  cond_->wait(mutex_);
  mutex_->unlock();
  return 0;
}
} // namespace s4u
} // namespace simgrid

/* **************************** Public C interface *************************** */

sg_bar_t sg_barrier_init(unsigned int count)
{
  return new simgrid::s4u::Barrier(count);
}

/** @brief Initializes a barrier, with count elements */
void sg_barrier_destroy(sg_bar_t bar)
{
  delete bar;
}

/** @brief Performs a barrier already initialized */
int sg_barrier_wait(sg_bar_t bar)
{
  return bar->wait();
}

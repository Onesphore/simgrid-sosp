/* Copyright (c) 2007-2018. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include "src/kernel/activity/ConditionVariableImpl.hpp"
#include "src/kernel/activity/MutexImpl.hpp"
#include "src/kernel/activity/SynchroRaw.hpp"
#include "src/simix/smx_synchro_private.hpp"
#include "xbt/ex.hpp"

XBT_LOG_NEW_DEFAULT_SUBCATEGORY(ConditionVariable, simix_synchro, "Condition variables");

static void _SIMIX_cond_wait(smx_cond_t cond, smx_mutex_t mutex, double timeout, smx_actor_t issuer,
                             smx_simcall_t simcall);

/********************************* Condition **********************************/

/**
 * \brief Initialize a condition.
 *
 * Allocates and creates the data for the condition.
 * It have to be called before the use of the condition.
 * \return A condition
 */
smx_cond_t SIMIX_cond_init()
{
  XBT_IN("()");
  smx_cond_t cond = new s_smx_cond_t();
  XBT_OUT();
  return cond;
}

/**
 * \brief Handle a condition waiting simcall without timeouts
 * \param simcall the simcall
 */
void simcall_HANDLER_cond_wait(smx_simcall_t simcall, smx_cond_t cond, smx_mutex_t mutex)
{
  XBT_IN("(%p)", simcall);
  smx_actor_t issuer = simcall->issuer;

  _SIMIX_cond_wait(cond, mutex, -1, issuer, simcall);
  XBT_OUT();
}

/**
 * \brief Handle a condition waiting simcall with timeouts
 * \param simcall the simcall
 */
void simcall_HANDLER_cond_wait_timeout(smx_simcall_t simcall, smx_cond_t cond, smx_mutex_t mutex, double timeout)
{
  XBT_IN("(%p)", simcall);
  smx_actor_t issuer = simcall->issuer;

  _SIMIX_cond_wait(cond, mutex, timeout, issuer, simcall);
  XBT_OUT();
}

static void _SIMIX_cond_wait(smx_cond_t cond, smx_mutex_t mutex, double timeout, smx_actor_t issuer,
                             smx_simcall_t simcall)
{
  XBT_IN("(%p, %p, %f, %p,%p)", cond, mutex, timeout, issuer, simcall);
  smx_activity_t synchro = nullptr;

  XBT_DEBUG("Wait condition %p", cond);

  /* If there is a mutex unlock it */
  /* FIXME: what happens if the issuer is not the owner of the mutex? */
  if (mutex != nullptr) {
    cond->mutex = mutex;
    mutex->unlock(issuer);
  }

  synchro = SIMIX_synchro_wait(issuer->host, timeout);
  synchro->simcalls.push_front(simcall);
  issuer->waiting_synchro = synchro;
  cond->sleeping.push_back(*simcall->issuer);
  XBT_OUT();
}

/**
 * \brief Signalizes a condition.
 *
 * Signalizes a condition and wakes up a sleeping process.
 * If there are no process sleeping, no action is done.
 * \param cond A condition
 */
void SIMIX_cond_signal(smx_cond_t cond)
{
  XBT_IN("(%p)", cond);
  XBT_DEBUG("Signal condition %p", cond);

  /* If there are processes waiting for the condition choose one and try
     to make it acquire the mutex */
  if (not cond->sleeping.empty()) {
    auto& proc = cond->sleeping.front();
    cond->sleeping.pop_front();

    /* Destroy waiter's synchronization */
    proc.waiting_synchro = nullptr;

    /* Now transform the cond wait simcall into a mutex lock one */
    smx_simcall_t simcall = &proc.simcall;
    smx_mutex_t mutex;
    if (simcall->call == SIMCALL_COND_WAIT)
      mutex = simcall_cond_wait__get__mutex(simcall);
    else
      mutex = simcall_cond_wait_timeout__get__mutex(simcall);
    simcall->call = SIMCALL_MUTEX_LOCK;

    simcall_HANDLER_mutex_lock(simcall, mutex);
  }
  XBT_OUT();
}

/**
 * \brief Broadcasts a condition.
 *
 * Signal ALL processes waiting on a condition.
 * If there are no process waiting, no action is done.
 * \param cond A condition
 */
void SIMIX_cond_broadcast(smx_cond_t cond)
{
  XBT_IN("(%p)", cond);
  XBT_DEBUG("Broadcast condition %p", cond);

  /* Signal the condition until nobody is waiting on it */
  while (not cond->sleeping.empty()) {
    SIMIX_cond_signal(cond);
  }
  XBT_OUT();
}

smx_cond_t SIMIX_cond_ref(smx_cond_t cond)
{
  if (cond != nullptr)
    intrusive_ptr_add_ref(cond);
  return cond;
}

void SIMIX_cond_unref(smx_cond_t cond)
{
  XBT_IN("(%p)", cond);
  XBT_DEBUG("Destroy condition %p", cond);
  if (cond != nullptr) {
    intrusive_ptr_release(cond);
  }
  XBT_OUT();
}

void intrusive_ptr_add_ref(s_smx_cond_t* cond)
{
  auto previous = cond->refcount_.fetch_add(1);
  xbt_assert(previous != 0);
}

void intrusive_ptr_release(s_smx_cond_t* cond)
{
  if (cond->refcount_.fetch_sub(1) == 1) {
    xbt_assert(cond->sleeping.empty(), "Cannot destroy conditional since someone is still using it");
    delete cond;
  }
}
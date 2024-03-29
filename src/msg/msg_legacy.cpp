/* Copyright (c) 2004-2018. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include "src/msg/msg_private.hpp"

#define MSG_CALL(type, oldname, args)

/* ************************** Engine *************************** */
void MSG_create_environment(const char* filename)
{
  sg_engine_load_platform(filename);
}

void MSG_launch_application(const char* filename)
{
  sg_engine_load_deployment(filename);
}
msg_error_t MSG_main()
{
  sg_engine_run();
  return MSG_OK;
}
void MSG_function_register(const char* name, xbt_main_func_t code)
{
  sg_engine_register_function(name, code);
}
void MSG_function_register_default(xbt_main_func_t code)
{
  sg_engine_register_default(code);
}
double MSG_get_clock()
{
  return sg_engine_get_clock();
}

/* ************************** Mailboxes ************************ */
void MSG_mailbox_set_async(const char* alias)
{
  sg_mailbox_set_receiver(alias);
}
int MSG_task_listen(const char* alias)
{
  return sg_mailbox_listen(alias);
}

/* ************************** Actors *************************** */
int MSG_process_get_PID(sg_actor_t actor)
{
  return sg_actor_get_PID(actor);
}
int MSG_process_get_PPID(sg_actor_t actor)
{
  return sg_actor_get_PPID(actor);
}
msg_process_t MSG_process_from_PID(int PID)
{
  return sg_actor_by_PID(PID);
}
const char* MSG_process_get_name(sg_actor_t actor)
{
  return sg_actor_get_name(actor);
}
sg_host_t MSG_process_get_host(sg_actor_t actor)
{
  return sg_actor_get_host(actor);
}
xbt_dict_t MSG_process_get_properties(sg_actor_t actor)
{
  return sg_actor_get_properties(actor);
}
const char* MSG_process_get_property_value(sg_actor_t actor, const char* name)
{
  return sg_actor_get_property_value(actor, name);
}
void MSG_process_suspend(sg_actor_t actor)
{
  sg_actor_suspend(actor);
}
void MSG_process_resume(sg_actor_t actor)
{
  sg_actor_resume(actor);
}
int MSG_process_is_suspended(sg_actor_t actor)
{
  return sg_actor_is_suspended(actor);
}
void MSG_process_restart(sg_actor_t actor)
{
  sg_actor_restart(actor);
}
void MSG_process_auto_restart_set(sg_actor_t actor, int auto_restart)
{
  sg_actor_set_auto_restart(actor, auto_restart);
}

void MSG_process_daemonize(sg_actor_t actor)
{
  sg_actor_daemonize(actor);
}
void MSG_process_migrate(sg_actor_t actor, sg_host_t host)
{
  sg_actor_migrate(actor, host);
}
void MSG_process_join(sg_actor_t actor, double timeout)
{
  sg_actor_join(actor, timeout);
}
void MSG_process_kill(sg_actor_t actor)
{
  sg_actor_kill(actor);
}
void MSG_process_killall()
{
  sg_actor_kill_all();
}
void MSG_process_set_kill_time(sg_actor_t actor, double kill_time)
{
  sg_actor_set_kill_time(actor, kill_time);
}
void MSG_process_yield()
{
  sg_actor_yield();
}

/* ************************** NetZones *************************** */
sg_netzone_t MSG_zone_get_root()
{
  return sg_zone_get_root();
}
const char* MSG_zone_get_name(sg_netzone_t zone)
{
  return sg_zone_get_name(zone);
}
sg_netzone_t MSG_zone_get_by_name(const char* name)
{
  return sg_zone_get_by_name(name);
}
void MSG_zone_get_sons(sg_netzone_t zone, xbt_dict_t whereto)
{
  return sg_zone_get_sons(zone, whereto);
}
const char* MSG_zone_get_property_value(sg_netzone_t zone, const char* name)
{
  return sg_zone_get_property_value(zone, name);
}
void MSG_zone_set_property_value(sg_netzone_t zone, const char* name, char* value)
{
  sg_zone_set_property_value(zone, name, value);
}
void MSG_zone_get_hosts(sg_netzone_t zone, xbt_dynar_t whereto)
{
  sg_zone_get_hosts(zone, whereto);
}

/* ************************** Storages *************************** */
const char* MSG_storage_get_name(sg_storage_t storage)
{
  return sg_storage_get_name(storage);
}
sg_storage_t MSG_storage_get_by_name(const char* name)
{
  return sg_storage_get_by_name(name);
}
xbt_dict_t MSG_storage_get_properties(sg_storage_t storage)
{
  return sg_storage_get_properties(storage);
}
void MSG_storage_set_property_value(sg_storage_t storage, const char* name, const char* value)
{
  sg_storage_set_property_value(storage, name, value);
}
const char* MSG_storage_get_property_value(sg_storage_t storage, const char* name)
{
  return sg_storage_get_property_value(storage, name);
}
xbt_dynar_t MSG_storages_as_dynar()
{
  return sg_storages_as_dynar();
}
void MSG_storage_set_data(sg_storage_t storage, void* data)
{
  sg_storage_set_data(storage, data);
}
void* MSG_storage_get_data(sg_storage_t storage)
{
  return sg_storage_get_data(storage);
}
const char* MSG_storage_get_host(sg_storage_t storage)
{
  return sg_storage_get_host(storage);
}
sg_size_t MSG_storage_read(sg_storage_t storage, sg_size_t size)
{
  return sg_storage_read(storage, size);
}
sg_size_t MSG_storage_write(sg_storage_t storage, sg_size_t size)
{
  return sg_storage_write(storage, size);
}

/* ************************** hosts *************************** */
xbt_dynar_t MSG_hosts_as_dynar()
{
  return sg_hosts_as_dynar();
}
size_t MSG_get_host_number()
{
  return sg_host_count();
}
sg_host_t MSG_get_host_by_name(const char* name)
{
  return sg_host_by_name(name);
}
sg_host_t MSG_host_by_name(const char* name)
{
  return sg_host_by_name(name);
}
const char* MSG_host_get_name(sg_host_t host)
{
  return sg_host_get_name(host);
}
void* MSG_host_get_data(sg_host_t host)
{
  return sg_host_user(host);
}
void MSG_host_set_data(sg_host_t host, void* data)
{
  return sg_host_user_set(host, data);
}
xbt_dict_t MSG_host_get_mounted_storage_list(sg_host_t host)
{
  return sg_host_get_mounted_storage_list(host);
}
xbt_dynar_t MSG_host_get_attached_storage_lists(sg_host_t host)
{
  return sg_host_get_attached_storage_list(host);
}
double MSG_host_get_speed(sg_host_t host)
{
  return sg_host_speed(host);
}
double MSG_host_get_power_peak_at(sg_host_t host, int pstate_index)
{
  return sg_host_get_pstate_speed(host, pstate_index);
}
int MSG_host_get_core_number(sg_host_t host)
{
  return sg_host_core_count(host);
}
int MSG_host_get_nb_pstates(sg_host_t host)
{
  return sg_host_get_nb_pstates(host);
}
int MSG_host_get_pstate(sg_host_t host)
{
  return sg_host_get_pstate(host);
}
void MSG_host_set_pstate(sg_host_t host, int pstate)
{
  sg_host_set_pstate(host, pstate);
}
void MSG_host_on(sg_host_t h)
{
  sg_host_turn_on(h);
}
void MSG_host_off(sg_host_t h)
{
  sg_host_turn_off(h);
}
int MSG_host_is_on(sg_host_t h)
{
  return sg_host_is_on(h);
}
int MSG_host_is_off(sg_host_t h)
{
  return sg_host_is_off(h);
}
xbt_dict_t MSG_host_get_properties(sg_host_t host)
{
  return sg_host_get_properties(host);
}
const char* MSG_host_get_property_value(sg_host_t host, const char* name)
{
  return sg_host_get_property_value(host, name);
}
void MSG_host_set_property_value(sg_host_t host, const char* name, const char* value)
{
  sg_host_set_property_value(host, name, value);
}
void MSG_host_get_process_list(sg_host_t host, xbt_dynar_t whereto)
{
  sg_host_get_actor_list(host, whereto);
}
sg_host_t MSG_host_self()
{
  return sg_host_self();
}
/* ************************** Virtual Machines *************************** */
sg_vm_t MSG_vm_create_core(sg_host_t pm, const char* name)
{
  return sg_vm_create_core(pm, name);
}
sg_vm_t MSG_vm_create_multicore(sg_host_t pm, const char* name, int coreAmount)
{
  return sg_vm_create_multicore(pm, name, coreAmount);
}
int MSG_vm_is_created(sg_vm_t vm)
{
  return sg_vm_is_created(vm);
}
int MSG_vm_is_running(sg_vm_t vm)
{
  return sg_vm_is_running(vm);
}
int MSG_vm_is_suspended(sg_vm_t vm)
{
  return sg_vm_is_suspended(vm);
}
const char* MSG_vm_get_name(sg_vm_t vm)
{
  return sg_vm_get_name(vm);
}
void MSG_vm_set_ramsize(sg_vm_t vm, size_t size)
{
  sg_vm_set_ramsize(vm, size);
}
size_t MSG_vm_get_ramsize(sg_vm_t vm)
{
  return sg_vm_get_ramsize(vm);
}
sg_host_t MSG_vm_get_pm(sg_vm_t vm)
{
  return sg_vm_get_pm(vm);
}
void MSG_vm_set_bound(sg_vm_t vm, double bound)
{
  sg_vm_set_bound(vm, bound);
}
void MSG_vm_start(sg_vm_t vm)
{
  sg_vm_start(vm);
}
void MSG_vm_suspend(sg_vm_t vm)
{
  sg_vm_suspend(vm);
}
void MSG_vm_resume(sg_vm_t vm)
{
  sg_vm_resume(vm);
}
void MSG_vm_shutdown(sg_vm_t vm)
{
  sg_vm_shutdown(vm);
}
void MSG_vm_destroy(sg_vm_t vm)
{
  sg_vm_destroy(vm);
}
/********* barriers ************/
sg_bar_t MSG_barrier_init(unsigned int count)
{
  return sg_barrier_init(count);
}

void MSG_barrier_destroy(sg_bar_t bar)
{
  sg_barrier_destroy(bar);
}

int MSG_barrier_wait(sg_bar_t bar)
{
  return sg_barrier_wait(bar);
}

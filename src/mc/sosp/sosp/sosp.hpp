#ifndef SOSP_HPP
#define SOSP_HPP

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <iostream>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE);} while(0)

using std::vector;

namespace SOSP {
class Sosp {
  public:
    void* pagestore_addr;              // addr where pages are stored
    size_t capacity;                   // how many pages can be stored, grows dynamically
//    vector<snapshot> snapshots_vector;
    pid_t which_pid; // if DMTCP => which_pid = getpid()
                     // if SIMGRID => which_pid is the pid of mced

    /******************************************************************
     * Singleton class                                                *
     * ---------------                                                *
     * Sosp class  will have only 1 instance.                         *
     * This instance, "theInstance", being a static member variable   *
     * needs to be initialized. When it's initialized, it will call   *
     * a constructor, which will do all the necesary initializations  *
     ******************************************************************/
    static Sosp theInstance;

    Sosp();
    ~Sosp();
    Sosp(const Sosp&)=delete;        // no copying allowed (copy constructor)
    Sosp(const Sosp&&)=delete;       // no copying allowed (move constructor)
    Sosp& operator=(Sosp&)=delete;   // no copying allowed (copy assignment operator)

  public:
    // member fxs: checkpointing
    sosp_procMaps& get_procMaps(); // read "/proc/pid/maps"
    snapshot& writememoryAreas(procMaps& maps); // update the in-memory page store
    void snapshots_append(snapshot& snapshot_); // add snapshot_ to snapshots_vector

    // update the on-disk pagestore and write the last snapshot to disk
    // useful for DMTCP.
    void sync_to_disk(int fd); // update the on-disk pagestore and write the last snapshot to disk

   // member fxs: restart
    restore_snapshot(snapshot& snapshot_); // used by simgrid. We must specify which snapshot to
                                           // to restore from. FIXME: need a way to know the PID.
    restore_snapshot(int fd);              // used by DMTCP. Only the last snapshot is kept on Disk.
};
};

#endif // SOSP_HPP

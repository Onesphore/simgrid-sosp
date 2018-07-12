#include "sosp.hpp"

using namespace SOSP;

/***************************************************************
* This will take care of all the necessary initialization of   *
* the singleton class Sosp.                                    *
* Other member variables can be accessed via the static member *
* variable "theInstance."                                      *
****************************************************************/
Sosp Sosp::theInstance;

/*** member functions implementation ***/

Sosp::Sosp() {
  // FIXME: capacity is obviously too small.
  capacity = 4096;
  if ((pagestore_addr = mmap(NULL, capacity, PROT_READ|PROT_WRITE,
               MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)) == (void*)-1) {
    handle_error("mmap()");
  }
  // FIXME: When SIMGRID is used this should be handled #ntly.
  if ((which_pid = getpid()) == -1) {
    handle_error("getpid()");
  }
}

Sosp::~Sosp() {
  if (munmap(pagestore_addr, 4096) == -1) {
    handle_error("munmap()");
  }
  pagestore_addr = NULL;
  capacity = 0;
}

// DMTCP has a clean implementation of procMaps.
// We can use it, no need to "reinvent the wheel."
sosp_procMaps& Sosp::get_procMaps() {

}

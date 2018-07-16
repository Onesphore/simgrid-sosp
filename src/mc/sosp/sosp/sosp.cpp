#include "sosp.hpp"

using std::cout;
using std::endl;

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
  pagestore = new PageStore(500);
  // FIXME: When SIMGRID is used this should be handled #ntly.
  if ((which_pid = getpid()) == -1) {
    handle_error("getpid()");
  }

  get_procMaps();
}

Sosp::~Sosp() {
//  // TODO: take care of the resources held by 'pagestore'
//  if (munmap(pagestore_addr, 4096) == -1) {
//    handle_error("munmap()");
//  }
//  pagestore_addr = NULL;
//  capacity = 0;

  delete pagestore;
  pagestore = NULL; // empty state
}

// DMTCP has a clean implementation of procMaps.
// We can use it, no need to "reinvent the wheel."
sosp_procMaps Sosp::get_procMaps() {
  // procPidMaps constructor reads /proc/pid/maps into 'data'
  // pid = SOSP::Sosp::theInstance.which_pid
  procPidMaps pidMaps;

  sosp_procMaps maps;
  maps.maps_num = pidMaps.getNumAreas();

  cout << "so far so good." << endl;
  Area area;
  int areas_count = 0;
  while(pidMaps.getNextArea(&area) != 0) {
    maps.maps_vector.emplace_back(area);
    areas_count++;
  }

  assert(areas_count == pidMaps.getNumAreas());

  // print the addresses
  for (int i=0; i<maps.maps_vector.size(); ++i) {
    cout << (void*)maps.maps_vector[i].addr << "--" << (void*)maps.maps_vector[i].endAddr << endl;
  }

  return maps;
}

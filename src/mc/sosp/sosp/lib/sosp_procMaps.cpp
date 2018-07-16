/****************************************************************************
 *   Copyright (C) 2006-2013 by Jason Ansel, Kapil Arya, and Gene Cooperman *
 *   jansel@csail.mit.edu, kapil@ccs.neu.edu, gene@ccs.neu.edu              *
 *                                                                          *
 *  This file is part of DMTCP.                                             *
 *                                                                          *
 *  DMTCP is free software: you can redistribute it and/or                  *
 *  modify it under the terms of the GNU Lesser General Public License as   *
 *  published by the Free Software Foundation, either version 3 of the      *
 *  License, or (at your option) any later version.                         *
 *                                                                          *
 *  DMTCP is distributed in the hope that it will be useful,                *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU Lesser General Public License for more details.                     *
 *                                                                          *
 *  You should have received a copy of the GNU Lesser General Public        *
 *  License along with DMTCP:dmtcp/src.  If not, see                        *
 *  <http://www.gnu.org/licenses/>.                                         *
 ****************************************************************************/

// ProcSelfMaps::~ProcSelfMaps()
// {
//   JALLOC_HELPER_FREE(data);
//   fd = -1;
//   dataIdx = 0;
//   numAreas = 0;
//   numBytes = 0;
//
//   // Verify that JAlloc doesn't expand memory (via mmap)
//   // while reading /proc/self/maps.
//   // FIXME:  Change from JWARNING to JASSERT when we have confidence in this.
//   JWARNING(numAllocExpands == jalib::JAllocDispatcher::numExpands())
//     (numAllocExpands)(jalib::JAllocDispatcher::numExpands())
//   .Text("JAlloc: memory expanded through call to mmap()."
//         "  Inconsistent JAlloc will be a problem on restart");
// }
//
// bool
// ProcSelfMaps::isValidData()
// {
//   // TODO(kapil): Add validation check.
//   return true;
// }

#include <fcntl.h>
#include <cassert>
#include <string>
#include <cstring>
#include <iostream>

#include "sosp_procMaps.hpp"
#include "sosp.hpp"

using namespace SOSP;
using std::string;

// procPidMaps() {
procPidMaps::procPidMaps()
  : dataIdx(0),
  numAreas(0),
  numBytes(0),
  fd(-1),
  numAllocExpands(0)
{
  char buf[4096];

  // NOTE: preExpand() verifies that we have at least 10 chunks pre-allocated
  // for each level of the allocator.  See jalib/jalloc.cpp:preExpand().
  // It assumes no allocation larger than jalloc.cpp:MAX_CHUNKSIZE.
  // Ideally, we would have followed the MTCP C code, and not allocated
  // any memory bewteen the constructor and destructor of ProcSelfMaps.
  // But since C++ is biased toward frequent mallocs (e.g., dynamic vectors),
  // we try to compensate here for the weaknesses of C++.
  // If we use this /proc/self/maps for checkpointing, we must not mmap
  // as part of the allocator prior to writing the memory to the ckpt image.

  // FIXME:commented out. @Ones_
  // jalib::JAllocDispatcher::preExpand();

  // numAllocExpands = jalib::JAllocDispatcher::numExpands();

  // FIXME:  Also, any memory allocated and not freed since the calls to
  // setcontext() on the various threads will be a memory leak on restart.
  // We should check for that.

  // FIXME: commented out, 2 next lines. @Ones_
  // fd = _real_open("/proc/self/maps", O_RDONLY);
  // JASSERT(fd != -1) (JASSERT_ERRNO);
  // FIXME: next x lines added. @Ones_
  char buff_[128];
  string fileName = "/proc/";
  sprintf(buff_, "%d", Sosp::theInstance.which_pid);
  fileName += std::string(buff_);
  fileName += "/maps";
  fd = open(fileName.c_str(), O_RDONLY);
  ssize_t numRead = 0;

  // Get an approximation of the required buffer size.
//  do {
//    numRead = Util::readAll(fd, buf, sizeof(buf));
//    if (numRead > 0) {
//      numBytes += numRead;
//    }
//  } while (numRead > 0);

  do {
    if ((numRead = read(fd, buf, sizeof(buf))) == -1) {
      handle_error("read()");
    }
    if (numRead > 0) {
      numBytes += numRead;
    }
  } while (numRead > 0);

  // Now allocate a buffer. Note that this will most likely change the layout
  // of /proc/self/maps, so we need to recalculate numBytes.
  size_t size = numBytes + 4096; // Add a one page buffer.
//  data = (char *)JALLOC_HELPER_MALLOC(size);
//  JASSERT(lseek(fd, 0, SEEK_SET) == 0);
  data = (char *) malloc(size);
  assert(lseek(fd, 0, SEEK_SET) == 0);

//  numBytes = Util::readAll(fd, data, size);
  numBytes = read(fd, data, size);
//  JASSERT(numBytes > 0) (numBytes);
  assert(numBytes > 0);

  // TODO(kapil): Replace this assert with more robust code that would
  // reallocate the buffer with an extended size.
//  JASSERT(numBytes < size) (numBytes) (size);

  // TODO(kapil): Validate the read data.
//  JASSERT(isValidData());

//  _real_close(fd);
  close(fd);

  for (size_t i = 0; i < numBytes; i++) {
    if (data[i] == '\n') {
      numAreas++;
    }
  }
}

procPidMaps::~procPidMaps() {

}

int
procPidMaps::getNextArea(ProcMapsArea *area)
{
  char rflag, sflag, wflag, xflag;

  if (dataIdx >= numBytes || data[dataIdx] == 0) {
    return 0;
  }

  area->addr = (VA)readHex();
  assert(area->addr != NULL);

  assert(data[dataIdx++] == '-');

  area->endAddr = (VA)readHex();
  assert(area->endAddr != NULL);

  assert(data[dataIdx++] == ' ');

  assert(area->endAddr >= area->addr);
  area->size = area->endAddr - area->addr;

  rflag = data[dataIdx++];
  assert((rflag == 'r') || (rflag == '-'));

  wflag = data[dataIdx++];
  assert((wflag == 'w') || (wflag == '-'));

  xflag = data[dataIdx++];
  assert((xflag == 'x') || (xflag == '-'));

  sflag = data[dataIdx++];
  assert((sflag == 's') || (sflag == 'p'));

  assert(data[dataIdx++] == ' ');

  area->offset = readHex();
  assert(data[dataIdx++] == ' ');

  area->devmajor = readHex();
  assert(data[dataIdx++] == ':');

  area->devminor = readHex();
  assert(data[dataIdx++] == ' ');

  area->inodenum = readDec();

  while (data[dataIdx] == ' ') {
    dataIdx++;
  }

  area->name[0] = '\0';
  if (data[dataIdx] == '/' || data[dataIdx] == '[' || data[dataIdx] == '(') {
    // absolute pathname, or [stack], [vdso], etc.
    // On some machines, deleted files have a " (deleted)" prefix to the
    // filename.
    size_t i = 0;
    while (data[dataIdx] != '\n') {
      area->name[i++] = data[dataIdx++];
      assert(i < sizeof(area->name));
    }
    area->name[i] = '\0';
  }

  assert(data[dataIdx++] == '\n');

  area->prot = 0;
  if (rflag == 'r') {
    area->prot |= PROT_READ;
  }
  if (wflag == 'w') {
    area->prot |= PROT_WRITE;
  }
  if (xflag == 'x') {
    area->prot |= PROT_EXEC;
  }

  area->flags = MAP_FIXED;
  if (sflag == 's') {
    area->flags |= MAP_SHARED;
  }
  if (sflag == 'p') {
    area->flags |= MAP_PRIVATE;
  }
  if (area->name[0] == '\0') {
    area->flags |= MAP_ANONYMOUS;
  }

  area->properties = 0;

  return 1;
}

unsigned long int
procPidMaps::readHex()
{
  unsigned long int v = 0;

  while (1) {
    char c = data[dataIdx];
    if ((c >= '0') && (c <= '9')) {
      c -= '0';
    } else if ((c >= 'a') && (c <= 'f')) {
      c -= 'a' - 10;
    } else if ((c >= 'A') && (c <= 'F')) {
      c -= 'A' - 10;
    } else {
      break;
    }
    v = v * 16 + c;
    dataIdx++;
  }
  return v;
}

unsigned long int
procPidMaps::readDec()
{
  unsigned long int v = 0;

  while (1) {
    char c = data[dataIdx];
    if ((c >= '0') && (c <= '9')) {
      c -= '0';
    } else {
      break;
    }
    v = v * 10 + c;
    dataIdx++;
  }
  return v;
}

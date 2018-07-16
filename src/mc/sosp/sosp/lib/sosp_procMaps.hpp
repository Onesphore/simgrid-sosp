/*************************************************
TODO:                                            *
 - Give credit to DMTCP                          *
 - Explain, extensively, what the class does     *
                                                 *
**************************************************/

#ifndef SOSP_PROCMAPS_HPP
#define SOSP_PROCMAPS_HPP

#include <stdint.h>
#include <sys/types.h>
#include <vector>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE);} while(0)

using std::vector;

namespace SOSP {

//const int FILENAMESIZE = 1024;

typedef char *VA;  /* VA = virtual address */
typedef union ProcMapsArea {
  struct {
    union {
      VA addr;   // args required for mmap to restore memory area
      uint64_t __addr;
    };
    union {
      VA endAddr;   // args required for mmap to restore memory area
      uint64_t __endAddr;
    };
    union {
      size_t size;
      uint64_t __size;
    };
    union {
      off_t offset;
      uint64_t __offset;
    };
    union {
      int prot;
      uint64_t __prot;
    };
    union {
      int flags;
      uint64_t __flags;
    };
    union {
      unsigned int long devmajor;
      uint64_t __devmajor;
    };
    union {
      unsigned int long devminor;
      uint64_t __devminor;
    };
    union {
      ino_t inodenum;
      uint64_t __inodenum;
    };

    uint64_t properties;

    // char name[FILENAMESIZE];
    char name[1024];
  };
  char _padding[4096];
} ProcMapsArea;

typedef ProcMapsArea Area;

class procPidMaps
{
  public:
    procPidMaps();
    ~procPidMaps();

    size_t getNumAreas() const { return numAreas; }

    int getNextArea(ProcMapsArea *area);

  private:
    unsigned long int readDec();
    unsigned long int readHex();
    bool isValidData();

    char *data;
    size_t dataIdx;
    size_t numAreas;
    size_t numBytes;
    int fd;
    int numAllocExpands;
};

typedef struct {
  size_t maps_num;
  vector<Area> maps_vector;
} sosp_procMaps;

}

#endif // #ifndef SOSP_PROCMAPS_HPP

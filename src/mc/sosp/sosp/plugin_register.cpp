/***************
TODO: Get rid of this file.
***************/

#include <stdio.h>
#include "dmtcp.h"
#include "sosp.hpp"

static void
pre_checkpoint() {
  printf("before checkpointing ....\n");
}

static void
post_checkpoint() {
  printf("after checkpointing ....\n");
}

static DmtcpBarrier barriers[] = {
  { DMTCP_GLOBAL_BARRIER_PRE_CKPT, pre_checkpoint, "pre_checkpoint" },
  { DMTCP_GLOBAL_BARRIER_RESUME, post_checkpoint, "post_checkpoint" }
};

DmtcpPluginDescriptor_t sosp_plugin = {
  DMTCP_PLUGIN_API_VERSION,
  DMTCP_PACKAGE_VERSION,
  "sosp",
  "DMTCP",
  "dmtcp@ccs.neu.edu",
  "sosp plugin",
  DMTCP_DECL_BARRIERS(barriers),
  NULL
};

DMTCP_DECL_PLUGIN(sosp_plugin);

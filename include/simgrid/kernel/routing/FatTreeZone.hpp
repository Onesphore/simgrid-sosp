/* Copyright (c) 2014-2018. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef SIMGRID_ROUTING_CLUSTER_FAT_TREE_HPP_
#define SIMGRID_ROUTING_CLUSTER_FAT_TREE_HPP_

#include <simgrid/kernel/routing/ClusterZone.hpp>

namespace simgrid {
namespace kernel {
namespace routing {

class XBT_PRIVATE FatTreeLink;

/** \brief A node in a fat tree (@ref FatTreeZone).
 * A FatTreeNode can either be a switch or a processing node. Switches are
 * identified by a negative ID. This class is closely related to fat
 */
class XBT_PRIVATE FatTreeNode {
public:
  /** Unique ID which identifies every node. */
  int id;
  /* Level into the tree, with 0 being the leafs.
   */
  unsigned int level;
  /* \brief Position into the level, starting from 0.
   */
  unsigned int position;
  /** In order to link nodes between them, each one must be assigned a label,
   * consisting of l integers, l being the levels number of the tree. Each label
   * is unique in the level, and the way it is generated allows the construction
   * of a fat tree which fits the desired topology.
   */
  std::vector<unsigned int> label;

  /** Links to the lower level, where the position in the vector corresponds to
   * a port number.
   */
  std::vector<FatTreeLink*> children;
  /** Links to the upper level, where the position in the vector corresponds to
   * a port number.
   */
  std::vector<FatTreeLink*> parents;

  /** Virtual link standing for the node global capacity.
   */
  resource::LinkImpl* limiter_link_;
  /** If present, communications from this node to this node will pass through it
   * instead of passing by an upper level switch.
   */
  resource::LinkImpl* loopback;
  FatTreeNode(ClusterCreationArgs* cluster, int id, int level, int position);
};

/** \brief Link in a fat tree (@ref FatTreeZone).
 *
 * Represents a single, duplex link in a fat tree. This is necessary to have a tree.
 * It is equivalent to a physical link.
 */
class FatTreeLink {
public:
  FatTreeLink(ClusterCreationArgs* cluster, FatTreeNode* source, FatTreeNode* destination);
  /** Link going up in the tree */
  resource::LinkImpl* up_link_;
  /** Link going down in the tree */
  resource::LinkImpl* down_link_;
  /** Upper end of the link */
  FatTreeNode* up_node_;
  /** Lower end of the link */
  FatTreeNode* down_node_;
};

/** @ingroup ROUTING_API
 * @brief NetZone using a Fat-Tree topology
 *
 * Generate fat trees according to the topology asked for, according to:
 * Eitan Zahavi, D-Mod-K Routing Providing Non-Blocking Traffic for Shift
 * Permutations on Real Life Fat Trees (2010).
 *
 * RLFT are PGFT with some restrictions to address real world constraints,
 * which are not currently enforced.
 *
 * The exact topology is described in the mandatory topo_parameters
 * field, and follow the "h ; m_1, ..., m_h ; w_1, ..., w_h ; p_1, ..., p_h" format.
 * h stands for the switches levels number, i.e. the fat tree is of height h,
 * without the processing nodes. m_i stands for the number of lower level nodes
 * connected to a node in level i. w_i stands for the number of upper levels
 * nodes connected to a node in level i-1. p_i stands for the number of
 * parallel links connecting two nodes between level i and i - 1. Level h is
 * the topmost switch level, level 1 is the lowest switch level, and level 0
 * represents the processing nodes. The number of provided nodes must be exactly
 * the number of processing nodes required to fit the topology, which is the
 * product of the m_i's.
 *
 * Routing is made using a destination-mod-k scheme.
 */
class XBT_PRIVATE FatTreeZone : public ClusterZone {
public:
  explicit FatTreeZone(NetZoneImpl* father, std::string name, resource::NetworkModel* netmodel);
  ~FatTreeZone() override;
  void get_local_route(NetPoint* src, NetPoint* dst, RouteCreationArgs* into, double* latency) override;

  /** \brief Generate the fat tree
   *
   * Once all processing nodes have been added, this will make sure the fat
   * tree is generated by calling generateLabels(), generateSwitches() and
   * then connection all nodes between them, using their label.
   */
  void seal() override;
  /** \brief Read the parameters in topo_parameters field.
   *
   * It will also store the cluster for future use.
   */
  void parse_specific_arguments(ClusterCreationArgs* cluster) override;
  void add_processing_node(int id);
  void generate_dot_file(const std::string& filename = "fat_tree.dot") const;

private:
  // description of a PGFT (TODO : better doc)
  unsigned long levels_ = 0;
  std::vector<unsigned int> num_children_per_node_; // number of children by node
  std::vector<unsigned int> num_parents_per_node_;  // number of parents by node
  std::vector<unsigned int> num_port_lower_level_;  // ports between each level l and l-1

  std::map<int, FatTreeNode*> compute_nodes_;
  std::vector<FatTreeNode*> nodes_;
  std::vector<FatTreeLink*> links_;
  std::vector<unsigned int> nodes_by_level_;

  ClusterCreationArgs* cluster_ = nullptr;

  void add_link(FatTreeNode* parent, unsigned int parent_port, FatTreeNode* child, unsigned int child_port);
  int get_level_position(const unsigned int level);
  void generate_labels();
  void generate_switches();
  int connect_node_to_parents(FatTreeNode* node);
  bool are_related(FatTreeNode* parent, FatTreeNode* child);
  bool is_in_sub_tree(FatTreeNode* root, FatTreeNode* node);
};
} // namespace routing
} // namespace kernel
} // namespace simgrid

#endif

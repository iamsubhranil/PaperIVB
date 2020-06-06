#pragma once

#include "common.h"
#include "utils.h"

//#define GRAPH_ENABLE_TEST_FLOYD_WARSHALL // enable test for Floyd's and
// Warshall's algorithm

// Processors

// Process an edge (u,v)
typedef void (*graph_process_edge)(siz u, siz v);
// Process a vertex u
typedef void (*graph_process_vertex)(siz u);
// Process a path stored in a previous visitor array
// If prev[3] = 1 then the vertex visited before vertex 3 is 1
typedef void (*graph_process_path)(siz prev[], siz vsource, siz vdest);
// Process a path stored in linked list
typedef void (*graph_process_linked_path)(LinkedNode *path);
// Process a linked path, along with its length
typedef void (*graph_process_linked_path_with_length)(LinkedNode *path,
                                                      i64         length);

// Algorithms
void graph_bfs(u8 **adjacency_matrix, siz vertices, siz vstart,
               graph_process_vertex process);
void graph_dfs(u8 **adjacency_matrix, siz vertices, siz vstart,
               graph_process_vertex process);
i64  graph_dijkstras_shortest_path(i64 **length_matrix, siz vertices,
                                   siz vsource, siz vdest,
                                   graph_process_path process);
void graph_floyds_algorithm(i64 **length_matrix, siz vertices,
                            graph_process_linked_path_with_length process);
void graph_warshalls_algorithm(u8 **adjacency_matrix, siz vertices,
                               graph_process_linked_path process);
i64  graph_prims_mst(i64 **length_matrix, siz vertices,
                     graph_process_edge process);
i64  graph_krushkals_mst(i64 **length_matrix, siz vertices,
                         graph_process_edge process);

// Utility Methods
void graph_print_path(siz prev[], siz vsource, siz vdest);
void graph_print_linked_path(LinkedNode *node);
void graph_print_linked_path_with_length(LinkedNode *node, i64 length);
void graph_print_vertex(siz u);
void graph_print_edge(siz u, siz v);

// The second argument must be comma separated (n x n) values,
// where each row and column will correspond to one vertex each
u8 ** graph_create_matrix_adjacency(siz n, ...);
i64 **graph_create_matrix_length(siz n, ...);

void test_graph();

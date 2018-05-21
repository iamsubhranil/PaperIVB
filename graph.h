#pragma once

#include "common.h"

// Process an edge (u,v)
typedef void (*graph_process_edge)(siz u, siz v);
// Process a vertex u
typedef void (*graph_process_vertex)(siz u);
// Process a path stored in a previous visitor array
// If prev[3] = 1 then the vertex visited before vertex 3 is 1
typedef void (*graph_process_path)(siz prev[], siz vsource, siz vdest);

void graph_bfs(u8 **adjacency_matrix, siz vertices, siz vstart, graph_process_vertex process);
void graph_dfs(u8 **adjacency_matrix, siz vertices, siz vstart, graph_process_vertex process);
i64 graph_dijkstras_shortest_path(i64 **length_matrix, siz vertices, siz vsource, siz vdest, graph_process_path process);
void graph_print_path(siz prev[], siz vsource, siz vdest);
void graph_print_vertex(siz u);
void graph_print_edge(siz u, siz v);

// The second argument must be comma separated (n x n) values,
// where each row and column will correspond to one vertex each
u8 **graph_create_matrix_adjacency(siz n, ...);
i64 **graph_create_matrix_length(siz n, ...);

void test_graph();

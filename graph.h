#pragma once

#include "common.h"

// Process an edge (u,v)
typedef void (*graph_process_edge)(siz u, siz v);
// Process a vertex u
typedef void (*graph_process_vertex)(siz u);

void graph_bfs(u8 **adjacency_matrix, siz vertices, siz vstart, graph_process_vertex process);

void test_graph();

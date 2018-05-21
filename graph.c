#include <stdarg.h>
#include <stdio.h>
#include <limits.h>

#include "display.h"
#include "graph.h"
#include "queue.h"
#include "stack.h"
#include "test.h"

void graph_print_path(siz prev[], siz vsource, siz vdest){
    if(vdest != vsource){
        graph_print_path(prev, vsource, prev[vdest]);
    }
    printf(ANSI_FONT_BOLD "%" Psiz ANSI_COLOR_RESET " --> ", vdest + 1);
}

void graph_print_vertex(siz u){
    printf("%" Psiz, u);
}

void graph_print_edge(siz u, siz v){
    printf("(%" Psiz ", %" Psiz")", u, v);
}

u8 **graph_create_matrix_adjacency(siz n, ...){
    u8 **adjacency = (u8 **)malloc(sizeof(u8 *) * n);
    for(siz i = 0;i < n;i++)
        adjacency[i] = (u8 *)malloc(sizeof(u8) * n);

    va_list vals;
    va_start(vals, n);
    
    for(siz i = 0;i < n;i++){
        for(siz j = 0;j < n;j++){
            adjacency[i][j] = va_arg(vals, int) ? 1 : 0;
        }
    }
    va_end(vals);
    return adjacency;
}

i64 **graph_create_matrix_length(siz n, ...){
    i64 **length = (i64 **)malloc(sizeof(i64 *) * n);
    for(siz i = 0;i < n;i++)
        length[i] = (i64 *)malloc(sizeof(i64) * n);

    va_list vals;
    va_start(vals, n);
    
    for(siz i = 0;i < n;i++){
        for(siz j = 0;j < n;j++){
            length[i][j] = va_arg(vals, int);
        }
    }

    va_end(vals);

    return length;
}

void graph_bfs(u8 **adjacency_matrix, siz vertices, siz vstart, graph_process_vertex process){
    u8 status[vertices];

    vstart--;

    for(siz i = 0; i < vertices; i++)
        status[i] = 0;

    status[vstart] = 1;
    process(vstart + 1);

    Queue q = queue_new(vertices);
    queue_insert(q, vstart);
    while(!queue_is_empty(q)){
        siz u = queue_delete(q);
        for(siz v = 0;v < vertices;v++){
            if(status[v] == 0){
                if(adjacency_matrix[u][v] == 1){
                    process(v + 1);
                    status[v] = 1;
                    queue_insert(q, v);
                }
            }
        }
        status[u] = 2;
    }
    queue_free(q);
}

void graph_dfs(u8 **adjacency_matrix, siz vertices, siz vstart, graph_process_vertex process){
    u8 status[vertices], all_visited = 0;
    
    vstart--;

    for(siz i = 0;i < vertices;i++)
        status[i] = 0;
    status[vstart] = 1;
    process(vstart + 1);

    siz vstar = vstart;

    Stack s = stack_new(vertices, 0);
    do{
        u8 found = 0;
        for(siz i = 0;i < vertices;i++){
            if(status[i] == 0){
                if(adjacency_matrix[vstar][i] == 1){
                    status[i] = 1;
                    stack_push_fast(s, vstar);
                    vstar = i;
                    process(i + 1);
                    found = 1;
                    break;
                }
            }
        }
        if(found == 0)
            vstar = stack_pop_fast(s);

        all_visited = 1;
        for(siz i = 0;i < vertices;i++){
            if(status[i] == 0){
                all_visited = 0;
                break;
            }
        }
    } while(!all_visited);
}

i64 graph_dijkstras_shortest_path(i64 **length_matrix, siz vertices, siz vsource, siz vdest, graph_process_path process){
    vsource--;
    vdest--;
    
    u8 status[vertices];
    i64 dist[vertices];
    siz prev[vertices];
    for(siz i = 0;i < vertices;i++){
        status[i] = 0;
        dist[i] = length_matrix[vsource][i];
        prev[i] = vsource;
    }

    status[vsource] = 1;
    dist[vsource] = 0;
    prev[vsource] = 0;

    while(!status[vdest]){
        siz vstar = 0;
        i64 mindist = i64_MAX;

        for(siz i = 0;i < vertices;i++){
            if(status[i] == 0 && dist[i] < mindist){
                vstar = i;
                mindist = dist[i];
            }
        }

        status[vstar] = 1;
        
        for(siz i = 0;i < vertices;i++){
            if(dist[i] > dist[vstar] + length_matrix[vstar][i]){
                dist[i] = dist[vstar] + length_matrix[vstar][i];
                prev[i] = vstar;
            }
        }
    }

    process(prev, vsource, vdest);
    
    return dist[vdest];
}

static siz graph_visited_vertices[8] = {0};
static siz graph_visited_vertices_pointer = 0;

static void graph_visit_vertex(siz v){
    graph_visited_vertices[graph_visited_vertices_pointer++] = v;
    //dbg("[BFS] Processed %" Psiz, v);
}

static u8 test_bfs(){
    /*
     *      1 ----- 4 ------- 7
     *      |     /   \       |
     *      |    3     6      |
     *      |     \   /       |
     *      2------ 5 ------- 8
     *
     *
     */
    u8 **adjacency_matrix = graph_create_matrix_adjacency(8,
                                    0, 1, 0, 1, 0, 0, 0, 0,
                                    1, 0, 0, 0, 1, 0, 0, 0,
                                    0, 0, 0, 1, 1, 0, 0, 0,
                                    1, 0, 1, 0, 0, 1, 1, 0,
                                    0, 1, 1, 0, 0, 1, 0, 1,
                                    0, 0, 0, 1, 1, 0, 0, 0,
                                    0, 0, 0, 1, 0, 0, 0, 1,
                                    0, 0, 0, 0, 1, 0, 1, 0);
    graph_bfs(adjacency_matrix, 8, 3, graph_visit_vertex);
    siz expected[] = {3, 4, 5, 1, 6, 7, 2, 8};
    for(siz i = 0;i < 8;i++){
        if(expected[i] != graph_visited_vertices[i])
            return 0;
    }
    return 1;
}

static u8 test_dfs(){
   /*
    *       2 -------- 1
    *       |          |
    *       |          |
    * 5 --- 3 -------- 4 --- 7
    *   \   |          |   /
    *    \  |          |  /
    *     \ |          | /
    *       6          8
    *
    *
    */

    u8 **adjacency = graph_create_matrix_adjacency(8,
                                0, 1, 0, 1, 0, 0, 0, 0,
                                1, 0, 1, 0, 0, 0, 0, 0,
                                0, 1, 0, 1, 1, 1, 0, 0,
                                1, 0, 1, 0, 0, 0, 1, 1,
                                0, 0, 1, 0, 0, 1, 0, 0,
                                0, 0, 1, 0, 1, 0, 0, 0,
                                0, 0, 0, 1, 0, 0, 0, 1,
                                0, 0, 0, 1, 0, 0, 1, 0);

    graph_visited_vertices_pointer = 0;
    graph_dfs(adjacency, 8, 1, graph_visit_vertex);
    siz expected [] =  {1, 2, 3, 4, 7, 8, 5, 6};
    for(siz i = 0;i < 8;i++){
        if(graph_visited_vertices[i] != expected[i])
            return 0;
    }
    return 1;
}

static void graph_store_path(siz prev[], siz vsource, siz vdest){
    if(vdest != vsource){
        graph_store_path(prev, vsource, prev[vdest]);
    }
    graph_visited_vertices[graph_visited_vertices_pointer++] = vdest + 1;
}

static u8 test_dijkstra(){
    /*          
     *         1 
     *      B --- D ------
     *  6 / |    / \     |
     *   /  |2  /1  |    | 4
     *   A  |  /    |2   |
     *    \ | /     |    |
     *   6 \|/  4   | 1  |
     *      C ----- E -- G
     *        \__  2|   /
     *         2 \  |  / 1
     *            \ | /
     *              F
     *
     */

    i64 **length_matrix = graph_create_matrix_length(7,
                            0, 3, 6, INT_MAX, INT_MAX, INT_MAX, INT_MAX,
                            3, 0, 2, 1, INT_MAX, INT_MAX, INT_MAX,
                            6, 2, 0, 1, 4, 2, INT_MAX,
                            INT_MAX, 1, 1, 0, 2, INT_MAX, 4,
                            INT_MAX, INT_MAX, 4, 2, 0, 2, 1,
                            INT_MAX, INT_MAX, 2, INT_MAX, 2, 0, 1,
                            INT_MAX, INT_MAX, INT_MAX, 4, 1, 1, 0);

    graph_visited_vertices_pointer = 0;
    i64 dist = graph_dijkstras_shortest_path(length_matrix, 7, 1, 7, graph_store_path);
    if(dist != 7)
        return 0;
    siz expected_path[] = {1, 2, 4, 5, 7};
    for(siz i = 0;i < 5;i++){
        if(graph_visited_vertices[i] != expected_path[i])
            return 0;
    }
    return 1;
}

void test_graph(){
    tst_suite_start("Graph", 3);
    TEST("Breadth First Search", test_bfs());
    TEST("Depth First Search", test_dfs());
    TEST("Dijkstra's Shortest Path", test_dijkstra());
    tst_suite_end();
}

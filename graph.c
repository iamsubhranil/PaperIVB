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

void graph_print_linked_path(LinkedNode *path){
    if(path == NULL)
        return;
    printf("\nPath : %" Pi64, path->val + 1);
    path = path->next;
    while(path != NULL){
        printf(" --> %" Pi64, path->val + 1);
        path = path->next;
    }
}

void graph_print_linked_path_with_length(LinkedNode *path, i64 length){
    if(path == NULL)
        return;
    graph_print_linked_path(path);
    printf("\t Length : %" Pi64, length);
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
    stack_free(s);
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

static LinkedNode* path_create(siz i, siz j){
    LinkedNode *n1 = linkednode_get(i);
    LinkedNode *n2 = linkednode_get(j);
    n1->next = n2;
    return n1;
}

static LinkedNode* path_combine(LinkedNode *p1, LinkedNode *p2){
    LinkedNode *root = NULL, *prev = NULL;
    while(p1->next != NULL){
        LinkedNode *n = linkednode_get(p1->val);
        if(prev == NULL)
            root = n;
        else
            prev->next = n;
        prev = n;
        p1 = p1->next;
    }
    while(p2 != NULL){
        LinkedNode *n = linkednode_get(p2->val);
        if(prev == NULL)
            root = n;
        else
            prev->next = n;
        prev = n;
        p2 = p2->next;
    }
    return root;
}

void graph_floyds_algorithm(i64 **length_matrix, siz vertices, graph_process_linked_path_with_length process){
    LinkedNode *paths[vertices][vertices];
    for(siz i = 0;i < vertices;i++){
        for(siz j = 0;j < vertices;j++){
            if(length_matrix[i][j] > 0 && length_matrix[i][j] < INT_MAX)
                paths[i][j] = path_create(i, j);
            else
                paths[i][j] = NULL;
        }
    }

    for(siz k = 0;k < vertices;k++){
        for(siz i = 0;i < vertices;i++){
            for(siz j = 0;j < vertices;j++){
                if(length_matrix[i][k] + length_matrix[k][j] < length_matrix[i][j]){
                    length_matrix[i][j] = length_matrix[i][k] + length_matrix[k][j];
                    LinkedNode *p1 = paths[i][k];
                    LinkedNode *p2 = paths[k][j];
                    linkednode_free(paths[i][j]);
                    paths[i][j] = path_combine(p1, p2);
                }
            }
        }
    }

    for(siz i = 0;i < vertices;i++){
        for(siz j = 0;j < vertices;j++){
            process(paths[i][j], length_matrix[i][j]);
            linkednode_free(paths[i][j]);
        }
    }
}

void graph_warshalls_algorithm(u8 **adjacency_matrix, siz vertices, graph_process_linked_path process){
    LinkedNode *paths[vertices][vertices];
    for(siz i = 0;i < vertices;i++){
        for(siz j = 0;j < vertices;j++){
            if(adjacency_matrix[i][j])
                paths[i][j] = path_create(i, j);
            else
                paths[i][j] = NULL;
        }
    }

    for(siz k = 0;k < vertices;k++){
        for(siz i = 0;i < vertices;i++){
            for(siz j = 0;j < vertices;j++){
                if(adjacency_matrix[i][j] == 0){
                    u8 can_reach = adjacency_matrix[i][k] && adjacency_matrix[k][j];
                    if(can_reach){
                        adjacency_matrix[i][j] = 1;
                        LinkedNode *p1 = paths[i][k];
                        LinkedNode *p2 = paths[k][j];
                        linkednode_free(paths[i][j]);
                        paths[i][j] = path_combine(p1, p2);
                    }
                }
            }
        }
    }

    for(siz i = 0;i < vertices;i++){
        for(siz j = 0;j < vertices;j++){
            process(paths[i][j]);
            linkednode_free(paths[i][j]);
        }
    }
}

i64 graph_prims_mst(i64 **length_matrix, siz vertices, graph_process_edge process){
    for(siz i = 0;i < vertices;i++){
        for(siz j = 0;j < vertices;j++){
            if(length_matrix[i][j] == 0)
                length_matrix[i][j] = i64_MAX;
        }
    }
    
    u8 status[vertices];
    for(siz i = 0;i < vertices;i++)
        status[i] = 0;
    status[0] = 1;

    siz number_of_edges = 0;
    i64 cost = 0;

    while(number_of_edges < vertices - 1){
        i64 min = i64_MAX;
        siz su, sv;
        for(siz u = 0;u < vertices;u++){
            if(status[u] == 1){
                for(siz v = 0;v < vertices;v++){
                    if(status[v] == 0 && length_matrix[u][v] < min){
                        min = length_matrix[u][v];
                        su = u;
                        sv = v;
                    }
                }
            }
        }
        process(su + 1, sv + 1);
        status[sv] = 1;
        length_matrix[su][sv] = i64_MAX;
        length_matrix[sv][su] = i64_MAX;
        number_of_edges++;
        cost += min;
    }
    return cost;
}

static siz root_of(siz parent[], siz vertex){
    while(parent[vertex] != 0)
        vertex = parent[vertex];
    return vertex;
}

i64 graph_krushkals_mst(i64 **length_matrix, siz vertices, graph_process_edge process){
    siz parent[vertices];
    for(siz i = 0;i < vertices;i++)
        parent[i] = 0;

    siz number_of_edges = 0;
    i64 cost = 0;
    while(number_of_edges < vertices - 1){
        i64 min = i64_MAX;
        siz u, v;
        for(siz i = 0;i < vertices;i++){
            for(siz j = 0;j < vertices;j++){
                if(i != j && length_matrix[i][j] < min){
                    min = length_matrix[i][j];
                    u = i;
                    v = j;
                }
            }
        }
        siz ru = root_of(parent, u);
        siz rv = root_of(parent, v);
        if(ru != rv){
            number_of_edges++;
            process(u + 1, v + 1);
            parent[rv] = ru;
            cost += length_matrix[u][v];
        }
        length_matrix[u][v] = i64_MAX;
        length_matrix[v][u] = i64_MAX;
    }
    return cost;
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

#ifdef GRAPH_ENABLE_TEST_FLOYD_WARSHALL
static u8 test_floyd(){
    info("Input graph");        
    info("        |----------------");
    info("  |---- 1 ------> 2     |");
    info("  |    /|\\  2     |     |");
    info("  |___  |_______  |5    |8");
    info("   10 |    3    \\ |     |");
    info("     \\|/         \\|/    |");
    info("      4 <-------- 3 <----");
    info("Output : ");
    i64 **length_matrix = graph_create_matrix_length(4,
            0, 2, 8, 10,
            INT_MAX, 0, 5, INT_MAX,
            3, INT_MAX, 0, 1,
            INT_MAX, INT_MAX, INT_MAX, 0);

    graph_floyds_algorithm(length_matrix, 4, graph_print_linked_path_with_length);
    printf("\n");
    return 1;
}

static u8 test_warshalls(){
    info("Input graph");        
    info("        |----------------");
    info("  |---- 1 ------> 2     |");
    info("  |    /|\\        |     |");
    info("  |___  |_______  |     | ");
    info("      |         \\ |     |");
    info("     \\|/         \\|/    |");
    info("      4 <-------- 3 <----");
    info("Output : ");

    u8 **adjacency_matrix = graph_create_matrix_adjacency(4,
            0, 1, 1, 0,
            0, 0, 1, 0,
            1, 0, 0, 1,
            1, 0, 0, 0);

    graph_warshalls_algorithm(adjacency_matrix, 4, graph_print_linked_path);
    printf("\n");
    return 1;
}
#endif

static siz graph_stored_edges[4][2] = {{0}};
static siz graph_stored_edge_pointer = 0;

static void graph_store_edge(siz u, siz v){
    graph_stored_edges[graph_stored_edge_pointer][0] = u;
    graph_stored_edges[graph_stored_edge_pointer][1] = v;
    graph_stored_edge_pointer++;
}

static u8 test_prims(){
    /* 
     *     2    3
     * (1)--(2)--(3)
     *  |   / \   |
     * 6| 8/   \5 |7
     *  | /     \ |
     * (4)-------(5)
     *      9          
     */
    
    i64 **length_matrix = graph_create_matrix_length(5,
            0, 2, INT_MAX, 6, INT_MAX,
            2, 0, 3, 8, 5,
            INT_MAX, 3, 0, INT_MAX, 7,
            6, 8, INT_MAX, 0, 9,
            INT_MAX, 5, 7, 9, 0);
    i64 cost = graph_prims_mst(length_matrix, 5, graph_store_edge);
    siz chosen_edges[4][2] = {{1, 2}, {2, 3}, {2, 5}, {1, 4}};
    for(siz i = 0;i < 4;i++){
        if(graph_stored_edges[i][0] != chosen_edges[i][0]
                || graph_stored_edges[i][1] != chosen_edges[i][1])
            return 0;
    }
    if(cost != 16)
        return 0;
    return 1;
}

static u8 test_krushkals(){
    /* 
     *     2    3
     * (1)--(2)--(3)
     *  |   / \   |
     * 6| 8/   \5 |7
     *  | /     \ |
     * (4)-------(5)
     *      9          
     */
    
    i64 **length_matrix = graph_create_matrix_length(5,
            0, 2, INT_MAX, 6, INT_MAX,
            2, 0, 3, 8, 5,
            INT_MAX, 3, 0, INT_MAX, 7,
            6, 8, INT_MAX, 0, 9,
            INT_MAX, 5, 7, 9, 0);

    graph_stored_edge_pointer = 0;
    i64 cost = graph_krushkals_mst(length_matrix, 5, graph_store_edge);
    siz chosen_edges[4][2] = {{1, 2}, {2, 3}, {2, 5}, {1, 4}};
    for(siz i = 0;i < 4;i++){
        if(graph_stored_edges[i][0] != chosen_edges[i][0]
                || graph_stored_edges[i][1] != chosen_edges[i][1])
            return 0;
    }
    if(cost != 16)
        return 0;
    return 1;
}

void test_graph(){
#ifdef GRAPH_ENABLE_TEST_FLOYD_WARSHALL
    tst_suite_start("Graph", 7);
#else
    tst_suite_start("Graph", 5);
#endif
    TEST("Breadth First Search", test_bfs());
    TEST("Depth First Search", test_dfs());
    TEST("Dijkstra's Shortest Path", test_dijkstra());
#ifdef GRAPH_ENABLE_TEST_FLOYD_WARSHALL
    TEST("Flyod's Algorithm", test_floyd());
    TEST("Warshall's Algorithm", test_warshalls());
#endif
    TEST("Prim's Minimal Spanning Tree", test_prims());
    TEST("Krushkal's Minimal Spanning Tree", test_krushkals());
    tst_suite_end();
}

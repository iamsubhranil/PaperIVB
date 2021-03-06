project(PaperIVB)

cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0003 NEW)

add_library(cargparser SHARED CargParser/cargparser.c)

add_library(display SHARED display.c)

add_library(utils SHARED utils.c)

add_library(test SHARED test.c)
target_link_libraries(test display)

add_library(array SHARED arr.c)
target_link_libraries(array test
                            display
                            utils)

add_library(dump SHARED dump.c)
target_link_libraries(dump  display)

add_library(stack SHARED stack.c)
target_link_libraries(stack test
                            utils)

add_library(queue SHARED queue.c queue_fast.c queue_linked.c)
target_link_libraries(queue test)

add_library(histogram SHARED histogram.c)

add_library(sort SHARED sort.c)
target_link_libraries(sort  test
                            stack
                            array
                            histogram
                            queue)

add_library(bst SHARED bst.c)
target_link_libraries(bst   test
                            array
                            utils
                            sort
                            stack
                            display)

add_library(avl SHARED avl.c)
target_link_libraries(avl   test
                            array
                            display
                            utils)

add_library(graph SHARED graph.c)
target_link_libraries(graph queue
                            stack
                            display
                            test)

add_library(ht SHARED hashtable.c)
target_link_libraries(ht    test
                            utils)

add_library(numerical SHARED numerical.c)
target_link_libraries(numerical test
                                m)

add_executable(paperivb main.c)
target_link_libraries(paperivb  array
                                stack
                                sort
                                bst
                                queue
                                ht
                                avl
                                dump
                                graph
                                numerical
                                cargparser)

add_definitions(-DSORT_ENABLE_TEST_ALL -DSORT_TEST_ITEM_COUNT=50000 -DBST_TEST_ITEM_COUNT=50000 -DAVL_TEST_ITEM_COUNT=50000)

add_library(display SHARED display.c)

add_library(utils SHARED utils.c)

add_library(test SHARED test.c)
target_link_libraries(test display)

add_library(array SHARED arr.c)
target_link_libraries(array test
                            display
                            utils)

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
                            utils)

add_executable(paperivb main.c)
target_link_libraries(paperivb sort
                                bst
                                queue)

add_definitions(-DSORT_ENABLE_TEST_ALL -DSORT_TEST_ITEM_COUNT=100000 -DBST_TEST_ITEM_COUNT=100000)

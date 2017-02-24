include_directories(${cxwin_INCLUDE_DIRS})

find_library(COCOA_FRAMEWORK AppKit)

add_executable(test_cxwin test_cxwin.c)
target_link_libraries(test_cxwin "${COCOA_FRAMEWORK}")
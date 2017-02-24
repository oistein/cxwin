include_directories(${cxwin_INCLUDE_DIRS})

if (APPLE)
    list(APPEND cxwin_test_LIBRARIES
            "-framework Cocoa")
endif ()

add_executable(test_cxwin test_cxwin.c)
target_link_libraries(test_cxwin "${cxwin_test_LIBRARIES}")
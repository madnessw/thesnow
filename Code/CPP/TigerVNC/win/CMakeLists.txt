add_definitions(-D_WIN32_IE=0x0500 -D_WIN32_WINNT=0x0500)

include_directories(${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/win)

configure_file(resdefs.h.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/resdefs.h)

add_subdirectory(rfb_win32)
add_subdirectory(vncviewer)
if(BUILD_WINVNC)
add_subdirectory(vncconfig)
add_subdirectory(winvnc)
endif()

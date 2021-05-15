file(GLOB_RECURSE SRC_FILES ../mahjong/*.cpp)
file(GLOB ENTRY_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if(EMSCRIPTEN)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_EXIT_RUNTIME=1 -s ALLOW_MEMORY_GROWTH=1 --bind --preload-file ${CMAKE_SOURCE_DIR}/data/config/@res")
endif()

foreach(ENTRY_FILE ${ENTRY_FILES})
  get_filename_component(BASE_NAME ${ENTRY_FILE} NAME_WE)
  add_executable(${BASE_NAME} ${SRC_FILES} ${ENTRY_FILE})
  target_link_libraries(${BASE_NAME} ${Boost_LIBRARIES} spdlog ${CMAKE_DL_LIBS})
endforeach(ENTRY_FILE ${ENTRY_FILES})

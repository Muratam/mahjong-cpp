file(GLOB_RECURSE SRC_FILES ../mahjong/*.cpp)
file(GLOB ENTRY_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if(EMSCRIPTEN)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1 --bind")
endif()

foreach(ENTRY_FILE ${ENTRY_FILES})
  get_filename_component(BASE_NAME ${ENTRY_FILE} NAME_WE)
  add_executable(${BASE_NAME} ${SRC_FILES} ${ENTRY_FILE})
  target_link_libraries(${BASE_NAME} ${Boost_LIBRARIES} spdlog ${CMAKE_DL_LIBS})
  if(EMSCRIPTEN)
    target_link_libraries(${BASE_NAME} nodefs.js)
  endif()
  add_custom_command(TARGET ${BASE_NAME} PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                     ${CMAKE_SOURCE_DIR}/data/config/ $<TARGET_FILE_DIR:${BASE_NAME}>)
endforeach(ENTRY_FILE ${ENTRY_FILES})

if(ENABLE_ASIO_LIB)
  install(FILES
      nghttp2/asio_http2.h
      nghttp2/asio_http2_client.h
      nghttp2/asio_http2_server.h
      nghttp2/http_router.hpp
      nghttp2/tree.h
      nghttp2/function_traits.hpp
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/nghttp2")
endif()
include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}")
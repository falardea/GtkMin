cmake_minimum_required(VERSION      3.16)

project(GtkMin VERSION          0.0.1  LANGUAGES C CXX)

set(CMAKE_C_STANDARD                11)
set(CMAKE_CXX_STANDARD              11)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)

# Dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)
set(GRESOURCE_C                     mini_app.gresource.c)
set(GRESOURCE_XML                   mini_app.gresource.xml)

set(PROJECT_TARGET mini_app)
set(PROJECT_SOURCE_DIR              ./src)
set(PROJECT_BINARY_DIR              ./target)

set(CSS_TARGET mini_app_style.css)

set(PROJECT_RESOURCE_DIR            ${CMAKE_SOURCE_DIR}/resources)
set(STYLE_DIR                       ${PROJECT_RESOURCE_DIR}/style)
set(SCSS_SOURCE                     ${STYLE_DIR}/scss/mini_app_style.scss)
set(CSS_OUTFILE                     ${STYLE_DIR}/${CSS_TARGET})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_compile_options(${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} -Wall -Wextra -rdynamic)
add_link_options(-rdynamic)

add_custom_command(OUTPUT ${CSS_OUTFILE} COMMAND sassc -t expanded ${SCSS_SOURCE} ${CSS_OUTFILE})

# This will build our resource bundle
# From: https://stackoverflow.com/questions/63697778/how-to-use-glib-compile-resources-with-cmake
add_custom_command(
      OUTPUT ${GRESOURCE_C}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMAND ${GLIB_COMPILE_RESOURCES}
      ARGS
      --generate-source
      --target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
      ${PROJECT_RESOURCE_DIR}/${GRESOURCE_XML}
      VERBATIM
      MAIN_DEPENDENCY ${PROJECT_RESOURCE_DIR}/${GRESOURCE_XML}
      DEPENDS
      ${PROJECT_RESOURCE_DIR}/mini_app.glade
      ${CSS_OUTFILE}
)
add_custom_target(mini_app-resource DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})

set(MY_SOURCES
    src/main.c
    src/comm.h
    src/version.h
    src/utils/parse_args.c
    src/utils/parse_args.h
    src/utils/logging.c
    src/utils/logging.h
    src/utils/sys_interface.c
    src/utils/sys_interface.h
    src/controllers/app_ctrl.c
    src/controllers/app_ctrl.h
    src/models/app_model.c
    src/models/app_model.h
    src/views/ui_builder.c
    src/views/ui_builder.h
    src/views/css_styler.c
    src/views/css_styler.h
    src/views/root_window.c
    src/views/root_window.h
    src/views/root_child_msgout.h
    src/views/root_child_msgout.c
    src/views/root_child_main_content.h
    src/views/root_child_main_content.c
      src/models/tictactoe.c
      src/models/tictactoe.h
      src/models/model_device.c
      src/models/model_device.h
      src/models/gtkdial.c
      src/models/gtkdial.h
)

include_directories(${PROJECT_SOURCE_DIR})
configure_file("${PROJECT_SOURCE_DIR}/version.h.in" "${PROJECT_SOURCE_DIR}/version.h")

# Executable
add_executable(${PROJECT_TARGET} ${MY_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})

target_include_directories(${PROJECT_TARGET} PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(${PROJECT_TARGET} ${GTK3_LIBRARIES} PkgConfig::deps)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C} PROPERTIES GENERATED TRUE )
add_dependencies(${PROJECT_TARGET} mini_app-resource)
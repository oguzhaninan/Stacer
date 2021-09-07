cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(stacer)

set(MANAGERS_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/Managers")
set(PAGES_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/Pages")

include_directories(
  "${PROJECT_ROOT}/stacer-core"
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${MANAGERS_DIR}"
  "${PAGES_DIR}/Dashboard"
  "${PAGES_DIR}/Processes"
  "${PAGES_DIR}/Resources"
  "${PAGES_DIR}/Services"
  "${PAGES_DIR}/Settings"
  "${PAGES_DIR}/StartupApps"
  "${PAGES_DIR}/SystemCleaner"
  "${PAGES_DIR}/Uninstaller"
  "${CMAKE_CURRENT_BINARY_DIR}" # Necessary for CMake 3.7 and older
)

# Sources
file(GLOB_RECURSE ${PROJECT_NAME}_srcs         "${CMAKE_CURRENT_SOURCE_DIR}/**.cpp")
file(GLOB_RECURSE ${PROJECT_NAME}_translations "${PROJECT_ROOT}/translations/**.ts")

# Translations
find_package(Qt${QT_VERSION_MAJOR}LinguistTools)
if ("${QT_VERSION_MAJOR}" STREQUAL "5")
  qt5_create_translation(QM_FILES  ${PROJECT_NAME}_translations  ${${PROJECT_NAME}_srcs})
else()
  qt6_create_translation(QM_FILES  ${PROJECT_NAME}_translations  ${${PROJECT_NAME}_srcs})
endif()
set_directory_properties(PROPERTIES  ADDITIONAL_MAKE_CLEAN_FILES  "${QM_FILES}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_srcs}
  "${CMAKE_CURRENT_SOURCE_DIR}/static.qrc"
  ${QM_FILES}
)

target_link_libraries(${PROJECT_NAME} stacer-core)
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Concurrent)

# Running LTO in Release builds, if the C++ compiler is GNU GCC
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release"  AND  "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(${PROJECT_NAME}  PRIVATE  "-flto")
  list(APPEND CMAKE_EXE_LINKER_FLAGS "-flto")
endif()

install(
  TARGETS  ${PROJECT_NAME}
  CONFIGURATIONS Release RelWithDebInfo MinSizeRel # Not allowing to install an unoptimized build
  RUNTIME  DESTINATION  bin
)

install(
  FILES           "${PROJECT_ROOT}/applications/stacer.desktop"
  DESTINATION     share/applications
  CONFIGURATIONS  Release RelWithDebInfo MinSizeRel
)

install(
  FILES           "${PROJECT_ROOT}/stacer/static/logo.png"
  DESTINATION     share/icons
  CONFIGURATIONS  Release RelWithDebInfo MinSizeRel
  RENAME          stacer.png
)

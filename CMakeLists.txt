# CMake最低版本号要求
cmake_minimum_required(VERSION 3.8)


# 项目名称
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_CXX_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
project(QWidget-Admin-Drag VERSION ${PROJECT_CXX_VERSION})


# 输出提示信息
message(STATUS "Operation System : ${CMAKE_SYSTEM}")
message(STATUS "CMAKE_INSTALL_PREFIX : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_C_COMPILER : ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER : ${CMAKE_CXX_COMPILER}")


# 配置文件
configure_file("${CMAKE_SOURCE_DIR}/config/VersionInfo.rc.in" "${CMAKE_BINARY_DIR}/VersionInfo.rc")


# 设置模块搜索路径
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_INSTALL_PREFIX}/lib/cmake)
if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{CXX_QT_DIR_x64}/lib/cmake)
else ()
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{CXX_QT_DIR}/lib/cmake)
endif ()
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_MODULE_PATH})
message(STATUS "CMAKE_MODULE_PATH : ${CMAKE_MODULE_PATH}")
message(STATUS "CMAKE_PREFIX_PATH : ${CMAKE_PREFIX_PATH}")


# 设置编译环境
if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall)
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Od")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /Od")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG : ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE : ${CMAKE_CXX_FLAGS_RELEASE}")


# 添加资源文件
set(QRC_SOURCE_FILES resources/XWindowApplication.qrc)
set(WRC_SOURCE_FILES resources/XWindowApplication.rc)


# 设置忽略路径
set(CMAKE_IGNORE_PATH ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
message(STATUS "CMAKE_IGNORE_PATH : ${CMAKE_IGNORE_PATH}")


# 项目依赖
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)


# 设置自动生成QT文件
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# C++支持
set(CMAKE_CXX_STANDARD 17)


# 设置宏
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DXANADU_BUILD_DEBUG)
else ()
	add_definitions(-DXANADU_BUILD_RELEASE)
endif ()


# 头文件搜索目录
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/source)


# 链接库搜索目录


# 包含源文件
aux_source_directory(./source SRC)
if(MSVC)
	list(APPEND SRC ${CMAKE_BINARY_DIR}/VersionInfo.rc)
endif()


# 链接库
if (WIN32)
	link_libraries(Ws2_32.lib)
endif ()
link_libraries(Qt5::Core)
link_libraries(Qt5::Gui)
link_libraries(Qt5::Widgets)


# 设置Mac RPATH
if (APPLE)
	set(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
	set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
endif ()


# 设置 Windows 链接标志
message(STATUS "CMAKE_EXE_LINKER_FLAGS : ${CMAKE_EXE_LINKER_FLAGS}")
if (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:\"mainCRTStartup\"")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /TSAWARE")
endif ()
message(STATUS "CMAKE_EXE_LINKER_FLAGS : ${CMAKE_EXE_LINKER_FLAGS}")


# 构建
add_executable(${PROJECT_NAME} WIN32 ${SRC} ${QRC_SOURCE_FILES} ${WRC_SOURCE_FILES})


# 设置项目属性
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)


# 设置安装文件(可执行文件)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

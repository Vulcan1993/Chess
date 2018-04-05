#设置所需最小版本为 2.8.11
cmake_minimum_required(VERSION 2.8.11)

#设置工程名
project(Chess)

#设置自动生成moc文件
set(CMAKE_AUTOMOC ON)
#指定qt的目录,
find_path(QT_DIR QT_DIR-NOTFOUND)

if(QT_DIR STREQUAL "QT_DIR-NOTFOUND")
    message(FATAL_ERROR "Can't find QT_DIR")
endif()

find_package(Qt5Widgets PATHS ${QT_DIR}/lib/cmake/ NO_SYSTEM_ENVIRONMENT_PATH)
find_package(Qt5Core PATHS ${QT_DIR}/lib/cmake/ NO_SYSTEM_ENVIRONMENT_PATH)
find_package(Qt5Gui PATHS ${QT_DIR}/lib/cmake/ NO_SYSTEM_ENVIRONMENT_PATH)
find_package(Qt5Network PATHS ${QT_DIR}/lib/cmake/ NO_SYSTEM_ENVIRONMENT_PATH)

add_subdirectory(ChinessChess ChinessChess)
add_subdirectory(Reversi Reversi)
add_subdirectory(Server Server)

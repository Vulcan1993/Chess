#设置所需最小版本为 2.8.11
cmake_minimum_required(VERSION 2.8.11)

#设置工程名
project(Reversi)

#设置自动生成moc文件
set(CMAKE_AUTOMOC ON)

find_path(QT_DIR QT_DIR-NOTFOUND)

if(QT_DIR STREQUAL "QT_DIR-NOTFOUND")
    message(FATAL_ERROR "Can't find QT_DIR")
endif()

find_package(Qt5Widgets PATHS ${QT_DIR}/lib/cmake/ NO_SYSTEM_ENVIRONMENT_PATH)
find_package(Qt5Core PATHS ${QT_DIR}/lib/cmake/ NO_SYSTEM_ENVIRONMENT_PATH)
find_package(Qt5Gui PATHS ${QT_DIR}/lib/cmake/ NO_SYSTEM_ENVIRONMENT_PATH)
find_package(Qt5Network PATHS ${QT_DIR}/lib/cmake/ NO_SYSTEM_ENVIRONMENT_PATH)

#设置包含当前目录
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEAD_FILES
    aimode.h 
    netmode.h 
    base.h 
    stacked.h 
    stackedpage.h 
    selectdialog.h 
    compconfig.h
)

set(SRC_FILES
    main.cpp 
    aimode.cpp 
    netmode.cpp 
    base.cpp 
    stacked.cpp 
    stackedpage.cpp 
    selectdialog.cpp
)

set(UI_FILES
	
)

set(RCC_FILES
   images.qrc
)
#通过.ui文件生成对应的文件
qt5_wrap_ui(WRAP_UI ${UI_FILES})

#生成的moc_xxx.cxx文件
qt5_wrap_cpp(WRAP_CPP ${HEAD_FILES})

#添加资源
qt5_add_resources(ADD_RCCS ${RCC_FILES})

#组织文件结构
source_group("Header Files" FILES ${HEAD_FILES})
source_group("Source Files" FILES ${SRC_FILES})
source_group("Form Files" FILES ${UI_FILES}) 
source_group("Resource Files" FILES ${RCC_FILES})
source_group("Generated Files" FILES ${WRAP_UI} ${WRAP_CPP} ${ADD_RCCS})

MESSAGE(STATUS "This is PROJECT_SOURCE_DIR dir " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "This is CMAKE_CURRENT_BINARY_DIR dir " ${CMAKE_CURRENT_BINARY_DIR})

message(STATUS "add  " ${ADD_RCCS})
message(STATUS "ui " ${WRAP_UI})
message(STATUS "cpp " ${WRAP_CPP})

include_directories(
	${PROJECT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)
#add_executable中需要的文件	
set(DPD_FILES
	${HEAD_FILES}
	${SRC_FILES}
	${WRAP_UI}
	#${WRAP_CPP}
	${ADD_RCCS}
)

#创建工程
add_executable(Reversi ${DPD_FILES})

#添加Qt5依赖项
target_link_libraries(Reversi
	Qt5::Widgets
	Qt5::Core
	Qt5::Gui
	Qt5::Network	
)


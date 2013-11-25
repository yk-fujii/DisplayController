# CMakeのバージョン
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
# リリースビルド用オプション
SET(CMAKE_C_FLAGS_RELEASE "-Wall -O2")
# デバッグビルド用オプション
SET(CMAKE_C_FLAGS_DEBUG "-g")
# ビルドのタイプはリリース
SET(CMAKE_BUILD_TYPE Release)

FIND_PACKAGE(X11)

# プロジェクト名
SET(target xdisp)
SET(target_source dispctrl.c)
SET(target_header "/usr/include")
SET(target_link Xmu X11 Xext)

SET(sample_source test.c)
# ビルドするためにインクルードするヘッダーファイルの場所
INCLUDE_DIRECTORIES("${target_header}")
ADD_LIBRARY(${target} ${target_source})
#TARGET_LINK_LIBRARIES(${target} ${target_link})
ADD_EXECUTABLE(sample ${sample_source} )
TARGET_LINK_LIBRARIES(sample ${target} ${target_link})


# CMakeのバージョン
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
# リリースビルド用オプション
SET(CMAKE_C_FLAGS_RELEASE "-Wall -O2")
# デバッグビルド用オプション
SET(CMAKE_C_FLAGS_DEBUG "-g")
# ビルドのタイプはリリース
SET(CMAKE_BUILD_TYPE Release)

# プロジェクト名

SET(target disp)
SET(target_source test.c dispctrl.c)
SET(target_header "/usr/include")
SET(target_link Xmu X11 Xext)

# ビルドするためにインクルードするヘッダーファイルの場所
INCLUDE_DIRECTORIES("${target_header}")
# 共有ライブラリとしてビルド
ADD_EXECUTABLE(${target} ${target_source})
TARGET_LINK_LIBRARIES(${target} ${target_link})


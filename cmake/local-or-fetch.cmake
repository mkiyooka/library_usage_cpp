# ローカルディレクトリが存在する場合はそれを使用し、
# 存在しない場合はURL（tar.gz等）またはGitHubから取得する関数
#
# 使用例:
#   # Git方式
#   add_external_package(CLI11 ext/CLI11-2.5.0
#       GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
#       GIT_TAG v2.5.0
#   )
#
#   # tar.gz方式
#   add_external_package(CLI11 ext/CLI11-2.5.0
#       URL https://github.com/CLIUtils/CLI11/archive/refs/tags/v2.5.0.tar.gz
#       URL_HASH SHA256=abc123...
#   )
#
#   FetchContent_MakeAvailable(CLI11)
#
# 引数:
#   LIBRARY_NAME: ライブラリ名（FetchContent_Declareで使用する名前）
#   LOCAL_PATH: ローカルディレクトリパス（プロジェクトルートからの相対パス）
#   GIT_REPOSITORY: GitリポジトリURL（オプション）
#   GIT_TAG: Gitタグまたはブランチ名（オプション）
#   URL: アーカイブファイルのURL（オプション）
#   URL_HASH: URLのハッシュ値（オプション、推奨）
#
# 注意:
#   FetchContent_MakeAvailable()は呼び出し側で実行してください。
#   これにより、MakeAvailable前にライブラリ固有のオプション設定が可能です。
#   例：libzmqの場合、BUILD_TESTS OFF などの設定が必要

function(add_external_package LIBRARY_NAME LOCAL_PATH)
    set(options "")
    set(oneValueArgs GIT_REPOSITORY GIT_TAG URL URL_HASH)
    set(multiValueArgs "")
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(FULL_LOCAL_PATH "${CMAKE_SOURCE_DIR}/${LOCAL_PATH}")
    if(EXISTS ${FULL_LOCAL_PATH})
        message(STATUS "Using local ${LIBRARY_NAME} from ${FULL_LOCAL_PATH}")
        FetchContent_Declare(${LIBRARY_NAME}
            SOURCE_DIR ${FULL_LOCAL_PATH}
        )
    elseif(DEFINED ARG_URL)
        message(STATUS "Downloading ${LIBRARY_NAME} from ${ARG_URL}")
        if(DEFINED ARG_URL_HASH)
            FetchContent_Declare(${LIBRARY_NAME}
                URL ${ARG_URL}
                URL_HASH ${ARG_URL_HASH}
                DOWNLOAD_EXTRACT_TIMESTAMP ON
            )
        else()
            FetchContent_Declare(${LIBRARY_NAME}
                URL ${ARG_URL}
                DOWNLOAD_EXTRACT_TIMESTAMP ON
            )
        endif()
    elseif(DEFINED ARG_GIT_REPOSITORY)
        message(STATUS "Fetching ${LIBRARY_NAME} from ${ARG_GIT_REPOSITORY}")
        FetchContent_Declare(${LIBRARY_NAME}
            GIT_REPOSITORY ${ARG_GIT_REPOSITORY}
            GIT_TAG ${ARG_GIT_TAG}
            GIT_SHALLOW TRUE
        )
    else()
        message(FATAL_ERROR "Either GIT_REPOSITORY or URL must be specified for ${LIBRARY_NAME}")
    endif()
endfunction()

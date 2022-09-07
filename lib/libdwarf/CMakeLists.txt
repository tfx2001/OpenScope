include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

message(CHECK_START "Fetching libdwarf...")
FetchContent_Declare(
        libdwarf
        GIT_REPOSITORY "https://github.com/davea42/libdwarf-code.git"
        GIT_TAG "v0.4.1"
        UPDATE_COMMAND git reset HEAD --hard
        PATCH_COMMAND git apply --ignore-whitespace "${CMAKE_CURRENT_SOURCE_DIR}/0001-fix-cmakelists.patch"
)
FetchContent_MakeAvailable(libdwarf)
message(CHECK_PASS "fetched")
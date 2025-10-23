set(OPENDAQ_ENABLE_TESTS false)

if(NOT SIMPLE_FB_MODULE_OPENDAQ_SDK_VER)
    message(FATAL_ERROR "SIMPLE_FB_MODULE_OPENDAQ_SDK_VER is not set")
endif()

include(FetchContent)

FetchContent_Declare(
    openDAQ
    GIT_REPOSITORY https://github.com/openDAQ/openDAQ.git
    GIT_TAG v${SIMPLE_FB_MODULE_OPENDAQ_SDK_VER}
    GIT_PROGRESS   ON
    SYSTEM
    FIND_PACKAGE_ARGS ${SIMPLE_FB_MODULE_OPENDAQ_SDK_VER} GLOBAL
)

FetchContent_MakeAvailable(openDAQ)

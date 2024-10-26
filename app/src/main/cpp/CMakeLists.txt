cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 ")

#find_package (oboe REQUIRED CONFIG)
#set (OBOE_DIR ./oboe)
#add_subdirectory (${OBOE_DIR} ./oboe)
#include_directories (${OBOE_DIR}/include)

add_definitions(-DHAVE_LIBDL)
add_subdirectory(lib/crypto)
add_subdirectory(lib/curve25519)
add_subdirectory(lib/ed25519)
add_subdirectory(lib/playfair)
add_subdirectory(lib/plist)
add_subdirectory(lib/fdk-aac)
add_subdirectory(lib)
add_subdirectory(lib/llhttp)

include_directories(./ffmpeg/${ANDROID_ABI}/include)


include_directories(lib
        lib/fdk-aac/libAACdec/include
        lib/fdk-aac/libAACenc/include
        lib/fdk-aac/libFDK/include
        lib/fdk-aac/libMpegTPDec/include
        lib/fdk-aac/libMpegTPEnc/include
        lib/fdk-aac/libPCMutils/include
        lib/fdk-aac/libSBRdec/include
        lib/fdk-aac/libSBRenc/include
        lib/fdk-aac/libSYS/include

        lib/fdk-aac/libArithCoding/include
        lib/fdk-aac/libDRCdec/include
        lib/fdk-aac/libSACdec/include
        lib/fdk-aac/libSACenc/include
        ./
        j4a
        common/inc
        )

set(my_lib_path ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/${ANDROID_ABI}/lib)
add_library( avcodec STATIC IMPORTED )
add_library( avformat STATIC IMPORTED )
add_library( avutil STATIC IMPORTED )
add_library( swresample STATIC IMPORTED )
add_library( swscale STATIC IMPORTED )
set_target_properties( avcodec PROPERTIES IMPORTED_LOCATION ${my_lib_path}/libavcodec.a )
set_target_properties( avformat PROPERTIES IMPORTED_LOCATION ${my_lib_path}/libavformat.a )
set_target_properties( avutil PROPERTIES IMPORTED_LOCATION ${my_lib_path}/libavutil.a )
set_target_properties( swresample PROPERTIES IMPORTED_LOCATION ${my_lib_path}/libswresample.a )
set_target_properties( swscale PROPERTIES IMPORTED_LOCATION ${my_lib_path}/libswscale.a )


set(openssl_lib_path ${CMAKE_CURRENT_SOURCE_DIR}/openssl/${ANDROID_ABI}/lib)
#add_library( crypto SHARED IMPORTED )
#add_library( ssl SHARED IMPORTED )
#set_target_properties( crypto PROPERTIES IMPORTED_LOCATION ${openssl_lib_path}/libcrypto.so )
#set_target_properties( ssl PROPERTIES IMPORTED_LOCATION ${openssl_lib_path}/libssl.so )
add_library( raop_server
        SHARED
         H264Decoder.cpp OpenSLRender.cpp j4a/j4a_base.c j4a/Build.c j4a/j4a_allclasses.c common/src/android_jni.c RaopServer_JNI.c jni_raop_server.cpp PcmPlayer.cpp)
add_subdirectory(mDNSResponder)
find_library( log-lib
              log )

target_link_libraries( raop_server
        play-lib
        jdns_sd
        ed25519
        fdk-aac
        mediandk
        android
        avcodec
        avformat
        avutil
        swresample
        swscale
        OpenSLES
#        crypto
#        ssl
#        oboe
        z
        ${log-lib} )
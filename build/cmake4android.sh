#!/bin/bash

_dir_="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

set -ex

export WORK_HOME="${_dir_}"
export ANDROID_NDK="$(dirname "$(which ndk-build)")"
export ANDTOOLCHAIN="${WORK_HOME}/cmake/android.toolchain.cmake"

echo -e "\033[31m clean cache begin \033[0m"
rm -rf ./CMakeFiles ./Makefile ./cmake_install.cmake ./CMakeCache.txt
echo -e "\033[31m clean cache end \033[0m"
echo -e "\033[32m start to build avatarlib for armeabi-v7a \033[0m"
cmake -DCMAKE_TOOLCHAIN_FILE=${ANDTOOLCHAIN} 		\
      -DANDROID_NDK=${ANDROID_NDK}           		\
      -DANDROID_NATIVE_API_LEVEL=android-18  		\
      -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.8\
      -DCMAKE_BUILD_TYPE=Release             		\
      -DANDROID_ABI="armeabi-v7a with NEON"  		\
      ${WORK_HOME}/..
make -j10

echo -e "\033[31m clean cache begin \033[0m"
rm -rf ./CMakeFiles ./Makefile ./cmake_install.cmake ./CMakeCache.txt
echo -e "\033[31m clean cache end \033[0m"
echo -e "\033[32m start to build avatarlib for x86 \033[0m"
cmake -DCMAKE_TOOLCHAIN_FILE=${ANDTOOLCHAIN} 		\
      -DANDROID_NDK=${ANDROID_NDK}           		\
      -DANDROID_NATIVE_API_LEVEL=android-18  		\
      -DANDROID_TOOLCHAIN_NAME=x86-4.8			\
      -DCMAKE_BUILD_TYPE=Release             		\
      -DANDROID_ABI="x86"  				\
      ${WORK_HOME}/..
make -j10


export WORK_HOME=$(pwd)/$(dirname ${0})

cmake -DIOS=1 -DCMAKE_TOOLCHAIN_FILE=${WORK_HOME}/cmake/ios.toolchain.cmake -DIOS_PLATFORM=OS -GXcode ..
#xcodebuild -target "avatar_static" -sdk iphoneos -configuration clean 

EMSCRIPTEN_ROOT=/usr/bin/emscripten-master/
export PATH=$PATH:$EMSCRIPTEN_ROOT
emconfigure cmake -DJS=1 ..
emmake make -j9
cd ../../libs/
mv libavatar_static.a libavatar.bc
emcc -O3 -s ALLOW_MEMORY_GROWTH=1 --bind -I../include/ ../src/jsbinds/webavatar.cpp libavatar.bc \
-o avatar.js -s NO_EXIT_RUNTIME=1 -s INLINING_LIMIT=1 --llvm-lto 1 --closure 0

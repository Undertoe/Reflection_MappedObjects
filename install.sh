PROJECT="libReflectionLib"
DEBUG="d"
PROJECTD=$PROJECT$DEBUG

mkdir build/
cd build/

rm /Development/libs/lib64/$PROJECT.a
rm /Development/libs/lib64/"$PROJECTD".a

make clean
echo "building $PROJECT Release"

/opt/cmake-3.13.3-Linux-x86_64/bin/cmake .. -D CMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:Path=/Development/libs && make all install

make clean
echo "Building $PROJECTD Debug"
/opt/cmake-3.13.3-Linux-x86_64/bin/cmake .. -D CMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:Path=/Development/libs && make all install

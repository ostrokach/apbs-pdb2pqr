#!/bin/bash

# Required for newer versions of ncurses
ln -s libtinfo.so $PREFIX/lib/libtermcap.so
ln -s libtinfo.so $PREFIX/lib/libtermcap.so.2
ln -s libtinfo.a $PREFIX/lib/libtermcap.a

mkdir build && cd build

cmake \
    -DCMAKE_INSTALL_PREFIX:PATH=$PREFIX \
    -DCMAKE_PREFIX_PATH:PATH="$PREFIX" \
    -DCMAKE_INCLUDE_PATH:PATH="$PREFIX/include" \
    -DCMAKE_LIBRARY_PATH:PATH="$PREFIX/lib" \
    -DENABLE_iAPBS=ON \
    -DENABLE_GEOFLOW=ON \
    -DENABLE_PBAM=ON \
    -DENABLE_PBSAM=ON \
    -DENABLE_BEM=ON \
    -DENABLE_FETK=ON \
    -DBUILD_SHARED_LIBS=ON \
    -DENABLE_PYTHON=ON \
    -DENABLE_OPENMP=ON \
    -DENABLE_MPI=OFF \
    -DGET_MSMS=OFF \
    -DGET_NanoShaper=OFF \
    ..
make
make install

# -DPYTHON_EXECUTABLE:FILEPATH="$PYTHON" \
# -DPYTHON_INCLUDE_DIR:PATH="$PREFIX/include/python3.5m" \
# -DPYTHON_LIBRARY:FILEPATH="$PREFIX/lib/libpython3.5m.so" \

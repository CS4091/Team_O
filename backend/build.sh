#! bin/bash

echo "Cleaning build..."

rm -rf ./build
mkdir ./build
cd ./build

echo "Building..."

cmake ..
make

cd ..

echo "Build Finished"
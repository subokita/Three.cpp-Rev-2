clear

pwd
shopt -s extglob

# Remove all the files
rm -rf !(run.sh)

# Run CMake
cmake ..

if [[ $? != 0 ]]; then
	echo "Cmake failed"
	rm -rf !(run.sh)

	exit
fi

# Run make file
make -j 4

if [[ $? != 0 ]]; then
	echo "make failed"
	rm -rf !(run.sh)

	exit
fi

# Copy our assets from the examples directory
cp -r ../examples ./examples

# Copy the library to lib directory
mkdir lib
mv "Three.cpp Rev.2/libthree_cpp.a" "lib/libthree_cpp.a"

# Remove everything except for lib, bin, includes, and examples directories
rm -rf !(run.sh|lib|bin|includes|examples)


./bin/main
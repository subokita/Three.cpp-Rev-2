
# First try to install all brew related stuff, and install everything that could be installed using brew
clear
	brew help

	if [[ $? != 0 ]]; then
		echo "Downloading homebrew"
		ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"
	fi

	brew list cmake
	if [[ $? != 0 ]]; then
		echo "Installing CMake"
		brew install cmake
	fi

	brew list glfw3
	if [[ $? != 0 ]]; then
		echo "Installing glfw3"
		brew install glfw3
	fi

	brew list glm
	if [[ $? != 0 ]]; then
		echo "Installing glm"
		brew install glm
	fi

	brew list assimp
	if [[ $? != 0 ]]; then
		echo "Installing glm"
		brew install assimp
	fi

	brew list freeimage
	if [[ $? != 0 ]]; then
		echo "Installing freeimage"
		brew install freeimage
	fi

# Then try to install three.cpp

clear
	pwd
	shopt -s extglob

	# Remove all the files
	rm -rf !(install.sh)

	# Run CMake
	echo "Running Cmake"
	cmake ..

	if [[ $? != 0 ]]; then
		echo "Cmake failed"
		rm -rf !(install.sh)

		exit
	fi

	echo "Running make"
	# Run make file
	make -j 4

	if [[ $? != 0 ]]; then
		echo "make failed"
		rm -rf !(install.sh)

		exit
	fi

	echo "Copying files"
	# Copy our assets from the examples directory
	cp -r ../examples ./examples

	# Copy the library to lib directory
	mkdir lib
	mv "Three.cpp Rev.2/libthree_cpp.a" "lib/libthree_cpp.a"

	# Remove everything except for lib, bin, includes, and examples directories
	rm -rf !(install.sh|lib|bin|includes|examples)

	echo "Test run"
	./bin/main
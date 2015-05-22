find . -type f -name "CMakeCache.txt" -exec rm -f {} \;
find . -type f -name "cmake_install.cmake" -exec rm -f {} \;
find . -type d -name "CMakeFiles" -exec rm -rf {} \;
find . -type f -name "Makefile" -exec rm -f {} \;
find . -type d -name ".svn" -exec rm -rf {} \;


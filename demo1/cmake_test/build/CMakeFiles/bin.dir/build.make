# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yan/test/cmake_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yan/test/cmake_test/build

# Include any dependencies generated for this target.
include CMakeFiles/bin.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bin.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bin.dir/flags.make

CMakeFiles/bin.dir/src/a.o: CMakeFiles/bin.dir/flags.make
CMakeFiles/bin.dir/src/a.o: ../src/a.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/yan/test/cmake_test/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/bin.dir/src/a.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/bin.dir/src/a.o   -c /home/yan/test/cmake_test/src/a.c

CMakeFiles/bin.dir/src/a.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bin.dir/src/a.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/yan/test/cmake_test/src/a.c > CMakeFiles/bin.dir/src/a.i

CMakeFiles/bin.dir/src/a.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bin.dir/src/a.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/yan/test/cmake_test/src/a.c -o CMakeFiles/bin.dir/src/a.s

CMakeFiles/bin.dir/src/a.o.requires:
.PHONY : CMakeFiles/bin.dir/src/a.o.requires

CMakeFiles/bin.dir/src/a.o.provides: CMakeFiles/bin.dir/src/a.o.requires
	$(MAKE) -f CMakeFiles/bin.dir/build.make CMakeFiles/bin.dir/src/a.o.provides.build
.PHONY : CMakeFiles/bin.dir/src/a.o.provides

CMakeFiles/bin.dir/src/a.o.provides.build: CMakeFiles/bin.dir/src/a.o

CMakeFiles/bin.dir/src/main.o: CMakeFiles/bin.dir/flags.make
CMakeFiles/bin.dir/src/main.o: ../src/main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/yan/test/cmake_test/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/bin.dir/src/main.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/bin.dir/src/main.o   -c /home/yan/test/cmake_test/src/main.c

CMakeFiles/bin.dir/src/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bin.dir/src/main.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/yan/test/cmake_test/src/main.c > CMakeFiles/bin.dir/src/main.i

CMakeFiles/bin.dir/src/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bin.dir/src/main.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/yan/test/cmake_test/src/main.c -o CMakeFiles/bin.dir/src/main.s

CMakeFiles/bin.dir/src/main.o.requires:
.PHONY : CMakeFiles/bin.dir/src/main.o.requires

CMakeFiles/bin.dir/src/main.o.provides: CMakeFiles/bin.dir/src/main.o.requires
	$(MAKE) -f CMakeFiles/bin.dir/build.make CMakeFiles/bin.dir/src/main.o.provides.build
.PHONY : CMakeFiles/bin.dir/src/main.o.provides

CMakeFiles/bin.dir/src/main.o.provides.build: CMakeFiles/bin.dir/src/main.o

# Object files for target bin
bin_OBJECTS = \
"CMakeFiles/bin.dir/src/a.o" \
"CMakeFiles/bin.dir/src/main.o"

# External object files for target bin
bin_EXTERNAL_OBJECTS =

bin: CMakeFiles/bin.dir/src/a.o
bin: CMakeFiles/bin.dir/src/main.o
bin: CMakeFiles/bin.dir/build.make
bin: CMakeFiles/bin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable bin"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bin.dir/build: bin
.PHONY : CMakeFiles/bin.dir/build

CMakeFiles/bin.dir/requires: CMakeFiles/bin.dir/src/a.o.requires
CMakeFiles/bin.dir/requires: CMakeFiles/bin.dir/src/main.o.requires
.PHONY : CMakeFiles/bin.dir/requires

CMakeFiles/bin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bin.dir/clean

CMakeFiles/bin.dir/depend:
	cd /home/yan/test/cmake_test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yan/test/cmake_test /home/yan/test/cmake_test /home/yan/test/cmake_test/build /home/yan/test/cmake_test/build /home/yan/test/cmake_test/build/CMakeFiles/bin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bin.dir/depend


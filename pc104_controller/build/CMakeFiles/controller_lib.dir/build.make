# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/codecraft94/Documents/nus_quadruped/pc104_controller

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/codecraft94/Documents/nus_quadruped/pc104_controller/build

# Include any dependencies generated for this target.
include CMakeFiles/controller_lib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/controller_lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/controller_lib.dir/flags.make

CMakeFiles/controller_lib.dir/src/misc_functions.cpp.o: CMakeFiles/controller_lib.dir/flags.make
CMakeFiles/controller_lib.dir/src/misc_functions.cpp.o: ../src/misc_functions.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/codecraft94/Documents/nus_quadruped/pc104_controller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/controller_lib.dir/src/misc_functions.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/controller_lib.dir/src/misc_functions.cpp.o -c /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/misc_functions.cpp

CMakeFiles/controller_lib.dir/src/misc_functions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/controller_lib.dir/src/misc_functions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/misc_functions.cpp > CMakeFiles/controller_lib.dir/src/misc_functions.cpp.i

CMakeFiles/controller_lib.dir/src/misc_functions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/controller_lib.dir/src/misc_functions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/misc_functions.cpp -o CMakeFiles/controller_lib.dir/src/misc_functions.cpp.s

CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.o: CMakeFiles/controller_lib.dir/flags.make
CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.o: ../src/odrive_can_functions.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/codecraft94/Documents/nus_quadruped/pc104_controller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.o -c /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/odrive_can_functions.cpp

CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/odrive_can_functions.cpp > CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.i

CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/odrive_can_functions.cpp -o CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.s

CMakeFiles/controller_lib.dir/src/task.cpp.o: CMakeFiles/controller_lib.dir/flags.make
CMakeFiles/controller_lib.dir/src/task.cpp.o: ../src/task.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/codecraft94/Documents/nus_quadruped/pc104_controller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/controller_lib.dir/src/task.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/controller_lib.dir/src/task.cpp.o -c /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/task.cpp

CMakeFiles/controller_lib.dir/src/task.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/controller_lib.dir/src/task.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/task.cpp > CMakeFiles/controller_lib.dir/src/task.cpp.i

CMakeFiles/controller_lib.dir/src/task.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/controller_lib.dir/src/task.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/task.cpp -o CMakeFiles/controller_lib.dir/src/task.cpp.s

CMakeFiles/controller_lib.dir/src/request_msg.cpp.o: CMakeFiles/controller_lib.dir/flags.make
CMakeFiles/controller_lib.dir/src/request_msg.cpp.o: ../src/request_msg.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/codecraft94/Documents/nus_quadruped/pc104_controller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/controller_lib.dir/src/request_msg.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/controller_lib.dir/src/request_msg.cpp.o -c /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/request_msg.cpp

CMakeFiles/controller_lib.dir/src/request_msg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/controller_lib.dir/src/request_msg.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/request_msg.cpp > CMakeFiles/controller_lib.dir/src/request_msg.cpp.i

CMakeFiles/controller_lib.dir/src/request_msg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/controller_lib.dir/src/request_msg.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/request_msg.cpp -o CMakeFiles/controller_lib.dir/src/request_msg.cpp.s

CMakeFiles/controller_lib.dir/src/signal_generator.cpp.o: CMakeFiles/controller_lib.dir/flags.make
CMakeFiles/controller_lib.dir/src/signal_generator.cpp.o: ../src/signal_generator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/codecraft94/Documents/nus_quadruped/pc104_controller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/controller_lib.dir/src/signal_generator.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/controller_lib.dir/src/signal_generator.cpp.o -c /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/signal_generator.cpp

CMakeFiles/controller_lib.dir/src/signal_generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/controller_lib.dir/src/signal_generator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/signal_generator.cpp > CMakeFiles/controller_lib.dir/src/signal_generator.cpp.i

CMakeFiles/controller_lib.dir/src/signal_generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/controller_lib.dir/src/signal_generator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/codecraft94/Documents/nus_quadruped/pc104_controller/src/signal_generator.cpp -o CMakeFiles/controller_lib.dir/src/signal_generator.cpp.s

# Object files for target controller_lib
controller_lib_OBJECTS = \
"CMakeFiles/controller_lib.dir/src/misc_functions.cpp.o" \
"CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.o" \
"CMakeFiles/controller_lib.dir/src/task.cpp.o" \
"CMakeFiles/controller_lib.dir/src/request_msg.cpp.o" \
"CMakeFiles/controller_lib.dir/src/signal_generator.cpp.o"

# External object files for target controller_lib
controller_lib_EXTERNAL_OBJECTS =

libcontroller_lib.a: CMakeFiles/controller_lib.dir/src/misc_functions.cpp.o
libcontroller_lib.a: CMakeFiles/controller_lib.dir/src/odrive_can_functions.cpp.o
libcontroller_lib.a: CMakeFiles/controller_lib.dir/src/task.cpp.o
libcontroller_lib.a: CMakeFiles/controller_lib.dir/src/request_msg.cpp.o
libcontroller_lib.a: CMakeFiles/controller_lib.dir/src/signal_generator.cpp.o
libcontroller_lib.a: CMakeFiles/controller_lib.dir/build.make
libcontroller_lib.a: CMakeFiles/controller_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/codecraft94/Documents/nus_quadruped/pc104_controller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library libcontroller_lib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/controller_lib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/controller_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/controller_lib.dir/build: libcontroller_lib.a

.PHONY : CMakeFiles/controller_lib.dir/build

CMakeFiles/controller_lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/controller_lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/controller_lib.dir/clean

CMakeFiles/controller_lib.dir/depend:
	cd /home/codecraft94/Documents/nus_quadruped/pc104_controller/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/codecraft94/Documents/nus_quadruped/pc104_controller /home/codecraft94/Documents/nus_quadruped/pc104_controller /home/codecraft94/Documents/nus_quadruped/pc104_controller/build /home/codecraft94/Documents/nus_quadruped/pc104_controller/build /home/codecraft94/Documents/nus_quadruped/pc104_controller/build/CMakeFiles/controller_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/controller_lib.dir/depend


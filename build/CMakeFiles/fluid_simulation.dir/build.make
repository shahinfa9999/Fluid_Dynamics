# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/build

# Include any dependencies generated for this target.
include CMakeFiles/fluid_simulation.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/fluid_simulation.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/fluid_simulation.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fluid_simulation.dir/flags.make

CMakeFiles/fluid_simulation.dir/codegen:
.PHONY : CMakeFiles/fluid_simulation.dir/codegen

CMakeFiles/fluid_simulation.dir/src/main.mm.o: CMakeFiles/fluid_simulation.dir/flags.make
CMakeFiles/fluid_simulation.dir/src/main.mm.o: /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/main.mm
CMakeFiles/fluid_simulation.dir/src/main.mm.o: CMakeFiles/fluid_simulation.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fluid_simulation.dir/src/main.mm.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fluid_simulation.dir/src/main.mm.o -MF CMakeFiles/fluid_simulation.dir/src/main.mm.o.d -o CMakeFiles/fluid_simulation.dir/src/main.mm.o -c /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/main.mm

CMakeFiles/fluid_simulation.dir/src/main.mm.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/fluid_simulation.dir/src/main.mm.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/main.mm > CMakeFiles/fluid_simulation.dir/src/main.mm.i

CMakeFiles/fluid_simulation.dir/src/main.mm.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/fluid_simulation.dir/src/main.mm.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/main.mm -o CMakeFiles/fluid_simulation.dir/src/main.mm.s

CMakeFiles/fluid_simulation.dir/src/simulation.cpp.o: CMakeFiles/fluid_simulation.dir/flags.make
CMakeFiles/fluid_simulation.dir/src/simulation.cpp.o: /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/simulation.cpp
CMakeFiles/fluid_simulation.dir/src/simulation.cpp.o: CMakeFiles/fluid_simulation.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/fluid_simulation.dir/src/simulation.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fluid_simulation.dir/src/simulation.cpp.o -MF CMakeFiles/fluid_simulation.dir/src/simulation.cpp.o.d -o CMakeFiles/fluid_simulation.dir/src/simulation.cpp.o -c /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/simulation.cpp

CMakeFiles/fluid_simulation.dir/src/simulation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/fluid_simulation.dir/src/simulation.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/simulation.cpp > CMakeFiles/fluid_simulation.dir/src/simulation.cpp.i

CMakeFiles/fluid_simulation.dir/src/simulation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/fluid_simulation.dir/src/simulation.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/simulation.cpp -o CMakeFiles/fluid_simulation.dir/src/simulation.cpp.s

CMakeFiles/fluid_simulation.dir/src/rendering.cpp.o: CMakeFiles/fluid_simulation.dir/flags.make
CMakeFiles/fluid_simulation.dir/src/rendering.cpp.o: /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/rendering.cpp
CMakeFiles/fluid_simulation.dir/src/rendering.cpp.o: CMakeFiles/fluid_simulation.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/fluid_simulation.dir/src/rendering.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fluid_simulation.dir/src/rendering.cpp.o -MF CMakeFiles/fluid_simulation.dir/src/rendering.cpp.o.d -o CMakeFiles/fluid_simulation.dir/src/rendering.cpp.o -c /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/rendering.cpp

CMakeFiles/fluid_simulation.dir/src/rendering.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/fluid_simulation.dir/src/rendering.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/rendering.cpp > CMakeFiles/fluid_simulation.dir/src/rendering.cpp.i

CMakeFiles/fluid_simulation.dir/src/rendering.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/fluid_simulation.dir/src/rendering.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/src/rendering.cpp -o CMakeFiles/fluid_simulation.dir/src/rendering.cpp.s

# Object files for target fluid_simulation
fluid_simulation_OBJECTS = \
"CMakeFiles/fluid_simulation.dir/src/main.mm.o" \
"CMakeFiles/fluid_simulation.dir/src/simulation.cpp.o" \
"CMakeFiles/fluid_simulation.dir/src/rendering.cpp.o"

# External object files for target fluid_simulation
fluid_simulation_EXTERNAL_OBJECTS =

fluid_simulation: CMakeFiles/fluid_simulation.dir/src/main.mm.o
fluid_simulation: CMakeFiles/fluid_simulation.dir/src/simulation.cpp.o
fluid_simulation: CMakeFiles/fluid_simulation.dir/src/rendering.cpp.o
fluid_simulation: CMakeFiles/fluid_simulation.dir/build.make
fluid_simulation: CMakeFiles/fluid_simulation.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable fluid_simulation"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fluid_simulation.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fluid_simulation.dir/build: fluid_simulation
.PHONY : CMakeFiles/fluid_simulation.dir/build

CMakeFiles/fluid_simulation.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fluid_simulation.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fluid_simulation.dir/clean

CMakeFiles/fluid_simulation.dir/depend:
	cd /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/build /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/build /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/build/CMakeFiles/fluid_simulation.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/fluid_simulation.dir/depend

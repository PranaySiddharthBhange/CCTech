# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Igeometry/include
LDFLAGS = -Lgeometry/lib -lmain
LIBRARY_DIR = geometry/lib
SRC_DIR = geometry/src
INCLUDE_DIR = geometry/include
APP_DIR = application
DATA_DIR = geometry/data

# Automatically find .cpp files only in the specified SRC_DIR
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(LIBRARY_DIR)/%.o, $(SRC_FILES))
HEADER_FILES = $(wildcard $(INCLUDE_DIR)/*.hpp)

# Default target builds the main executable
all: $(APP_DIR)/main.exe

# Rule to build the shared library (DLL)
$(LIBRARY_DIR)/libmain.dll: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^

# Rule to build the main program executable
$(APP_DIR)/main.exe: $(APP_DIR)/main.cpp $(LIBRARY_DIR)/libmain.dll
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile source files into object files
$(LIBRARY_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_FILES)
	@if not exist "$(LIBRARY_DIR)" mkdir "$(LIBRARY_DIR)"
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(APP_DIR)/main.exe
	@powershell -Command "$$env:PATH='$(LIBRARY_DIR);' + $$env:PATH; .\\$(APP_DIR)\\main.exe"

clean:
	powershell -Command "Remove-Item -Path '$(LIBRARY_DIR)\libmain.dll','$(APP_DIR)\main.exe'$(foreach obj, $(OBJ_FILES), ,'$(obj)') -ErrorAction SilentlyContinue"



# Компилятор. Может быть как и mingw32-g++, так и g++
CXX = g++

# Defin'ы для указания новой версии Windows
CXXFLAGS += -D__USE_MINGW_ANSI_STDIO=0
CXXFLAGS += -D_WIN32_IE=0x501
CXXFLAGS += -DWINVER=0x0501
CXXFLAGS += -D_WIN32_WINNT=0x501
CXXFLAGS += -DUNICODE

# Каталоги .h файлов
CXXFLAGS += -I include
#CXXFLAGS += -I include/AGG
#CXXFLAGS += -I include/EasyBMP

# Общие флаги
CXXFLAGS += -O3
CXXFLAGS += -std=gnu++11
CXXFLAGS += -w
CXXFLAGS += -fpermissive
# CXXFLAGS += -static-libgcc -static-libstdc++
CXXFLAGS += -mconsole
CXXFLAGS += -mwindows

#-mwindows

# Библиотеки для линковки
CXXFLAGS += -lgdi32 -lwinmm -lmsimg32 -lcomctl32 -lcomdlg32 -lole32
CXXFLAGS += -lstdc++
# Эти надо компилировать самому
CXXFLAGS += -lagg

# Оптимизация размера
CXXFLAGS += -s -Wl,--gc-sections -Wl,--strip-all 
CXXFLAGS += -fdata-sections -ffunction-sections
CXXFLAGS += -ffast-math
CXXFLAGS += -Wunused
CXXFLAGS += -flto
#CXXFLAGS += -g

###############################################################################

OBJECTS = $(patsubst src/%, obj/%, $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)))
EXAMPLES = $(patsubst examples/%.cpp, obj/%.o,$(wildcard examples/*.cpp))
EXES = $(patsubst examples/%.cpp, bin/%.exe,$(wildcard examples/*.cpp))

###############################################################################

all: dirs $(OBJECTS) $(EXAMPLES) $(EXES)

dirs:
	if not exist "bin" mkdir bin
	if not exist "obj" mkdir obj

bin/%.exe: $(OBJECTS) obj/%.o
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(patsubst bin/%.exe, obj/%.o, $@) -o $@

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: examples/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	if exist "bin" rd bin /S /Q
	if exist "obj" rd obj /S /Q
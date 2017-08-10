# Компилятор. Может быть как и mingw32-g++, так и g++
CXX = g++

# Defin'ы для указания новой версии Windows
CXXFLAGS += -D__USE_MINGW_ANSI_STDIO=0
CXXFLAGS += -D_WIN32_IE=0x501
CXXFLAGS += -DWINVER=0x0501
CXXFLAGS += -D_WIN32_WINNT=0x501
CXXFLAGS += -DUNICODE

# Библиотеки для линковки, они у меня почему-то не работаеют, поэтому линкую напрямую .a файлы
# CXXFLAGS += -lgdi32 -lwinmm -lmsimg32 -lcomctl32 -lcomdlg32 -lole32

CXXFLAGS += -lstdc++

# Эти надо компилировать самому
CXXFLAGS += -lagg

# Каталоги .h файлов
CXXFLAGS += -I include

# Это если вы не хотите помещать эти библиотеки в mingw/include, поместите их в локальный include
#CXXFLAGS += -I include/AGG
#CXXFLAGS += -I include/EasyBMP

# Общие флаги
CXXFLAGS += -O3
CXXFLAGS += -std=gnu++11
CXXFLAGS += -w
CXXFLAGS += -fpermissive

# Сильно увеличивает размер, но делает проги независимыми на чужом компе
# CXXFLAGS += -static-libgcc -static-libstdc++

# Если эту опцию поместить перед mwindows, то получится консоль с окном
# CXXFLAGS += -mconsole 

CXXFLAGS += -mwindows

# Оптимизация размера
CXXFLAGS += -s -Wl,--gc-sections -Wl,--strip-all 
CXXFLAGS += -fdata-sections -ffunction-sections
CXXFLAGS += -ffast-math
CXXFLAGS += -Wunused
CXXFLAGS += -flto
#CXXFLAGS += -g

###############################################################################

OBJECTS = $(patsubst src/%, obj/%, $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)))
# OBJECTS += $(patsubst examples/%.rc, obj/%.o,$(wildcard examples/*.rc))
INCLUDES = $(wildcard include/*.h)
INCLUDES += $(wildcard include/ctrl/*.h)
INCLUDES += $(wildcard include/image/*.h)
INCLUDES += $(wildcard include/window/*.h)
EXAMPLES = $(patsubst examples/%.cpp, obj/%.o,$(wildcard examples/*.cpp))
EXES = $(patsubst examples/%.cpp, bin/%.exe,$(wildcard examples/*.cpp))
ALIBS = $(patsubst -l%, D:/ProgFiles/MinGW/lib/lib%.a, -lgdi32 -lwinmm -lmsimg32 -lcomctl32 -lcomdlg32 -lole32)

###############################################################################

all: dirs $(OBJECTS) $(EXAMPLES) $(EXES)

dirs:
	if not exist "bin" mkdir bin
	if not exist "obj" mkdir obj

bin/%.exe: $(OBJECTS) obj/%.o
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(patsubst bin/%.exe, obj/%.o, $@) -o $@ $(ALIBS)

obj/%.o: src/%.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: examples/%.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# obj/%.o: examples/%.rc
#	windres $< $@

clean:
	if exist "bin" rd bin /S /Q
	if exist "obj" rd obj /S /Q
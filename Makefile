ifeq ($(OS),Windows_NT)
	CXX=cl
	FLAG_LINK=/Fo
	OBJ=windows
	EXT=.obj
	SERVER=server_windows.cpp
	CLIENT=client_windows.cpp
	PLATFORM=WIN
else
	CXX = g++ -Wall
	FLAG_LINK = -o $@
	OBJ = linux
	EXT = .o
	SERVER = server.cpp
	CLIENT = client.cpp
	PLATFORM = LIN
endif
SOURCEDIR = src
BUILDDIR = build

.PHONY: all, clean, move, mkdir

all: mkdir server client move

mkdir:
ifeq ($(PLATFORM),WIN)
	if not exist $(BUILDDIR) md $(BUILDDIR)
else
	mkdir -p $(BUILDDIR)
endif

move:
ifeq ($(PLATFORM),WIN)
	copy server_windows.exe $(BUILDDIR)
	copy server_windows$(EXT) $(BUILDDIR)
	copy client_windows.exe $(BUILDDIR)
	copy client_windows$(EXT) $(BUILDDIR)
	copy $(OBJ)$(EXT) $(BUILDDIR)
	del /F server_windows.exe
	del /F server_windows$(EXT)
	del /F client_windows.exe
	del /F client_windows$(EXT)
	del /F $(OBJ)$(EXT)
else
	mv server client $(OBJ)$(EXT) -t $(BUILDDIR)
endif

client:$(SOURCEDIR)/$(CLIENT)
	$(CXX) $(SOURCEDIR)/$(CLIENT) $(FLAG_LINK)

server:  $(SOURCEDIR)/$(SERVER) $(OBJ)$(EXT)
	$(CXX) $(SOURCEDIR)/$(SERVER) $(FLAG_LINK) $(OBJ)$(EXT)

$(OBJ)$(EXT): $(SOURCEDIR)/$(OBJ).cpp
	$(CXX) -c $(SOURCEDIR)/$(OBJ).cpp

clean:
ifeq ($(PLATFORM),WIN)
	rmdir /S /Q $(BUILDDIR)
else
	rm -rf $(BUILDDIR)
endif

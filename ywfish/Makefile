
CC=gcc
CXX=g++

FISH_DIR:=./
PROTO_SRC:=./
MAYDAY_DIR:=./mayday/
MAYDAY_BASE_DIR:=./mayday/base/
MAYDAY_NET_DIR:=./mayday/net/

mkdir -p $(MAYDAY_BASE_DIR)
mkdir -p $(MAYDAY_NET_DIR)

FISHSERVER_OBJDIR:=../Obj/SRC
MAYDAY_OBJDIR:=../Obj/mayday
MAYDAY_BASE_OBJDIR:=../Obj/mayday/base
MAYDAY_NET_OBJDIR:=../Obj/mayday/net
COMMON_OBJDIR:=../Obj/SRC

#匹配SceneServer目录下的cpp文件
FISHSERVER_SRC:=$(wildcard $(FISH_DIR)*.cpp )
MAYDAY_SRC:=$(wildcard $(MAYDAY_DIR)*.cpp )
MAYDAY_BASE_SRC:=$(wildcard $(MAYDAY_BASE_DIR)*.cpp )
MAYDAY_NET_SRC:=$(wildcard $(MAYDAY_NET_DIR)*.cpp )
COMMON_SRC:=$(wildcard $(PROTO_SRC)*.cc)

FISHSERVER_OBJ:=$(patsubst %.cpp,$(FISHSERVER_OBJDIR)/%.o,$(notdir $(FISHSERVER_SRC)))
MAYDAY_OBJ:=$(patsubst %.cpp,$(MAYDAY_OBJDIR)/%.o,$(notdir $(MAYDAY_SRC)))
MAYDAY_BASE_OBJ:=$(patsubst %.cpp,$(MAYDAY_BASE_OBJDIR)/%.o,$(notdir $(MAYDAY_BASE_SRC)))
MAYDAY_NET_OBJ:=$(patsubst %.cpp,$(MAYDAY_NET_OBJDIR)/%.o,$(notdir $(MAYDAY_NET_SRC)))
COMMON_OBJ:=$(patsubst %.cc,$(COMMON_OBJDIR)/%.o,$(notdir $(COMMON_SRC)))
OBJ:= ${FISHSERVER_OBJ} ${MAYDAY_OBJ} ${MAYDAY_BASE_OBJ} ${MAYDAY_NET_OBJ} ${COMMON_OBJ}

CXXFLAGS = -g -O0 -std=c++0x -DPROTO_LOG_ -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free -Werror -Wsign-compare -Wuninitialized -Winline -Wparentheses -I./ -I../BaseLib/include  -m64\

LDFLAGS =  -L./ -L../BaseLib -ldl -lprotobuf_64 -lpthread -ltcmalloc_minimal_64\


all: fishServer

clean:
	rm -f ./fishServer
	rm -rf ${FISHSERVER_OBJDIR}/*.o 
	rm -rf ${MAYDAY_OBJDIR}/*.o
	rm -rf ${MAYDAY_BASE_OBJDIR}/*.o
	rm -rf ${MAYDAY_NET_OBJDIR}/*.o

fishServer: ${OBJ}
	${CXX} $(CXXFLAGS) -o ./$@ $^ $(LDFLAGS)

.PHONY: all clean

$(FISHSERVER_OBJDIR)/%.o:$(FISH_DIR)%.cpp
	${CXX} -c $< -o $@ $(CXXFLAGS)
$(MAYDAY_OBJDIR)/%.o:$(MAYDAY_DIR)%.cpp
	${CXX} -c $< -o $@ $(CXXFLAGS)

$(MAYDAY_BASE_OBJDIR)/%.o:$(MAYDAY_BASE_DIR)%.cpp
	${CXX} -c $< -o $@ $(CXXFLAGS)

$(MAYDAY_NET_OBJDIR)/%.o:$(MAYDAY_NET_DIR)%.cpp
	${CXX} -c $< -o $@ $(CXXFLAGS)

$(COMMON_OBJDIR)/%.o:$(PROTO_SRC)%.cc
	${CXX} -c $< -o $@ $(CXXFLAGS)
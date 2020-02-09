CC = gcc  
C++ = g++  
LINK = g++  

LIBS = -L/usr/local/lib -L/usr/local/lib64 -lpthread -lrt -lz -lm -ldl -lavfilter -lavformat -lavcodec -lavutil -lpostproc -lswscale -lswresample -lx264 -lx265 -lfdk-aac -lsrt -lenc

#must add -fPIC option  
CCFLAGS = $(COMPILER_FLAGS) -c -g  -fPIC -Wno-deprecated-declarations -Wno-unused-result -Wall
C++FLAGS = $(COMPILER_FLAGS) -c -g -fPIC -Wno-deprecated-declarations -Wno-unused-result -Wall -std=c++11
  
TARGET=srt_encoder
  
INCLUDES = -I. -I./include -I../ -I/usr/local/include

C++FILES = ./srt_encoder.cpp

CFILES = 

OBJFILE = $(CFILES:.c=.o) $(C++FILES:.cpp=.o)  

all:$(TARGET)  
  
$(TARGET): $(OBJFILE)  
	$(LINK) $^ -Wall $(LIBS) -o $@
 
%.o:%.c  
	$(CC) -o $@ $(CCFLAGS) $< $(INCLUDES)  
  
%.o:%.cpp  
	$(C++) -o $@ $(C++FLAGS) $< $(INCLUDES)  

clean:  
	rm -rf $(TARGET)  
	rm -rf $(OBJFILE)  

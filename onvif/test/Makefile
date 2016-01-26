TARGET = calc
OBJ += main.o
OBJ += soapC.o
OBJ += soapcalcProxy.o

GSOAP_DIR = ../gsoap
LDFLAGS += $(GSOAP_DIR)/stdsoap2.cpp
CROSS_COMPILE =
CC = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)g++

all: $(TARGET)
$(TARGET): $(OBJ)
#	$(LD) $^ -lgsoap++ -o $@
	$(LD) $^ $(LDFLAGS) -o $@
clean:
	rm $(OBJ) -rf	
	rm $(TARGET) -rf

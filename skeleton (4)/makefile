SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))
CXXFLAGS := -g -std=c++11

.PHONY: all clean
all: PA4
clean:
	$(RM) $(OBJECTS) $(DEPENDS) PA4
PA4: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@
-include $(DEPENDS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
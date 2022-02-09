CXX = g++
CXXFLAGS := -g
BUILD_DIR = ./build
EXEC = snake

.PHONY: rebuild
rebuild: clean $(EXEC)

$(EXEC): $(BUILD_DIR)/snake.o $(BUILD_DIR)/main.o $(BUILD_DIR)/MainSDLWindow.o $(BUILD_DIR)/score.o $(BUILD_DIR)/playground.o \
         $(BUILD_DIR)/scorerenderer.o $(BUILD_DIR)/playgroundrenderer.o
	@echo "Linking $@..."
	$(CXX) -o $@ $^ -lSDL2 -lSDL2_ttf

#$(BUILD_DIR)/main.o: dorian/main.cpp
#$(BUILD_DIR)/Snake.o: dorian/Snake.cpp
#$(BUILD_DIR)/MainSDLWindow.o: dorian/MainSDLWindow.cpp

$(BUILD_DIR)/main.o: dorian/main.cpp dorian/MainSDLWindow.hpp dorian/snake.hpp dorian/score.hpp dorian/playground.hpp dorian/playgroundrenderer.hpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/scorerenderer.o: dorian/scorerenderer.cpp dorian/scorerenderer.hpp dorian/score.hpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/playground.o: dorian/playground.cpp dorian/playground.hpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/playgroundrenderer.o: dorian/playgroundrenderer.cpp dorian/playgroundrenderer.hpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/score.o: dorian/score.cpp dorian/score.hpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/snake.o: dorian/snake.cpp dorian/snake.hpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/MainSDLWindow.o: dorian/MainSDLWindow.cpp dorian/MainSDLWindow.hpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

#CPPFLAGS := $(INC_FLAGS) -MMD -MP

#$(BUILD_DIR)/%.o: dorian/%.cpp
#	mkdir -p $(dir $@)
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
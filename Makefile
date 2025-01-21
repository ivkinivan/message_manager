LIB_NAME = libmessage_manager_lib.so 
APP_NAME = demo_app
TEST_NAME = test_runner

CC = g++
CFLAGS = -fPIC -Wall -O2 -std=c++17 
LDFLAGS = -shared

LIB_SRCS = message_manager_lib.cpp 
APP_SRCS = demo_app.cpp
TEST_SRCS = tests.cpp

LIB_OBJS = $(LIB_SRCS:.cpp=.o)
APP_OBJS = $(APP_SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

.PHONY: all clean run_tests run

all: $(LIB_NAME) $(APP_NAME) 
	
$(LIB_NAME): $(LIB_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(LIB_OBJS) -lstdc++ #сборка библиотеки

$(APP_NAME): $(APP_OBJS) $(LIB_NAME)
	$(CC) -pthread -o $@ $(APP_OBJS) -L. -lmessage_manager_lib -lstdc++ -Wl,-rpath=. #сборка приложения
	
run_tests: $(TEST_NAME)
	./$(TEST_NAME)
	
$(TEST_NAME): $(TEST_OBJS) $(LIB_NAME)
	$(CC) -pthread -o $@ $(TEST_OBJS) -L. -lmessage_manager_lib -lstdc++ -Wl,-rpath=. #сборка тестов
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	
	
clean:
	rm -f *.o $(LIB_NAME) $(APP_NAME) $(TEST_NAME)


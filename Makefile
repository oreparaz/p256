SRCS := p256.c \
		tests/test_openssl.cc \
		tests/unittest_main.cc

BUILD_DIR := build
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
OS := $(shell uname)

CPPFLAGS += -MMD -MP -Os -g

ifeq ($(OS),Linux)
	ifeq ($(P256_LINUXARCH),32)
		CPPFLAGS += -m32
		LDFLAGS += -m32
	endif
endif

# ideally we add -Werror here. too many warnings as of now.
CPPFLAGS += -Wall -Wextra

ifeq ($(OS),Darwin)
	CPPFLAGS += -Weverything -Wno-poison-system-directories -Wno-format-pedantic
endif

CPPFLAGS += -Wno-c++98-compat
CFLAGS = --std=c99
CXXFLAGS = --std=c++17

$(BUILD_DIR)/p256_unittests: $(OBJS) Makefile
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c Makefile
	mkdir -p $(dir $@) && $(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cc.o: %.cc Makefile
	mkdir -p $(dir $@) && $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/p256_unittests.timestamp: $(BUILD_DIR)/p256_unittests
	$(BUILD_DIR)/p256_unittests && touch $(BUILD_DIR)/p256_unittests.timestamp

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

.DEFAULT_GOAL := $(BUILD_DIR)/p256_unittests.timestamp

-include $(DEPS)
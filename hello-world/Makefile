include enclave/header.mk

# ------------------- SGX ----------------------
#
INCLUDES = -I$(SGX_SDK)/include

LIBS := -L$(SGX_LIBRARY_PATH) -l$(Urts_Library_Name) -lpthread -Wl,-rpath=$(CURDIR) -pthread

ifneq ($(SGX_MODE), HW)
	LIBS += -lsgx_uae_service_sim
else
	LIBS += -lsgx_uae_service
endif

CFLAGS := $(SGX_COMMON_CFLAGS) -fPIC -Wno-attributes $(INCLUDES)
CPPFLAGS := $(CFLAGS) -std=c++11
CFLAGS := -std=gnu11 $(CFLAGS)

# ------------------- SGX ----------------------


main_objs := \
	hello_main.o


objs := enclave_u.o $(main_objs)


build = \
        @if [ -z "$V" ]; then \
                echo '    [$1]  $@'; \
                $2; \
        else \
                echo '$2'; \
                $2; \
        fi


prog := hello-world

all: $(prog)
	make -C enclave


$(prog): $(objs)
	$(call build,CC,$(CC) $(CFLAGS) $^ -o $@ $(LIBS))

enclave_u.c: enclave/enclave.edl
	$(call build,SGX,$(SGX_EDGER8R) --untrusted --untrusted-dir ./ $^)

enclave_u.o: enclave_u.c
	$(call build,CC,$(CC) $(CFLAGS) -c $< -o $@)

%.o: %.c
	$(call build,CC,$(CC) $(CFLAGS) -c $< -o $@)

.PHONY: clean
clean:
	rm -rf $(prog) $(objs) enclave_u.*
	make clean -C enclave

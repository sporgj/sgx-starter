include sgx_untrusted.mk

main_objs := \
	ocalls.o \
	app_main.o

libnexus_objs := \
	nexus_encode.o \
	nexus_util.o \
	nexus_raw_file.o


objs := enclave_u.o $(main_objs) $(foreach o,$(libnexus_objs),libnexus/$(o))


build = \
        @if [ -z "$V" ]; then \
                echo '    [$1]  $@'; \
                $2; \
        else \
                echo '$2'; \
                $2; \
        fi


prog := sealer-app

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

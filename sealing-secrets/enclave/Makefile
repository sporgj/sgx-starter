include sgx_trusted.mk

APP_PATH = ..

main_objs := \
	ecalls_main.o \
	libnexus_trusted/nexus_log.o \
	libnexus_trusted/nexus_util.o


objs := enclave_t.o $(main_objs)


build = \
        @if [ -z "$V" ]; then \
                echo '    [$1]  $@'; \
                $2; \
        else \
                echo '$2'; \
                $2; \
        fi



.PHONY: all
all: enclave.signed.so

Enclave_C_Flags += -I../

%.o: %.c
	$(call build,CC,$(CC) $(Enclave_C_Flags) -c $< -o $@)

private_key.pem:
	openssl genrsa -out $@ -3 3072

enclave_t.c: enclave.edl
	@$(SGX_EDGER8R) --trusted --trusted-dir . $^
	@echo "GEN +> $@"

enclave_t.o: enclave_t.c
	$(call build,CC,$(CC) $(Enclave_C_Flags) -c $< -o $@)

enclave.so: enclave_t.o $(objs)
	$(call build,CC,$(CC) $^ -o $@ $(Enclave_Link_Flags))

enclave.signed.so: enclave.so private_key.pem
	@$(SGX_ENCLAVE_SIGNER) sign -key private_key.pem -enclave $< -out $@ -config config.xml
	@echo "SIGN => $@"

.PHONY: clean
clean:
	rm -rf *.so *.o enclave_t.*

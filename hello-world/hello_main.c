#include <stdio.h>

#include <sgx_urts.h>

#include "enclave_u.h"  // generated by edger8

#include "nexus_log.h"


#define ENCLAVE_PATH "./enclave/enclave.signed.so"

static sgx_enclave_id_t global_enclave_id;

static char buffer[32];

static int
create_enclave()
{
    sgx_launch_token_t launch_token        = { 0 };
    int                launch_token_update = 0;

    int ret = sgx_create_enclave(ENCLAVE_PATH,
                                 SGX_DEBUG_FLAG,
                                 &launch_token,
                                 &launch_token_update,
                                 &global_enclave_id,
                                 NULL);

    if (ret != SGX_SUCCESS) {
        log_error("Error, call sgx_create_enclave fail. ret=%x\n", ret);
        return -1;
    }

    return 0;
}

static void
destroy_enclave()
{
    sgx_destroy_enclave(global_enclave_id);
}

int
main(int argc, char ** argv)
{
    log_info("Enclave hello world\n");

    if (create_enclave()) {
        log_error("could not create enclave\n");
        return -1;
    }

    {
        int err = -1; // SGX-level error
        int ret = -1; // enclave program error

        log_info("Calling enclave hello_world\n");

        err = ecall_hello_world(global_enclave_id, &ret, buffer);

        if (err || ret) {
            log_error("ecall_hello_world() FAILED. err=%x, ret=%d\n", err, ret);
            goto err;
        }
    }

    printf("buffer: %s\n", buffer);

    destroy_enclave();

    return 0;
err:
    destroy_enclave();

    return -1;
}

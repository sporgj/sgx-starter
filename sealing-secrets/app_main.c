#include "untrusted_header.h"


#define ENCLAVE_PATH "./enclave/enclave.signed.so"

static sgx_enclave_id_t global_enclave_id;

static char buffer[32];

static void
usage();



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
handle_unseal_secret(int argc, char ** argv)
{
    int ret = -1;

    if (argc < 1) {
        log_error("not enough arguments\n");
        usage();
        return -1;
    }

    {
        char    * input_filepath      = strndup(argv[1], 1024);

        uint8_t * sealed_input        = NULL;
        char    * sealed_input_str    = NULL;
        size_t    sealed_input_len    = 0;

        char    * unsealed_output     = NULL;
        size_t    unsealed_output_len = 0;

        int err = -1;


        if (nexus_read_raw_file(input_filepath, &sealed_input, &sealed_input_len)) {
            nexus_free(input_filepath);

            log_error("nexus_read_raw_file() FAILED\n");
            return -1;
        }

        err = ecall_unseal_data(global_enclave_id,
                                &unsealed_output,
                                sealed_input,
                                sealed_input_len,
                                &unsealed_output_len);

        if (err == -1 || unsealed_output == NULL) {
            log_error("ecall_unseal_data FAILED\n");
            goto out;
        }

        sealed_input_str = nexus_alt64_encode(sealed_input, sealed_input_len);

        log_info("UNSEALED: `%s` -> `%s`\n", input_filepath, unsealed_output);

        nexus_free(sealed_input_str);

        ret = 0;
out:
        if (unsealed_output) {
            nexus_free(unsealed_output);
        }

        free(sealed_input);
        free(input_filepath);
    }

    return ret;
}

int
handle_seal_secret(int argc, char ** argv)
{
    int ret = -1;

    if (argc < 2) {
        log_error("not enough arguments\n");
        usage();
        return -1;
    }

    {
        char * secret_string     = strndup(argv[1], 30);
        char * output_filepath   = strndup(argv[2], 1024);

        uint8_t * sealed_output     = NULL;
        char    * sealed_output_str = NULL;
        size_t    sealed_output_len = 0;

        int err = -1;

        err = ecall_seal_data(global_enclave_id,
                              &sealed_output,
                              secret_string,
                              strlen(secret_string),
                              &sealed_output_len);

        if (err == -1 || sealed_output == NULL) {
            log_error("ecall_unseal_data FAILED\n");
            goto out;
        }

        if (nexus_write_raw_file(output_filepath, sealed_output, sealed_output_len)) {
            log_error("nexus_write_raw_file() FAILED\n");
            goto out;
        }

        sealed_output_str = nexus_alt64_encode(sealed_output, sealed_output_len);

        log_info("SEALED: `%s` -> `%s`\n", secret_string, output_filepath);

        nexus_free(sealed_output_str);

        ret = 0;
out:
        if (sealed_output) {
            nexus_free(sealed_output);
        }

        free(secret_string);
        free(output_filepath);
    }

    return ret;
}



struct _cmd {
    char * name;
    int (*handler)(int argc, char ** argv);
    char * desc;
    char * usage;
};

static struct _cmd cmds[]
    = { { "seal", handle_seal_secret, "seal secret into file", "secret filepath" },
        { "unseal", handle_unseal_secret, "unseal secret from file", "filepath" },
        { 0, 0, 0, 0 } };

static void
usage(char * prog)
{
    int i = 0;

    printf("Usage: %s <command> [args...]\n", prog);
    printf("Commands:\n");

    while (cmds[i].name) {
        printf("%-5s -- %s\n", cmds[i].name, cmds[i].desc);
        printf("\t%s %s %s\n", prog, cmds[i].name, cmds[i].usage);
        i++;
    }
}


int
main(int argc, char ** argv)
{
    int ret = -1;
    int i   = 0;


    if (create_enclave()) {
        log_error("could not create the enclave (%s)\n", ENCLAVE_PATH);
        return -1;
    }

    log_info("Created enclave (%s)", ENCLAVE_PATH);

    if (argc < 2) {
        destroy_enclave();
        usage(argv[0]);
        exit(-1);
    }

    while (cmds[i].name) {
        if (strncmp(cmds[i].name, argv[1], strlen(argv[1])) == 0) {
            ret = cmds[i].handler(argc - 1, &argv[1]);

            destroy_enclave();

            exit(ret);
        }

        i++;
    }

}

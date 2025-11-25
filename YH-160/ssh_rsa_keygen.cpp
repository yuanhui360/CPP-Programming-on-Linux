#include <iostream>
#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <fcntl.h>

#include <sys/stat.h>

/*
 * This example generates rsa SSH private key and public key file
 */

int main(int argc, char* argv[]) {

    int rc = 0;

    if ( argc != 3 ) {
        printf("Usage : %s <Key size (bits)> <key file>\n", argv[0]);
        std::cout << "bits  : rsa, 1024, 2048, 3072, 4096, 8192 " << std::endl;
        std::cout << "Example : " << std::endl;
        std::cout << "        " << argv[0] << " 4096 my_id_rsa" << std::endl;
        return -1;
    }

    /* Get and Assign Key Type amd key bits  */
    ssh_key   my_key = NULL;
    enum ssh_keytypes_e my_key_type = SSH_KEYTYPE_RSA;
    unsigned long my_key_bits = (unsigned long) std::stoi(argv[1]);
    printf("INFO: SSH Key Type = %s size %d\n", ssh_key_type_to_char(my_key_type), (int)my_key_bits);

    /* Generate a new private key pair */
    rc = ssh_pki_generate(my_key_type, my_key_bits, &my_key);
    if ( rc != SSH_OK ) {
        printf("ERROR: ssh_pki_generate(%d)\n", rc);
        return -1;
    }
    printf("INFO: ssh_pki_generate() - Success\n");

    /* Export Private key to file */
    rc = ssh_pki_export_privkey_file(my_key, NULL, NULL, NULL, argv[2]);
    if ( rc != SSH_OK ) {
        printf("ERROR: ssh_pki_export_privkey_file(%d)\n", rc);
        ssh_key_free(my_key);
        return -1;
    }
    printf("INFO: ssh_pki_export_privkey_file() - Success\n");

    /* Export the public key to file */
    char *pubkey_file = NULL;
    pubkey_file = (char *)malloc(strlen(argv[2]) + 5);
    sprintf(pubkey_file, "%s.pub", argv[2]);

    rc = ssh_pki_export_pubkey_file(my_key, pubkey_file);
    if ( rc != SSH_OK ) {
        printf("ERROR: ssh_pki_export_pubkey_file(%d)\n", rc);
        ssh_key_free(my_key);
        return -1;
    }
    printf("INFO: ssh_pki_export_pubkey_file() - Success\n");

    ssh_key_free(my_key);
    free(pubkey_file);

    return rc;
}

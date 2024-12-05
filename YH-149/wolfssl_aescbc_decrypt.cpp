#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/memory.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/ssl.h>
#include <wolfssl/error-ssl.h>

/*
 * the input message is AES block length aligned, and expects the input length
 * to be a multiple of the block length, which will optionally be checked
 */

#define BLOCK_BUFFER_SIZE  AES_BLOCK_SIZE * 16

int main(int argc, char **argv)
{

    Aes enc;
    int ret;
    const byte *key = (byte *)"01234567890123456789012345678901"; // A 256 bit key 32 bytes
    const byte *iv =  (byte *)"0123456789012345";                 // A 128 bit IV  16 bytes
    assert (key || iv );

    byte text_buf[BLOCK_BUFFER_SIZE];
    byte cipher_buf[BLOCK_BUFFER_SIZE];
    memset(text_buf, '\0', BLOCK_BUFFER_SIZE);
    memset(cipher_buf, '\0', BLOCK_BUFFER_SIZE);

    if ( argc != 3 ) {
        printf("usage: %s <encypted file> <text file>\n", argv[0]);
        printf("Example : %s my_encrypted.dat my_message.txt\n", argv[0]);
        ret = -1;
        return ret;
    }

    // Open text file for reading into text_buf

    XFILE text_fp = XFOPEN(argv[2], "wb");
    XFILE cipher_fp = XFOPEN(argv[1], "rb");
    assert( text_fp || cipher_fp );

    word32 n_read = (word32)XFREAD(cipher_buf, 1, BLOCK_BUFFER_SIZE, cipher_fp);
    if ( n_read <= 0 ) {
        printf("File %s read error\n", argv[1]);
        ret = -1;
        return ret;
    }

    wc_AesInit(&enc, NULL, INVALID_DEVID);      // <-- Initialize Aes structure.

        //   ... TODO ...
        ret = wc_AesSetKey(&enc, key, AES_BLOCK_SIZE, iv, AES_DECRYPTION);  // <-- Set AES struct Key and
        ret = wc_AesCbcDecrypt(&enc, text_buf, cipher_buf, BLOCK_BUFFER_SIZE);
        XFWRITE(text_buf, 1, BLOCK_BUFFER_SIZE, text_fp);

        XFFLUSH(text_fp);
        XFCLOSE(text_fp);
        XFCLOSE(cipher_fp);

    wc_AesFree(&enc);                           // <-- free resources associated with the Aes structure

    return 0;
}

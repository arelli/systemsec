#include <openssl/evp.h>
#include <openssl/kdf.h>
#include "params.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    EVP_KDF *kdf;
    EVP_KDF_CTX *kctx = NULL;  // context?
    unsigned char derived[32];  // output
    OSSL_PARAM params[5], *p = params;

    /* Find and allocate a context for the HKDF algorithm */
    if ((kdf = EVP_KDF_fetch(NULL, "hkdf", NULL)) == NULL) {
        error("EVP_KDF_fetch");
    }
    kctx = EVP_KDF_CTX_new(kdf);
    EVP_KDF_free(kdf);    /* The kctx keeps a reference so this is safe */
    if (kctx == NULL) {
        error("EVP_KDF_CTX_new");
    }

    /* Build up the parameters for the derivation */
    *p++ = OSSL_PARAM_construct_utf8_string("digest", "sha256", (size_t)7);
    *p++ = OSSL_PARAM_construct_octet_string("salt", "salt", (size_t)4);
    *p++ = OSSL_PARAM_construct_octet_string("key", "secret", (size_t)6);
    *p++ = OSSL_PARAM_construct_octet_string("info", "label", (size_t)5);
    *p = OSSL_PARAM_construct_end();
    if (EVP_KDF_CTX_set_params(kctx, params) <= 0) {
        error("EVP_KDF_CTX_set_params");
    }

    /* Do the derivation */
    if (EVP_KDF_derive(kctx, derived, sizeof(derived)) <= 0) {
        error("EVP_KDF_derive");
    }

    /* Use the 32 bytes as a Key and IV */
    const unsigned char *key = derived+0;
    const unsigned char  *iv = derived+16;
  
    printf("Key: ");
    for (size_t i=0; i<16; ++i)
        printf("%02x ", key[i]);
    printf("\n");

    printf("IV:  ");
    for (size_t i=0; i<16; ++i)
        printf("%02x ", iv[i]);
    printf("\n");

    EVP_KDF_CTX_free(kctx);

    return 0;
}

/*
https://wiki.openssl.org/index.php/EVP_Key_Derivation

You can compile the program using C99 with the following command.

openssl$ gcc -std=c99 test.c -o test.exe -l:libcrypto.a -pthread -ldl
openssl$
Running the program results in the following output.

$ ./test.exe
Key: 2a c4 36 9f 52 59 96 f8 de 13 73 1f 56 22 4f 34
IV:  df 0e 4c 96 ca a9 3b fd ec cf 23 7b 50 39 c8 db
*/
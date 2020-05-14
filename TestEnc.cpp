/*
 * Copyright � 2015
 * by Arxan Technologies, Inc.
 *
 * This is the TransformIT AES reference sample.
 *
 * The sample demonstrates using two TransformIT AES-256-CBC instances to
 * encrypt and then decrypt a buffer of data. Additionally, rather than using
 * pre-created whitebox keys, the keys are created using dynamic key 
 * preparation, which converts "colored" data to whitebox form.
 */

#include <string.h>
 
#include "TFIT_AES_CBC_Encrypt_iAES3.h"
#include "TFIT_DK_AES_CBC_Encrypt_iAES3_red.h"
#include "TFIT_AES_CBC_Decrypt_iAES4.h"
#include "TFIT_DK_AES_CBC_Decrypt_iAES4_red.h"

/*
 * Record the return value of interim calls for reference later.
 */
static int retval = 0;

/*
 * Store the plaintext, ciphertext, and plaintext copy on the heap.
 */
static unsigned char plaintext[64] = {0};
static unsigned char ciphertext[64] = {0};
static unsigned char recovered_plaintext[64] = {0};

/*
 * Store the "red" key and two copies of the IV on the heap.
 */
static unsigned char red_key_data[256/8] = {0};
static unsigned char iv_encrypt_data[128/8] = {0};
static unsigned char iv_decrypt_data[128/8] = {0};

int aes_cbc_dk_reference_sample()
{
    /*
     * These are the whitebox keys that will be populated by the sample.
     */
    TFIT_key_iAES3_t whitebox_encryption_key = {0};
    TFIT_key_iAES4_t whitebox_decryption_key = {0};

    unsigned int outputLen = 0;

    /*
     * Create some random plaintext to encrypt
     */
    for(int i = 0; i < sizeof(plaintext); i++)
        plaintext[i] = rand();

    /*
     * Create a random buffer of data to act as a "red" colored key.
     *
     * Note that this data is not compatible with the 
     * "red" data produced by other instances. It is simply being used
     * here for demonstration purposes.
     *
     * In a production scenario, you can obtain "red" data from the output
     * of other TransformIT instances or from the network.
     */
    for(int i = 0; i < sizeof(red_key_data); i++)
        red_key_data[i] = rand();

    /*
     * Initialize both copies of the initialization vector.
     *
     * Note that two copies are used, since the encrypt and decrypt operations
     * both will overwrite the original contents of the buffer.
     */
    for(int i = 0; i < sizeof(iv_encrypt_data); i++)
        iv_decrypt_data[i] = iv_encrypt_data[i] = rand();

    /*
     * Convert the "red" key buffer into whitebox form for use with the iAES3
     * TransformIT AES-CBC-256 encryption instance.
     *
     * Note that this whitebox key will not be compatible with any other
     * TransformIT instances besides iAES3; not even other 
     * AES-CBC-256 encryption instances.
     */
    retval = TFIT_prepare_dynamic_key_iAES3_red(
        red_key_data, 
        sizeof(red_key_data), 
        (uint8_t *)&whitebox_encryption_key, 
        sizeof(whitebox_encryption_key), 
        &outputLen
    );
    if(retval != TFIT_WBAES_OK)
        return 1;

    /*
     * Convert the "red" key buffer into whitebox form for use with the iAES4
     * TransformIT AES-CBC-256 decryption instance.
     *
     * Note that this whitebox key will not be compatible with any other
     * TransformIT instances besides iAES4; not even other 
     * AES-CBC-256 decryption instances.
     */
    retval = TFIT_prepare_dynamic_key_iAES4_red(
        red_key_data, 
        sizeof(red_key_data), 
        (uint8_t *)&whitebox_decryption_key, 
        sizeof(whitebox_decryption_key), 
        &outputLen
    );
    if(retval != TFIT_WBAES_OK)
        return 2;

    /*
     * Encrypt the plaintext using a TransformIT whitebox AES-CBC-256 
     * encryption instance with the whitebox encryption key and the 
     * initialization vector.
     */
    retval = TFIT_wbaes_cbc_encrypt_iAES3(
        &whitebox_encryption_key, 
        plaintext, 
        sizeof(plaintext), 
        iv_encrypt_data, 
        ciphertext
    );
    if(retval != TFIT_WBAES_OK)
        return 5;

    /*
     * Decrypt the ciphertext using a TransformIT whitebox AES-CBC-256
     * decryption instance with the whitebox decryption key and the
     * initialization vector.
     */
    retval = TFIT_wbaes_cbc_decrypt_iAES4(
        &whitebox_decryption_key, 
        ciphertext, 
        sizeof(ciphertext), 
        iv_decrypt_data, 
        recovered_plaintext
    );
    if(retval != TFIT_WBAES_OK)
        return 6;

    /*
     * Compare and confirm that the original plaintext matches the decrypted
     * plaintext.
     */
    retval = memcmp(plaintext, recovered_plaintext, sizeof(plaintext));
    if(retval != 0)
        return 7;

    return 0;
}

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
#include <iostream>
#include <string>
using namespace std;
#include <stdlib.h>
#include <stdio.h>
#include "TFIT_key_iAES3.h" 
#include "TFIT_key_iAES4.h" 
#include "TFIT_key_iAES3.c" 
#include "TFIT_key_iAES4.c" 
#include "TFIT_AES_CBC_Encrypt_iAES3.h"
#include "TFIT_AES_CBC_Decrypt_iAES4.h"


/*
 * Record the return value of interim calls for reference later.
 */

/*
 * Store the plaintext, ciphertext, and plaintext copy on the heap.
 */


static unsigned char recovered_plaintext[64] = {0};

/*
 * Store the "red" key and two copies of the IV on the heap.
 */


static unsigned char iv_decrypt_data[128/8] = {0};



unsigned char* encrypt(string plaintext, string ciphertext, string iv_to_be_passed)
{
    int retvalE = 0;

    unsigned char iv_encrypt_data[128/8] = "AcynMwik";; 
    /*for(int i = 0; i < iv_to_be_passed.length(); i++)
        iv_encrypt_data[i] = rand();*/

    unsigned char ciphertextArr[64] = {0}; 
    
    unsigned char plaintextArr[64] = "865 6767 1234 9089***23/04/2023";;

    /*for ( int cnt_char = 0;  cnt_char < ciphertext.length(); cnt_char++) { 
        plaintextArr[cnt_char] = plaintext[cnt_char]; 
        
    } */
    
    /*
     * These are the whitebox keys that will be populated by the sample.
     */
    TFIT_key_iAES3_t whitebox_encryption_key = TFIT_key_iAES3;
    

    int outputLen = 0;

    printf("Encryption : Plain Text :\t %s " , plaintextArr);
    printf("Encryption : IV :\t %s " , iv_encrypt_data);
    /*
     * Encrypt the plaintext using a TransformIT whitebox AES-CBC-256 
     * encryption instance with the whitebox encryption key and the 
     * initialization vector.
     */
    retvalE = TFIT_wbaes_cbc_encrypt_iAES3(
        &whitebox_encryption_key, 
        plaintextArr, 
        sizeof(plaintextArr), 
        iv_encrypt_data, 
        ciphertextArr
    );
    
     
    if(retvalE != TFIT_WBAES_OK) {
        printf("FAILURE: Encryption:\t  %d" , retvalE);
        retvalE = -1;
    } else {   
        printf("Encryption Value :\t %s " , ciphertextArr);
       
        retvalE = 0;
    }

    //unsigned char charPointer = ciphertextArr;
    return ciphertextArr;
    }

unsigned char* decrypt(string plaintext, string ciphertext, string iv_to_be_passed)
{
    int retvalD = 0;
    /*
     * These are the whitebox keys that will be populated by the sample.
     */
    
    TFIT_key_iAES4_t whitebox_decryption_key = TFIT_key_iAES4;

    unsigned int outputLen = 0;


    unsigned char iv_decrypt_data[128/8] = "AcynMwik";; 
    /*for(int i = 0; i < iv_to_be_passed.length(); i++)
        iv_encrypt_data[i] = rand();*/

    unsigned char ciphertextArr[256] = "P????i?%?{??S?&Aʨ'HܰDj?7vꀌ̔)ZG?ݧ??49??}Y?C??w???It9??}Y?C??w???It"; 
    
    unsigned char plaintextArr[64] = {0};;

    

    

    printf("Decryption : Cipher Text :\t %s " , ciphertextArr);
    /*
     * Decrypt the ciphertext using a TransformIT whitebox AES-CBC-256
     * decryption instance with the whitebox decryption key and the
     * initialization vector.
     */
    retvalD = TFIT_wbaes_cbc_decrypt_iAES4(
        &whitebox_decryption_key, 
        ciphertextArr, 
        sizeof(ciphertextArr), 
        iv_decrypt_data, 
        plaintextArr
    );
    
    if(retvalD != TFIT_WBAES_OK) {
        printf("FAILURE: Decryption:\t  %d" , retvalD);
        retvalD = -1;
    } else {   
        printf("Decryption Value :\t %s " , plaintextArr);
       
        retvalD = 0;
    }

        

    
    return plaintextArr;
}

int main (void)
{

    string  plaintext = "BIBHUBIBHUBHU1";
    string  ciphertext;
    string  iv_to_be_passed = "TestTestTestTest";
	unsigned char *retVal = encrypt(plaintext, ciphertext, iv_to_be_passed);
    
    printf("ENCRYPTION RESPONSE ----- %s \t", retVal);

    unsigned char *retValD = decrypt(plaintext, ciphertext, iv_to_be_passed);
    
    printf("DECRYPTION RESPONSE ----- %s \t", retValD);
    return 0;

}
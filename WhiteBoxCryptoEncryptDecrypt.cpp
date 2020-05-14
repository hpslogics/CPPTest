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
#include <algorithm>
using namespace std;
#include <stdlib.h>
#include <stdio.h>
#include "TFIT_key_iAES3.h" 
#include "TFIT_key_iAES4.h" 
#include "TFIT_key_iAES3.c" 
#include "TFIT_key_iAES4.c" 
#include "TFIT_AES_CBC_Encrypt_iAES3.h"
#include "TFIT_AES_CBC_Decrypt_iAES4.h"



string rtrim(const string s)
{
    const  string WHITESPACE = " \n\r\t\f\v";
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}


string encrypt(char* plaintextInput, char* iv_to_be_passed)
{
   
    int retval = 0;
    /*
    * Store the plaintext, ciphertext, and plaintext copy on the heap.
    */
    unsigned char plaintext[1024] = {0};
    unsigned char ciphertext[1024] = {0};
   

    /*
    * Store the "red" key and two copies of the IV on the heap.
    */
   
    unsigned char iv_encrypt_data[128/8] = {0};
   
        
    /*
     * Create some random plaintext to encrypt
     */
    memcpy(plaintext, plaintextInput, 1024);
    memcpy(iv_encrypt_data, iv_to_be_passed, 16);

    
    
    /*
     * These are the whitebox keys that will be populated by the sample.
     */
    TFIT_key_iAES3_t whitebox_encryption_key = TFIT_key_iAES3;
    

    
    printf("\nEncryption : Plain Text=== %s%s" , plaintext, "===");
    printf("\nEncryption : IV===%s%s" , iv_encrypt_data, "===");
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
    
    if(retval != TFIT_WBAES_OK) {
        printf("\nFAILURE : Encryption:  %d" , retval);
        retval = -1;
        
    } else {  
        retval = 0; 
        printf("\nEncryption : Value===%s%s" , ciphertext, "===");
        
    
        
    }

    std::string returnText(ciphertext, ciphertext + 1024);
    return returnText;
}

string decrypt(char* cipherTextInput, char* iv_to_be_passed)
{
   
    int retval = 0;
    /*
    * Store the plaintext, ciphertext, and plaintext copy on the heap.
    */
    
    unsigned char ciphertext[1024] = {0};
    unsigned char recovered_plaintext[1024] = {0};

    /*
    * Store the "red" key and two copies of the IV on the heap.
    */
   
    
    unsigned char iv_decrypt_data[128/8] = {0};
        
    /*
     * Create some random plaintext to encrypt
     */
    memcpy(ciphertext, cipherTextInput, 1024);
    memcpy(iv_decrypt_data, iv_to_be_passed, 16);
    

    
    
    /*
     * These are the whitebox keys that will be populated by the sample.
     */
    
    TFIT_key_iAES4_t whitebox_decryption_key = TFIT_key_iAES4;

    
    printf("\nEncryption : Plain Text=== %s%s" , ciphertext, "===");
    printf("\nEncryption : IV===%s%s" , iv_decrypt_data, "===");
    
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
    if(retval != TFIT_WBAES_OK) {
        printf("\nFAILURE : Decryption:  %d" , retval);
        retval = -1;
        
    } else {  
        retval = 0; 
        printf("\nDecryption : Value===%s%s" , recovered_plaintext, "===");
        
    
        
    }
    std::string returnText(recovered_plaintext, recovered_plaintext + 1024);
    return returnText;
}


int main (int argc, char* argv[])
{
    printf("\nNumber Of Arguments Passed===%d",argc); 
    printf("\nFirst Arguments Passed===%s",argv[1]); 
    printf("\nSecond Arguments Passed===%s",argv[2]); 
    
    //string  plaintext = argv[1];
    string ciphertext;
    string plaintext;
    
	ciphertext = encrypt(argv[1], argv[2]);
    
    printf("\nENCRYPTION : RESPONSE ===%s%s", ciphertext.c_str(),"===");
    
    char* cipherValue = strcpy(new char[ciphertext.length() + 1], ciphertext.c_str());
    plaintext = decrypt(cipherValue, argv[2]);
   
    printf("\nDECRYPTION : RESPONSE ===%s%s", plaintext.c_str(),"===");
    
    return 0;

}
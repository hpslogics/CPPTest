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
#include "TFIT_key_iAES3.c" 
#include "TFIT_AES_CBC_Encrypt_iAES3.h"



string rtrim(const string s)
{
    const  string WHITESPACE = " \n\r\t\f\v";
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}


string encrypt(string plaintext, string iv_to_be_passed)
{
    int retvalE = 0;

    
    unsigned char ciphertextArr[] = {0}; 
    
    
    /*
     * Create  plaintext array to encrypt
     */
    int plaintextsize = ((plaintext.length()/16) + 1) * 16;
    unsigned char plaintextArr[plaintextsize];
    std::copy( plaintext.begin(), plaintext.end(), plaintextArr );
    plaintextArr[plaintext.length()] = 0;
    

    /*
     * Create  IV array to encrypt
     */
    
    unsigned char iv_encrypt_data[128/8]; 
    std::copy( iv_to_be_passed.begin(), iv_to_be_passed.end(), iv_encrypt_data);
    iv_encrypt_data[iv_to_be_passed.length()] = 0;
    

    
    /*
     * These are the whitebox keys that will be populated by the sample.
     */
    TFIT_key_iAES3_t whitebox_encryption_key = TFIT_key_iAES3;
    

    int outputLen = 0;

    printf("\nEncryption : Plain Text=== %s%s" , plaintextArr, "===");
    printf("\nEncryption : IV===%s%s" , iv_encrypt_data, "===");
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
        printf("\nFAILURE : Encryption:  %d" , retvalE);
        retvalE = -1;
        
    } else {   
        printf("\nEncryption : Value===%s%s" , ciphertextArr, "===");
        
        int charcount = sizeof(ciphertextArr);
       
        printf("\nEncryption : Value Size :%d" , charcount);
        std::string ciphertext(ciphertextArr, ciphertextArr + 256);
        ciphertext = rtrim(ciphertext);
        printf("\nEncryption : String===%s%s" , ciphertext.c_str(), "===");
        retvalE = 0;
        return ciphertext;
    }

    //unsigned char charPointer = ciphertextArr;
    return "F";
    }




int main (int argc, char* argv[])
{
    printf("\nNumber Of Arguments Passed===%d",argc); 
    printf("\nFirst Arguments Passed===%s",argv[1]); 
    printf("\nSecond Arguments Passed===%s",argv[2]); 
    
    //string  plaintext = argv[1];
    string  ciphertext;
    
    //string  iv_to_be_passed = argv[2];
	ciphertext = encrypt(argv[1], argv[2]);
    
    printf("\nENCRYPTION : RESPONSE ===%s%s", ciphertext.c_str(),"===");
    
    return 0;

}
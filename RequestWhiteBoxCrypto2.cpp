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

    printf("Encryption : Plain Text=== %s%s" , plaintextArr, "===");
    printf("Encryption : IV===%s%s" , iv_encrypt_data, "===");
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
        printf("Encryption Value===%s%s" , ciphertextArr, "===");
        
        int charcount = sizeof(ciphertextArr);
       
        printf("Encryption Value Size :\t %d " , charcount);
        std::string ciphertext(ciphertextArr, ciphertextArr + 256);
        ciphertext = rtrim(ciphertext);
        printf("Encryption String===%s%s" , ciphertext.c_str(), "===");
        retvalE = 0;
        return ciphertext;
    }

    //unsigned char charPointer = ciphertextArr;
    return "F";
    }

string decrypt(string ciphertext, string iv_to_be_passed)
{
    int retvalD = 0;
    /*
     * These are the whitebox keys that will be populated by the sample.
     */
    
    TFIT_key_iAES4_t whitebox_decryption_key = TFIT_key_iAES4;

    unsigned int outputLen = 0;

    unsigned char plaintextArr[64] = {0};

    /*
     * Create  CipherText array to Decrypt
     */
    int ciphertextsize = 0;
    int remainder = (ciphertext.length()%16);
    if (remainder != 0) 
    {
        ciphertextsize = ((ciphertext.length()/16) + 1) * 16;
        
    } else {
         ciphertextsize = ciphertext.length();
    }
    
    unsigned char ciphertextArr[ciphertextsize];
    std::copy(ciphertext.begin(), ciphertext.end(), ciphertextArr);
    plaintextArr[ciphertext.length()] = 0;
    

    /*
     * Create  IV array to Decrypt
     */
    
    unsigned char iv_decrypt_data[128/8]; 
    std::copy( iv_to_be_passed.begin(), iv_to_be_passed.end(), iv_decrypt_data);
    iv_decrypt_data[iv_to_be_passed.length()] = 0;

    

    printf("Decryption : Cipher Text===%s%s" , ciphertextArr, "===");
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
        printf("FAILURE: Decryption:%d" , retvalD);
        retvalD = -1;
        
    } else {   
        printf("Decryption Value===%s" , plaintextArr);
        
        int charcount = sizeof(plaintextArr);
       
        printf("Decryption Value Size :%d" , charcount);
        std::string plaintext(plaintextArr, plaintextArr + 256);
        plaintext = rtrim(plaintext);
        printf("Decryption String===%s%s" , plaintext.c_str(), "===");
        retvalD = 0;
        return plaintext;
    }

    //unsigned char charPointer = ciphertextArr;
    return "F";

}



int main (int argc,char* argv[])
{
    printf("\nNumber Of Arguments Passed===%d",argc); 
    printf("\nFirst Arguments Passed===%s",argv[1]); 
    printf("\nSecond Arguments Passed===%s",argv[2]); 
    
    //string  plaintext = argv[1];
    string  ciphertext;
    
    //string  iv_to_be_passed = argv[2];
	ciphertext = encrypt(argv[1], argv[2]);
    
    printf("ENCRYPTION RESPONSE ===%s%s", ciphertext.c_str(),"===");
    
     string retValDEcryption = decrypt(ciphertext, argv[2]);
    
    printf("DECRYPTION RESPONSE ===%s%s", retValDEcryption.c_str(),"===");
    return 0;

}
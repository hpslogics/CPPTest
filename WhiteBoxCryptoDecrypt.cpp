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


#include "TFIT_key_iAES4.h" 

#include "TFIT_key_iAES4.c" 

#include "TFIT_AES_CBC_Decrypt_iAES4.h"


string rtrim(const string s)
{
    const  string WHITESPACE = " \n\r\t\f\v";
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
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
    
    unsigned char ciphertextArr[64];
    std::copy(ciphertext.begin(), ciphertext.end(), ciphertextArr);
    plaintextArr[ciphertext.length()] = 0;
    

    /*
     * Create  IV array to Decrypt
     */
    
    unsigned char iv_decrypt_data[128/8] = "AcynMwik"; 
    std::copy( iv_to_be_passed.begin(), iv_to_be_passed.end(), iv_decrypt_data);
    iv_decrypt_data[iv_to_be_passed.length()] = 0;

    

    printf("\nDecryption : Cipher Text===%s%s" , ciphertextArr, "===");
    printf("\nDecryption : IV===%s%s" , iv_decrypt_data, "===");
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
        printf("\nFAILURE: Decryption:%d" , retvalD);
        retvalD = -1;
        
    } else {   
        printf("\nDecryption :Value===%s%s" , plaintextArr, "===");
        
        int charcount = sizeof(plaintextArr);
       
        printf("\nDecryption : Value Size :%d" , charcount);
        std::string plaintext(plaintextArr, plaintextArr + 64);
        plaintext = rtrim(plaintext);
        printf("\nDecryption : String===%s%s" , plaintext.c_str(), "===");
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
    
   
    string retValDEcryption = decrypt(argv[1], argv[2]);
    
    printf("\nDECRYPTION :  RESPONSE ===%s%s", retValDEcryption.c_str(),"===\n");
    return 0;

}
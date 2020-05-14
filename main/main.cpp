// Copyright (C) Arxan Technologies Inc. 2016
// All rights Reserved.
//
// main.cpp - generic main program for TransformIT samples

#include "RequestWhiteBoxCrypto.cpp"

// entry point
int main (void)
{
	
	int retval2 = 0;
	// prototypes for test functions
	string plaintextM = "Test";
	string ciphertextM = "Test";
	string iv_testM = "Test";
	retval2 = aes_cbc_encrypt (plaintextM, ciphertextM, iv_testM);
	
	string plaintext2 = "Test";
	string ciphertext2 = "Test";
	string iv_test2 = "Test";
	retval2= aes_cbc_decrypt (plaintext2, ciphertext2, iv_test2);
	return retval2;
}

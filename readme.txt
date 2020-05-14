This is a buildable version of the TransformIT code samples.

It works on OSX, Linux and Windows.

Instructions
------------
1. Download and install OpenSSL. On OSX and Windows you will probably need to build it too

2. Edit makefile to point to your TransformIT installation and your OpenSSL installation

3. 'make run' will build and run all the samples. On Windows: 'nmake -f nmakefile run'

OpenSSL on Windows
------------------
To build OpenSSL v1.1.0 for Windows:

1. install ActiveState Perl
2. download & unpack the OpenSSL tarball
3. open a Visual Studio command prompt & cd to the OpenSSL dir
4. perl Configure VC-WIN64A no-asm --prefix=C:\Arxan\openssl
5. nmake
6. nmake install

makerules.py
------------
A script to scan the code samples to generate most of the build rules, based on the
TransformIT instances and keys #included in the code.

These are the commands to re-run the script, if you need to:

Unix:     ./makerules.py > rules.mk
Windows:  py makerules.py > rules_win.mk

Also AES_GCM keys are included in text and binary formats:

aes_gcm_enc_key.txt
aes_gcm_dec_key.dat





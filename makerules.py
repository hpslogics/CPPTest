#!/usr/bin/env python
#
# Copyright (C) Arxan Technologies Inc. 2016
# All rights Reserved.
#
# makerules.py - script to scan TFIT samples looking for the instances used

import os.path
import sys
import re
import platform

srcdir = "."

print("# Copyright (C) Arxan Technologies Inc. 2017")
print("# All rights Reserved.")
print("#")
print("# ************ AUTO GENERATED ***************")
print("# ****  run makerules.py to re-generate  ****")
print("# *******************************************")

# are we setting up a Windows build?
windows = False
if platform.system() is "Windows":
	windows = True

exe = ""
obj = ".o"
slash = "/"
if windows:
	exe = ".exe"
	obj = ".obj"
	slash = "\\"


# scan all the samples
allfiles = os.listdir(srcdir)
alltests = []

for filename in allfiles:
    # ignore anything that isn't C++
    if not filename.endswith(".cpp"):
        continue

    #print(filename)
    
    # work out what the corresponding object file will be called 
    sample   = filename.replace(".cpp", "")
    srcfile  = "$(SRC)" + slash + filename
    objfile  = "$(OBJ)" + slash + filename.replace(".cpp", obj)
    binfile  = "$(BIN)" + slash + sample + exe
    tfitlibs = []
    tfitkeys = []
    testfunc = []
    mainobj  = objfile.replace("RequestWhiteBoxCrypto", "MAIN")
    line = ""
    
    # read the whole file
    with open(srcdir + slash + filename, 'r') as cplusplus:
        for line in cplusplus:
            line = line.strip()
            
            # look for #include "TFIT_....." lines
            if line.startswith("#include"):
                words = line.split()            
                hfile = words[1].lstrip('"').rstrip('"')
                if not hfile.startswith("TFIT_"):
                    continue
                
                # work out the library names
                if hfile.startswith("TFIT_DK_") or hfile.startswith("TFIT_DS_"):
                    continue
                elif hfile.startswith("TFIT_nbv_"):
                    continue
                elif hfile[5].islower():
                    # its a key, make sure it gets generated
                    tfitkeys.append(hfile)
                else:
                    # its a TFIT instance, remember all the library names
                    tfitlibs.append(hfile.replace(".h", ""))  

            # look for the test function(s)
            else:
                m = re.match("int\s+([\w_]+)\(\w*\)", line)
                if m:
                    testfunc.append(m.group(1))     
        
    # make sure there are some TFIT dependencies
    # this avoids creating a rule for the SAMPLE_common.cpp file 
    if tfitlibs is "":
        continue
    
    # did we find entry points for the test code?
    num = len(testfunc)
    if num is 0: 
        continue
    
    defines = "-DNAME=" + sample + " -DTEST=" + testfunc[0]
    if num is 2:
        defines = defines + " -DTEST2=" + testfunc[1]
    
    # print a separator
    print("\n")
    print("#-------------------------------------------------------------------------------------")
    print("# " + sample)
    print("#-------------------------------------------------------------------------------------")

    # print a rule to compile a main function for this test
    print(mainobj + ": $(SRC)" + slash + "main" + slash + "main.cpp")         
    if windows:
    	print("\t$(CXX) /Fo" + mainobj + " $(CFLAGS) -c $(SRC)\main\main.cpp " + defines)
    else:
    	print("\t$(CXX) -o " + mainobj + " $(CFLAGS) -c $(SRC)/main/main.cpp " + defines)

    # print rules to make sure keys get generated
    for k in tfitkeys:
        print(srcfile + ": $(KEYS)" + slash + k)

    for k in tfitkeys:
        if not k.startswith("TFIT_nbv_"):      
        	print(binfile + ": $(OBJ)" + slash + k.replace(".h", obj))               

    # print a rule to link the sample
    if windows:
    	print(binfile + ": " + objfile + " " + mainobj) 
    	cmd = "\t$(LD) $(LFLAGS) "
    	for l in tfitlibs:
    		cmd = cmd + l + ".lib " ;    
    	print(cmd + "TFIT_Utils.lib")
    else:
    	print(binfile + ": " + objfile + " " + mainobj) 
    	cmd = "\t$(LD) $(LFLAGS)"
    	for l in tfitlibs:
    		cmd = cmd + " -l" + l;    
    	print(cmd + " -lTFIT_Utils")
    	
    # keep a list of all test cases
    alltests.append(binfile)
    

# print a makefile rule for all the tests
all_tests = ""
for test in alltests :
    all_tests += "\t" + test + " \\\n"
all_tests = all_tests.rstrip("  \\\n")
print("\n\nALL_SAMPLES = " + all_tests)

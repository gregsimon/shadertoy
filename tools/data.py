#!/usr/bin/env python
"""
    http://acm.zhihua-lai.com
    Convert Files to *.h, *.pas or *.asm
    And Include it in Your Applications.
"""

from sys import argv
from os import path

def gethex(c):
    """
        return hex str: char[4]
        padding zero e.g. 0xa => 0x0a 
    """
    s = hex(ord(c)).upper()
    if len(s) == 3:
        return s[0:2] + '0' + s[2:]
    return s

def getpashex(c):
    """
        return pas hex
    """
    s = gethex(c)
    return "$" + s[2:]  

def getbytes(filename):
    """
        return byte array of a file
    """
    if not path.isfile(filename):
        return ""
    try:
        fh = open(filename, 'rb')
    except IOError as e:
        print "({})".format(e)
    else:
        return fh.read()
    finally:
        fh.close()
   
def converttoasm(s, f):
    """
        return asm include 
    """
    j = len(s)
    if j == 0:
        return ""
    output = f + ": \n" + chr(9) + "db "
    i = 0
    while i < j - 1:
        if i % 16 == 15:
            output += gethex(s[i]) + ";" + str(i + 1) + "\n" + chr(9) + "db "
        else:
            output += gethex(s[i]) + ","
        i += 1
    output += gethex(s[j - 1]) + ";" + str(i + 1) + "\n"
    output += "Output_End:\n"
    output += f + "_Size EQU " + str(j)
    return output

def converttopas(s, f):
    """
        return *.pas include
    """
    j = len(s)
    if j == 0:
        return ""
    output = "const " + f + ": packed array [0 .. " + str(j - 1) + "] of Byte = (\n" + chr(9)
    i = 0
    while i < j - 1:
        output += getpashex(s[i]) + ","
        if i % 16 == 15:
            output += "//" + str(i + 1) +  "\n" + chr(9)
        i += 1
    output += getpashex(s[j - 1]) + "//" + str(i + 1) + "\n);\n"
    output += "const " + f + "_size = " + str(j) + ";\n"
    return output

def converttocpp(s, f):
    """
        return *.h include
    """
    j = len(s)
    if j == 0:
        return ""
    output = "unsigned char " + f + "[] = {\n" + chr(9)
    i = 0
    while i < j - 1:
        output += gethex(s[i]) + ","
        if i % 16 == 15:
            output += "//" + str(i + 1) +  "\n" + chr(9)
        i += 1
    output += gethex(s[j - 1]) + "//" + str(i + 1) + "\n};\n"
    output += "#define " + f + "_size " + str(j) + "\n"
    return output

def converttophp(s, f):
    """
        return *.php include
    """
    j = len(s)
    if j == 0:
        return ""
    output = "<?php\n$" + f + " = array(\n" + chr(9)
    i = 0
    while i < j - 1:
        output += gethex(s[i]) + ","
        if i % 16 == 15:
            output += "//" + str(i + 1) +  "\n" + chr(9)
        i += 1
    output += gethex(s[j - 1]) + "//" + str(i + 1) + "\n);\n"
    output += "define ('" + f + "_size', " + str(j) + ");\n?>\n"
    return output

def usage():
    print "Usage: %s asm|pas|cpp|php filename|string" % argv[0] 
    print "Filename will be treated as string if not found."

if __name__ == "__main__":
    if len(argv) != 3:
        usage()
    elif not path.isfile(argv[2]):
        if argv[1] == 'asm':
            print converttoasm(argv[2], "OutputString" + str(len(argv[2])))
        elif argv[1] == 'cpp':
            print converttocpp(argv[2], "OutputString" + str(len(argv[2])))
        elif argv[1] == 'pas':
            print converttopas(argv[2], "OutputString" + str(len(argv[2])))
        elif argv[1] == 'php':
            print converttophp(argv[2], "OutputString" + str(len(argv[2])))
        else:
            usage()
    elif argv[1] == 'asm':
        print converttoasm(getbytes(argv[2]), "OutputFile" + str(len(argv[2])))
    elif argv[1] == 'cpp':
        print converttocpp(getbytes(argv[2]), "OutputFile" + str(len(argv[2])))
    elif argv[1] == 'pas':
        print converttopas(getbytes(argv[2]), "OutputFile" + str(len(argv[2])))
    elif argv[1] == 'php':
        print converttopas(getbytes(argv[2]), "OutputFile" + str(len(argv[2])))
    else:
        usage()

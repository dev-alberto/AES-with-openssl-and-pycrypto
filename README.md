# Symmetric-Cryptography-and-Hash-functions
First assignment for Information Security class
# Problem statement

1. Write a C/C++ program that uses API EVP to implement a simple dictionary attack
to find the encryption key for two files, one encrypted with AES in ECB mode, one with
AES in CFB mode.
2. Check to see if MD5, SHA256 give any information on whether two plain texts are similar(plain texts differe by one byte).
3. Implement a Simple Key Exchange protocol

# Investigation/Research

I have used the documentation given in the homework PDF, as well as my handwritten notes, taken during class. I have also used http://www.cplusplus.com/reference/ for
various language related hurdles. For the python implementation I have used the pycrypto documentation, available here https://www.dlitz.net/software/pycrypto/api/2.6/
, as well as the actual python documentation https://docs.python.org/2/

# Describing the environment used/ how to compile

For first and second exercises a standard C compiler needed, with openssl installed. For the third problem, python 2.7 is required, see here http://askubuntu.com/questions/101591/how-do-i-install-python-2-7-2-
on-ubuntu Then pip is needed : http://www.saltycrane.com/blog/2010/02/how-install-
pip-ubuntu/ Pycrypto library is installed like this: sudo pip install pycrypto If there are
installation issues, use: http://stackoverflow.com/questions/7845963/python-pycrypto-
installation-error
To compile the first exercise, I have included a makefile, as well as a bash file, named
encrypt. The bash file is used to encrypt the plain text with in two ways (ECB and
CFB). Two files with encrypted content will be therefore created, which will be used in
the script.
Compile flow : use bash to encrypt plain text in 2 ways : ./encrypt
use makefile : make
compile : ./out
To compile the second exercise, use the included makefile, than give as command line
arguments file f1 and file f2
To compile third exercise, open 3 consoles in the directory where the files .py are located.
Compile in this order: python KM.py ; python B.py ; python A.py
In order to change the mode of encryption for this exercise, modify in the script globals.py
the variable mode.

# Construction/Implementation

Exercise 1
To solve this, I iterated through the words in the dictionary file, in their given order. If
they were smaller than 16 bytes I padded them with spaces. Than I read into a string the
entire contents of the encrypted file, I decrypted the string using keys from the dictionary
file and compare the result with the plain text. When a match has been found, I returned
the key.
Exercise 2
I made two methods to compute the md5 and sha256 hashes. Both methods take in file
paths and return strings representing the two hashes. Than we use the method compare
files, in order to see if there are similarities between the hashes of similar files (plain texts
have one different byte, as per specification). We return the number of similar bytes.
Exercise 3
To implement this, I used the python programming language, modeling the problem in
terms of a server/client architecture.
- The Key manager is a TCP server, which for every accepted connection, it expect a 3
byte message(encryption mode). It will send the encrypted key to the client.
- The A node is a TCP client which will start a connection with the KM in order to find
out which key to use in the encryption of the text. Than it will start a connection with
B, in order to send it the type of encryption used. After it receives the key, it will decrypt
and start sending encrypting 16 bytes of plain text at a time and sending the result(in
base 64) to B. - The Node B is a TCP server, which awaits the receival of the mode of
encrytion. After he receives the encrypted key, it will decrypt it and use it to decrypt
the text received from A.
In the script aes_ut.py I have implemented a class that encrypts and decrypts data for
aes cfb, ecb. It is worth noting the fact that, unlike the C counterpart library openssl,
padding is not done by default here.

# Analysis & Testing

Exercise 1
To test this, I used encrypted files in the command line (https://linuxconfig.org/using-openssl-to-encrypt-messages-and-files-on-linux), then I used my script to check whether
the right key was returned. I used this tool to convert string to hex http://string-functions.com/hex-string.aspx
Exercise 2
To test this exercise, we used three types of files. Files smaller than 16 bytes, files between
16 and 32 bytes and files that are larger in size, for example a Wikipedia page. For the
first two types of file, the bytes will always be different for both hash algorithms. For the
2big files, the SHA256 algorithms still gives 0 similar and at most 1 similar byte for the
md5 algorithm.
Exercise 3
I have used files of different sizes to test how well the server performs.

# Final Remarks

Exercise 1
The key provided here is not secure, because it is vulnerable to a simple dictionary attack.
If the key used were a random 16 bytes string, to brute force the key we would require
2 128 iterations, which is not feasible.
Exercise 2
After the performed tests, I observed that the result of the hash functions is deterministic.
A small change of the message will alter the result of the hash function so that similar
messages appear to be uncorrelated.
Exercise 3
If an attacker can see the messages between A and B, and determine that the mode of
encryption used is ECB, he could determine whether 2 blocks of 16 bytes have the same
plain text, which is an information leak. As a rule of thumb, never use ECB for encrypting
more than one block of data.

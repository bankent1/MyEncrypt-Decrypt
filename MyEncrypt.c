/*
MyEncrypt [filename1.txt] [filename2.txt] ...

Author: Travis Banken

This program will encrypt a file by hashing a password and using bitwise XOR
to encrypt in blocks.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encryptFile(char[], int, FILE*, FILE*);
void hash(char[], int);
int successiveSquares(int, int, int);
int reverseBinary(int, char[]);
int getLength(char[]);
void delExtension(char[]);

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("Missing Arguments!!\n");
		printf("Usage: MyEncrypt [filename1.txt] [filename2.txt] ...\n");
		exit(1);
	}
	
	char password[32];
	printf("Enter password: ");
	scanf("%s", password);
	int length = getLength(password);	
	
	// Encrypt each file
	for (int i = 1; i < argc; i++) {
		// create outfile name
		char outName[32];
		strcpy(outName, argv[i]);
		delExtension(outName);
		char ext[] = "enc";
		strcat(outName, ext);
		FILE *outfile = fopen(outName, "w");
		FILE *infile = fopen(argv[i], "r");
		if (infile == NULL) {
			printf("ERROR: File not found!\n");
			exit(1);
		}	
		// encrypt file
		printf("Encrypting %s . . . . ", argv[i]);
		encryptFile(password, length, infile, outfile);
		printf("DONE\n");
		fclose(infile);
		fclose(outfile);
	}
	
}

/*
Encrypts the file by XOR each char with a corresponding char in the 
user password. Once all char in password are used, the password is
hashed to a new password. A new file will be written containing the 
cipher text.

Params: password - user password to serve as encryption key
	length - length of the user password
	infile - file to be decrypted
*/
void encryptFile(char password[], int length, FILE *infile, FILE *outfile) {
	int plaintext;	
	int count = 0;

	while ((plaintext = getc(infile)) != EOF) {
		if (count == length) {
			hash(password, length);
			count = 0;
		}
		char ciphertext = password[count] ^ plaintext;
		fputc(ciphertext, outfile);
		count++;
	}
}

/*
Hashes each character of the password to form a new password.

Params: password - encryption key to be hashed into new key
	length - length of the encryption key 	
*/
void hash(char password[], int length) {
	int prod = 1;
	for (int i = 0; i < length; i++) {
		int asciiLen = 127;
		prod = prod * (password[i] + 2);
		password[i] = successiveSquares(prod, password[i], asciiLen);
	}

}

/*
Calculates a^b (mod n) using successive squares. Returns the
result.
*/
int successiveSquares(int a, int b, int n) {
	int prod = 1;
	char binRep[10];
	int binLen = reverseBinary(b, binRep);	
	int prev = a % n;
	for (int i = 0; i < binLen; i++) {
		if (binRep[i] == '1') {
			prod = (prod * prev) % n;
		}
		prev = (prev*prev) % n;
	}
	return prod;
}

/*
Returns the string of the binary representation of the given num
in reverse.

Params: num - number to convert to binary
	binRep - array to store string
Returns: The length of the string representation.
*/
int reverseBinary(int num, char binRep[]) {
	int i = 0;
	while (num > 0) {
		if (num % 2 == 1) {
			binRep[i] = '1';
		}
		else {
			binRep[i] = '0';
		}
		num = num / 2;
		i++;
	}
	
	return i;
}

/*
Finds the length of the given string.

Params: str - string to find length of.
Return: int index of the null char
*/
int getLength(char str[]) {
	int i = 0;
	while (str[i] != '\0') {
		i++;
	}
	return i;
}

/*
Deletes the file extension sub-string.

Params: filename - name of file
*/
void delExtension(char filename[]) {
	int flag = 0;
	int i = 0;
	while (filename[i] != '\0') {
		if (flag) {
			filename[i] = '\0';
		}	
		else if (filename[i] == '.') {
			flag = 1;
		}
		i++;
	}
}

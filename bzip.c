#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *compress(char*);
char *decompress(char*);

#define MAX_INPUT_SIZE 1024

int main(int argc, char **args) {
	int info = 0; //Flag for printing additional info
	int compressInput = 1; //Flag for determining to compress or decompress
	char *input = '\0';
	//Parse for flags and input
	if (argc > 1) {
		int i;
		for (i = 1; i < argc; i++) {
			if (strcmp(args[i], "--info") == 0) {
				info = 1;
			} else if (strcmp(args[i], "--decompress") == 0) {
				compressInput = 0;
			} else {
				//If an argument didn't match a flag, treat it as input
				input = strdup(args[i]);
			}
		}
	}

	char *str;
	//If we got input as an argument, use it
	if (input) {
		str = input;
	//Else grab from stdin
	} else {
		str = malloc(MAX_INPUT_SIZE);
		fgets(str, MAX_INPUT_SIZE, stdin);
		if ((strlen(str)>0) && (str[strlen (str) - 1] == '\n'))
        	str[strlen (str) - 1] = '\0';
	}
	int ogLen = strlen(str);
	
	char *result;
	//If we were specified to compress
	if (compressInput) {
		char* compressed = compress(str);
		result = compressed;
	//Else decompress
	} else {
		char* decompressed = decompress(str);
		result = decompressed;
	}
	//Print additional info for compression, if specified
	if (info && compressInput) {
		printf("Before:\"%s\", length: %d\n", str, ogLen);
		printf("After: \"%s\", length: %lu\n", result, strlen(result));
		printf("Percent of original size: %.4f\n", (double) (100 * ((double) strlen(result) / (double) ogLen)));
	//Print additional info for decompression, if specified
	} else if (info && !compressInput) {
		printf("Before: \"%s\"\n", str);
		printf("After: \"%s\"\n", result);	
	}

	//Print the output to stdout
	fprintf(stdout, "%s", result);

	//Free allocated memory
	free(str);
	free(result);
	return 0;
}

char *compress(char* in) {
	int length = strlen(in);
	int i;
	//Keep track of the last character
	char lastLetter = in[0];
	//Count how many of the last characters there were in a row
	int count = 0;
	//See how many characters in total we've kept track of
	int lettersAdded = 0;
 	//We can assume that in the worst case scenario, we're doubling the string size + null character
	char *res = malloc(sizeof(char) * length * 2 + sizeof(char));
	memset(res, '\0', sizeof(char));
	for (i = 0; i < length; i++) {
		//If the current character is the same as the last, just increment counter
		//Ensure count does not go over 256
		if (lastLetter == in[i] && count < 256) {
			count++;
		//Else add it to the result string along with how many times it occured in a row,
		//and reset counters where necessary
		} else{
			//*2 because each added letter has the letter and number of occurences.
			int index = lettersAdded * 2;
			res[index] = lastLetter;
			res[index + 1] = count;
			lettersAdded++;
			lastLetter = in[i];
			count = 1;
		}
	}
	//Add the last character to the result
	int index = lettersAdded * 2;
	res[index] = lastLetter;
	res[index + 1] = count;
	res[index + 2] = '\0';
	//Copy only the relevant part of the result
	char *final = strdup(res);
	//Free the original result
	free(res);
	//Return the trimmed copy of the result
	return final;
}

char *decompress(char* in) {
	//First get the length of the output string
	int length = 0;
	int i;
	for (i = 1; i < strlen(in); i += 2) {
		length += in[i];
	}
	//Allocate the result string
	char *result = malloc(sizeof(char) * length + 1);
	int resIndex = 0;
	//Add each letter the appropriate amount of times
	for (i = 0; i < strlen(in); i += 2) {
		int j;
		for (j = 0; j < in[i + 1]; j++) {
			result[resIndex++] = in[i];
		}
	}
	//Terminate with null character
	result[length] = '\0';
	return result;
}

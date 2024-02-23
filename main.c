#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#define cmdAdd(cmd, ...) cmdAddNull(cmd, __VA_ARGS__, NULL)

#define ERROR "[ERROR]: %s\n"
typedef struct {
	char* content;
	int capacity;
	int len;			
} list;


void append(list* a, char c);
void crash(char* str);

void cmdAddNull(list* cmd, ...) {
	va_list valist;
	va_start(valist, cmd);
	for (;;) {
		char* str = va_arg(valist, char*);
		if (str == NULL) break;
		append(cmd, ' ');
		for (int i = 0; str[i] != 0; i++) {
			append(cmd, str[i]);
		}
	}
	va_end(valist);
}


void crash(char* str) {
	printf("ERROR: %s\n", str);
	printf("Terminating program!\n");
	exit(1);
}

void append(list* a, char c) {
	if (a->capacity == 0) {
		a->capacity = 4;
		a->content = malloc(a->capacity);
		if (a->content == NULL) assert(0 && "skill issue");
		a->len = 0;
	}
	if (a->len >= a->capacity-1) {
		list new = { 0 };
		new.len = a->len;
		new.capacity = 2*a->capacity;		
		new.content = malloc(new.capacity);
		if (new.content == NULL) assert(0 && "skill issue");
		for (int i = 0; i < a->len; i++) {
			new.content[i] = a->content[i];
		}
		free(a->content);
		a->len = new.len;
		a->content = new.content;
		a->capacity = new.capacity;
	}
	a->content[a->len] = c;
	a->content[a->len+1] = 0;
	a->len++;
}

void genName (char* src, char* dst) {
	int offset = 0;
	int i = 0;
	for (;; i++) {
		while (src[i+offset] == ' ') offset++;
		dst[i] = src[i+offset];
		if (src[i+offset] == 0) break;
	}
	i--;
	if (dst[i] == '4') dst[i] = '3';
	else {
		printf("UNREACHABLE\n");
		assert(0 && "the file is not a mp3");
	}	
}

int main (int argc, char** argv) {	
	if (argc == 1) {
		printf(ERROR, "Useage: ./ext *.mp4");
		return 1;
	}
	if (strcmp(argv[1], "*.mp4") == 0) {
		printf(ERROR, "*.mp4 is not a valid file (there are probbably no files ending with .mp4 in this directory)");
		return 2;
	}
	char name[128];
	char inputarr[16];
	for (int i = 1; i < argc; i++) {
		if (strlen(argv[i]) > 100) {
			printf(ERROR, "file name is too long");
			return 3;
		}
		printf("[INFO] processing: %s\n", argv[i]);
		list cmd = { 0 };
		cmdAdd(&cmd, "ffmpeg -i");
		cmdAdd(&cmd, argv[i]);
		cmdAdd(&cmd, "-vn");
		genName(argv[i], name);
		cmdAdd(&cmd, name);
		cmd.content++;
		while (1) {
			printf("Do you want to run: %s  [Y/n] > ", cmd.content);
			char input = 0;
	//		scanf("%c", &input);	
			fgets(inputarr, 16, stdin);
			input = inputarr[0];
			printf("%s\n", inputarr);
			printf("%c\n", input);
			if (input == 'y' || input == 'Y' || input == '\n') {
				printf("[INFO] running command: %s\n", cmd.content);
				if (system(cmd.content) != 0) {
					printf(ERROR, "promlem while running command... Terminating\n");
					return 4;
				}
				break;
			}
			else if (input == 'n' || input == 'N') {
				printf("[INFO] Skipping command\n");
				break;
			}
		}
	}
	printf("[INFO] processed all files sucsessfuly\n");
}


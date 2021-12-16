#include <stdlib.h>                 // Standard library
#include <stdio.h>					// C library
#include <stdbool.h>				// Bool library
#include <string.h>					// String library

// path example: "file.json"
// wjsonClear will create the file if it doesn't exist
// rJsonFindInd is made so you can use the name instead of manually finding the index when using for example wJsonEdit and wJsonDelete
// jsonEmpty checks for "
// jsonLength is based on the : separating the name from the value

bool fileExsit(char* path)
{
	FILE *file;
	if ((file = fopen(path, "r")) == NULL) return false;
	return true;
}

int fileLength(char* path)
{
	FILE *file;
   	if ((file = fopen(path, "r")) == NULL) { printf("\033[0;31mError while opening file\n\033[0m"); return -1; }

	fseek(file, 0, SEEK_END);
 	size_t length = ftell(file);
	fclose(file);
	return length;
}

bool jsonEmpty(char* path)
{
	FILE *file;
	if ((file = fopen(path, "r")) == NULL) { printf("\033[0;31mError while opening file\n\033[0m"); return true; }
	char ch;

	while (!feof(file))
	{
		ch = fgetc(file);
		if (ch == '\"') return false;
	}
	return true;
}

int jsonLength(char* path)
{
	FILE *file;
	if ((file = fopen(path, "r")) == NULL) { printf("\033[0;31mError while opening file\n\033[0m"); return -1; }
	char ch;

	int n = 0;
	while (!feof(file))
	{
		ch = fgetc(file);
		if (ch == ':') n++;
	}
	fclose(file);
	return n;
}

int rJsonFindInd(char* path, char* name)
{
	FILE *file;
	if ((file = fopen(path, "r")) == NULL) { printf("\033[0;31mError while opening file\n\033[0m"); return -1; }
	int length = fileLength(path);

	char str[length];
	char ch;
	int n, i = 0;

	//Read from file
	while (!feof(file))
	{
		ch = fgetc(file);
		if (ch != '\n' && ch != '\t' && ch != ' ' && (int)ch > -1 && (int)ch < 127)
		{
			if (ch == '\"')
			{
				ch = fgetc(file);
				n = 0;
				while (ch != '\"')
				{
					str[n] = ch;
					n++;
					ch = fgetc(file);
				}
				str[n] = '\0';
				if (strcmp(str, name) == 0)
				{
					fclose(file);
					return i;
				}
			}
			if (ch == ',') i++;
		}
	}
	fclose(file);
	printf("\033[0;31mName not found\n\033[0m");
	return -1;
}

void wjsonClear(char* path)
{
	FILE *file;
	if ((file = fopen(path, "w")) == NULL) { printf("\033[0;31mError while writing to file\n\033[0m"); return; }
	fprintf(file,"%s", "{\n\n}");
   	fclose(file);
	return;
}

void wJsonAppend(char* path, char* name, char* value)
{
	FILE *file;
	if ((file = fopen(path, "r")) == NULL) { printf("\033[0;31mError while opening file\n\033[0m"); return; }
	int length = fileLength(path);

	char str[length * 2];
	char ch;
	int n = 0;

	//Read from file
	while (!feof(file))
	{
		ch = fgetc(file);
		if (ch != '\n' && ch != '\t' && ch != ' ' && (int)ch > -1 && (int)ch < 127)
		{ 
			str[n] = ch; 
			n++;
			if (ch == ',' || ch == '{') { str[n] = '\n'; str[n + 1] = '\t'; n += 2; }
		}
	}
	fclose(file);
	str[n - 1] = '\0';
	if (str[n - 4] != '{') strcat(str, ",");

	// Write to file
	if ((file = fopen(path, "w")) == NULL) { printf("\033[0;31mError while writing to file\n\033[0m"); return; }
	fprintf(file,"%s\n\t\"%s\":\"%s\"\n}\n", str, name, value);
   	fclose(file);
	return;
}

void wJsonInsert(char* path, int index, char* name, char* value)
{
	FILE *file;
	if ((file = fopen(path, "r")) == NULL) { printf("\033[0;31mError while opening file\n\033[0m"); return; }
	if (index < 0 || index > jsonLength(path)) { printf("\033[0;31mIndex out of range\n\033[0m"); return; }
	int length = fileLength(path);

	char str[length * 2];
	char ch;
	int n = 0, i = -1;

	//Read from file
	while (!feof(file))
	{
		ch = fgetc(file);
		if (ch != '\n' && ch != '\t' && ch != ' ' && (int)ch > -1 && (int)ch < 127)
		{
			// Insert name and value
			if (i == index || (index == jsonLength(path) && ch == '}'))
			{
				if (ch == '}') { strcat(str, ",\n\t"); n += 3;}
				strcat(str, "\"");
				strcat(str, name);
				strcat(str, "\":\"");
				strcat(str, value);
				strcat(str, "\"");
				n += strlen(name) + strlen(value) + 5;
				if (n + 3 < length) { str[n] = ','; str[n + 1] = '\n'; str[n + 2] = '\t'; n += 3; }
				i++;
			}
			str[n] = ch;
			n++;
			if (ch == ',' || ch == '{') { str[n] = '\n'; str[n + 1] = '\t'; n += 2; i++; }
		}
	}
	fclose(file);
	str[n - 1] = '\n';
	str[n] = '}';
	str[n + 1] = '\0';

	// Write to file
	if ((file = fopen(path, "w")) == NULL) { printf("\033[0;31mError while writing to file\n\033[0m"); return; }
	fprintf(file,"%s", str);
   	fclose(file);
	return;
}

void wJsonDelete(char* path, int index)
{
	FILE *file;
	if ((file = fopen(path, "r")) == NULL) { printf("\033[0;31mError while opening file\n\033[0m"); return; }
	int length = fileLength(path);

	char str[length];
	char ch;
	int n = 0, i = 0;

	//Read from file
	while (!feof(file))
	{
		ch = fgetc(file);
		if (ch != '\n' && ch != '\t' && ch != ' ' && (int)ch > -1 && (int)ch < 127)
		{
			if (i != index)
			{
				str[n] = ch;
				n++;
				if (ch == ',' || ch == '{') { str[n] = '\n'; str[n + 1] = '\t'; n += 2; }
			}
			if (ch == ',') i++;
		}
	}
	fclose(file);
	if (str[n - 3] == ',') n -= 2;
	str[n - 1] = '\n';
	str[n] = '}';
	str[n + 1] = '\0';

	// Write to file
	if ((file = fopen(path, "w")) == NULL) { printf("\033[0;31mError while writing to file\n\033[0m"); return; }
	fprintf(file,"%s", str);
   	fclose(file);
	return;
}

void wJsonEdit(char* path, int index, char* newValue)
{
	FILE *file;
	if ((file = fopen(path, "r")) == NULL) { printf("\033[0;31mError while opening file\n\033[0m"); return; }
	int length = fileLength(path);

	char str[length];
	char ch;
	int n = 0, i = 0;

	//Read from file
	while (!feof(file))
	{
		ch = fgetc(file);
		if (ch != '\n' && ch != '\t' && ch != ' ' && (int)ch > -1 && (int)ch < 127)
		{
			str[n] = ch;
			n++;
			if (i == index && ch == ':')
			{
				ch = fgetc(file);
				ch = fgetc(file);
				str[n] = '\"';
				str[n + 1] = '\0';
				strcat(str, newValue);
				n += (int)strlen(newValue) + 1;
				str[n] = '\"';
				n++;
				while (ch != '"') ch = fgetc(file);
			}
			if (ch == ',' || ch == '{') { str[n] = '\n'; str[n + 1] = '\t'; n += 2; }
			if (ch == ',') i++;
		}
	}
	fclose(file);
	str[n - 1] = '\n';
	str[n] = '}';
	str[n + 1] = '\0';

	// Write to file
	if ((file = fopen(path, "w")) == NULL) { printf("\033[0;31mError while writing to file\n\033[0m"); return; }
	fprintf(file,"%s", str);
   	fclose(file);
	return;
}

char ***rJson(char* path)
{
	FILE *file;
	if ((file = fopen(path, "r")) == NULL) { printf("\033[0;31mError while opening file\n\033[0m"); return (char ***) malloc(0); }
	int length = fileLength(path);

	char ***str = (char ***) malloc(length);
	char **groups = (char **) malloc(length);
	char *s = (char *) malloc(length);
	char ch;
	int n = 0, num = 0, i;

	//Read from file
	while (!feof(file))
	{
		ch = fgetc(file);
		if (ch != '\n' && ch != '\t' && ch != ' ' && (int)ch > -1 && (int)ch < 127 && ch != '{' && ch != '}')
		{
			if (ch == '\"')
			{
				// Add to s
				ch = fgetc(file);
				i = 0;
				while (ch != '\"')
				{
					s[i] = ch;
					i++;
					ch = fgetc(file);
				}

				// Add s to groups
				s[i] = '\0';
				groups[n] = s;
				s  = (char *) malloc(length);
				n++;

				// Add group to str
				if(n > 1)
				{ 
					str[num] = groups;
					groups = (char **) malloc(length);
					n = 0;
					num++;
				}
			}
		}
	}
	fclose(file);
	return str;
}

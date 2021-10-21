# extra functions / used with the others
bool fileExsit(char* path)                                              // checks if the file exsists
int fileLength(char* path)							                    // Returns the length of the file
int jsonLength(char* path)							                    // Returns the amout of elements in the json (based on the ":")
void wjsonClear(char* path)							                    // Clears the json
int rJsonFindInd(char* path, char* name)					            // Find the index of a element based on the name

# write to json
void wJsonAppend(char* path, char* name, char* value)				    // Appends to the json
void wJsonInsert(char* path, int index, char* name, char* value)		// Insert into the json
void wJsonDelete(char* path, int index)						            // Deletes a element of the json
void wJsonEdit(char* path, int index, char* new value)				    // Edits a value in the json

# read to json
char*** rJson(char* path)							                    // Reads the json
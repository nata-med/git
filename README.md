The program implements several functions for creating and using your own Git

"init"
"What it accepts: Nothing\n"
"Data handling: 1. Creates a structure: the root directory .vcs/ and .vcs/commits/ to store future files.\n"
"2. Initializes the project index: creates an empty file .vcs/index.\n"
"3. Initializes the state pointer: creates a file .vcs/HEAD and writes the initial text value '0' to it.\n"
        
"add"      
"What it accepts: The filename or relative path to a file in the current directory.\n"
"Data handling: 1. Opens the current index .vcs/index for reading and creates a temporary file .vcs/index.tmp for writing.\n"
"2. In a while loop, reads [status] [filename] pairs from the index. If the passed file was already in the index with the status 'R' (removed), it changes its status to 'A' (added). All remaining lines are copied as is to .vcs/index.tmp.\n"
"3. If the file was not in the index, appends a line of the following format to the end of the temporary file: A filename.\n"
        
"remove"
"What it accepts: The filename (to be marked as removed in the next commit).\n"
"Data handling: 1. Opens the index and creates a temporary index file.\n"
"2. Scans the index. If the file is already listed in the index, it changes its status to 'R' (Removed). If it's not in the index yet (it means that index was cleared after commit), it appends a new line 'R filename'.\n"
"3. Replaces the old index with the updated temporary index.\n"
        
"commit"
"What it accepts: Text message for the commit.\n"
"Data handling: 1. Reads the parent hash from HEAD and generates a new unique hash using the parent hash, message, and current time.\n"
"2. Creates a new directory .vcs/commits/[hash]/ and copies all physical files marked 'A' from the index into it.\n"
"3. Creates metadata.txt with the commit header. Takes unmodified file records from the parent's metadata.txt and appends the new added files.\n"
"4. Clears the index and updates HEAD with the newly generated commit hash.\n"
        
"log"
"What it accepts: Starting commit hash (or reads from HEAD by default) and an integer print limit.\n"
"Data handling: 1. Loops through history starting from the given hash by reading metadata.txt of each commit.\n"
"2. Extracts and prints the commit message, time, and hash to the console.\n"
"3. Shifts the pointer to the parent hash, moving backward in time until it hits the print limit or the initial '0' commit.\n"
        
"diff"
"What it accepts: Nothing\n"
"Data handling: 1. Opens metadata.txt of the target commit and skips the header.\n"
"2. For each file, opens both the saved version in .vcs/commits/ and the physical file in the current working directory.\n"
"3. Compares them line by line and prints the differences (+ for added lines, - for removed lines) to the console.\n"
        
"status"
"What it accepts: Nothing.\n"
"Data handling: 1. Opens the .vcs/index file for reading.\n"
"2. Reads each line and prints 'Added [filename]' if the status is 'A', or 'Removed [filename]' if the status is 'R'.\n"
        
"checkout"
"What it accepts: The target commit hash to restore.\n"
"Data handling: 1. Opens metadata.txt of the target commit and skips the technical header.\n"
"2. Reads the file list and copies each stored file from .vcs/commits/[hash]/ back to the main working directory, overwriting current files.\n"
"3. Updates HEAD to the target hash and clears the index.\n"
        
"--help" or "help"
"What it accepts: Nothing.\n"
"Data handling: It's a general user manual (func_print_help) explaining what each command does from a user's perspective.\n"

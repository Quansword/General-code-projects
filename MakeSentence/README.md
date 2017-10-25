# MakeSentence Specifications
### Outline:

- Implement the function MakeSentence().  
   - It should create and return a sentence from the passed array of words, adding spaces between the words and a period at the end.  
- It should:  
   1. Determine the space needed to hold the sentence.  
   2. Allocate EXACTLY the amount of memory needed, no extra.  
   3. Build the sentence into the allocated memory from the words.  
   4. Return the string.  
- Modify the sample app to query the user for the words in the sentence, instead of using a fixed set of strings. End the input when the user enters just ENTER.  
- Identify and use assert() where you may need it.

### Requirements:

- You can use malloc() to allocate the needed memory.
- You can not use anything else from C runtime library or STL.  
   - You can not use any built in string functions (std::string, strlen(), strdup, etc).  
- You should allocate exactly how much memory the built string will require, no extra.
- Your program should release all the memory it allocates.

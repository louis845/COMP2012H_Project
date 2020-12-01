# Parser
## Parser modified
* I added the function getInfo(), which returns the internal info of the parser.
* To get previous parse result
* getInfo() will not be called before some parse()
## Strange bug
<pre>
    Parser p{""};
    p.reset_input("[[3,4,4+t][5,6]]");
    p.parse();
    p.reset_input("[[3,4,4+t,],[5,6]]");
    p.parse();
</pre>
* Seems to be an error in the hashmap, in the new variable list.
## Test strange bug in GUI
* Pull and run GUI in debugger
* Type something in ascii text field
* Text field calls parser whenever there is update
* Bug shows up for some input
## If you wonder about async problem
* There is code in GUI which takes care of multithread.
* Parser's values will not be called concurrently
* Means at most one thread will access (read/write/call function) of parser at a time
* Bug probably not due to async problem

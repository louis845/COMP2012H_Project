# Parser
## Strange bug
<pre>
    Parser p{""};
    p.reset_input("[[3,4,4+t][5,6]]");
    p.parse();
    p.reset_input("[[3,4,4+t,],[5,6]]");
    p.parse();
</pre>

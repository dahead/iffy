Small "if this than that" tool in c for Linux/CLI.

The application loads a script in which the conditions are defined as follows:

```
command1=parameter1;command2=parameter2
```

Each line gets parsed where command1 is executed with parameter1 as the first argument.
If command1 gets executed successfully (results with 0), command2 with parameter2 gets
executed as well.

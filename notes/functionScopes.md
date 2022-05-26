We've decided to make the scope of the function contain the parametr list of the list.
This is done so that you can't define a variable in the function scope with the same name as the parameter.

Implementation ?

```
 /* Parameter list for function definition ( parameter_list ) = ( int x, int y)*/
function_declaration: INT_DECLARATION ID {create_new_scope} '(' parameter_list ')' {decrement currentScope Variable} block  
parameter_list: parameter |
                parameter_list ',' parameter|

parameter: INT_DECLARATION ID {add_parameter_to_current_scope}

```
**N.B.** Now multiple

```
def create_scope():
    global currentScope, scopeCount
    global scopeMap

    if (scopeMap[currentScope + 1] == None):
        # scope doesn't exist
        create a new scope
    else:
        # if scope already exists
        currentScope += 1


```

```
block : '{' {create_scope} stmt_list  {end_scope}'}' |
        '{' '}'

def end_scope():
    global currentScope, scopeCount
    global scopeMap
    if (globalScope):
        return
    currentScope = scopeMap[currentScope].parent

```


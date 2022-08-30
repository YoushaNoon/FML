# FML
![FML logo](./Logo.png "FML logo")
### A simple but powerful HTML preprocessor

# Getting Started
1. Make a file called `test.fml`

	```
	div
  	    h1
	        "Hi!"
	```

2. Make a cpp file in the same directory that includes the FML header

        #include <iostream>
        #include <vector>
        #include "FML/main.h"
        
        #include <fstream>
        #include <streambuf>
        
        std::ifstream t("test.fml");
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        
        int main() {
            FML::parse(&str);
            FML::gen("generated.html");
            std::cin.get();
            return 0;
        }
3. Compile and then open the file called `generated.html` (Or whatever you named it) and observe the output:

	`<div><h1>Hi!</h1></div>`

Shortcuts:
- [Nesting by identation](#nesting-by-identation)
- [Adding content to elements](#adding-content-to-elements)
- [Shortcuts for the class and id attribute](shortcuts-for-the-class-and-id-attribute)

# Docs
## Nesting by Identation
    div
        img src="myimage.png" width=1280 height=720
becomes:

    <div>
        <img src="myimage.png" width=1280 height=720/>
    </div>

and

    div
        object src="1.png"
        object src="2.png"
        
becomes:

    <div>
        <object src="1.png"/>
        <object src="2.png"/>
    </div>

## Adding content to elements
    h1
        "Testing"
        123
        
becomes:

    <h1>
        Testing 123
    </h1>
    
and if you remove the quotes, `FML` will think `Testing` is an element.
but for numbers this rule does not apply

## Shortcuts for the class and id attribute
    div .blue
        div #green
    div .blue
        div #red

becomes:

    <div class="blue">
        <div id="green"/>
    </div>
    <div class="blue">
        <div id="red"/>
    </div>

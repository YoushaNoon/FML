#pragma once
#ifndef fmlmain_h
#define fmlmain_h

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// The Namespace
namespace FML
{
    // Identifiers enum
    enum IDEN {
        NAME,
        CLASS,
        ID,
        NUMBER,
        STRING,
        EQ,
        INDENT
    };

    // Used for representing a token
    struct Token
    {
        std::string val;
        IDEN ident;
        int line, charn;

        Token(std::string val, IDEN ident, int line, int charn) {
            this->val=val;
            this->ident = ident;
            this->line=line;
            this->charn=charn;
        }
    };

    // Used for representing an attribute
    struct Attribute 
    {
        std::string name;
        std::string val;

        Attribute(std::string name, std::string val) 
        {
            this->name = name;
            this->val = val;
        }
    };

    // Used for representing a node with the ability to bear children
    struct Node
    {
        std::string elname;
        std::vector<Attribute> attributes;
        std::vector<Node*> nodes;
        std::string content;

        Node(std::string elname) {
            this->elname = elname;
        }

        int gen = 0;
        void print() {
            std::cout << elname;
            for (Attribute attribute : attributes) {
                std::cout << " ";
                std::cout << attribute.name << "=" << attribute.val;
            }

            std::cout << std::endl;

            for (Node* node : nodes) {
                for (int i = 0; i < gen+1; i++) {
                    std::cout << " ";
                }
                node->gen=gen+1;
                node->print();
            }
        }

        void html(std::string* str) {
            // Adding Attributes
            *str += "<" + elname;
            for (Attribute attribute : attributes) {
                *str += " " + attribute.name + "=" + attribute.val;
            }

            // Adding a backslash before the left angle bracket if there is no content and there are no children
            if (content.empty() && nodes.empty()) {
                *str+="/";
            }

            *str += ">";

            // If there is content add it
            if (!content.empty()) {
                *str += content;
            }

            // Adding child nodes
            for (Node* sub_node : nodes) {
                sub_node->html(str);
            }   

            // Adding the closing tag
            if (!content.empty() || !nodes.empty()) {
                *str += "</" + elname + ">";
            }
        }
    };

    std::vector<Token> tokens;
    std::vector<Node*> nodes;

    bool rit(std::vector<Token>* tokens, int i, IDEN ident) {
        return i < tokens->size() && tokens->at(i).ident == ident;
    }

    void parse(std::string* str)
    {
        // -- Lexing:
        // Make a string for storing the node name
        std::string curtok = "";

        // Store the current line num
        int line = 0;

        // Loop over all characters
        for (size_t i = 0; i < str->length(); i++)
        {
            // If the character is a line break
            if (str->at(i) == '\n') 
            {
                line++;
            }
            // If the charachter is a space
            else if (str->at(i) == ' ') 
            {
                size_t count = 0;

                for (size_t k = i; k < str->length() && str->at(k) == ' '; k++) {
                    count++;
                    if (count == 4) {
                        tokens.push_back(Token("    ", INDENT, line, k));
                        i=k;
                        count=0;
                    }
                }
            }
            // If the charachter is a dot
            else if (str->at(i) == '.') 
            {
                tokens.push_back(Token(".", CLASS, line, i));
            }
            // If the charachter is a hash
            else if (str->at(i) == '#') 
            {
                tokens.push_back(Token("#", ID, line, i));
            }
            // If the charachter is an equal sign
            else if (str->at(i) == '=') 
            {
                tokens.push_back(Token("=", EQ, line, i));
                // i-=1;
            }
            // If the character is a quote
            else if (str->at(i) == '"') 
            {
                for (size_t k = i+1; k < str->size() && str->at(k) != '"'; k++)
                {
                    curtok+=str->at(k);
                }

                tokens.push_back(Token(curtok, STRING, line, i));
                i+=curtok.size()+2;
                curtok="";
            }
            // If the character is a letter
            else if (std::isalpha(str->at(i)))
            {
                for (size_t k = i; k < str->length() && str->at(k) != ' ' && (std::isalpha(str->at(k)) || std::isdigit(str->at(k))); k++)
                {
                    curtok+=str->at(k);
                }

                tokens.push_back(Token(curtok, NAME, line, i));
                i+=curtok.size()-1;
                curtok="";
            }
            // If the character is a digit
            else if (std::isdigit(str->at(i))) 
            {
                for (size_t k = i; k < str->length() && std::isdigit((str->at(k))); k++)
                {
                    curtok+=str->at(k);
                }

                tokens.push_back(Token(curtok, NUMBER, line, i));
                i+=curtok.size()-1;
                curtok="";
            }
            else
            {
                //std::cout << "unknown: {" << i << "}" << std::endl;
            }
        }

        // -- Parsing:
        std::vector<Node*> parents;
        parents.resize(1);

        size_t incount = 0;
        Node* curnode;

        for (int i = 0; i < tokens.size(); i++) {
            if (rit(&tokens, i, NAME) && rit(&tokens, i+1, EQ) && (rit(&tokens, i+2, STRING) | rit(&tokens, i+2, NUMBER)))
            {
                parents.at(incount)->attributes.push_back(Attribute(tokens.at(i).val, tokens.at(i+2).val));
                i+=2;
                incount = 0;
            }
            else if (rit(&tokens, i, INDENT))
            {
                incount++;
            }
            else if (rit(&tokens, i, NAME))
            {
                Node* node = new Node(tokens.at(i).val);
                curnode=node;

                if (incount == 0) 
                {
                    parents.at(0) = node;
                    nodes.push_back(node);
                }
                else 
                {
                    if (incount+1 > parents.size()) {
                        parents.resize(incount+1);
                    }

                    parents.at(incount) = node;
                    parents.at(incount-1)->nodes.push_back(node);
                }

                incount = 0;
            }
            else if (rit(&tokens, i, CLASS) && rit(&tokens, i+1, NAME)) 
            {
                curnode->attributes.push_back(Attribute("class", tokens.at(i+1).val));
                i++;
                incount = 0;
            }
            else if (rit(&tokens, i, ID) && rit(&tokens, i+1, NAME)) 
            {
                curnode->attributes.push_back(Attribute("id", tokens.at(i+1).val));
                i++;
                incount = 0;
            }
            else if (rit(&tokens, i, STRING) || rit(&tokens, i, NAME)) 
            {
                parents.at(incount-1)->content+=tokens.at(i).val;

                incount = 0;
            }
        }
    }

    std::string gen_string() {
        std::string str;
        for (Node* node : nodes) {
            node->html(&str);
        }
        return str;
    }

    void gen(std::string file_name) {
        std::ofstream file;
        file.open(file_name);
        file << gen_string();
        file.close();
    }
};

#endif
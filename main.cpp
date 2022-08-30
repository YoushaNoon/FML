#include <iostream>
#include <vector>
#include "FML/main.h"

#include <fstream>
#include <streambuf>

std::ifstream t("syntax.fml");
std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

int main() {
    // Lexing
    FML::parse(&str);
    std::cout << FML::gen_string();
    std::cin.get();
    return 0;
}
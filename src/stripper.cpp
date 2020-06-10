/**
 * stripper utility to strip code files of comments or other
 * unwanted syntax.  Uses docopts as a dependency.
 *
 * Author: Todd Saharchuk
 * License: MIT
 *
 * Copyright 2020 (c) Todd Saharchuk
 */
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <variant>
#include <iostream>
#include "docopt/docopt.h"

constexpr auto VERSION = "stripper 0.1.0 (c) Todd Saharchuk, 2020.";
constexpr auto USAGE = 
R"(stripper.

     Usage:
       stripper [options] INFILE OUTFILE

       Read INFILE, strip comments and other syntax object as determined
         by options and write to OUTFILE.

     Options:
       -h --help                      Show this screen
       --version                      Show version
       -c, --comments                 Strip comments.  Can be comments in the set
                                        {('/*','*/'), ('<!--','-->'), ('//'), 
                                         ('#'), ('--')}
       --shebang                      Keep first line if '#!'
)";


constexpr auto WHITESPACE = " \r\n\t\f\v";

std::string ltrim(const std::string& str) {
    std::size_t start = str.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : str.substr(start);
}

bool isStartComment(const std::string& line) {
    return ((line.at(0) == '/' && line.at(1) == '*') ||
            (line.at(0) == '<' && line.at(1) == '!' &&
             line.at(2) == '-' && line.at(3) == '-') ||
            (line.at(0) == '#') ||
            (line.at(0) == '-' && line.at(1) == '-') );
}

std::size_t startCommentPos(const std::string& str) {
    auto pos = str.find("/*");
    if(pos == std::string::npos) {
        pos = str.find("<!--");
    }
    return pos;
}

std::size_t endCommentPos(const std::string& str) {
    auto pos = str.rfind("*/");
    if(pos == std::string::npos) {
        pos = str.rfind("-->");
    }
    return pos;
}

std::optional<std::string> removeComment(const std::string str)
{
    std::string out;
    std::string trimmedStr = ltrim(str);

    // Algorithm assumes either /*...*/ or <!--...--> exlusively
    if(trimmedStr.find("#") == 0) return {};
    if(trimmedStr.find("--") == 0) return {};
    
    auto start = startCommentPos(str);
    auto end = endCommentPos(str);
    if(end > start) {
        return {};
    }
    else if(start != std::string::npos) {
        return str.substr(0,str.size()-start);
    }
    else
        return str;
}

int main(int argc, char** argv)
{
    std::map<std::string, docopt::value> args =
        docopt::docopt(USAGE,
                       {argv + 1, argv + argc},
                       true,
                       VERSION);

    // Open input file
    auto inFilename = args["INFILE"].asString();
    std::fstream inFile(inFilename, std::ios_base::in);
    std::ofstream outFile(args["OUTFILE"].asString(), std::ios_base::out |
                         std::ios_base::trunc);
    
    std::string line;
    std::vector<std::string> stck;
    bool comment = false;
    while(std::getline(inFile,line)) {
        auto outLine = removeComment(line);
        if(outLine != std::nullopt) {
            if(outLine.value().length() != line.length()) {
                outFile << outLine.value() << '\n';
                comment = true;
            }
            else if(!comment) {
                outFile << outLine.value() << '\n';
            }
        }
    }
             
    if(inFile.is_open()) inFile.close();

    return EXIT_SUCCESS;
}


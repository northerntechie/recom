/**
 * recom - GTest unit testing file
 *
 * recom (Remove Comments) utility to strip code files of comments or other
 * unwanted syntax.  Uses docopts as a dependency.
 *
 * Author: Todd Saharchuk
 * License: MIT
 *
 * Copyright 2020 (c) Todd Saharchuk
 */
#define __XUNIT_TESTS_ENABLED
#include "recom.cpp"
#include <gtest/gtest.h>

bool False = false;
bool True = true;

TEST(fnRemoveComment, noComments) {
    ASSERT_EQ("Line 1", removeComment("Line 1",False));
    ASSERT_EQ("Line 2-3", removeComment("Line 2-3",False));
    ASSERT_EQ("", removeComment("",False));
}

TEST(fnRemoveComment, oneLineComments) {
    ASSERT_EQ(std::nullopt, removeComment("/* comment */",False));
    ASSERT_EQ(std::nullopt, removeComment("    /* comment */",False));
    ASSERT_EQ(std::nullopt, removeComment("    /* comment */    ",False));
    ASSERT_EQ(std::nullopt, removeComment("/**/",False));
}

TEST(fnRemoveComment, commentRegion) {
    ASSERT_EQ(std::nullopt, removeComment("Line 1",True));
}

//TEST(fnRemoveComment, enclosedStartComments) {
//    ASSERT_EQ(std::nullopt, removeComment("Line 1 // Comment",False));
//}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

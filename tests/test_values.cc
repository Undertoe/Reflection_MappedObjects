#include "gtest/gtest.h"
#include "error.hh"

#include <string>
#include <vector>
#include <chrono>

#include "reflected_value.hh"


std::vector<std::string> str_split(std::string str, const std::string &del = " ")
{
    size_t loc = 0;
    size_t prevLoc = 0;
    std::vector<std::string> retval;
    std::string push;
    while(loc != std::string::npos)
    {
        std::cout << prevLoc << std::endl;
        loc = str.find(del, prevLoc);
        if(loc != std::string::npos)
        {
            push = str.substr(prevLoc, loc);
        }
        else
        {
            push = str.substr(prevLoc, str.length() - 1);
        }
        retval.push_back(push);
        prevLoc = loc;
    }

    return retval;
}


TEST(ReferencedType, str_split)
{
    std::string test = "0.0 0.0 0.0 0.0";
//    auto split = str_split(test, " ");
//    std::cout << split.size() << std::endl;
}



TEST(ReferencedType, Complex)
{
    std::complex<float> val(0.0, 1.0);

    Terryn::ReferencedType type;
    type.Set(&val);

    std::cout << type.Get() << std::endl;



    type.ExternalAssign("2.0 3.0");
    std::cout << type.Get() << std::endl;
    std::cout << std::to_string(val.real()) << " " << std::to_string(val.imag()) << std::endl;


    type.ExternalAssign("5 4");
    std::cout << type.Get() << std::endl;
    std::cout << std::to_string(val.real()) << " " << std::to_string(val.imag()) << std::endl;
}


TEST(ReferencedType, boolean)
{
    bool myVal = true;

    Terryn::ReferencedType type;
    type.Set(&myVal);

    std::cout << type.Get() << std::endl;

    ASSERT_EQ(type.Get(), "true");

    type.ExternalAssign("false");

    ASSERT_EQ(type.Get(), "false");
    ASSERT_EQ(myVal, false);
    type.ExternalAssign("t");
    ASSERT_EQ(type.Get(), "true");
    ASSERT_EQ(myVal, true);

    type.ExternalAssign("z");
    ASSERT_EQ(type.Get(), "false");

    ASSERT_FALSE(myVal);
}


TEST(ReferencedType, arrays)
{
    uint32_t vals[3] = {1, 2, 3};
    std::cout << std::boolalpha << std::is_array_v<decltype (vals)> << std::endl;
    std::cout << std::extent_v<decltype (vals)> << std::endl;
    Terryn::ReferencedType type;
//    std::cout << "Test" << std::endl;

    type.Set(vals);

//    type.TMP(vals);

    auto str = type.Get();
    std::cout << str << std::endl;

}


TEST(ReferencedType, stdarray)
{
    std::array<uint32_t, 3> vals = {1, 2, 3};
    std::cout << std::boolalpha << std::is_array_v<decltype (vals)> << std::endl;
    std::cout << std::extent_v<decltype (vals)> << std::endl;
    Terryn::ReferencedType type;
//    std::cout << "Test" << std::endl;

    type.Set(&vals);
    std::cout << "Test" << std::endl;


//    type.TMP(vals);

    auto str = type.Get();
    std::cout << str << std::endl;

}

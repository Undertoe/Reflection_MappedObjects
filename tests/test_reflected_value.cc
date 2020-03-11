#include "gtest/gtest.h"
#include "error.hh"

#include <chrono>

//#include "reflected_value.hh"
#include "reflected_functions.hh"
#include "reflected_layered_object.hh"

#include <cmath>

struct Point
{
    float x = 1.1f;
    float y = 2.2f;
    uint32_t value = 0u;
    int32_t intVal = -1;
    bool myBool = false;

    int Test1(const std::string &args)
    {
        std::cout << "Testing with args: " << args << std::endl;
        return 0;
    }

    int Test2(const std::string &args)
    {
        std::cout << "Magnitude is: " << std::sqrt((x * x) + (y * y)) << std::endl;
        std::cout << "\tArgs are: " << args << std::endl;
        return args.size();
    }
};

struct NewPoint
{
    double x = 0.0;
    double y = 1.1;
    double z = 2.2;
};

REFL_AUTO(
    type(Point),
    field(x),
    field(y),
    field(value),
    field(intVal),
    field(myBool),
    func(Test1),
    func(Test2)
)

REFL_AUTO(
    type(NewPoint),
    field(x),
    field(y),
    field(z)
)

using PointFunctionPointers = FunctionPointer<Point>;


TEST(ReflectedValue, Constructor)
{
    using namespace std::chrono;
    using clock = std::chrono::steady_clock;
    using namespace std::chrono_literals;
    using tp = clock::time_point;

    Point p;
    Terryn::ReflectedObject r(&p);


    std::cout << "Size: " << r.Size();
    ASSERT_TRUE(r.Size() == 5);

    ASSERT_EQ(r.Get("x"), std::to_string(p.x));
    p.x = 2.3f;
    ASSERT_EQ(r.Get("x"), std::to_string(p.x));

    NewPoint newPoint;
    Terryn::ReflectedObject newRefl(&newPoint);

    std::vector<std::string> Keys = {"x", "y", "z"};

    for(const auto & key : Keys)
    {
        std::cout << key << "'s value is: " << newRefl.Get(key) << std::endl;
    }


    std::cout << "Setting x to 23.456" << std::endl;

    auto startSet = clock::now();
    newRefl.Set("x", "23.456");
    auto endSet = clock::now();

    auto startGet = clock::now();
    auto get = newRefl.Get("x");
    auto endGet = clock::now();

    std::cout << "x's value: " << newRefl.Get("x") << std::endl;
    std::cout << "Checking raw value: " << newPoint.x << std::endl;

    std::cout << "Duration for set: " << duration_cast<nanoseconds>(endSet - startSet).count()
              << std::endl;
    std::cout << "Duration of get: " << duration_cast<nanoseconds>(endGet - startGet).count()
              << std::endl;


    ASSERT_EQ(r.Get("myBool"), "false");
    r.Set("myBool", "true");
    ASSERT_TRUE(p.myBool);
    r.Set("myBool", "false");
    ASSERT_FALSE(p.myBool);
    r.Set("myBool", "t");
    ASSERT_TRUE(p.myBool);

}


TEST(ReflectedFunctions, test)
{
    using namespace std::chrono;
    using clock = std::chrono::steady_clock;
    using namespace std::chrono_literals;
    using tp = clock::time_point;

    Point p;

    PointFunctionPointers funcs(&p);

    std::cout << funcs.Keys().size() << std::endl;
    std::cout << funcs.Size() << std::endl;

    ASSERT_TRUE(funcs.Keys().size() == 2);

    for(const auto & key : funcs.Keys())
    {
        auto start = clock::now();
        funcs.Call(key, "Hello Reflection!");
        auto ende = clock::now();


        std::cout << "Duration for call: " << duration_cast<nanoseconds>(ende - start).count()
                  << std::endl;
    }
}

//struct ArrayStruct
//{
//    uint32_t fiveVals[5] = {1, 2, 3, 4, 5};
//    float threeFloats[3] = {1.1f, 2.2f, 3.3f};
//    double fourDoubles[4] = {4.4, 5.5, 6.6, 7.7};
//};

struct STDArrayStruct
{
    std::array<uint32_t, 5> fiveVals = {1, 2, 3, 4, 5};
    std::array<float, 3> threeFloats = {1.1f, 2.2f, 3.3f};
    std::array<double, 4> fourDoubles = {4.4, 5.5, 6.6, 7.7};
//    double fourDoubles[4] = {4.4, 5.5, 6.6};
};


REFL_AUTO(
    type(STDArrayStruct),
    field(fiveVals),
    field(threeFloats),
    field(fourDoubles)
)


/// will remain unsupported until we can figure out how to add them into refl.hpp
TEST(ReflectedValue, stl_array)
{
    STDArrayStruct myArrays;
    Terryn::ReflectedObject reflArrays(&myArrays);

    std::cout << "KeyCount: " << reflArrays.Keys().size() << std::endl;

    for(const auto & key : reflArrays.Keys())
    {
        auto values = reflArrays.Get(key);
        std::cout << key << ": " << values << std::endl;
    }

    std::cout << "Setting index 2 to 3.5" << std::endl;
    reflArrays.Set("fourDoubles", "[2]=3.5");
    std::cout << myArrays.fourDoubles[2] << std::endl;
    uint32_t index = 0;
    for(auto & v : myArrays.fourDoubles)
    {
        std::cout << "[" << index++ << "]: " << v << std::endl;
    }


    std::cout << "Checking Three Floats Before Set" << std::endl;
    index = 0;
    for(auto & v : myArrays.threeFloats)
    {
        std::cout << "[" << index++ << "]: " << v << std::endl;
    }
    reflArrays.Set("threeFloats", "[0]=5.5");
    std::cout << "Checking Three Floats after set" << std::endl;
    index = 0;
    for(auto & v : myArrays.threeFloats)
    {
        std::cout << "[" << index++ << "]: " << v << std::endl;
    }
}


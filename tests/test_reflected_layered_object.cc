#include "gtest/gtest.h"
#include "error.hh"

#include <chrono>
#include <complex>

#include "reflected_layered_object.hh"


struct StringTest
{
    std::string v1 = "asdf";
    uint64_t v2 = 0xa;
    uint32_t vSmall = 0xa;
    std::string v3 = "fdsa";
};

REFL_AUTO(
        type(StringTest),
        field(v1),
        field(v2),
        field(v3)
        )

struct PointTwo
{
    float x = 1.1f;
    float y = 2.2f;
    uint32_t value = 0u;
    int32_t intVal = -1;
//    std::complex<float> z = std::complex<float>(9.9f, 11.11f);

    constexpr PointTwo() = default;

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
//    std::complex<double> w = std::complex<double>(3.3, 4.4);

    constexpr NewPoint() = default;
};

REFL_AUTO(
        type(PointTwo),
        field(x),
        field(y),
        field(value),
        field(intVal),
//        field(z),
        func(Test1),
        func(Test2)
        )

REFL_AUTO(
        type(NewPoint),
        field(x),
        field(y),
        field(z)/*,
        field(w)*/
        )

struct CombinedPoint
{
    PointTwo point;
    NewPoint newPoint;
};

REFL_AUTO(
        type(CombinedPoint),
        field(point),
        field(newPoint)
        )


union SharedPoint
{
    PointTwo point;
    NewPoint newPoint;

    SharedPoint()
    {

    }
};

REFL_AUTO(
        type(SharedPoint),
        field(point),
        field(newPoint)
        )

struct ABunchOfInt32s
{
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    uint32_t e;
    uint32_t f;
    uint32_t g;
    uint32_t h;

    ABunchOfInt32s()
    {

    }
};


REFL_AUTO(
        type(ABunchOfInt32s),
        field(a),
        field(b),
        field(c),
        field(d),
        field(e),
        field(f),
        field(g),
        field(h)
        )

struct ABunchOfInt64s
{
    uint64_t one = 1;
    uint64_t two = 2;
    uint64_t three = 3;
    uint64_t four = 4;

    ABunchOfInt64s()
    {

    }
};


REFL_AUTO(
        type(ABunchOfInt64s),
        field(one),
        field(two),
        field(three),
        field(four)
        )

union IntsUnion
{
    ABunchOfInt32s thirtyTwos;
    ABunchOfInt64s sixtyFours;

    IntsUnion()
    {

    }
};


REFL_AUTO(
        type(IntsUnion),
        field(thirtyTwos),
        field(sixtyFours)
        )

struct AnotherLayer
{
    SharedPoint shared;
    IntsUnion ints;
    CombinedPoint combined;
    uint32_t uid = 0x100;
    double someFloat = 0.0;

    AnotherLayer()
    {

    }
};

REFL_AUTO(
        type(AnotherLayer),
        field(shared),
        field(ints),
        field(combined),
        field(uid),
        field(someFloat)
        )




TEST(ReflectedLayered, Layer1)
{
    std::cout << "Layered by 1" << std::endl;

    CombinedPoint combinedPoint;
    Terryn::ReflectedObject layer1(&combinedPoint);

    std::cout << std::endl << "Keys: " << std::endl;
    auto keys = layer1.Keys();
    for(const auto & k : keys)
    {
        std::cout << k << std::endl;
    }

    combinedPoint.point.x = 5.5f;
    std::cout << layer1.Get("point.x") << std::endl;
    layer1.Set("point.y", "33");
    std::cout << "Verifying point.y value: " << combinedPoint.point.y << std::endl;

    combinedPoint.newPoint.x = 100.0;
    std::cout << "newPoint.x: " << layer1.Get("newPoint.x") << std::endl;

    layer1.Set("newPoint.y", "250.0");
    std::cout << "newPoint.y: " << combinedPoint.newPoint.y << std::endl;

//    ASSERT_EQ(layer1.Get("point.x"), "5.5");

}

TEST(ReflectedLayered, Start)
{

    using namespace std::chrono;
    using clock = std::chrono::steady_clock;
    using namespace std::chrono_literals;
    using tp = clock::time_point;

    PointTwo p;
    Terryn::ReflectedObject r(&p);


    std::cout << "Layered Reflection Test" << std::endl;
    std::cout << "Size: " << r.Size() << std::endl;
    ASSERT_TRUE(r.Size() == 4);

    std::cout << std::endl << "Keys: " << std::endl;
    auto keys = r.Keys();
    for(const auto & k : keys)
    {
        std::cout << k << std::endl;
    }

}


TEST(ReflectedLayered, AnotherLayer)
{
    std::cout << "Layered by 2" << std::endl;
    AnotherLayer layer;
    Terryn::ReflectedObject reflection(&layer);


    std::cout << std::endl << "Keys: " << std::endl;
    auto keys = reflection.Keys();
    for(const auto & k : keys)
    {
        std::cout << k << std::endl;
        std::cout << "Current values: " << reflection.Get(k) << std::endl;
    }


    std::cout << "\n\n\nVerifying Data" << std::endl;
    std::cout << "UID: " << reflection.Get("uid") << std::endl;
    layer.uid = 0x200;
    std::cout << "UID after change: " << reflection.Get("uid") << std::endl;


    std::cout << "combined.point.intVal: " << layer.combined.point.intVal << std::endl;
    std::cout << "Getting combined.point.intVal: " << reflection.Get("combined.point.intVal") << std::endl;
    reflection.Set("combined.point.intVal", "250");
    std::cout << "combined.point.intVal: " << layer.combined.point.intVal << std::endl;
    std::cout << "Getting combined.point.intVal: " << reflection.Get("combined.point.intVal") << std::endl;


    std::cout << "Some float: " << reflection.Get("someFloat") << std::endl;
    std::cout << "Setting some float to 1337.1337" << std::endl;

    reflection.Set("someFloat", "1337.1337");
    std::cout << "Val raw: " << layer.someFloat << std::endl
              << "Reflected val: " << reflection.Get("someFloat") << std::endl;
    ///
}


TEST(ReflectedLayered, SharedLayer)
{
    AnotherLayer layer;
    Terryn::ReflectedObject reflection(&layer);

    std::cout << "Testing Shared portion of layer" << std::endl;

    std::cout << "shared.point.x: " << layer.shared.point.x << std::endl;
    std::cout << "Getting value: " << reflection.Get("shared.point.x") << std::endl;
    std::cout << "shared.point.y: " << layer.shared.point.y << std::endl;
    std::cout << "Getting value: " << reflection.Get("shared.point.y") << std::endl;
    std::cout << "shared.newPoint.x: " << layer.shared.newPoint.x << std::endl;
    std::cout << "Getting value: " << reflection.Get("shared.newPoint.x") << std::endl;

    std::cout << std::endl << std::endl << "************************************" << std::endl;

    std::cout << "Checking ints.thirtyTwos.a & ints.thirtyTwos.b to ints.sixtyFours.one" << std::endl;
    std::cout << "\tints.thirtyTwos.a: " << layer.ints.thirtyTwos.a << std::endl;
    std::cout << "\tints.thirtyTwos.b: " << layer.ints.thirtyTwos.b << std::endl;
    std::cout << "\tints.sixtyFours.one: " << layer.ints.sixtyFours.one << std::endl;
    std::cout << "Now checking with reflection: " << std::endl;
    std::cout << "\tints.thirtyTwos.a: " << reflection.Get("ints.thirtyTwos.a") << std::endl;
    std::cout << "\tints.thirtyTwos.b: " << reflection.Get("ints.thirtyTwos.b") << std::endl;
    std::cout << "\tints.sixtyFours.one: " << reflection.Get("ints.sixtyFours.one") << std::endl;

    std::cout << "Setting sixtyFours.one to 0x0000100000002000" << std::endl;
    layer.ints.sixtyFours.one = 0x0000100000002000;
    std::cout << "Checking ints.thirtyTwos.a & ints.thirtyTwos.b to ints.sixtyFours.one" << std::endl;
    std::cout << "\tints.thirtyTwos.a: " << std::hex << layer.ints.thirtyTwos.a << std::endl;
    std::cout << "\tints.thirtyTwos.b: " << std::hex << layer.ints.thirtyTwos.b << std::endl;
    std::cout << "\tints.sixtyFours.one: " << std::hex << layer.ints.sixtyFours.one << std::endl;
    std::cout << "Now checking with reflection: " << std::endl;
    std::cout << "\tints.thirtyTwos.a: " << reflection.Get("ints.thirtyTwos.a") << std::endl;
    std::cout << "\tints.thirtyTwos.b: " << reflection.Get("ints.thirtyTwos.b") << std::endl;
    std::cout << "\tints.sixtyFours.one: " << reflection.Get("ints.sixtyFours.one") << std::endl;

    std::cout << "Setting sixtyFours.two to 0x0000300000004000 through reflection" << std::endl;

    std::string value = "ints.sixtyFours.two";
    bool check = reflection.Set(value, "0x0000300000004000");
    if(!check)
    {
        std::cout << "FAILED TO SET: " << value << std::endl;
    }
    ASSERT_TRUE(check);
    std::cout << "Checking ints.thirtyTwos.c & ints.thirtyTwos.d to ints.sixtyFours.two" << std::endl;
    std::cout << "\tints.thirtyTwos.c: " << std::hex << layer.ints.thirtyTwos.c << std::endl;
    std::cout << "\tints.thirtyTwos.d: " << std::hex << layer.ints.thirtyTwos.d << std::endl;
    std::cout << "\tints.sixtyFours.two: " << std::hex << layer.ints.sixtyFours.two << std::endl;
    std::cout << "Now checking with reflection: " << std::endl;
    std::cout << "\tints.thirtyTwos.c: " << reflection.Get("ints.thirtyTwos.c") << std::endl;
    std::cout << "\tints.thirtyTwos.d: " << reflection.Get("ints.thirtyTwos.d") << std::endl;
    std::cout << "\tints.sixtyFours.two: " << reflection.Get("ints.sixtyFours.two") << std::endl;

    std::cout << "Setting thirtyTwos.f to 0x00005000 and thirtyTwos.e to 0x00006000" << std::endl;
    layer.ints.thirtyTwos.e = 0x00006000;
    layer.ints.thirtyTwos.f = 0x00005000;
    std::cout << "Checking ints.thirtyTwos.e & ints.thirtyTwos.f to ints.sixtyFours.three" << std::endl;
    std::cout << "\tints.thirtyTwos.e: " << std::hex << layer.ints.thirtyTwos.e << std::endl;
    std::cout << "\tints.thirtyTwos.f: " << std::hex << layer.ints.thirtyTwos.f << std::endl;
    std::cout << "\tints.sixtyFours.three: " << std::hex << layer.ints.sixtyFours.three << std::endl;
    std::cout << "Now checking with reflection: " << std::endl;
    std::cout << "\tints.thirtyTwos.e: " << reflection.Get("ints.thirtyTwos.e") << std::endl;
    std::cout << "\tints.thirtyTwos.f: " << reflection.Get("ints.thirtyTwos.f") << std::endl;
    std::cout << "\tints.sixtyFours.three: " << reflection.Get("ints.sixtyFours.three") << std::endl;

    std::cout << "Setting sixtyFours.two to 0x0000300000004000 through reflection" << std::endl;

    std::string name = "ints.thirtyTwos.h";
    value = "0x00007000";
    check = reflection.Set(name, value);
    if(!check)
    {
        std::cout << "FAILED TO SET: " << value << std::endl;
    }
    ASSERT_TRUE(check);
    name = "ints.thirtyTwos.g";
    value = "0x00008000";
    check = reflection.Set(name, value);
    if(!check)
    {
        std::cout << "FAILED TO SET: " << value << std::endl;
    }
    ASSERT_TRUE(check);
    std::cout << "Checking ints.thirtyTwos.g & ints.thirtyTwos.h to ints.sixtyFours.four" << std::endl;
    std::cout << "\tints.thirtyTwos.g: " << std::hex << layer.ints.thirtyTwos.g << std::endl;
    std::cout << "\tints.thirtyTwos.h: " << std::hex << layer.ints.thirtyTwos.h << std::endl;
    std::cout << "\tints.sixtyFours.four: " << std::hex << layer.ints.sixtyFours.four << std::endl;
    std::cout << "Now checking with reflection: " << std::endl;
    std::cout << "\tints.thirtyTwos.g: " << reflection.Get("ints.thirtyTwos.g") << std::endl;
    std::cout << "\tints.thirtyTwos.h: " << reflection.Get("ints.thirtyTwos.h") << std::endl;
    std::cout << "\tints.sixtyFours.four: " << reflection.Get("ints.sixtyFours.four") << std::endl;
}

TEST(ReflectedLayered, UnionMemoryVerification)
{
    AnotherLayer layer;
    Terryn::ReflectedObject reflection(&layer);

    auto memoryMap = reflection.AddressMap();

    ASSERT_TRUE(reflection.Set("ints.thirtyTwos.a", "0"));

    ASSERT_EQ(memoryMap["ints.thirtyTwos.a"], memoryMap["ints.sixtyFours.one"]);
    ASSERT_EQ(memoryMap["ints.thirtyTwos.c"], memoryMap["ints.sixtyFours.two"]);
    ASSERT_EQ(memoryMap["ints.thirtyTwos.e"], memoryMap["ints.sixtyFours.three"]);
    ASSERT_EQ(memoryMap["ints.thirtyTwos.g"], memoryMap["ints.sixtyFours.four"]);


    ASSERT_EQ(memoryMap["ints.thirtyTwos.b"] - sizeof(int32_t), memoryMap["ints.sixtyFours.one"]);
    ASSERT_EQ(memoryMap["ints.thirtyTwos.d"] - sizeof(int32_t), memoryMap["ints.sixtyFours.two"]);
    ASSERT_EQ(memoryMap["ints.thirtyTwos.f"] - sizeof(int32_t), memoryMap["ints.sixtyFours.three"]);
    ASSERT_EQ(memoryMap["ints.thirtyTwos.h"] - sizeof(int32_t), memoryMap["ints.sixtyFours.four"]);
}


TEST(ReflectedLayered, StringVerification)
{
    StringTest str;
    Terryn::ReflectedObject reflection(&str);

    auto keys = reflection.Keys();
    ASSERT_TRUE(keys.size() > 0);
    for(const auto & key : keys)
    {
        std::cout << key << ": " << reflection.Get(key) << std::endl;
    }

    auto addresses = reflection.AddressMap();
    for(auto & [key, addr] : addresses)
    {
        std::cout << key << " = " << addr << std::endl;
    }


    std::string newVal = "a cow jumped over the moon";
    bool check = reflection.Set("v1", newVal);
    ASSERT_TRUE(check);
    auto reflStr = reflection.Get("v1");
    auto structStr = str.v1;
    ASSERT_EQ(reflStr, structStr);
    ASSERT_EQ(newVal, reflStr);
}


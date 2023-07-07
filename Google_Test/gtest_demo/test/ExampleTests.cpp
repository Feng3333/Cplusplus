#include <gtest/gtest.h>
#include "../Example.hpp"

/* TEST_1 */
bool F()
{
    return false;
}

TEST(ExampleTests, DemonstrateGTestMacros)
{
    EXPECT_TRUE(true);
    EXPECT_EQ(true, true);
    const bool result = F();
    EXPECT_EQ(true, result) << "F() running failed! ";
}

/* TEST_2 */
TEST(ExampleTests, MAC)
{
    int x = 16;
    int y = 42;
    int sum = 100;
    int oldSum = sum;
    int expectedNewSum = oldSum + x * y;
    EXPECT_EQ(
        expectedNewSum,
        MAC(x, y, sum)
    );
    EXPECT_EQ(
        expectedNewSum,
        sum
    );
};

/* TEST_3 */
TEST(ExampleTests, Square)
{
    int x = 5;
    int expectedSquare = x * x;
    EXPECT_EQ(
        expectedSquare,
        Square(x)
    );
}

/* 进阶版 */
// struct ExampleTests : public ::testing::Test {

//     int* x;

//     int GetX(){
//         return *x;
//     }

//     virtual void SetUp() override
//     {
//         fprintf(stderr, "Starting up!\n");
//         x = new int(42);
//     };

//     virtual void TearDown() override 
//     {
//         fprintf(stderr, "Tearing Down!\n");
//         delete x;
//     };
// };

// TEST_F(ExampleTests, MAC)
// {
//     int y = 16;
//     int sum = 100;
//     int oldSum = sum;
//     int expectedNewSum = oldSum + GetX() * y;
//     EXPECT_EQ(
//         expectedNewSum,
//         MAC(GetX(), y, sum)
//     );
//     EXPECT_EQ(
//         expectedNewSum,
//         sum
//     );
// };

// TEST_F(ExampleTests, Square)
// {
//     int expectedSquare = GetX() * GetX();
//     EXPECT_EQ(
//         expectedSquare,
//         Square(GetX())
//     );
// }



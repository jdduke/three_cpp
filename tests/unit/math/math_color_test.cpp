#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/color.h>

using namespace three;

TEST(math_color_test, constructor) {
    auto c = Color();
    EXPECT_TRUE( c.r );
    EXPECT_TRUE( c.g );
    EXPECT_TRUE( c.b );
}

TEST(math_color_test, copyHex) {
    auto c = Color();
    auto c2 = Color(0xF5FFFA);
    c.copy(c2);
    EXPECT_TRUE(c.getHex() == c2.getHex());
}

//TEST(math_color_test, copyColorString) {
//    auto c = Color();
//    auto c2 = Color('ivory');
//    c.copy(c2);
//    EXPECT_TRUE(c.getHex() == c2.getHex());
//}

TEST(math_color_test, setRGB) {
    auto c = Color();
    c.setRGB(255, 2, 1);
    EXPECT_TRUE( c.r == 255 );
    EXPECT_TRUE( c.g == 2 );
    EXPECT_TRUE( c.b == 1 );
}

TEST(math_color_test, copyGammaToLinear) {
    auto c = Color();
    auto c2 = Color();
    c2.setRGB(2, 4, 8);
    c.copyGammaToLinear(c2);
    EXPECT_TRUE( c.r == 4 );
    EXPECT_TRUE( c.g == 16 );
    EXPECT_TRUE( c.b == 64 );
}

TEST(math_color_test, copyLinearToGamma) {
    auto c = Color();
    auto c2 = Color();
    c2.setRGB(4, 9, 16);
    c.copyLinearToGamma(c2);
    EXPECT_TRUE( c.r == 2 );
    EXPECT_TRUE( c.g == 3 );
    EXPECT_TRUE( c.b == 4 );
}


TEST(math_color_test, convertGammaToLinear) {
    auto c = Color();
    c.setRGB(2, 4, 8);
    c.convertGammaToLinear();
    EXPECT_TRUE( c.r == 4 );
    EXPECT_TRUE( c.g == 16 );
    EXPECT_TRUE( c.b == 64 );
}


TEST(math_color_test, convertLinearToGamma) {
    auto c = Color();
    c.setRGB(4, 9, 16);
    c.convertLinearToGamma();
    EXPECT_TRUE( c.r == 2 );
    EXPECT_TRUE( c.g == 3 );
    EXPECT_TRUE( c.b == 4 );
}

TEST(math_color_test, setWithNum) {
    auto c = Color();
    c.set(0xFF0000);
    EXPECT_TRUE( c.r == 1 );
    EXPECT_TRUE( c.g == 0 );
    EXPECT_TRUE( c.b == 0 );
}


//TEST(math_color_test, setWithString) {
//    auto c = Color();
//    c.set('silver');
//    EXPECT_TRUE(c.getHex() == 0xC0C0C0 );
//}


//TEST(math_color_test, clone) {
//    auto c = Color('teal');
//    auto c2 = c.clone();
//    EXPECT_TRUE(c2.getHex() == 0x008080 );
//}

TEST(math_color_test, lerp) {
    auto c = Color();
    auto c2 = Color();
    c.setRGB(0, 0, 0);
    c.lerp(c2, 2);
    EXPECT_TRUE( c.r == 2 );
    EXPECT_TRUE( c.g == 2 );
    EXPECT_TRUE( c.b == 2 );
    
}


//TEST(math_color_test, setStyleRGBRed) {
//    auto c = Color();
//    c.setStyle('rgb(255,0,0)');
//    EXPECT_TRUE( c.r == 1 );
//    EXPECT_TRUE( c.g == 0 );
//    EXPECT_TRUE( c.b == 0 );
//}
//
//TEST(math_color_test, setStyleRGBRedWithSpaces) {
//    auto c = Color();
//    c.setStyle('rgb(255, 0, 0)');
//    EXPECT_TRUE( c.r == 1 );
//    EXPECT_TRUE( c.g == 0 );
//    EXPECT_TRUE( c.b == 0 );
//}
//
//TEST(math_color_test, setStyleRGBPercent) {
//    auto c = Color();
//    c.setStyle('rgb(100%,50%,10%)');
//    EXPECT_TRUE( c.r == 1 );
//    EXPECT_TRUE( c.g == 0.5 );
//    EXPECT_TRUE( c.b == 0.1 );
//}

//TEST(math_color_test, setStyleRGBPercentWithSpaces) {
//    auto c = Color();
//    c.setStyle('rgb(100%,50%,10%)');
//    EXPECT_TRUE( c.r == 1 );
//    EXPECT_TRUE( c.g == 0.5);
//    EXPECT_TRUE( c.b == 0.1 );
//}
//
//TEST(math_color_test, setStyleHexSkyBlue) {
//    auto c = Color();
//    c.setStyle('#87CEEB');
//    EXPECT_TRUE(c.getHex() == 0x87CEEB );
//}
//
//TEST(math_color_test, setStyleHex2Olive) {
//    auto c = Color();
//    c.setStyle('#F00');
//    EXPECT_TRUE(c.getHex() == 0xFF0000 );
//}
//
//TEST(math_color_test, setStyleColorName) {
//    auto c = Color();
//    c.setStyle('powderblue');
//    EXPECT_TRUE(c.getHex() == 0xB0E0E6 );
//}
//
//
//TEST(math_color_test, getHex) [
//    auto c = Color('red');
//    auto res = c.getHex();
//    EXPECT_TRUE( res == 0xFF0000 );
//}

//TEST(math_color_test, setHex) [
//    auto c = Color();
//    c.setHex(0xFA8072);
//    EXPECT_TRUE( c.getHex() == 0xFA8072 );
//}

//TEST(math_color_test, getHexString) [
//    auto c = Color('tomato');
//    auto res = c.getHexString();
//    EXPECT_TRUE( res == 'ff6347' );
//}
//
//TEST(math_color_test, getStyle) [
//    auto c = Color('plum');
//    auto res = c.getStyle();
//    EXPECT_TRUE( res == 'rgb(221,160,221)' );
//}

TEST(math_color_test, getHSL) {
    auto c = Color( 0x80ffff );
    auto hsl = c.getHSL();

    EXPECT_TRUE( hsl.h == 0.5 );
    EXPECT_TRUE( hsl.s == 1.0 );
    EXPECT_TRUE( (Math::round(hsl.l*100)/100) == 0.75 );
}

TEST(math_color_test, setHSL) {
    auto c = Color();
    c.setHSL(0.75, 1.0, 0.25);
    auto hsl = c.getHSL();

    EXPECT_TRUE( hsl.h == 0.75 );
    EXPECT_TRUE( hsl.s == 1.00 );
    EXPECT_TRUE( hsl.l == 0.25 );
}
//***************************************************************************************
// test.cpp by potoshopDev (C) 2023 All Rights Reserved.
//***************************************************************************************
#include "App.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(CreateWindow, ValidWindow)
{
  HWND hWnd = CreateWindow(TEXT("STATIC"), TEXT("Test Window"), WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, NULL, NULL);
  EXPECT_NE(hWnd, (HWND)NULL);
}
int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

#ifndef __CXX_APP_LIB_TEST_HARNESS_H
#define __CXX_APP_LIB_TEST_HARNESS_H

// CXXAPPLibTestHarness - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include <cxxtest/TestSuite.h>

class CXXAPPLibTestHarness : public CxxTest::TestSuite
{
    public:
        CXXAPPLibTestHarness(void)
        {
        }

        ~CXXAPPLibTestHarness(void) override
        {
        }

        void setUp() override
        {

        }

        void tearDown() override
        {

        }

        void test_1(void)
        {
            TS_ASSERT(true);
        }
};

#endif // __CXX_APP_LIB_TEST_HARNESS_H

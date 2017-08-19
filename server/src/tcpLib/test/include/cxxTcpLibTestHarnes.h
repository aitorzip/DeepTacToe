#ifndef __CXX_TCP_LIB_TEST_HARNESS_H
#define __CXX_TCP_LIB_TEST_HARNESS_H

// CXXTCPLibTestHarness - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include <cxxtest/TestSuite.h>

class CXXTCPLibTestHarness : public CxxTest::TestSuite
{
    public:
        CXXTCPLibTestHarness(void)
        {
        }

        ~CXXTCPLibTestHarness(void) override
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

#endif // __CXX_TCP_LIB_TEST_HARNESS_H

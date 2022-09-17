#include <gtest/gtest.h>

#include "config.h"

#include "gpsdata/types/ObjectId.hpp"

using gpsdata::types::ObjectId;

TEST(ObjectIdTest, ZeroTest)
{
	ObjectId t;
	EXPECT_FALSE (t);
}

TEST(ObjectIdTest, InitZeroTest)
{
	ObjectId t = 0;
	EXPECT_FALSE (t);
}

TEST(ObjectIdTest, Init1Test)
{
	ObjectId t = 10;
	ASSERT_TRUE (t);
	EXPECT_EQ (t, 10);
}

TEST(ObjectIdTest, Init2Test)
{
	ObjectId t = 2192476;
	ASSERT_TRUE (t);
	EXPECT_EQ (t, 2192476);
}

TEST(ObjectIdTest, ConstructorZeroTest)
{
	ObjectId t(0);
	EXPECT_FALSE (t);
}

TEST(ObjectIdTest, Constructor1Test)
{
	ObjectId t(10);
	ASSERT_TRUE (t);
	EXPECT_EQ (t, 10);
}

TEST(ObjectIdTest, Constructor2Test)
{
	ObjectId t(2192476);
	ASSERT_TRUE (t);
	EXPECT_EQ (t, 2192476);
}

TEST(ObjectIdTest, AssignZeroTest)
{
	ObjectId t;
	t = 0;
	EXPECT_FALSE (t);
}

TEST(ObjectIdTest, Assign1Test)
{
	ObjectId t;
	t = 10;
	ASSERT_TRUE (t);
	EXPECT_EQ (t, 10);
}

TEST(ObjectIdTest, Assign2Test)
{
	ObjectId t;
	t = 2192476;
	ASSERT_TRUE (t);
	EXPECT_EQ (t, 2192476);
}

TEST(ObjectIdTest, CopyConstructorTest)
{
	ObjectId t1(2192476);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1, 2192476);
	ObjectId t2(t1);
	EXPECT_TRUE (t1);
	EXPECT_EQ (t1, 2192476);
	EXPECT_TRUE (t2);
	EXPECT_EQ (t2, 2192476);
}

TEST(ObjectIdTest, MoveConstructorTest)
{
	ObjectId t1(2192476);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1, 2192476);

	ObjectId t2(std::move (t1));

	EXPECT_FALSE (t1);
	EXPECT_TRUE (t2);
	EXPECT_EQ (t2, 2192476);
}

TEST(ObjectIdTest, CopyAssignment1Test)
{
	ObjectId t1(2192476);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1, 2192476);
	ObjectId t2;
	ASSERT_FALSE (t2);

	t2 = t1;

	EXPECT_TRUE (t2);
	EXPECT_EQ (t2, 2192476);
}

TEST(ObjectIdTest, CopyAssignment2Test)
{
	ObjectId t1(2192476);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1, 2192476);
	ObjectId t2(10);
	ASSERT_TRUE (t2);
	ASSERT_EQ (t2, 10);

	t2 = t1;

	EXPECT_TRUE (t2);
	EXPECT_EQ (t2, 2192476);
}

TEST(ObjectIdTest, MoveAssignment1Test)
{
	ObjectId t1(2192476);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1, 2192476);
	ObjectId t2;
	ASSERT_FALSE (t2);

	t2 = std::move (t1);

	EXPECT_FALSE (t1);
	EXPECT_TRUE (t2);
	EXPECT_EQ (t2, 2192476);
}

TEST(ObjectIdTest, MoveAssignment2Test)
{
	ObjectId t1(2192476);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1, 2192476);
	ObjectId t2(10);
	ASSERT_TRUE (t2);
	ASSERT_EQ (t2, 10);

	t2 = std::move (t1);

	EXPECT_FALSE (t1);
	EXPECT_TRUE (t2);
	EXPECT_EQ (t2, 2192476);
}

TEST(ObjectIdTest, Compare1Test)
{
	ObjectId t1(1024);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1, 1024);

	ObjectId t2(1024);
	ASSERT_TRUE (t2);
	ASSERT_EQ (t2, 1024);

	EXPECT_EQ (t1, t2);
}

TEST(ObjectIdTest, Compare2Test)
{
	ObjectId t1(1024);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1, 1024);

	ObjectId t2(1025);
	ASSERT_TRUE (t2);
	ASSERT_EQ (t2, 1025);

	EXPECT_NE (t1, t2);
}

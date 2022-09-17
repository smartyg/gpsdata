#include <gtest/gtest.h>

#include "config.h"

#include "gpsdata/types/ObjectTime.hpp"

using gpsdata::types::ObjectTime;

TEST(ObjectTimeTest, ZeroTest)
{
	ObjectTime t;
	EXPECT_FALSE (t);
}

TEST(ObjectTimeTest, InitZeroTest)
{
	ObjectTime t = 0l;
	EXPECT_FALSE (t);
}

TEST(ObjectTimeTest, Init1Test)
{
	ObjectTime t = 10l;
	ASSERT_TRUE (t);
	EXPECT_EQ (t.get (), 10l);
}

TEST(ObjectTimeTest, Init2Test)
{
	ObjectTime t = 1662280911901;
	ASSERT_TRUE (t);
	EXPECT_EQ (t.get (), 1662280911901);
}

TEST(ObjectTimeTest, ConstructorZeroTest)
{
	ObjectTime t(0);
	EXPECT_FALSE (t);
}

TEST(ObjectTimeTest, Constructor1Test)
{
	ObjectTime t(10l);
	ASSERT_TRUE (t);
	EXPECT_EQ (t.get (), 10l);
}

TEST(ObjectTimeTest, Constructor2Test)
{
	ObjectTime t(1662280911901);
	ASSERT_TRUE (t);
	EXPECT_EQ (t.get (), 1662280911901);
}

TEST(ObjectTimeTest, AssignZeroTest)
{
	ObjectTime t;
	t = 0l;
	EXPECT_FALSE (t);
}

TEST(ObjectTimeTest, Assign1Test)
{
	ObjectTime t;
	t = 10l;
	ASSERT_TRUE (t);
	EXPECT_EQ (t.get (), 10l);
}

TEST(ObjectTimeTest, Assign2Test)
{
	ObjectTime t;
	t = 1662280911901;
	ASSERT_TRUE (t);
	EXPECT_EQ (t.get (), 1662280911901);
}

TEST(ObjectTimeTest, AssignDate1Test)
{
	ObjectTime t(1970, 1, 1, 0, 0, 0.0);
	EXPECT_FALSE (t);
}

TEST(ObjectTimeTest, AssignDate2Test)
{
	ObjectTime t(1970, 1, 1, 0, 0, 0.001);
	ASSERT_TRUE (t);
	EXPECT_EQ (t.get (), 1);
}

TEST(ObjectTimeTest, AssignDate3Test)
{
	ObjectTime t(2021, 3, 1, 11, 59, 59.123);
	ASSERT_TRUE (t);
	EXPECT_EQ (t.get (), 1614599999123);
}

TEST(ObjectTimeTest, CopyConstructorTest)
{
	ObjectTime t1(1614599999123);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1.get (), 1614599999123);
	ObjectTime t2(t1);
	EXPECT_TRUE (t1);
	EXPECT_EQ (t1.get (), 1614599999123);
	EXPECT_TRUE (t2);
	EXPECT_EQ (t2.get (), 1614599999123);
}

TEST(ObjectTimeTest, MoveConstructorTest)
{
	ObjectTime t1(1614599999123);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1.get (), 1614599999123);

	ObjectTime t2(std::move (t1));

	EXPECT_FALSE (t1);
	EXPECT_TRUE (t2);
	EXPECT_EQ (t2.get (), 1614599999123);
}

TEST(ObjectTimeTest, CopyAssignment1Test)
{
	ObjectTime t1(1614599999123);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1.get (), 1614599999123);
	ObjectTime t2;
	ASSERT_FALSE (t2);

	t2 = t1;

	EXPECT_TRUE (t2);
	EXPECT_EQ (t2.get (), 1614599999123);
}

TEST(ObjectTimeTest, CopyAssignment2Test)
{
	ObjectTime t1(1614599999123);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1.get (), 1614599999123);
	ObjectTime t2(1662280911901);
	ASSERT_TRUE (t2);
	ASSERT_EQ (t2.get (), 1662280911901);

	t2 = t1;

	EXPECT_TRUE (t2);
	EXPECT_EQ (t2.get (), 1614599999123);
}

TEST(ObjectTimeTest, MoveAssignment1Test)
{
	ObjectTime t1(1614599999123);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1.get (), 1614599999123);
	ObjectTime t2;
	ASSERT_FALSE (t2);

	t2 = std::move (t1);

	EXPECT_FALSE (t1);
	EXPECT_TRUE (t2);
	EXPECT_EQ (t2.get (), 1614599999123);
}

TEST(ObjectTimeTest, MoveAssignment2Test)
{
	ObjectTime t1(1614599999123);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1.get (), 1614599999123);
	ObjectTime t2(1662280911901);
	ASSERT_TRUE (t2);
	ASSERT_EQ (t2.get (), 1662280911901);

	t2 = std::move (t1);

	EXPECT_FALSE (t1);
	EXPECT_TRUE (t2);
	EXPECT_EQ (t2.get (), 1614599999123);
}

TEST(ObjectTimeTest, GetDateTest)
{
	ObjectTime t(2021, 3, 1, 11, 58, 59.123);
	ASSERT_TRUE (t);
	ASSERT_EQ (t.get (), 1614599939123);

	EXPECT_EQ (t.getYear (), 2021);
	EXPECT_EQ (t.getMonth (), 3);
	EXPECT_EQ (t.getDay (), 1);
	EXPECT_EQ (t.getHours (), 11);
	EXPECT_EQ (t.getMinutes (), 58);
	EXPECT_EQ (t.getSeconds (), 59);
	EXPECT_EQ (t.getMilliseconds (), 123);
}

TEST(ObjectTimeTest, Conversion1Test)
{
	ObjectTime t(1614599939123);
	ASSERT_TRUE (t);
	ASSERT_EQ (t.get (), 1614599939123);

	ObjectTime::Representation r = t;

	EXPECT_EQ (r.count (), 1614599939123);
}

TEST(ObjectTimeTest, DateAdd1Test)
{
	ObjectTime t(2021, 3, 1, 11, 58, 59.123);
	ASSERT_TRUE (t);
	ASSERT_EQ (t.get (), 1614599939123);

	std::chrono::hours h(1);
	ObjectTime t2 = t + h;
	ObjectTime t3 = h + t;

	EXPECT_EQ (t2.get (), t3.get ());
	EXPECT_EQ (t2.getHours (), 12);
}

TEST(ObjectTimeTest, Compare1Test)
{
	ObjectTime t1(2021, 3, 1, 11, 58, 59.123);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1.get (), 1614599939123);

	ObjectTime t2(2021, 3, 1, 11, 58, 59.123);
	ASSERT_TRUE (t2);
	ASSERT_EQ (t2.get (), 1614599939123);

	EXPECT_EQ (t1, t2);
	EXPECT_LE (t1, t2);
	EXPECT_GE (t1, t2);
}

TEST(ObjectTimeTest, Compare2Test)
{
	ObjectTime t1(2021, 3, 1, 11, 58, 59.123);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1.get (), 1614599939123);

	ObjectTime t2(2021, 3, 1, 11, 58, 59.124);
	ASSERT_TRUE (t2);
	ASSERT_EQ (t2.get (), 1614599939124);

	EXPECT_NE (t1, t2);
	EXPECT_LT (t1, t2);
}

TEST(ObjectTimeTest, Compare3Test)
{
	ObjectTime t1(2021, 3, 2, 11, 58, 59.123);
	ASSERT_TRUE (t1);
	ASSERT_EQ (t1.get (), 1614686339123);

	ObjectTime t2(2021, 3, 1, 11, 58, 59.123);
	ASSERT_TRUE (t2);
	ASSERT_EQ (t2.get (), 1614599939123);

	EXPECT_NE (t1, t2);
	EXPECT_GT (t1, t2);
}

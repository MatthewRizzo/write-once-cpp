#include "gtest/gtest.h"
#include <cstdlib>
#include <ctime>
#include "write_once.hpp"

class WriteOnceTest : public ::testing::Test {
protected:

    virtual void SetUp()
    {
        srand(time(0));
    };

    virtual void TearDown() {};

    WriteOnce<int> m_intContainer;
    WriteOnce<std::vector<float>> m_vectorContainer;
};

struct NonTrivialObject
{
    NonTrivialObject() = delete;
    NonTrivialObject(int foo) :
        m_foo(foo)
    {}

    int m_foo;
};

template<class T>
bool WriteRandomData(int num_writes, WriteOnce<T>& once)
{
    for(int i = 0; i < num_writes; i++)
    {
        T data = rand() * rand();
        // We should never successfully write again
        if(once.Write(data))
        {
            return false;
        }
    }

    return true;
}

TEST_F(WriteOnceTest, BasicMembers)
{
    {
        int val_to_set {5};
    
        EXPECT_DEATH(m_intContainer.Get(), "");
        EXPECT_TRUE(m_intContainer.Write(val_to_set));
        EXPECT_EQ(val_to_set, m_intContainer.Get());
        EXPECT_FALSE(m_intContainer.Write(10));
        EXPECT_TRUE(WriteRandomData(100, m_intContainer));
        EXPECT_EQ(val_to_set, m_intContainer.Get());
    }
    {
        std::vector<float> floats {3.14, 9.81, 2.718};
        std::vector<float> floats2 {5.1, 3.2, 1.9};
        EXPECT_DEATH(m_vectorContainer.Get(), "");
        EXPECT_TRUE(m_vectorContainer.Write(floats));
        EXPECT_EQ(floats[0], m_vectorContainer.Get()[0]);
        EXPECT_EQ(floats[1], m_vectorContainer.Get()[1]);
        EXPECT_EQ(floats[2], m_vectorContainer.Get()[2]);
        EXPECT_FALSE(m_vectorContainer.Write(floats2));
        EXPECT_NE(floats2[0], m_vectorContainer.Get()[0]);
        EXPECT_NE(floats2[1], m_vectorContainer.Get()[1]);
        EXPECT_NE(floats2[2], m_vectorContainer.Get()[2]);
    }

}

TEST_F(WriteOnceTest, InstantiatedWith)
{
    WriteOnce<int> instantiateTogether {7};
    EXPECT_EQ(7, instantiateTogether.Get());
    EXPECT_FALSE(instantiateTogether.Write(9));
    EXPECT_EQ(7, instantiateTogether.Get());
    EXPECT_TRUE(WriteRandomData(100, instantiateTogether));
    EXPECT_EQ(7, instantiateTogether.Get());
}

TEST_F(WriteOnceTest, Test_NonTrivialObject_InstantiatedWith)
{
    WriteOnce<NonTrivialObject> nonTrivialOnce {NonTrivialObject(10)};
    EXPECT_EQ(10, nonTrivialOnce.Get().m_foo);
    EXPECT_FALSE(nonTrivialOnce.Write(70));
    EXPECT_TRUE(WriteRandomData(100, nonTrivialOnce));
    EXPECT_EQ(10, nonTrivialOnce.Get().m_foo);
}

TEST_F(WriteOnceTest, Test_NonTrivialObject_InstantiatedWithout)
{
    WriteOnce<NonTrivialObject> nonTrivialOnceWithout;
    int val_to_set = 150;
    EXPECT_DEATH(nonTrivialOnceWithout.Get(), "");
    EXPECT_TRUE(nonTrivialOnceWithout.Write(val_to_set));
    EXPECT_EQ(val_to_set, nonTrivialOnceWithout.Get().m_foo);
    EXPECT_FALSE(nonTrivialOnceWithout.Write(70));
    EXPECT_TRUE(WriteRandomData(100, nonTrivialOnceWithout));
    EXPECT_EQ(val_to_set, nonTrivialOnceWithout.Get().m_foo);
}

//
// Created by Dmytro Babenko on 2018-12-18.
//

#include <gtest/gtest.h>
#include <algorithm>
#include "my_vector.h"

class my_vector_test : public ::testing::Test {
public:
    my_vector_test() { /* init protected members here */ }
    ~my_vector_test() { /* free protected members here */ }
    void SetUp() { /* called before every test */ }
    void TearDown() { /* called after every test */ }

protected:
    /* none yet */
};



//class for test the next case, where size >= capacity and others
// v.push_back(v[2]), insert, erase
//
class A
{
public:
    A()
    :m_ptr_value(new int(0))
    {
    }

    A(int value)
    {
        m_ptr_value = new int(value);
    }

    A(const A& iOther)
    {
        m_ptr_value = new int(*iOther.m_ptr_value);
    }

    A&operator=(const A& iOther)
    {
        delete m_ptr_value;

        m_ptr_value = new int(*iOther.m_ptr_value);

        return *this;
    }

    int get_value() const
    {
        return *m_ptr_value;
    }

    ~A()
    {
        delete m_ptr_value;
        m_ptr_value = nullptr;
    }

private:
    int* m_ptr_value;
};


TEST_F(my_vector_test, DefualtConstrtuctor)
{
    my_vector<int> v;
    ASSERT_TRUE(v.is_empty());
}

TEST_F(my_vector_test, ConstructorWithSize)
{
    const size_t  n = 10;
    my_vector<int> v(n);
    ASSERT_FALSE(v.is_empty());
    ASSERT_EQ(n, v.size());
}

TEST_F(my_vector_test, ConstructorWithSizeAndValue)
{
    const size_t  n = 10;
    const int value = 15;
    my_vector<int> v(n, value);

    ASSERT_EQ(value, v[1]);
}


TEST_F(my_vector_test, ContructorInitializerList)
{
    my_vector<int> v({1, 2, 3, 4});

    ASSERT_EQ(4, v.size());
    ASSERT_EQ(3, v[2]);
}
TEST_F(my_vector_test, CopyContructor)
{
    my_vector<int> v1(2, 2);
    my_vector<int> v2 = v1;

    ASSERT_EQ(v1, v2);
}

TEST_F(my_vector_test, MoveContructor)
{
    my_vector<int> v1(2, 2);
    my_vector<int> v2 = std::move(v1);

    ASSERT_EQ(2, v2[1]);

    ASSERT_TRUE(v1.is_empty());
}

TEST_F(my_vector_test, CopyAssignment)
{
    my_vector<int> v1 (2, 2);

    my_vector<int> v2;

    ASSERT_NE(v1, v2);

    v2 = v1;

    ASSERT_EQ(v1, v2);

}


TEST_F(my_vector_test, MoveAssignment)
{
    my_vector<int> v1 (2, 2);

    my_vector<int> v2;

    ASSERT_NE(v1, v2);

    v2 = std::move(v1);

    ASSERT_EQ(2, v2[1]);

    ASSERT_TRUE(v1.is_empty());

}


TEST_F(my_vector_test, Reserve)
{
    my_vector<int> v1(2, 3);

    v1.reserve(10);

    ASSERT_EQ(10, v1.capacity());
    ASSERT_EQ(2, v1.size());
    ASSERT_EQ(3, v1[1]);
}

TEST_F(my_vector_test, Index)
{
    my_vector<int> v(2, 3);

    ASSERT_EQ(3, v[0]);
    ASSERT_EQ(3, v[1]);
}

TEST_F(my_vector_test, At)
{
    my_vector<int> v(2, 3);

    ASSERT_EQ(3, v.at(0));
    ASSERT_EQ(3, v.at(1));

}

TEST_F(my_vector_test, AtException)
{
    my_vector<int> v(2, 3);

    ASSERT_THROW(v.at(2), std::out_of_range);
}


TEST_F(my_vector_test, push_backc_capacity)
{
    my_vector<int> v;
    v.push_back(10);
    v.push_back(11);

    ASSERT_EQ(10, v[0]);
    ASSERT_EQ(11, v[1]);
}


TEST_F(my_vector_test, push_back_vector_element)
{
    my_vector<A> v(10, A(10));
    v.push_back(v[2]);

    ASSERT_EQ(10, v[10].get_value());
}


TEST_F(my_vector_test, emplace_back)
{
    my_vector<std::pair<int, int>> v;
    v.push_back(std::make_pair(1, 2));
    v.emplace_back(2, 3);

    ASSERT_EQ(2, v.size());
    std::pair<int, int> p(2, 3);
    ASSERT_EQ(p, v[1]);

}


TEST_F(my_vector_test, pop_back)
{
    my_vector<A> v(5, A(10));
    v.pop_back();
    v.pop_back();
    v.pop_back();
    v.pop_back();
    v.pop_back();

    ASSERT_TRUE(v.is_empty());

}

TEST_F(my_vector_test, shrink_to_fit)
{
    my_vector<A> v(5, A(10));
    v.pop_back();
    v.pop_back();

    ASSERT_EQ(3, v.size());
    ASSERT_EQ(5, v.capacity());

    v.shrink_to_fit();

    ASSERT_EQ(v.size(), v.capacity());

}

TEST_F(my_vector_test, resize_less)
{
    my_vector<A> v(5, A(10));

    ASSERT_EQ(5, v.size());
    v.resize(3);
    ASSERT_EQ(3, v.size());
}



TEST_F(my_vector_test, resize_more)
{
    my_vector<A> v(5, A(10));

    ASSERT_EQ(5, v.size());
    v.resize(8);
    ASSERT_EQ(8, v.size());
}


TEST_F(my_vector_test, insert)
{
    my_vector<A> v(5, A(10));

    auto second_iter = v.begin() + 1;

    auto new_iter = v.insert(second_iter, A(7));

    ASSERT_EQ(7, v[1].get_value());
    ASSERT_EQ(7, new_iter->get_value());

}


TEST_F(my_vector_test, insert_array)
{
    A arr[5] = {1, 2, 3, 4, 5};

    my_vector<A> v(2, 11);

    auto new_iter = v.insert(v.begin() + 1, arr, arr + 5);

    ASSERT_EQ(7, v.size());

    ASSERT_EQ(1, new_iter->get_value());

    ASSERT_EQ(11, v[0].get_value());
    ASSERT_EQ(1, v[1].get_value());
    ASSERT_EQ(2, v[2].get_value());
    ASSERT_EQ(3, v[3].get_value());
    ASSERT_EQ(4, v[4].get_value());
    ASSERT_EQ(5, v[5].get_value());
    ASSERT_EQ(11, v[6].get_value());

}

TEST_F(my_vector_test, erase)
{
    {
        my_vector<A> v(3);
        v[0] = A(1);
        v[1] = A(2);
        v[2] = A(3);

        auto iter = v.erase(v.begin() + 1);

        ASSERT_EQ(2, v.size());
        ASSERT_EQ(v.begin() + 1, iter);
        ASSERT_EQ(3, iter->get_value());
    }
    int  a = 10;
}


TEST_F(my_vector_test, erase_sub)
{
    {
        my_vector<A> v(7);
        v[0] = A(11);
        v[1] = A(1);
        v[2] = A(2);
        v[3] = A(3);
        v[4] = A(4);
        v[5] = A(5);
        v[6] = A(11);

        auto iter = v.erase(v.begin() + 1, v.begin() + 6);

        ASSERT_EQ(2, v.size());
        ASSERT_EQ(v.begin() + 1, iter);
        ASSERT_EQ(11, iter->get_value());

        ASSERT_EQ(11, v.front().get_value());
        ASSERT_EQ(11, v.back().get_value());
    }
}

TEST_F(my_vector_test, swap)
{
    my_vector<int> v1(10, 11);
    my_vector<int> v2(7, 8);

    v1.swap(v2);

    ASSERT_EQ(7, v1.size());
    ASSERT_EQ(8, v1.back());

    ASSERT_EQ(10, v2.size());
    ASSERT_EQ(11, v2.back());
}



TEST_F(my_vector_test, operatorLessGreater)
{
    my_vector<int> v1 = {1, 2, 3};
    my_vector<int> v2 = {2, 3};

    ASSERT_TRUE(v1 < v2);
    ASSERT_TRUE(v2 > v1);

}




TEST_F(my_vector_test, test)
{
    my_vector<int> v(10, 10);


    v[4] = 7;
    v[5] = 4;

    int sum = 0;
    std::for_each(v.cbegin(), v.cend(), [&](int val)
    {
        sum += val;
    });

    int a = 10;
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
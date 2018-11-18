//
// Created by Виталий on 010 10.11.18.
//

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testVector

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/BinaryHeap/heap.h"
#include "../../src/BinaryHeap/vector.h"


TEST(VectorConstructor, VectorSize){
    Vector<int> vector = Vector<int> (5);
    EXPECT_EQ(vector.Size(), 5);
    EXPECT_ANY_THROW(Vector<int> (0));
}

TEST(VectorBasic, VectorNthElement){
    Vector<int> vector = Vector<int> (5);
    vector.SetNthElementValue(2, 10);
    EXPECT_EQ(vector.GetNthElement(2), 10);
}

TEST(VectorBasic, VectorMemoryAccessViolation){
    Vector<int> vector = Vector<int> (5);
    EXPECT_ANY_THROW(vector.GetNthElement(10));
}

TEST(VectorBasic, VectorPushBack){
    Vector<int> vector = Vector<int> ();
    for (int i = 0; i < 32; i++){
        vector.PushBack(i);
        EXPECT_EQ(vector.GetNthElement(i), i);
    }
    EXPECT_EQ(vector.GetNthElement(10), 10);
}

TEST(VectorBasic, VectorPopBack){
    Vector<int> vector = Vector<int> ();
    for (int i = 0; i < 32; i++){
        vector.PushBack(i);
    }
    for (int i = 0; i < 32-8; i++) vector.PopBack();
    EXPECT_EQ(vector.GetNthElement(3), 3);
    EXPECT_ANY_THROW(vector.GetNthElement(30));
}

TEST(THeapConstructor, THeapConstructor){
    THeap<int> heap = THeap<int>();
    EXPECT_EQ(heap.IsEmpty(), true);
}

TEST(THeapBasic, THeapBasicOperations){
    THeap<int> heap = THeap<int>();
    THeap<int>::Pointer pointers[101];
    for (int i = 100; i >= 0; --i){
        pointers[i] = heap.Insert(i);
        EXPECT_EQ(heap.GetMin(), i);
    }
    for (int i = 0; i <= 100; ++i){
        EXPECT_EQ(heap.ExtractMin(), i);
    }
    EXPECT_ANY_THROW(heap.GetMin());
    EXPECT_ANY_THROW(heap.ExtractMin());
}

TEST(THeapAdvanced, THeapChange){
    THeap<int> heap = THeap<int>();
    THeap<int>::Pointer pointers[11];
    for (int i = 10; i >= 0; --i){
        pointers[i] = heap.Insert(i);
    }
    heap.Change(pointers[3], -1);
    EXPECT_EQ(heap.ExtractMin(), -1);
    EXPECT_EQ(heap.ExtractMin(), 0);
    EXPECT_EQ(heap.ExtractMin(), 1);
    EXPECT_EQ(heap.ExtractMin(), 2);
    EXPECT_EQ(heap.ExtractMin(), 4);
    EXPECT_EQ(heap.ExtractMin(), 5);
    heap.Change(pointers[6], 11);
    for (int i = 7; i <= 11; ++i){
        EXPECT_EQ(heap.ExtractMin(), i);
    }
}

TEST(THeapAdvanced, THeapDelete){
    THeap<int> heap = THeap<int>();
    THeap<int>::Pointer pointers[11];
    for (int i = 10; i >= 0; --i){
        pointers[i] = heap.Insert(i);
    }
    heap.Delete(pointers[0]);
    heap.Delete(pointers[3]);
    heap.Delete(pointers[6]);
    heap.Insert(100);
    EXPECT_ANY_THROW(heap.Delete(pointers[6]));
    heap.Delete(pointers[10]);
    EXPECT_EQ(heap.ExtractMin(), 1);
    EXPECT_EQ(heap.ExtractMin(), 2);
    EXPECT_EQ(heap.ExtractMin(), 4);
    EXPECT_EQ(heap.ExtractMin(), 5);
    EXPECT_EQ(heap.ExtractMin(), 7);
    EXPECT_EQ(heap.ExtractMin(), 8);
    EXPECT_EQ(heap.ExtractMin(), 9);
    EXPECT_EQ(heap.ExtractMin(), 100);
    EXPECT_EQ(heap.IsEmpty(), true);
}

TEST(THeapAdvanced, THeapMassConstructor){
    std::vector<int> vector(10);
    vector = {5, 3, 1, 0, 4, 6, 7, 2, 9, 8};
    THeap<int> first_heap = THeap<int>(vector.begin(), vector.end());
    for (int i = 0; i < 10; ++i){
        EXPECT_EQ(first_heap.ExtractMin(), i);
    }

    std::set<int> set;
    for (int i = 0; i < 10; ++i){
        set.insert(i);
    }

    THeap<int> second_heap = THeap<int>(set.begin(), set.end());
    for (int i = 0; i < 10; ++i){
        EXPECT_EQ(second_heap.ExtractMin(), i);
    }
}


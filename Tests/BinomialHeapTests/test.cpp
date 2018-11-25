//
// Created by Виталий on 010 10.11.18.
//

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testVector

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/BinomialHeap/BinomialHeap.h"

TEST(BinomialHeap, BinomialHeapInsert){
    BinomialHeap<int> heap = BinomialHeap<int>();
    BinomialHeap<int>::Pointer pointers[101];
    for (int i = 100; i >= 0; --i){
        pointers[i] = heap.Insert(i);
        EXPECT_EQ(heap.GetMin(), i);
    }
}

TEST(BinomialHeap, BinomialHeapExtractMin){
    BinomialHeap<int> heap = BinomialHeap<int>();
    BinomialHeap<int>::Pointer pointers[101];
    for (int i = 100; i >= 0; --i){
        pointers[i] = heap.Insert(i);
        EXPECT_EQ(heap.GetMin(), i);
    }
    for (int i = 0; i <= 100; ++i){
        EXPECT_EQ(heap.ExtractMin(), i);
    }
    EXPECT_ANY_THROW(heap.ExtractMin());
    EXPECT_EQ(heap.IsEmpty(), true);
}

TEST(BinomialHeap, BinomialHeapMerge){
    BinomialHeap<int> first_heap = BinomialHeap<int>();
    BinomialHeap<int> second_heap = BinomialHeap<int>();
    for (int i = 100; i >= 0; --i){
        first_heap.Insert(i);
        second_heap.Insert(101+i);
    }
    first_heap.merge(second_heap);
    EXPECT_EQ(second_heap.IsEmpty(), true);
    for (int i = 0; i <= 201; ++i){
        EXPECT_EQ(first_heap.ExtractMin(), i);
    }
    EXPECT_EQ(first_heap.IsEmpty(), true);
}

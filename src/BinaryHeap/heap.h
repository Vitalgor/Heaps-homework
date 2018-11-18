//
// Created by Виталий on 026 26.10.18.
//

#ifndef HEAP_HEAP_H
#define HEAP_HEAP_H

#include "vector.h"
#include <memory>

template <typename Key>
class THeap {
    public:

        class Pointer;
        struct Node;

        THeap(){
            size_ = 0;
        }

        template<class Iterator>
        explicit THeap(Iterator begin, Iterator end){
            size_ = 0;
            for (Iterator it = begin; it != end; ++it){
                std::shared_ptr<Node> new_element = std::make_shared<Node>();
                new_element->value = (*it);
                new_element->index = size_;
                new_element->deleted = false;
                array_.PushBack(new_element);
                ++size_;
            }
            Heapify();
        }

        ~THeap(){
            while (!IsEmpty()) ExtractMin();
        }

        bool IsEmpty(){
            return (size_ == 0);
        }

        Key GetMin(){
            if (size_ == 0){
                throw std::logic_error("Attempt to get element from an empty heap");
            }
            return array_.GetNthElement(0)->value;
        }

        Pointer Insert(Key value){
            std::shared_ptr<Node> new_element = std::make_shared<Node>();
            new_element->value = value;
            new_element->index = size_;
            new_element->deleted = false;
            array_.PushBack(new_element);
            Pointer inserted_element = Pointer();
            inserted_element.SetIndex(new_element);
            ++size_;
            SiftUp(size_-1);
            return inserted_element;
        }

        Key ExtractMin(){
            if (size_ == 0){
                throw std::logic_error("Attempt to get element from an empty heap");
            }
            Key min_value = array_.GetNthElement(0)->value;
            if (size_ > 1){
                array_.Swap(0, size_-1);
                std::swap(array_.GetNthElement(0)->index, array_.GetNthElement(size_-1)->index);
            }
            array_.GetNthElement(size_-1)->deleted = true;
            array_.GetNthElement(size_-1).reset();
            array_.PopBack();
            --size_;
            if (size_ != 0){
                SiftDown(0);
            }
            return min_value;
        }

        void Change(Pointer ptr, Key key){
            if (ptr.GetIndex() == nullptr){
                throw std::logic_error("Changing element, that doesn't exist");
            }
            if (ptr.GetIndex()->deleted){
                throw std::logic_error("Changing element, that doesn't exist");
            }
            size_t index = ptr.GetIndex()->index;
            Key old_key = array_.GetNthElement(index)->value;
            array_.GetNthElement(index)->value = key;
            CorrectAfterChangingElementsValue(index, old_key, key);
        }

        void Delete(Pointer ptr){
            if (ptr.GetIndex() == nullptr) {
                throw std::logic_error("Deleting element, that doesn't exist");
            }
            if (ptr.GetIndex()->deleted){
                throw std::logic_error("Changing element, that doesn't exist");
            }
            size_t index = ptr.GetIndex()->index;
            Key value_of_deleted_element = array_.GetNthElement(index)->value;
            Key value_of_last_element = array_.GetNthElement(size_-1)->value;
            if (size_ - 1 != index){
                array_.Swap(index, size_-1);
                std::swap(array_.GetNthElement(index)->index, array_.GetNthElement(size_-1)->index);
            }
            array_.GetNthElement(size_-1)->deleted = true;
            array_.GetNthElement(size_-1).reset();
            array_.PopBack();
            --size_;
            if (index != size_){
                CorrectAfterChangingElementsValue(index, value_of_deleted_element, value_of_last_element);
            }
        }

        class Pointer{
            public:
                Pointer(){
                }
            private:
                std::shared_ptr<Node> GetIndex(){
                    return index_;
                }

                void SetIndex(std::shared_ptr<Node> index){
                    index_ = index;
                }

                std::shared_ptr<Node> index_;
                friend class THeap;
        };

        struct Node{
            Key value;
            size_t index;
            bool deleted;
        };

    private:
        size_t GetLeftSonIndex(size_t index){
            return 2*index+1;
        }

        size_t GetRigthSonIndex(size_t index){
            return 2*index+2;
        }

        size_t GetParentIndex(size_t index){
            return (index-1)/2;
        }

        void CorrectAfterChangingElementsValue(size_t index, Key old_key, Key new_key){
            if (new_key < old_key){
                SiftUp(index);
            }
            else{
                SiftDown(index);
            }
        }

        void SiftUp(size_t index){
            if (index == 0) return;
            size_t parent_index = GetParentIndex(index);
            if (array_.GetNthElement(parent_index)->value > array_.GetNthElement(index)->value){
                array_.Swap(parent_index, index);
                std::swap(array_.GetNthElement(parent_index)->index, array_.GetNthElement(index)->index);

                SiftUp(parent_index);
            }
        }

        void SiftDown(size_t index){
            size_t left_son_index = GetLeftSonIndex(index);
            size_t rigth_son_index = GetRigthSonIndex(index);
            size_t min_value_index = index;
            if (left_son_index < size_ &&
                array_.GetNthElement(left_son_index)->value < array_.GetNthElement(index)->value) {
                min_value_index = left_son_index;
            }
            if (rigth_son_index < size_ &&
                array_.GetNthElement(rigth_son_index)->value < array_.GetNthElement(min_value_index)->value) {
                min_value_index = rigth_son_index;
            }
            if (min_value_index != index) {
                array_.Swap(min_value_index, index);
                std::swap(array_.GetNthElement(min_value_index)->index, array_.GetNthElement(index)->index);

                SiftDown(min_value_index);
            }
        }

        void Heapify(){
            //Не используем size_t, т.к. он беззнаковый, и, на последнем шаге, когда index == 0 происходит переполнение.
            for (int index = size_ - 1; index >= 0; --index){
                SiftDown(index);
            }
        }

        Vector<std::shared_ptr<Node>> array_;
        size_t size_;
};


#endif //HEAP_HEAP_H
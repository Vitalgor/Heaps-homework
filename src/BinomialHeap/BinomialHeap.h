//
// Created by Виталий on 012 12.11.18.
//

#ifndef BINOMIALHEAP_BINOMIALHEAP_H
#define BINOMIALHEAP_BINOMIALHEAP_H

#include <memory>

template <typename Key>
class BinomialHeap{
    public:

        class Pointer;
        struct Node;

        BinomialHeap(){
            size_ = 0;
        }

        ~BinomialHeap(){
            Clear();
            size_ = 0;
        }

        void Clear(){
            while (first_root_ != nullptr){
                std::shared_ptr<Node> pointer_for_delete = first_root_;
                first_root_ = first_root_->next_element_in_list;
                if (first_root_ != nullptr){
                    first_root_->previous_element_in_list.reset();
                }
                pointer_for_delete.reset();
            }
        }

        bool IsEmpty(){
            return (size_ == 0);
        }

        size_t Size(){
            return size_;
        }

        Pointer Insert(Key key){
            std::shared_ptr<std::shared_ptr<Node>> pointer_to_new_element;
            if (IsEmpty()){
                pointer_to_new_element = InsertOneElementIntoEmptyHeap(key);
            }
            else{
                BinomialHeap<Key> one_element_heap = BinomialHeap<Key>();
                pointer_to_new_element = one_element_heap.InsertOneElementIntoEmptyHeap(key);
                merge(one_element_heap);
            }
            Pointer returning_pointer = Pointer();
            returning_pointer.SetIndex(pointer_to_new_element);
            return returning_pointer;
        }

        Key GetMin(){
            if (size_ == 0){
                throw std::logic_error("Attempt to get element from an empty heap");
            }
            return (minimum_->value);
        }

        Key ExtractMin(){
            Key minimum_key = GetMin();

            //Удаление корня из списка
            minimum_->deleted = true;

            if (minimum_ == first_root_){
                first_root_ = first_root_->next_element_in_list;
            }
            else{
                minimum_->previous_element_in_list->next_element_in_list = minimum_->next_element_in_list;
                if (minimum_->next_element_in_list != nullptr){
                    minimum_->next_element_in_list->previous_element_in_list = minimum_->previous_element_in_list;
                }
            }

            if (minimum_->degree != 0){
                MergeRoots(minimum_->first_child, first_root_);
            }
            minimum_.reset();
            FindMininimumAfterMerging();
            --size_;
            return minimum_key;
        }

        void merge(BinomialHeap & OtherHeap){
            MergeRoots(first_root_, OtherHeap.first_root_);
            size_ += OtherHeap.Size();
            OtherHeap.~BinomialHeap();
            FindMininimumAfterMerging();
        }

        void Change(Pointer ptr, Key key){
            if ((*ptr.GetIndex()) == nullptr){
                throw std::logic_error("Changing element, that doesn't exist");
            }
            if ((*ptr.GetIndex())->deleted){
                throw std::logic_error("Changing element, that doesn't exist");
            }
            std::shared_ptr<Node> element_to_change = (*ptr.GetIndex());
            Key old_key = element_to_change->value;
            if (key >= old_key){
                throw std::logic_error("New key should be less, than old");
            }
            element_to_change->value = key;
            SiftUp(element_to_change, false);
            if (element_to_change->value < minimum_->value){
                minimum_ = element_to_change;
            }
        }

        class Pointer{
            public:
                Pointer(){
                }
            private:
                std::shared_ptr<std::shared_ptr<Node>> GetIndex(){
                    return index_;
                }

                void SetIndex(std::shared_ptr<std::shared_ptr<Node>> index){
                    index_ = index;
                }

                std::shared_ptr<std::shared_ptr<Node>> index_;
                friend class BinomialHeap;
        };

        struct Node{
            Key value;
            bool deleted;
            size_t degree;
            std::shared_ptr<Node> parent;
            std::shared_ptr<Node> first_child;
            std::shared_ptr<Node> next_element_in_list, previous_element_in_list;
            std::shared_ptr<std::shared_ptr<Node>> pointer_to_return;
        };
    private:

        void SiftUp(std::shared_ptr<Node> element, bool forcibly){
            if (element->parent == nullptr){
                return;
            }
            if (forcibly || element->parent->value > element->value){
                std::swap(*(element->pointer_to_return), *(element->parent->pointer_to_return));
                std::swap(element->parent->value, element->value);
                SiftUp(element->parent, forcibly);
            }
        }

        void FindMininimumAfterMerging(){
            minimum_ = first_root_;
            std::shared_ptr<Node> current = first_root_;
            while (current != nullptr){
                if (current->value < minimum_->value){
                    minimum_ = current;
                }
                current = current->next_element_in_list;
            }
        }

        std::shared_ptr<std::shared_ptr<Node>> InsertOneElementIntoEmptyHeap(Key value){
            size_ = 1;
            std::shared_ptr<Node> new_element = std::make_shared<Node>();
            new_element->deleted = false;
            new_element->value = value;
            new_element->degree = 0;
            new_element->next_element_in_list = nullptr;
            new_element->previous_element_in_list = nullptr;
            new_element->parent = nullptr;

            std::shared_ptr<Node> pointer_to_new_element = new_element;
            new_element->pointer_to_return = std::make_shared<std::shared_ptr<Node>>(pointer_to_new_element);
            minimum_ = new_element;
            first_root_ = new_element;

            return new_element->pointer_to_return;
        }

        void AddChildToRoot(std::shared_ptr<Node> root, std::shared_ptr<Node> new_child){
            new_child->previous_element_in_list = nullptr;
            new_child->next_element_in_list = nullptr;
            if (root->first_child == nullptr){
                root->first_child = new_child;
            }
            else{
                new_child->next_element_in_list = root->first_child;
                root->first_child->previous_element_in_list = new_child;
                root->first_child = new_child;
            }
        }

        std::shared_ptr<Node> MergeBinomialTrees(std::shared_ptr<Node> first_tree, std::shared_ptr<Node> second_tree){
            std::shared_ptr<Node> new_tree;
            if (first_tree->value < second_tree->value){
                second_tree->parent = first_tree;
                AddChildToRoot(first_tree, second_tree);
                second_tree.reset();
                new_tree = first_tree;
            }
            else{
                first_tree->parent = second_tree;
                AddChildToRoot(second_tree, first_tree);
                first_tree.reset();
                new_tree = second_tree;
            }
            ++(new_tree->degree);
            return new_tree;
        }

        void MergeRoots(std::shared_ptr<Node> first_roots_list, std::shared_ptr<Node> second_roots_list){
            size_t max_degree = 0;
            std::shared_ptr<Node> current = first_roots_list;
            while (current != nullptr){
                max_degree = std::max(max_degree, current->degree);
                current->parent = nullptr;
                current = current->next_element_in_list;
            }
            current = second_roots_list;
            while (current != nullptr){
                max_degree = std::max(max_degree, current->degree);
                current->parent = nullptr;
                current = current->next_element_in_list;
            }

            ++max_degree;
            std::shared_ptr<Node> first_roots[max_degree+1];
            std::shared_ptr<Node> second_roots[max_degree+1];

            current = first_roots_list;
            while (current != nullptr){
                first_roots[current->degree] = current;
                current = current->next_element_in_list;
            }

            current = second_roots_list;
            while (current != nullptr){
                second_roots[current->degree] = current;
                current = current->next_element_in_list;
            }

            std::shared_ptr<Node> new_roots[max_degree+2];

            std::shared_ptr<Node> carry;


            for (auto i = 0; i < max_degree; ++i){
                new_roots[i] = nullptr;
                std::shared_ptr<Node> first_element = first_roots[i];
                std::shared_ptr<Node> second_element = second_roots[i];

                if (first_element == nullptr && second_element == nullptr){
                    new_roots[i] = carry;
                    carry.reset();
                    continue;
                }
                if (first_element == nullptr){
                    swap(first_element, second_element);
                }

                if (second_element == nullptr){
                    if (carry == nullptr){
                        new_roots[i] = first_element;
                    }
                    else{
                        carry = MergeBinomialTrees(first_element, carry);
                    }
                }
                else{
                    new_roots[i] = carry;
                    carry = MergeBinomialTrees(first_element, second_element);
                }
            }
            if (carry != nullptr){
                new_roots[carry->degree] = carry;
                carry.reset();
            }
            //Очищаем старую кучу
            Clear();
            for (size_t i = 0; i < max_degree+2; ++i){
                if (new_roots[i] == nullptr) continue;
                new_roots[i]->previous_element_in_list = nullptr;
                new_roots[i]->next_element_in_list = nullptr;
                if (first_root_ == nullptr){
                    first_root_ = new_roots[i];
                }
                else{
                    first_root_->previous_element_in_list = new_roots[i];
                    new_roots[i]->next_element_in_list = first_root_;
                    first_root_ = new_roots[i];
                }
            }

        }

        size_t size_;
        std::shared_ptr<Node> first_root_;
        std::shared_ptr<Node> minimum_;
};

#endif //BINOMIALHEAP_BINOMIALHEAP_H

//
// Created by Виктор on 014 14.11.18.
//

#ifndef FIBONACCIHEAP_FIBONACCIHEAP_H
#define FIBONACCIHEAP_FIBONACCIHEAP_H

#include <memory>

template <typename Key>
class FibonacciHeap{
    public:

        class Pointer;
        struct Node;

        FibonacciHeap(){
            size_ = 0;
        }

        ~FibonacciHeap(){
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
            last_root_ = nullptr;
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
                FibonacciHeap<Key> one_element_heap = FibonacciHeap<Key>();
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
                if (first_root_ != nullptr){
                    first_root_->previous_element_in_list = nullptr;
                }
                else{
                    last_root_ = nullptr;
                }
            }
            else{
                if (minimum_ == last_root_){
                    last_root_ = last_root_->previous_element_in_list;
                    if (last_root_ != nullptr){
                        last_root_->next_element_in_list = nullptr;
                    }
                    else{
                        first_root_ = nullptr;
                    }
                }
                else{
                    minimum_->previous_element_in_list->next_element_in_list = minimum_->next_element_in_list;
                    minimum_->next_element_in_list->previous_element_in_list = minimum_->previous_element_in_list;
                }
            }

            if (minimum_->degree != 0){
                if (first_root_ != nullptr){
                    last_root_->next_element_in_list = minimum_->first_child;
                    minimum_->first_child->previous_element_in_list = last_root_;
                    last_root_ = minimum_->last_child;
                }
                else{
                    first_root_ = minimum_->first_child;
                    last_root_ = minimum_->last_child;
                }
            }
            minimum_.reset();
            --size_;
            if (size_ != 0){
                Consolidate();
                FindMininimum();
            }
            return minimum_key;
        }

        void merge(FibonacciHeap & OtherHeap){
            last_root_->next_element_in_list = OtherHeap.first_root_;
            OtherHeap.first_root_->previous_element_in_list = last_root_;
            last_root_ = OtherHeap.last_root_;
            if (minimum_->value > OtherHeap.minimum_->value){
                minimum_ = OtherHeap.minimum_;
            }
            size_ += OtherHeap.Size();
            OtherHeap.first_root_ = nullptr;
            OtherHeap.last_root_ = nullptr;
            OtherHeap.size_ = 0;
        }

        void Decrease(Pointer ptr, Key key){
            std::shared_ptr<Node> element_to_decrease = *ptr.GetIndex();
            if (element_to_decrease->deleted){
                throw std::logic_error("Attempt to decrease element that doesn't exist");
            }
            element_to_decrease->value = key;
            if (key < minimum_->value){
                minimum_ = element_to_decrease;
            }

            if (element_to_decrease->parent != nullptr) {
                std::shared_ptr<Node> parent = element_to_decrease->parent;
                CutFromChildrenList(element_to_decrease, parent);
                AddToRoots(element_to_decrease);
                std::shared_ptr<Node> current_element = parent;
                while (current_element->mark && current_element->parent != nullptr){
                    CutFromChildrenList(current_element, current_element->parent);
                    AddToRoots(current_element);
                }
                parent->mark = true;
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
                friend class FibonacciHeap;
        };

        struct Node{
            Key value;
            bool deleted;
            size_t degree;
            bool mark;
            std::shared_ptr<Node> parent;
            std::shared_ptr<Node> first_child, last_child;
            std::shared_ptr<Node> next_element_in_list, previous_element_in_list;
            std::shared_ptr<std::shared_ptr<Node>> pointer_to_return;
        };
    private:

        void AddToRoots(std::shared_ptr<Node> element){
            if (first_root_ != nullptr){
                last_root_->next_element_in_list = element;
                element->previous_element_in_list = last_root_;
                last_root_ = element;
            }
            else{
                first_root_ = element;
                last_root_ = element;
            }
        }

        void CutFromChildrenList(std::shared_ptr<Node> element, std::shared_ptr<Node> parent){
            if (element == parent->first_child){
                parent->first_child = parent->first_child->next_element_in_list;
                if (parent->first_child != nullptr){
                    parent->first_child->previous_element_in_list = nullptr;
                }
                else{
                    parent->last_child = nullptr;
                }
            }
            else {
                if (element == parent->last_child) {
                    parent->last_child = parent->last_child->previous_element_in_list;
                    if (parent->last_child != nullptr) {
                        parent->last_child->next_element_in_list = nullptr;
                    } else {
                        parent->first_child = nullptr;
                    }
                } else {
                    element->previous_element_in_list->next_element_in_list = element->next_element_in_list;
                    element->next_element_in_list->previous_element_in_list = element->previous_element_in_list;
                }
            }
        }

        void FindMininimum(){
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
            new_element->mark = false;
            std::shared_ptr<Node> pointer_to_new_element = new_element;
            new_element->pointer_to_return = std::make_shared<std::shared_ptr<Node>>(pointer_to_new_element);
            minimum_ = new_element;
            first_root_ = new_element;
            last_root_ = new_element;

            return new_element->pointer_to_return;
        }

        void AddChildToRoot(std::shared_ptr<Node> root, std::shared_ptr<Node> new_child){
            if (root->first_child == nullptr){
                new_child->previous_element_in_list = nullptr;
                new_child->next_element_in_list = nullptr;
                root->first_child = new_child;
                root->last_child = new_child;
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

        void Consolidate(){
            //Найдем максимальную степень дерева в куче
            size_t max_degree = 0;
            size_t upper_bound_on_count_of_elements = 1;
            while (upper_bound_on_count_of_elements <= size_){
                upper_bound_on_count_of_elements *= 2;
                ++max_degree;
            }
            ++max_degree;
            std::shared_ptr<Node> new_roots[max_degree+2];
            for (size_t i = 0; i < max_degree+2; ++i){
                new_roots[i] = nullptr;
            }

            std::shared_ptr<Node> current = first_root_;
            while (current != nullptr){
                size_t current_degree = current->degree;
                std::shared_ptr<Node> next_root = current->next_element_in_list;
                while (new_roots[current_degree] != nullptr){
                    new_roots[current_degree]->previous_element_in_list = nullptr;
                    new_roots[current_degree]->next_element_in_list = nullptr;
                    current->previous_element_in_list = nullptr;
                    current->next_element_in_list = nullptr;
                    current = MergeBinomialTrees(current, new_roots[current_degree]);
                    new_roots[current_degree] = nullptr;
                    ++current_degree;
                }
                new_roots[current_degree] = current;
                current = next_root;
            }
            Clear();
            for (size_t i = 0; i < max_degree+2; ++i){
                if (new_roots[i] == nullptr) continue;
                if (first_root_ == nullptr){
                    first_root_ = new_roots[i];
                    last_root_ = new_roots[i];
                }
                else{
                    first_root_->previous_element_in_list = new_roots[i];
                    new_roots[i]->next_element_in_list = first_root_;
                    first_root_ = new_roots[i];
                }
            }

        }

        size_t size_;
        std::shared_ptr<Node> first_root_, last_root_;
        std::shared_ptr<Node> minimum_;
};

#endif //FIBONACCIHEAP_FIBONACCIHEAP_H

//
// Created by Виталий on 026 26.10.18.
//

#ifndef HEAP_VECTOR_H
#define HEAP_VECTOR_H


template <typename T>
class Vector{
    public:
        Vector(size_t size = 1) {
            if (size < 1){
                throw std::logic_error("Invalid size of vector");
            }
            array_ = new T [size];
            size_ = size;
            count_of_elements_ = 0;
        }

        size_t Size(){
            return size_;
        }

        void SetNthElementValue(size_t index, T value){
            if (index < 0 || index >= size_){
                throw std::out_of_range("Invalid index");
            }
            array_[index] = value;
        }

        T GetNthElement(size_t index){
            if (index < 0 || index >= size_){
                throw std::out_of_range("Invalid index");
            }
            return array_[index];
        }

        void Swap(size_t first_index, size_t last_index){
            if (first_index != last_index){
                std::swap(array_[first_index], array_[last_index]);
            }
        }

        void PushBack(T value){
            if (count_of_elements_ == size_){
                Resize(2*size_);
            }
            array_[count_of_elements_] = value;
            ++count_of_elements_;
        }

        void PopBack(){
            if (count_of_elements_ == 0){
                throw std::logic_error("Deleting element from an empty array.");
            }
            --count_of_elements_;
            if (count_of_elements_ == 0){
                return;
            }
            if (count_of_elements_ == (size_/4)){
                Resize(size_/2);
            }
        }

        ~Vector(){
            delete [] array_;
            size_ = 0;
        }

    private:

        void Resize(size_t new_size){
            T* new_array = new T [new_size];
            if (count_of_elements_ > 0){
                for (int i = 0; i < count_of_elements_; ++i) new_array[i] = array_[i];
            }
            size_ = new_size;
            delete [] array_;
            array_ = new_array;
        }

        size_t size_;
        size_t count_of_elements_;
        T* array_;
};

#endif //HEAP_VECTOR_H


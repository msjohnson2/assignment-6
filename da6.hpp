// da6.hpp
// Matthew Johnson
// 04/11/2023
// data stucture for associative data set that uses a linked list

#pragma once

#include <memory>
// For std::unique_ptr


#include<utility>
// for std::pair

#include <functional>
// for std::function

#include<iostream>

#include"llnode2.hpp"

template<typename ValType>
void reverseList(std::unique_ptr<LLNode2<ValType>> & head)
{
    
    int counter{0};
    std::unique_ptr<LLNode2<ValType>> newHead;

    while(head)
    {
        auto copyNext = std::move(head->_next);

        if(counter == 0)
        {
            head->_next.reset();
        }
        else
        {
            head->_next = std::move(newHead);
        }

        newHead = std::move(head);
        head = std::move(copyNext);
        ++counter;

    }

    head = std::move(newHead);

}



template<typename KeyType, typename DataType>
class SlowMap
{
    public:

        SlowMap() = default;
        ~SlowMap() noexcept
        {
            _keyval.reset();
            _keyval.release();
        }
        SlowMap(const SlowMap & other) = delete;
        SlowMap & operator=(const SlowMap & other) = delete;
        SlowMap(SlowMap && other) = delete;
        SlowMap & operator=(SlowMap && other) = delete;
        

    public: 

        std::size_t size() const
        {
            auto p = _keyval.get();      // Iterates through list
            std::size_t counter = 0;  // Number of nodes so far
            while (p != nullptr)
            {
                p = p->_next.get();
                ++counter;
            }
            return counter;
        }

        bool empty() const
        { 
            return !(_keyval);
        }

        bool present(const KeyType & key) const
        {
            auto p = _keyval.get();      // Iterates through list

            while (p != nullptr)
            {
                if(p->_data.first == key)
                {
                    return true;
                }

                p = p->_next.get();
                
            }

            return false;

        }

        DataType &get(const KeyType & key)
        {
            auto p = _keyval.get();      // Iterates through list
            while (p != nullptr)
            {
                if(p->_data.first == key)
                    {
                        return p->_data.second;
                    }            
                p = p->_next.get();
                
            }

            throw std::out_of_range("Key does not exist.");
        }

        const DataType &get(const KeyType & key) const
        {

            auto p = _keyval.get();   // Iterates through list

            while (p != nullptr)
            {
                if(p->_data.first == key)
                {
                   return p->_data.second;
                }
                        
                p = p->_next.get();
            }

            
            throw std::out_of_range("Key does not exist.");

        }

        void set(const KeyType & key, const DataType & data)
        {
            if(present(key))
            {
                get(key) = data;
                return;
            }

            push_front(_keyval, {key, data});

        }

        void erase(const KeyType & key)
        {
            if(present(key))
            {
                auto prior = _keyval.get();
                auto curr = prior->_next.get();

                if(prior->_data.first == key)
                {
                    pop_front(_keyval);
                    return;
                }

                while (curr != nullptr)
                {
                    if(curr->_data.first == key)
                    {
                        prior->_next = std::move(curr->_next);
                    }

                    prior = curr;
                    curr = prior->_next.get();
                }

            }
            
        }

        void traverse(const std::function<void(KeyType, DataType)> & func)
        {
            auto p = _keyval.get();

            while (p != nullptr)
            {
                
                func(p->_data.first, p->_data.second);

                p = p->_next.get();
            }

        }

    private:

        std::unique_ptr<LLNode2<std::pair<KeyType, DataType>>> _keyval;

};
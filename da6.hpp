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

// reverseList
// function takes a linked list of LLNode2 and reverses it
// Pre:
//      head must be a pointer to the head of a linked list
// No-Throw Gaurentee
template<typename ValType>
void reverseList(std::unique_ptr<LLNode2<ValType>> & head)
{
    
    int counter{0}; // counts iterations
    std::unique_ptr<LLNode2<ValType>> newHead;

    while(head)
    {
        auto copyNext = std::move(head->_next);

        if(counter == 0)
        {
            head->_next.reset(); // first item in the list must point to nothing
        }
        else
        {
            head->_next = std::move(newHead); // makes head->_next point to previous node
        }

        newHead = std::move(head); // keeps newHead one behind head
        head = std::move(copyNext); // moves head one foward
        ++counter;

    }

    head = std::move(newHead); // sets head to newHead

}


// class SlowMap
// Linked list that hold an associative data set
// Invariants: 
//              _keyval must point to a LLnode2
// Requirements on types:
// KeyType must have a copy ctor, non throwing dctor, and an equality operator
// DataType must have a copy ctor and a non throwing dctor
template<typename KeyType, typename DataType>
class SlowMap
{
    public:

        // default ctor
        // sets _keyval to nullptr
        // No-Throw Guarentee
        SlowMap() = default;

        // de-constructor
        // should call dctor of LLnode2
        // No-Throw Guarentee
        ~SlowMap() noexcept
        {
            _keyval.reset();
            _keyval.release();
        }

        // no copy or move operations
        SlowMap(const SlowMap & other) = delete;
        SlowMap & operator=(const SlowMap & other) = delete;
        SlowMap(SlowMap && other) = delete;
        SlowMap & operator=(SlowMap && other) = delete;
        

    public: 
        
        // size
        // returns the size of a slow map
        // Pre:
        //      none
        // No-Throw Guarantee
        std::size_t size() const
        {
            auto p = _keyval.get();
            std::size_t counter = 0;  // Number of nodes so far

            // Iterates through list
            while (p != nullptr)
            {
                p = p->_next.get();
                ++counter; // adds one for each node
            }
            return counter;
        }

        // empty
        // returns a bool that is true if slowmap is empty
        // Pre:
        //      none
        // No-Throw Guarantee
        bool empty() const
        { 
            return !(_keyval); // !(_keyval) returns true if nullptr
        }

        // empty
        // returns a bool that is true if slowmap is empty
        // Pre:
        //      none
        // No-Throw Guarantee
        bool present(const KeyType & key) const
        {
            auto p = _keyval.get();

            // Iterates through list
            while (p != nullptr)
            {
                // returns true if it finds the key
                if(p->_data.first == key)
                {
                    return true;
                }

                p = p->_next.get();
                
            }

            return false; // went through the list and didnt find it

        }

        // get and const get
        // returns the data at a given key
        // pre:
        //      none
        // Strong Gaurantee
        // Throws std::out_of_range
        // Exception Neutral
        DataType &get(const KeyType & key)
        {
            auto p = _keyval.get(); 

            // Iterates through list
            while (p != nullptr)
            {
                // returns data value if it finds the key
                if(p->_data.first == key)
                {
                    return p->_data.second;
                }

                p = p->_next.get();
                
            }

            // key isnt present so throws 
            throw std::out_of_range("Key does not exist.");
        }

        const DataType &get(const KeyType & key) const
        {
            auto p = _keyval.get(); 

            // Iterates through list
            while (p != nullptr)
            {
                // returns data value if it finds the key
                if(p->_data.first == key)
                {
                    return p->_data.second;
                }

                p = p->_next.get();
                
            }

            // key isnt present so throws 
            throw std::out_of_range("Key does not exist.");
        }

        // set
        // sets a know key's data to a value or creates a new key and data pair
        // Pre:
        //      None
        // Strong Guarantee
        // Exception Neutral
        void set(const KeyType & key, const DataType & data)
        {
            // if key is present it sets the data at the key to new data
            if(present(key))
            {
                get(key) = data;
                return;
            }
            // key is not present to construct a new node with a new key
            push_front(_keyval, {key, data});

        }

        // erase
        // erases a node if it entered key exists, does nothing if the key is not present
        // Pre:
        //      None
        // No-Throw Guarentee
        void erase(const KeyType & key)
        {

            auto prior = _keyval.get();
            auto curr = prior->_next.get();
            
            // if key is at the front we can just pop_front
            if(prior->_data.first == key)
            {
                pop_front(_keyval);
                return;
            }
                
            // Iterates through list
            while (curr != nullptr)
            {
                // once it finds the key it sets prior next to the node after current
                if(curr->_data.first == key)
                {
                    prior->_next = std::move(curr->_next);
                }

                // moves prior and curr one step foward
                prior = curr;
                curr = prior->_next.get();
            }
            
        }

        // traverse
        // applys a function to all key and data pairs in a linked list
        // Pre: 
        //      Function must be void
        // 
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

        std::unique_ptr<LLNode2<std::pair<KeyType, DataType>>> _keyval; // Pointer to llnode2 that stores associative data pair

}; // end of class
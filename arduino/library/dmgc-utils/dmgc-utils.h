#ifndef _DMGC_UTILS_H
#define _DMGC_UTILS_H

#include <stdint.h>
#include <stdio.h>

class Adafruit_NeoPixel;

namespace DMGC_UTILS
{   
    // Implemented Custom linked list because stl not available in Arduino
    template<typename T>
    class List
    {
    private:
        struct Node {
            T data = NULL;
            Node * next = NULL;
        };
    public:
        List():first(NULL),_size(0){

        }
        ~List(){
            if (_size && first != NULL){
                Node * current = first;
                while (current->next != NULL){
                    Node * next = current->next;
                    delete current;
                    current = next;
                }
            }
        }

        void push(T & data)
        {
            Node * newNode = new Node();
            newNode->data = data;
            newNode->next = NULL;

            if (isEmpty()){
                first = newNode;
            } else {
                Node * current = first;
                while (current->next != NULL){
                    current = current->next;
                }
                current->next = newNode;
            }
            _size++;
        }

        /*
        Node * remove(T & data)
        {
            Node * current = first;
            Node * previous = NULL;
            while (current && current->data != data){
                previous = current;
                if (current)
                    current = current->next;
            }
            if (current != NULL && previous != NULL){
                previous->next = current->next;
                delete current;
                current = previous->next;
                _size--;
            } else if (current != NULL) {
                Node * next = current->next;
                delete current;
                current = next;
                _size--;
            }
            return current;
        }
        */

        T popFront(){
            Node * current = first;
            T data = current->data;
            first = first->next;
            delete current;
            _size--;
            return data;
        }

        T popBack() {
            Node * current = first;
            Node * previous = NULL;
            while (current->next != NULL){
                previous = current;
                current = current->next;
            }
            if (current != NULL){
                T data = current->data;
                previous->next = NULL;
                delete current;
                _size--;
                return data;
            }
            return NULL;
        }

        void clear() noexcept
        {
            if (!isEmpty())
            {
                Node * current = first;
                while (current != NULL)
                {
                    Node * next = current->next;
                    //temp = NULL;
                    delete current;
                    current = next;
                }
                first = NULL;
                _size = 0;
            }
        }

        uint8_t size() const { return _size; }
        bool isEmpty() const { return size() == 0; }

        struct iterator {
            friend class List;
            iterator():previousNode(NULL),currentNode(NULL) { }
            iterator(Node * node):currentNode(node){}
            iterator(const iterator & i):previousNode(i.previousNode),currentNode(i.currentNode){}
            iterator & operator++(){
                if (currentNode != NULL){
                    previousNode = currentNode;
                    currentNode = currentNode->next;
                }
                return *this;
            }
            iterator operator++(int){
                iterator temp = *this;
                ++*this;
                return temp;
            }
            bool operator!=(const iterator & other){
                return this->currentNode != other.currentNode;
            }
            T operator*() const {
                return this->currentNode->data;
            }
        private:
            Node * previousNode;
            Node * currentNode;
        };

        iterator begin() const {
            return iterator(this->first);
        }

        iterator end() const {
            return iterator();
        }

        // Broken needs fixing
        iterator erase(iterator & value) {
            Node * next = value.currentNode->next;
            if (value.previousNode){
                value.previousNode->next = next;
            }
            delete value.currentNode;
            _size--;
            value.currentNode = next;

            if (_size == 0)
                return iterator(NULL);
            return value;
        }

    private:
        Node * first;
        uint8_t _size;
    }; 

    class Button 
    {
    public:
        enum STATE
        {
            UP,
            DOWN,
            HOLD,
            PRESS,
            RELEASE
        };
        
        Button(int pin);
        ~Button();

        enum STATE getState() const {
            return state;
        }

        bool isDown() const {
            return state == DOWN || state == HOLD;
        }

        bool isClicked() const {
            return state == PRESS || state == RELEASE;
        }

        void poll(int pull);

    private: 
        int pin;
        enum STATE state;
    };

    class ButtonHandler
    {
    public:
        ButtonHandler();
        ~ButtonHandler();

        Button * add(int pin);

        void poll(int pull);

    private:
        int totalPins;
        List<Button *> buttonList;
    };

    void dmgc_intro(Adafruit_NeoPixel &, bool skipDelays = false);
}

#endif
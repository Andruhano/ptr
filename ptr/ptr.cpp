#include <iostream>
using namespace std;

template<typename T>
class UniquePtr 
{
T* ptr; 

public:
    UniquePtr() : ptr(nullptr) {}
    UniquePtr(T* p) : ptr(p) {}

    ~UniquePtr() 
    {
        delete ptr;
    }
   
    UniquePtr(UniquePtr&& other) : ptr(other.release()) {}

    UniquePtr& operator=(UniquePtr&& other) 
    {
        if (this != &other) 
        {
            delete ptr;
            ptr = other.release();
        }
        return *this;
    }

    
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    operator bool() const { return ptr != nullptr; }

    T* release() 
    {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }
};

template<typename T>
class SharedPtr 
{
T* ptr;       
size_t* count; 

public:
    SharedPtr() : ptr(nullptr), count(nullptr) {}
    SharedPtr(T* p) : ptr(p), count(new size_t(1)) {}

    ~SharedPtr() 
    {
        if (count && (*count)-- == 0) 
        {
            delete ptr;
            delete count;
        }
    }

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), count(other.count) 
    {
        if (count) 
        {
            (*count)++;
        }
    }

    SharedPtr& operator=(const SharedPtr& other) 
    {
        if (this != &other) 
        {
            if (count && (*count)-- == 0) 
            {
                delete ptr;
                delete count;
            }

            ptr = other.ptr;
            count = other.count;
            if (count) 
            {
                (*count)++;
            }
        }
        return *this;
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    operator bool() const { return ptr != nullptr; }

    size_t use_count() const 
    {
        return count ? *count : 0;
    }
};

int main()
{
    {
        UniquePtr<int> ptr(new int(42));
        cout << *ptr << endl; 

        UniquePtr<int> ptr2 = move(ptr);
        cout << *ptr2 << endl; 
    } 

    {
        SharedPtr<int> ptr(new int(42));
        cout << *ptr << endl; 

        SharedPtr<int> ptr2 = ptr;
        cout << *ptr2 << endl; 

        cout << "Count: " << ptr.use_count() << endl; 
    } 

    return 0;
}

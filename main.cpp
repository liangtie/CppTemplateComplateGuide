#include <iostream>
#include <vector>

namespace Cpp17Features
{
    struct Person
    {
        int age;
        std::string name;
    };

} // namespace Cpp17Features

namespace chapter2
{
    template <typename T>
    class Stack
    {
    private:
        std::vector<T> elems; // elements
    public:
        Stack(T elem) // initialize stack with one element by value
            : elems({std::move(elem)})
        {
        }
        auto showElementType()
        {
            std::cout << typeid(this->elems).name() << std::endl;
            std::cout << "The elements count is " << elems.size() << std::endl;
        }
        virtual ~Stack(){
            std::cout <<"Bye ~ stack\n";
        }
    };
    // 1. Deduction guide , while passing char* constuct std::vector<std::string> instead
    Stack(char const *)->Stack<std::string>;

    template <typename T>
    struct ValueWithComment
    {
        T value;
        std::string comment;
    };
    ValueWithComment(char const *, char const *)
        ->ValueWithComment<std::string>;     // 2.Templatized Aggregates

} // namespace chapter2

int main()
{
    {
        using namespace Cpp17Features;
        Person alex{12, "Alex"};
        auto [a, b] = alex;
        std::cout << "The name of person is " << b << std::endl;
    }

    //chapter2
    {
        using namespace chapter2;
        //Verify constructor deduction
        {
            auto stringStack = new Stack{"vvvv"};
            auto holder = std::shared_ptr<std::remove_pointer_t<decltype(stringStack)>>(stringStack);
            stringStack->showElementType();
        }
        //Verify templatized aggregates
        {
            auto valueWithComment = ValueWithComment{"Nice", "Person"};
            std::cout<< valueWithComment.comment + valueWithComment.value << std::endl;
        }
        
    }

    return 0;
}

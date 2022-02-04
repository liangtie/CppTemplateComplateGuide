#include <iostream>
#include <vector>
#include <memory>
#include <string>

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
        virtual ~Stack()
        {
            std::cout << "Bye ~ stack\n";
        }
    };
    // NOTE 1. Deduction guide , while passing char* constuct std::vector<std::string> instead
    Stack(char const *)->Stack<std::string>;

    template <typename T>
    struct ValueWithComment
    {
        T value;
        std::string comment;
    };
    ValueWithComment(char const *, char const *)
        ->ValueWithComment<std::string>; // 2.Templatized Aggregates

} // namespace chapter2

namespace chapter3
{
    // NOTE 2. Non-type parameters can be used as template param
    template <int Msg>
    void greeting()
    {
        std::cout << " Hello " << Msg << std::endl;
    }

    // NOTE 3. Using auto to accept any possible non-type template parameters
    template <auto Msg, int times>
    void greeting()
    {
        for (auto i = 0; i < times; i++)
            std::cout << " Hello " << Msg << std::endl;
    }

    template <auto MaxSize, typename T = decltype(MaxSize)>
    class Stack
    {
    public:
        Stack() : numCount_(0) {}
        auto size()
        {
            return MaxSize;
        }

    private:
        std::vector<T> elements_;
        T numCount_;
    };

    // NOTE 4. decltype(auto)
    template <decltype(auto) N>
    class DecltypeAuto
    {
    public:
        DecltypeAuto() : value_(N)
        {
        }
        void operate()
        {
            std::cout << value_ << std::endl;
        }

    private:
        decltype(N) value_;
    };

} // namespace chapter3

namespace chapter4
{
    // NOTE 5. Variadic templates: class or function which can accept variaties of params
    template <typename T, typename... Types>
    void print(T firstArg, Types... args)
    {
        std::cout << firstArg << '\n'; // print first argument
        print(args...);                // call print() for remaining arguments
    }
    void print()
    {
        std::cout << "This is the end\n";
    }

} // namespace chapter4

int main()
{
    {
        using namespace Cpp17Features;
        Person alex{12, "Alex"};
        auto [a, b] = alex;
        std::cout << "The name of person is " << b << std::endl;
    }

    // chapter2
    {
        using namespace chapter2;
        // Verify constructor deduction
        {
            auto stringStack = new Stack{"vvvv"};
            auto holder = std::shared_ptr<std::remove_pointer_t<decltype(stringStack)>>(stringStack);
        }
        // Verify templatized aggregates
        {
            auto valueWithComment = ValueWithComment{"Nice", "Person"};
            std::cout << valueWithComment.comment + valueWithComment.value << std::endl;
        }
    }

    // chapter3
    {
        using namespace chapter3;
        // Cannot use float number as non-type template parameter
        // Stack<3.14> errorInstance;
        Stack<100u> unsignedStack;
        Stack<100l> longStack;
        if (!std::is_same_v<decltype(unsignedStack.size()), decltype(longStack.size())>)
            std::cout << "Type differs\n";
        greeting<3>();
        // NOTE static type
        static const char msg[] = "World";
        greeting<msg, 10>();
        constexpr int cout = 0;
        DecltypeAuto<cout> bb;
        bb.operate();
        std::cout << cout << std::endl;
    }
    // chapter4
    {
        using namespace chapter4;
        print(1, 3, "Hello world", "666");
        print(32, 34, 34, 45);
    }

    return 0;
}

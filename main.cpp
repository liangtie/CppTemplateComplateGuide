#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <string>
#include <assert.h>
#include <tuple>
#include <variant>
#include <array>
#include <unordered_set>

void Clear()
{
#if defined _WIN32
    system("cls");
    // clrscr(); // including header file : conio.h
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    // std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences
#elif defined(__APPLE__)
    system("clear");
#endif
}

namespace Cpp17Features
{

    struct Person
    {
        int age;
        std::string name;
    };
    /**
     * NOTE 1. struct binding
     *  auto [ a , b ] = Person{ 12 , "Alex"}; // a = 12 ; b = "Alex"
     */
    enum class Operators
    {
        Add,
        Sub,
        Mut,
        Div
    };

    /**
     * NOTE 2. Compiling time if
     */
    template <Operators op>
    struct Operation
    {
        template <typename L, typename R = L>
        auto operator()(L lhs, R rhs)
        {
            if constexpr (Operators::Add == op)
                return lhs + rhs;
            else if constexpr (Operators::Sub == op)
                return lhs - rhs;
            else if constexpr (Operators::Mut == op)
                return lhs * rhs;
            else if constexpr (Operators::Div == op)
                return lhs / rhs;
        }
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

    /**
     * NOTE 5. Introduction of Variadic templates
     * Template which can accept variadic template arguments
     */
    /*
        template <typename T, typename... Types>
        void print(T firstArg, Types... args)
        {

            std::cout << firstArg; // print first argument
            print(args...);        // Will call empty implementation if no argument remain
        }
        void print()
        {
            std::cout << std::endl;
        }
    */

    void print()
    {
        std::cout << std::endl;
    }
    template <typename T, typename... Types>
    void print(T firstArg, Types... args)
    {

        std::cout << firstArg; // print first argument
        print(args...);        // Will call empty implementation if no argument remain
    }

    /**
     * NOTE 6. Another way to overwrite variadic template
     */
    namespace overload
    {
        template <typename T>
        void print(T arg)
        {
            std::cout << arg << std::endl;
        }
        template <typename T, typename... P>
        void print(T firstArg, P... args)
        {
            std::cout << firstArg;
            print(args...); // Will call the one without trailing arg if only one argument remain
        }

    } // namespace overload

    /**
     * NOTE 7. Usage of sizeof since c++11 in variadic template
     * Applying in either template paramter pack or function parameter pack will result in the same result:
     * showing the number of the reseting elements in the pack
     */
    namespace usageOfSizeof
    {
        void print()
        {
            std::cout << std::endl;
        }
        template <typename T, typename... Types>
        void print(T firstArg, Types... args)
        {
            std::cout << sizeof...(Types) << std::endl;
            std::cout << sizeof...(args) << std::endl;
            std::cout << firstArg << std::endl; // print first argument
            print(args...);                     //
        }

    } // namespace usageOfSizeof
    /**
     * NOTE 8. Since C++17 , there is a feature called fold expression to compute the result of using a binary operator
     * over all the arguments of a paramter pack
     */

    /**
     * NOTE 9.1 Usage of variadic  template : fold expression
     */
    namespace variadicExpression
    {

        template <typename... Types>
        auto foldSum(Types... arg)
        {
            return (... + arg); // return arg0 + (arg1 +... + (argn-1 + argn))
        }

        template <typename... Types>
        auto calcDouble(Types... arg)
        {
            return (... + arg) + (... + arg); // return arg0 + (arg1 +... + (argn-1 + argn))
        }

        template <typename... T>
        auto addOne(T const &...arg)
        {
            return foldSum((arg + 1)...); // return (arg0 + 1) + ... + (argn + 1);
        }

        template <auto value, typename... Types>
        auto calcSum(Types... arg)
        {
            return value + (... + arg); // return value + arg0 + (arg1 +... + (argn-1 + argn))
        }

        template <auto value, typename... Types>
        auto foldMutiple(Types... arg)
        {
            return (arg * ...); // return ((arg0 * arg1)* ...)* argn
        }

        template <auto value, typename... Types>
        auto calcSumTwo(Types... arg)
        {
            return value + (... + arg); // return value + arg0 + ... + argn
        }

    } // namespace variadicExpression

    /**
     * NOTE 9.2 Usage of variadic template : variadic indices
     */

    template <typename T, typename... Ts>
    auto printHelper(const T &arg0, const Ts &...args)
    {
        std::cout << arg0;
        printHelper(args...);
    }

    auto printHelper()
    {
    }

    namespace variadicIndices
    {

        template <typename T, typename... P>
        auto printElements(const T &container, P... idx)
        {
            printHelper(container[idx]...);
        }

        // NOTE 9.2.1 Non type template paramter pack can also be parameter pack , e.g. int
        template <int... idx, typename P>
        auto printIndex(const P &container)
        {
            printHelper(container[idx]...);
        }

    } // namespace variadicIndices

    /**
     * NOTE 9.3 Usage of variadic template : Variadic class template
     *
     * e.g. Implementation of tuple && variant
     */
    namespace variadicClassTemplates
    {
        // NOTE 9.3.1 Declaration of tuple and variant
        template <typename... T>
        class Tuple;
        template <typename... T>
        class Variant;

        // NOTE 9.3.2 Meta-programming
        template <int... idx>
        struct Indices
        {
        };

        template <int... idx, typename T>
        auto printByIndex(T ct, Indices<idx...>)
        {
            printHelper(std::get<idx>(ct)...);
        }

    } // namespace variadicClassTemplates

    /**
     * NOTE 9.4 Usage of variadic template : Deduction guide
     */
    namespace variadicDeductionGuide
    {
#if _HAS_CXX17
        template <class _Ty, size_t _Size>
        class array;
        template <class _First, class... _Rest>
        struct _Enforce_same
        {
            static_assert(conjunction_v<is_same<_First, _Rest>...>,
                          "N4687 26.3.7.2 [array.cons]/2: "
                          "Requires: (is_same_v<T, U> && ...) is true. Otherwise the program is ill-formed.");
            using type = _First;
        };

        template <class _First, class... _Rest>
        array(_First, _Rest...) -> array<typename _Enforce_same<_First, _Rest...>::type, 1 + sizeof...(_Rest)>;
#endif // _HAS_CXX17

    } // namespace variadicDeductionGuide

    namespace variadicBaseClassAndUsing
    {
        class Customer
        {
        private:
            std::string name;

        public:
            Customer(std::string const &n) : name(n) {}
            std::string getName() const { return name; }
        };
        struct CustomerEq
        {
            bool operator()(Customer const &c1, Customer const &c2) const
            {
                return c1.getName() == c2.getName();
            }
        };
        struct CustomerHash
        {
            std::size_t operator()(Customer const &c) const
            {
                return std::hash<std::string>()(c.getName());
            }
        };
        // define class that combines operator() for variadic base classes:
        template <typename... Bases>
        struct Overloader : Bases...
        {
            using Bases::operator()...; // OK since C++17
        };

    } // namespace variadicBaseClassAndUsing

    namespace application
    {
        // e.g. 1. Operate on each element of the parameter pack , e.g. add space or add 1
        template <typename T>
        class AddSpace
        {
        private:
            T const &ref; // refer to argument passed in constructor
        public:
            AddSpace(T const &r) : ref(r)
            {
            }
            friend std::ostream &operator<<(std::ostream &os, AddSpace<T> s)
            {
                return os << s.ref << ' '; // output passed argument and a space
            }
            ~AddSpace()
            {
            }
        };
        template <typename... T>
        auto addOne(T const &...arg)
        {
            return (... + (1 + arg));
        }

        template <typename... T>
        auto addOneAndPrint(T const &...arg)
        {
            std::cout << addOne(arg...);
        }

        template <typename... Args>
        void printWithSpace(Args... args)
        {
            (std::cout << ... << AddSpace(args)) << '\n';
        }

        //  e.g. 2. Define binary tree structure and traverse helpers:
        struct Node
        {
            int value;
            Node *left;
            Node *right;
            Node(int i = 0) : value(i), left(nullptr), right(nullptr)
            {
            }
            //...
        };
        auto left = &Node::left;
        auto right = &Node::right;

        // e.g. 3. traverse tree, using fold expression:
        template <typename T, typename... TP>
        Node *traverse(T np, TP... paths)
        {
            return (np->*...->*paths); // np ->* paths1 ->* paths2 ...
        }

        template <typename T1, typename... TN>
        constexpr bool isHomogeneous(T1, TN...)
        {
            return (std::is_same<T1, TN>::value && ...); // since C++17
        }

    } // namespace exampleAddSpace

} // namespace chapter4

int main()
{
    {
        using namespace Cpp17Features;
        Person alex{12, "Alex"};
        auto [a, b] = alex;
        std::cout << "The name of person is " << b << std::endl;
        Operation<Operators::Mut> operationMutiple;
        auto theSum = operationMutiple(1, 5L);
        assert(5 == theSum);
    }

    // chapter2
    {
        using namespace chapter2;
        // Constructor deduction
        {
            auto stringStack = new Stack{"vvvv"};
            auto holder = std::shared_ptr<std::remove_pointer_t<decltype(stringStack)>>(stringStack);
            holder->showElementType();
        }
        // Templatized aggregates
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
        assert(!(std::is_same_v<decltype(unsignedStack.size()), decltype(longStack.size())>));
        greeting<3>();
        // NOTE static type
        static const char msg[] = "World";
        greeting<msg, 10>();
        constexpr int cout = 0;
        DecltypeAuto<cout> bb;
        bb.operate();
    }
    // chapter4
    {
        using namespace chapter4;
        print(1, 3, " Hello world");
        overload::print(1, 3, " Hello world");
        usageOfSizeof::print("One", "Two", " Hello world", "VV", "MM");
        {
            using namespace variadicExpression;
            constexpr auto additional = 10;
            assert(calcSum<additional>(1, 2, 3, 4, 5, 6, 7) - calcSum<0>(1, 2, 3, 4, 5, 6, 7) == additional);
            assert(calcDouble(1, 2, 3) == 12);
            assert(addOne(1, 2, 3) == 9);
        }
        {
            using namespace variadicIndices;
            // NOTE Deduction guide performed in the standard library
            std::list numbeList = {12, 3.0};
            std::vector array = {"HelloElementsPrinter\n", "World\n", "Alex\b", "Alice\b", "Mike\n"};
            printElements(array, 1, 2, 4);
            printIndex<0, 1, 2>(array);
        }
        {
            using namespace variadicClassTemplates;
            auto testTuple = std::make_tuple("HelloTuplePrinter\n", 11L, 9.0, 3, std::string("World"));
            std::array<int, 5> testArray = {1, 3, 7, 8, 9};
            printByIndex(testTuple, Indices<0, 1, 3>());
            printByIndex(testArray, Indices<0, 1, 3>());
        }
        {
            using namespace variadicBaseClassAndUsing;
            using CustomerOP = Overloader<CustomerHash, CustomerEq>;
            std::unordered_set<Customer, CustomerHash, CustomerEq> coll1;
            std::unordered_set<Customer, CustomerOP, CustomerOP> coll2;
        }
        {
            using namespace application;
            Node *root = new Node{0};
            root->left = new Node{1};
            root->left->right = new Node{2};
            root->left->right->left = new Node{7};
            Node *node = traverse(root, left, right, left);
            assert(node->value == 7 && 7 == (root->*(left)->*(right)->*(left))->value);
            assert(isHomogeneous(1, 2, 3, 4, "1") == false);
            assert(isHomogeneous(1, 2, 3, 4, 6.1) == false);
            assert(isHomogeneous(1, 2, 3, 4, 6) == true);
            printWithSpace(12, 4, 5, 6);
            addOneAndPrint(1, 2, 3, 4, 5);
        }
    }

    return 0;
}

#ifndef TEMPLATE_COMPLETE_GUIDE_H
#define TEMPLATE_COMPLETE_GUIDE_H

#pragma once

#include "std.h"

namespace TemplateCompleteGuide
{
    namespace constexprValidation
    {

        struct FoolArrayList
        {
        };
        struct MoreFoolArrayList : FoolArrayList
        {
        };
        struct Animal
        {
            virtual bool isAnimal() const
            {
                return true;
            }
            virtual void speak()
            {
            }
        };
        struct Dog : Animal
        {
            bool isDog() const
            {
                return true;
            }
            void speak()
            {
                std::cout << "Wang\n";
            }
        };

        template <typename T>
        constexpr auto isArray()
        {
            return std::string::npos != std::string(typeid(T).name()).find("ArrayList");
        }

        struct Cat : Animal
        {
            bool isCat() const
            {
                return true;
            }
            void speak()
            {
                std::cout << "Meow\n";
            }
        };
        struct DispatchHelperBase
        {
            constexpr auto operator()(Animal *animal)
            {
            }
        };

        template <typename Q>
        struct DispatchHelper : DispatchHelperBase
        {
            constexpr DispatchHelper(Q * = nullptr)
            {
            }
            constexpr void operator()(Animal *animal)
            {
                if (auto devi = dynamic_cast<Q *>(animal))
                    devi->speak();
            }
        };
        template <typename T>
        const auto adapter = [](const T &v)
        {
            return isArray<T>();
        };

    } // namespace constexprTest

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

        /**
         * NOTE 6. Another way to overwrite variadic template
         * NOTICE The empty/more specified version shall appear before more general ones
         *
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

            namespace preferred
            {
                auto detailedPrint()
                {
                }
                template <typename T, typename... Ts>
                auto detailedPrint(T const &arg, Ts const &...args)
                {
                    std::cout << "Calling generic printer:"
                              << "\n\btype:" << typeid(arg).name() << "\n\bvalue:" << arg << std::endl;
                    detailedPrint(args...);
                }
                template <typename... T>
                auto detailedPrint(int arg, T const &...args)
                {
                    std::cout << "Calling int printer: " << arg << std::endl;
                    detailedPrint(args...);
                }
                template <typename... T>
                auto detailedPrint(double arg, T const &...args)
                {
                    std::cout << "Calling double printer: " << arg << std::endl;
                    detailedPrint(args...);
                }
                template <typename... T>
                auto detailedPrint(const std::string &arg, T const &...args)
                {
                    std::cout << "Calling string printer: " << arg << std::endl;
                    detailedPrint(args...);
                }

            } // namespace preferred

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

        auto printHelper()
        {
        }

        template <typename T, typename... Ts>
        auto printHelper(const T &arg0, const Ts &...args)
        {
            std::cout << arg0;
            printHelper(args...);
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

} // namespace TemplateCompleteGuide

#endif
#include "utils.h"
#include "template_complete_guide.h"
#include "cpp_features.h"

int main()
{

    {
        using namespace CppFeatures;
        Person alex{12, "Alex"};
        auto [a, b] = alex;
        std::cout << "The name of person is " << b << std::endl;
        Operation<Operators::Mut> operationMutiple;
        auto theSum = operationMutiple(1, 5L);
        assert(5 == theSum);
    }

    // chapter2
    {
        using namespace TemplateCompleteGuide::chapter2;
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
        using namespace TemplateCompleteGuide::chapter3;
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
        using namespace TemplateCompleteGuide::chapter4;
        print(1, 3, " Hello world");
        overload::print(1, 3, " Hello world");
        overload::preferred::detailedPrint(1, 3, 8.8, " Hello world", std::string("Hello\n"));
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
        std::cout << u8"\0xE2 0x80 0xA8";
    }

    {
        using namespace TemplateCompleteGuide::constexprValidation;
        int *b = new int(0);
        std::cout << "\nVerifying\n";
        std::cout << isArray<decltype(2)>();
        std::cout << isArray<decltype(b)>();
        MoreFoolArrayList cc;
        std::cout << isArray<decltype(new FoolArrayList)>();
        std::cout << isArray<decltype(cc)>();
        std::cout << typeid(int).name() << std::endl;
        std::cout << typeid(FoolArrayList).name() << std::endl;

        if (typeid(int).name() == "int")
        {
            std::cout << "It is in\n";
        }
        auto bb = int{};
    }

    return 0;
}

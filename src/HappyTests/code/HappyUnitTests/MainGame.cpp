//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 03/11/2012
#include "HappyUnitTestsPCH.h" 

#include "MainGame.h"
#include "List.h"
#include "Random.h"

#include "NodeGraph.h"
#include "NodeGraphNode.h"

#include <JsonFileReader.h>
#include <JsonFileWriter.h>

namespace hut {

MainGame::MainGame()
{
}


MainGame::~MainGame()
{
}

void MainGame::init()
{
    //listUnitTest();
    //nodeGraphUnitTest();
    //guidUnitTest();
    //mat33UnitTest();

    jsonUnitTest();
    HAPPYENGINE->quit();
}

void MainGame::destroy()
{

}

void MainGame::tick( float dTime )
{
    he::ge::Game::tick(dTime);
}

void MainGame::guidUnitTest()
{
    for (size_t i(0); i < 1000; ++i)
    {
        he::Guid g(he::Guid::generateGuid());
        HE_INFO("Guid: %s", g.toString().c_str());
        he::Guid g2(g.toString().c_str());
        HE_INFO("Guid2: %s", g2.toString().c_str());
        HE_ASSERT(g2 == g, "g2 != g! %s != %s", g.toString().c_str(), g2.toString().c_str());
        HE_ASSERT(g == g2, "g != g2! %s != %s", g2.toString().c_str(), g.toString().c_str());
    }
}

void MainGame::nodeGraphUnitTest()
{
#pragma region GraphClasses
    enum Type
    {
        Type_Int,
        Type_Float,
    };
    enum NodeType
    {
        NodeType_ConstInt,
        NodeType_AddInt,
        NodeType_PrintInt,
    };
    struct InputOutput
    {
        InputOutput(): type(Type_Int), name("Error"), data(nullptr), dataSize(0) {}
        ~InputOutput() { he_free(data); }
        InputOutput(const InputOutput& other): type(other.type), name(other.name), data(he_malloc(other.dataSize)), dataSize(other.dataSize) 
        { setData(other.data, other.dataSize); }
        InputOutput& operator=(const InputOutput& other)
        {
            type = other.type;
            name = other.name;
            setData(other.dataSize);
            setData(other.data, other.dataSize);
            return *this;
        }
        Type type;
        he::String name;
        void* data;
        size_t dataSize;
        void setData(size_t size)
        {
            he_free(data);
            dataSize = size;
            data = he_malloc(dataSize);
            he_memset(data, 0, dataSize);
        }
        void setData(void* copyData, size_t size)
        {
            HE_ASSERT(size <= dataSize, "Invalid set data");
            he_memcpy(data, copyData, size);
        }
    };
    class NodeBase : public he::NodeGraphNode<InputOutput, InputOutput> 
    { 
    public:
        virtual ~NodeBase() {} 

        virtual bool evaluate(he::NodeGraphError<InputOutput, InputOutput>& error) = 0;
        virtual bool canConnect(const InputOutput& fromOutput, const InputOutput& toInput, he::NodeGraphError<InputOutput, InputOutput>& /*error*/) const
        {
            return fromOutput.type == toInput.type;
        }
        virtual NodeType getType() = 0;
    };
    struct IntNode : public NodeBase
    {
        IntNode(int val): value(val) 
        {
            InputOutput conn;
            conn.name = "Int";
            conn.type = Type_Int;
            conn.setData(sizeof(int));
            addOutput(conn);
        } 
        virtual ~IntNode() {}

        virtual bool evaluate(he::NodeGraphError<InputOutput, InputOutput>& /*error*/)
        {
            getOutput(0).setData(&value, sizeof(int));
            return true;
        }
        virtual NodeType getType() { return NodeType_ConstInt; }

        int value;
    };
    struct AddNode : public NodeBase
    {
        AddNode()
        {
            InputOutput conn;
            conn.name = "Int";
            conn.type = Type_Int;
            conn.setData(sizeof(int));
            addOutput(conn);
            addInput(conn);
            addInput(conn);
        } 
        virtual ~AddNode() {}

        virtual bool evaluate(he::NodeGraphError<InputOutput, InputOutput>& /*error*/)
        {
            int a = *static_cast<int*>(getInputConnection(0).node->getOutput(getInputConnection(0).connecter).data);
            int b = *static_cast<int*>(getInputConnection(1).node->getOutput(getInputConnection(1).connecter).data);
            int out = a + b;
            std::cout << a << " + " << b;
            getOutput(0).setData(&out, sizeof(int));

            return true;
        }
        virtual NodeType getType() { return NodeType_AddInt; }
    };
    struct PrintIntNode : public NodeBase
    {
        PrintIntNode()
        {
            InputOutput conn;
            conn.name = "Int";
            conn.type = Type_Int;
            conn.setData(sizeof(int));
            addInput(conn);
        } 
        virtual ~PrintIntNode() {}

        virtual bool evaluate(he::NodeGraphError<InputOutput, InputOutput>& /*error*/)
        {
            int a = *static_cast<int*>(getInputConnection(0).node->getOutput(getInputConnection(0).connecter).data);
            std::cout << " = " << a << "\n";
            return true;
        }
        virtual NodeType getType() { return NodeType_PrintInt; }
    };
#pragma endregion
    
    he::NodeGraph<InputOutput, InputOutput> graph;

    IntNode const1(5);
    IntNode const2(10);
    IntNode const3(2);
    AddNode addNode1;
    AddNode addNode2;
    PrintIntNode printNode1;
    PrintIntNode printNode2;

    he::NodeGraphError<InputOutput, InputOutput> error;
    addNode1.connectToInput(&const1, 0, 0, error);
    addNode1.connectToInput(&const2, 0, 1, error);
    addNode2.connectToInput(&addNode1, 0, 0, error);
    addNode2.connectToInput(&const3, 0, 1, error);
    printNode1.connectToInput(&addNode1, 0, 0, error);
    printNode2.connectToInput(&addNode2, 0, 0, error);
    graph.addRootNode(&printNode1);
    graph.addRootNode(&printNode2);
    he::ObjectList<he::NodeGraphError<InputOutput, InputOutput>> errors;
    graph.evalute(errors);

    HE_ASSERT(errors.size() == 0, "Errors occurred");

}

void MainGame::listUnitTest()
{   
    // Primitive List
    {
        he::Random rand;
        he::PrimitiveList<int> list;   
        he::PrimitiveList<int>::Sorter sorter([](const int& a, const int& b) -> int { if (a == b) return 0; else if (a < b) return -1; else return 1; });

        for (size_t iter(0); iter < 10; ++iter)
        {
            std::cout << "\n******** Iteration " << iter << "********\n";
            /*if (iter == 0)
            {
                list.add(39);
                list.add(31);
                list.add(10);
                list.add(7);
                list.add(37);
                list.add(15);
                list.add(31);
                list.add(39);
                list.add(23);
                list.add(14);
            }
            else
            {*/
                for (size_t i(0); i < 10; ++i)
                {
                    list.add(rand.nextInt(0, 40));
                }
            //}
            list.trim();

            list.forEach([](const int& element){ std::cout << element << ", "; });
            std::cout << "\n\n";

            size_t index;
            list.find(list[5], index);
            HE_ASSERT(list[index] == list[5], "ERROR");
            std::cout << "Element 5 = " << list[index] << "\n\n";

            list.removeAt(5);
            std::cout << "Removed element @5 = " << "\n";
            list.forEach([](const int& element){ std::cout << element << ", "; });
            std::cout << "\n\n";

            list.insert(100, 2);
            std::cout << "Inserted 100 @2 = " << "\n";
            list.forEach([](const int& element){ std::cout << element << ", "; });
            std::cout << "\n\n";

            list.remove(100);
            std::cout << "Removed 100" << "\n";
            list.forEach([](const int& element){ std::cout << element << ", "; });
            std::cout << "\n\n";

            list[2] = 20;
            std::cout << "Changed @2 to 20" << "\n";
            list.forEach([](const int& element){ std::cout << element << ", "; });
            std::cout << "\n\n";

            list.orderedRemove(20);
            std::cout << "Ordered Removed 20" << "\n";
            list.forEach([](const int& element){ std::cout << element << ", "; });
            std::cout << "\n\n";

            list.sort(sorter);
            std::cout << "Sorted list" << "\n";
            list.forEach([](const int& element){ std::cout << element << ", "; });
            std::cout << "\n\n";

            list.binFind(list[5], sorter, index);
            HE_ASSERT(list[index] == list[5], "ERROR");
            std::cout << "Element 5 = " << list[index] << "\n\n";

            list.clear();
        }
        list.trim();


        for (size_t i(0); i < 10000; ++i)
        {
            list.add(rand.nextInt(0, 1000));
        }
        list.sort(sorter);
        std::cout << list[0];
        for (size_t i(1); i < 10000; ++i)
        {
            HE_ASSERT(list[i-1] <= list[i], "List sort fail");
            std::cout << ", " << list[i];
        }
        std::cout << "\n\n";
    }

    // ObjectList
    {
        struct TestStruct
        {
            TestStruct() { std::cout << "List Allocated\n"; }
            explicit TestStruct(int test): data(test) { std::cout << "Allocated by user: " << test << "\n"; }

            ~TestStruct() { std::cout << "Deallocated\n"; }

        private:
            int data;
        };

        he::ObjectList<TestStruct> list;
        for (int i(1); i < 10; ++i)
        {
            list.add(TestStruct(i));
        }
    }
}

void printMat33(const he::mat33& mat)
{
    HE_INFO("|%6.3f %6.3f %6.3f|", mat(0, 0), mat(0, 1), mat(0, 2));
    HE_INFO("|%6.3f %6.3f %6.3f|", mat(1, 0), mat(1, 1), mat(1, 2));
    HE_INFO("|%6.3f %6.3f %6.3f|", mat(2, 0), mat(2, 1), mat(2, 2));
}
void printVec3(const he::vec3& vec)
{
    HE_INFO("(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
}

void MainGame::mat33UnitTest()
{
    using namespace he;
    mat33 test1(mat33::createRotation3D(vec3::forward, piOverFour / 2) * mat33::createRotation3D(vec3::up, piOverTwo) * mat33::createRotation3D(vec3::right, piOverFour));
    HE_INFO("Test1:");
    printMat33(test1);
    HE_INFO("result:");
    vec3 euler(test1.getEulerAngles());
    printVec3(euler);
    HE_INFO("result2:");
    mat33 result2(mat33::createRotation3D(euler));
    printMat33(result2);
    
}

//////////////////////////////////////////////////////////////////////////
// Json test
//////////////////////////////////////////////////////////////////////////
struct JsonConfig
{
    JsonConfig(): m_Vec4Test(2.0f, 0.f, -243.0f, 0.256f), m_Vec2Test(0.0f, 1.0f), 
        m_StringTest("Hallo ik ben een string"), m_GuidTest(he::Guid::generateGuid()) {}
    he::vec4 m_Vec4Test;
    he::vec2 m_Vec2Test;
    he::String m_StringTest;
    he::Guid m_GuidTest;
    struct ObjTest
    {
        ObjTest(): m_Float(235.1245f), m_Int8Test(-69), m_Uint32Test(12156483) {}
        float m_Float;
        he::int8 m_Int8Test;
        he::uint32 m_Uint32Test;
        void visit(he::io::StructuredVisitor* visitor)
        {
            visitor->visit(HTFS::strFloatTest, m_Float);
            visitor->visit(HTFS::strInt8Test, m_Int8Test, "//int8 test");
            visitor->visit(HTFS::strUInt32Test, m_Uint32Test);
        }
    };
    ObjTest m_ObjectTest;

    void visit(he::io::StructuredVisitor* visitor)
    {
        visitor->visit(HTFS::strVec4Test, m_Vec4Test, "//Vec4 Testje");
        visitor->visit(HTFS::strVec2Test, m_Vec2Test);
        visitor->visit(HTFS::strStringTest, m_StringTest);
        visitor->visit(HTFS::strGuidTest, m_GuidTest);
        if (visitor->enterNode(HTFS::strObjectTest, "//Object Test"))
        {
            m_ObjectTest.visit(visitor);
            visitor->exitNode(HTFS::strObjectTest);
        }
    }
};

void MainGame::jsonUnitTest()
{
    using namespace he;

    JsonConfig config;
    JsonConfig configCopy(config);

    {
        he::io::JsonFileWriter writer;
        writer.open(he::Path("jsonTest.txt"));
        config.visit(&writer);
        writer.close();
    }
    {
        he::io::JsonFileReader reader;
        if (reader.open(he::Path("jsonTest.txt")))
        {
            config.visit(&reader);
            reader.close();
        }
    }
    {
        he::io::JsonFileWriter writer;
        writer.open(he::Path("jsonTest2.txt"));
        config.visit(&writer);
        writer.close();
    }
}

} //end namespace

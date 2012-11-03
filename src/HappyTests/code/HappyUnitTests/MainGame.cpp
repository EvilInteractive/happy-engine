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

namespace hut {

MainGame::MainGame()
{
}


MainGame::~MainGame()
{
}

void MainGame::init()
{
}

void MainGame::load()
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

    HAPPYENGINE->quit();
}

void MainGame::tick( float dTime )
{
    he::ge::Game::tick(dTime);
}

} //end namespace

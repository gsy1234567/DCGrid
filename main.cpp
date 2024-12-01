#include "DCGrid.hpp"
#include <iostream>
#include <bit>
#include "mortonCode.hpp"


int main() {
    gsy::DenseGrid<u32, float, 3> denseGrid {0.05, {1000, 1000, 1000}, {0,0,0}};
    gsy::SpaceHash<u32, float, 3> spaceHash {denseGrid};
    spaceHash.insert_rd({0, 0, 0}, 0);
    spaceHash.insert_rd({0.05, 0, 0}, 1);
    spaceHash.insert_rd({0, 0.05, 0}, 2);
    spaceHash.insert_rd({0.05, 0.05, 0}, 3);
    spaceHash.insert_rd({0, 0, 0.05}, 4);
    spaceHash.insert_rd({0.05, 0, 0.05}, 5);
    spaceHash.insert_rd({0, 0.05, 0.05}, 6);
    spaceHash.insert_rd({0.05, 0.05, 0.05}, 7);

    std::cout << *spaceHash.find_rd({0, 0, 0}) << std::endl;
    std::cout << *spaceHash.find_rd({0.05, 0, 0}) << std::endl;
    std::cout << *spaceHash.find_rd({0, 0.05, 0}) << std::endl;
    std::cout << *spaceHash.find_rd({0.05, 0.05, 0}) << std::endl;
    std::cout << *spaceHash.find_rd({0, 0, 0.05}) << std::endl;
    std::cout << *spaceHash.find_rd({0.05, 0, 0.05}) << std::endl;
    std::cout << *spaceHash.find_rd({0, 0.05, 0.05}) << std::endl;
    std::cout << *spaceHash.find_rd({0.05, 0.05, 0.05}) << std::endl;

    std::cout << *spaceHash.find_right_rd({0, 0, 0}) << std::endl;
    std::cout << *spaceHash.find_front_rd({0, 0, 0}) << std::endl;
    std::cout << *spaceHash.find_top_rd({0, 0, 0}) << std::endl;

    std::cout << *spaceHash.find_left_rd({0.05, 0.05, 0.05}) << std::endl;
    std::cout << *spaceHash.find_back_rd({0.05, 0.05, 0.05}) << std::endl;
    std::cout << *spaceHash.find_bottom_rd({0.05, 0.05, 0.05}) << std::endl;


    return 0;
} 
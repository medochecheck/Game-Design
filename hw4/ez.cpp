#include <iostream>
#include <cstdint>
#include <cstdlib>

int main() {

/*
    int a = 10;
    int b = 20;
    int c = 30;
*/

    //int a, b, c;
    int a = 0;
    int b = 0;
    int c = 0;   
    
    std::cout << "Enter three integers:\n";
    std::cin >> a >> b >> c;


    std::cout << "Stack addresses:\n";
    std::cout << "Address of a: " << &a << "\n";
    std::cout << "Address of b: " << &b << "\n";
    std::cout << "Address of c: " << &c << "\n";

    uint32_t *ptr1 = (uint32_t *)std::calloc(250, sizeof(uint32_t));
    char *ptr2 = (char *)std::calloc(1000000, sizeof(char));

    std::cout << "\nHeap addresses:\n";
    std::cout << "Address of ptr1 (250 uint32_t): " << static_cast<void*>(ptr1) << "\n";
    std::cout << "Address of ptr2 (1000000 char): " << static_cast<void*>(ptr2) << "\n";

    std::free(ptr1);
    std::free(ptr2);

    return 0;
}

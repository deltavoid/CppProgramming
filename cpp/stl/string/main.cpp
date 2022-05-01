#include <cstdio>
#include <cstdlib>


#include <string>
#include <iostream>

#include <algorithm>


int main(int argc, char** argv, char** env)
{

    std::string s1("hello");
    std::string s2 = s1;
    std::string s3 = "world";

    std::cout << "s1: " << s1 << std::endl;
    std::cout << "s2: " << s2 << std::endl;
    std::cout << "s3: " << s3 << std::endl;
    std::cout << std::endl;


    
    s1 += " ";
    s1.append(s3);    
    std::cout << "s1: " << s1 << std::endl;
    std::cout << std::endl;


    s2.append(1, ' ');
    s2.append(1, 'c');
    std::cout << "s2: " << s2 << std::endl;
    std::cout << std::endl;


    printf("s1.size(): %d\n", s1.size());

    for (int i = 0; i < s1.size(); i++)
        printf("%c ", s1[i]);
    printf("\n");
    printf("\n");


    std::string::iterator it;
    for (it = s2.begin(); it != s2.end(); it++)
        printf("%c ", *it);
    printf("\n");


    std::reverse(s2.begin(), s2.end());
    std::cout << "reversed s2: " << s2 << std::endl;
    std::cout << std::endl;


    return 0;
}
#include <cstdio>
#include <cstdlib>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
using namespace std;

const char input_file[] = "input.txt";
const char output_file[] = "output.txt";



int main()
{
    ifstream fin(input_file);
    ofstream fout(output_file);

    if  (!fin || !fout)
    {   perror("bad files");
        return 1;
    }


    istream_iterator<string> is(fin);
    istream_iterator<string> eof;

    vector<string> text;
    copy(is, eof, back_inserter(text));

    sort(text.begin(), text.end());

    ostream_iterator<string> os(fout, " ");
    copy(text.begin(), text.end(), os);





    // printf("hello world\n");
    return 0;
}
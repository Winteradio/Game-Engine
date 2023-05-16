#include <iostream>
#include <filesystem>

using namespace std;

int main()
{
    cout << filesystem::current_path() << endl;
    return 0;
};

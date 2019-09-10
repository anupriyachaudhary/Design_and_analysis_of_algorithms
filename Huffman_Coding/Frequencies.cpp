#include <iostream>
#include <string>
using namespace std;

#define ASCII_SIZE 256

int main()
{
	// initialize frequency array for ascii characters
    int arr[ASCII_SIZE];
    
    for(int i = 0; i < ASCII_SIZE; i++)
        arr[i] = 0;
   
	unsigned char ch;
    while(true)
    {
		cin.read((char*)&ch, 1);
		if (cin.eof())
			break;
        int j = ch;
		if (j < 0 || j > (ASCII_SIZE - 1))
		{
			continue;
		}
        arr[j] += 1;
    }
    
    for(int k = 0; k < ASCII_SIZE; k++)
    {
        if(arr[k] != 0)
            cout << k << '\t' << arr[k] << endl;
    }
    return 0;
}

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    ifstream code(argv[1]);
    
    string array[256];
    for(int i = 0; i <= 255; i++)
        array[i] = "";
    
    for(string line; getline(code, line);)
    {
        string str1, str2 = "";
        unsigned i;
        for (i = 0; line[i] != '\t'; i++)
            str1 = str1 + line[i];
        
        for (i = i+1; line[i] != '\0'; i++)
            str2 = str2 + line[i];
        
        array[stoi(str1)] = str2;
    }

	code.close();
    
    unsigned char ch;
    string buffer;
	unsigned char b = '\0';
    while(true)
    {
		// read 1 byte from stream
		cin.read((char*)&ch, 1);
		
		if (cin.eof())
		{
            if(buffer.size() == 0)
            {
                cout.write((char*)&b, 1);
                b = '\0';
            }
			else if (buffer.size() > 0)
			{
				for (unsigned i = 0; i < buffer.size(); i++)
				{
					if (buffer[i] == '1')
						b = b + (unsigned char)pow(2, 7 - i);
				}
				cout.write((char*)&b, 1);
				b = '\0';
			}

			// add one more byte containing numbers of bits to be read from previous byte
			b = (unsigned char)buffer.size();
			cout.write((char*)&b, 1);
			break;
		}

		buffer = buffer + array[int(ch)];

		// check if we have 8 or more bytes
		if (buffer.size() >= 8)
		{
			// write in chunks of 8 bits till we have less than 8 bits left
			do
			{
				string oneByte = buffer.substr(0, 8);

				for (unsigned i = 0; i < oneByte.size(); i++)
				{
					if (oneByte[i] == '1')
						b = b + (unsigned char)pow(2, 7 - i);
				}
				cout.write((char*)&b, 1);
				b = '\0';

				buffer.erase(0, 8);
			} while (buffer.size() >= 8);
		}
    }
	return 0;
}

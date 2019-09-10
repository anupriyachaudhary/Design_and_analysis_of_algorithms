#include <iostream>
#include <vector>
#include <string.h>
using namespace std;

struct Position {
    int x_pos;
    int y_pos;
};

void print_LCS(vector<vector<char> >& b, string x, int m, int n);
void LCS(string x, string y, string arg);
void print_allLCS(vector<vector<int> >& c,vector<vector<char> >& b, string x, string y, int m, int n, int current_LCS, int max_LCS, vector<Position> &output);
void print_output(vector<Position> &output);

int main(int argc,char* argv[])
{
    string arg = "";
    if (argc == 2)
        arg = argv[1];
    
    int num_test_cases = 0;
    int count = 0;
    for(string line; getline(cin, line);)
    {
        if (count == 0)
            num_test_cases = stoi(line);
        
        else if (count == num_test_cases + 1)
            break;
        
        else
        {
            string x = "", y = "";
            unsigned i;
            for (i = 0; line[i] != ' '; i++)
                x = x + line[i];
            
            for (i = i + 1; line[i] != '\0'; i++)
                y = y + line[i];
            LCS(x, y, arg);
        }
        count++;
    }
}

void LCS(string x, string y, string arg)
{
    int m = int(x.length()), n = int(y.length());
    vector<vector<int> > c(m + 1, vector<int>(n + 1));
    vector<vector<char> > b(m + 1, vector<char>(n + 1));
    
    
    for(int i = 1; i <= m; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if (x[i-1] == y[j-1])
            {
                c[i][j] = c[i - 1][j - 1] + 1;
                b[i][j] = 'D';
            }
            
            else if (c[i - 1][j] >= c[i][j - 1])
            {
                c[i][j] = c[i - 1][j];
                b[i][j] = 'U';
            }
            
            else
            {
                c[i][j] = c[i][j - 1];
                b[i][j] = 'L';
            }
        }
    }
    
    int length_lcs = c[m][n];
    
    if (arg == "-all")
    {
        vector<Position> output;
        
        if (length_lcs == 0)
        {
            print_output(output);
            return;
        }
        
        print_allLCS(c, b, x, y, m, n, length_lcs, length_lcs, output);
        cout << endl;
    }
    else
    {
        cout << length_lcs << " ";
        
        if (length_lcs == 0)
        {
            cout << endl;
            return;
        }
        
        print_LCS(b, x, m, n);
        cout << endl;
    }
}

void print_LCS(vector<vector<char> >& b, string x, int m, int n)
{
    if (m == 0 or n == 0)
        return;
    
    if (b[m][n] == 'D')
    {
        print_LCS(b, x, m-1, n-1);
        cout << x[m-1];
    }
    else if (b[m][n] == 'U')
        print_LCS(b, x, m-1, n);
    else
        print_LCS(b, x, m, n-1);
    
}

void print_allLCS(vector<vector<int> >& c,vector<vector<char> >& b, string x, string y, int m, int n, int current_LCS, int max_LCS, vector<Position> &output)
{
    if (c[m][n] == 0)
        return;
    
    bool vertical_end = false;
    for(int i = m; i > 0; i--)
    {
        if (c[i][n] == current_LCS && b[i][n] == 'D')
        {
            Position pos;
            pos.x_pos = i;
            pos.y_pos = n;
            output.push_back(pos);
            print_allLCS(c, b, x, y, i-1, n-1, current_LCS-1, max_LCS, output);
            if (output.size() == max_LCS)
                print_output(output);
            output.pop_back();
        }
        if (c[i][n] < current_LCS or i == 1)
        {
            vertical_end = true;
            break;
        }
    }
    
    bool horizontal_end = false;
    for(int j = n - 1; j > 0; j--)
    {
        if (c[m][j] == current_LCS && b[m][j] == 'D')
        {
            Position pos;
            pos.x_pos = m;
            pos.y_pos = j;
            output.push_back(pos);
            print_allLCS(c, b, x, y, m-1, j-1, current_LCS-1, max_LCS, output);
            if (output.size() == max_LCS)
                print_output(output);
            output.pop_back();
        }
        if (c[m][j] < current_LCS or j == 1)
        {
            horizontal_end = true;
            break;
        }
    }
    
    if (vertical_end && horizontal_end)
        print_allLCS(c, b, x, y, m-1, n-1, current_LCS, max_LCS, output);
}

void print_output(vector<Position> &output)
{
    cout << '(';
    for(int k = int(output.size())-1; k >= 0; --k)
    {
        cout << "<" << output[k].x_pos << ", " << output[k].y_pos << ">";
        if (k != 0)
            cout << ", ";
    }
    cout << ')' << endl;
}

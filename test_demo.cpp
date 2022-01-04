
// 2022/01/03

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::max;
using std::min;
using std::pair;
using std::string;
using std::vector;

struct TreeNode
{
    int _i;
    int _w;
    int _s;

    TreeNode(int i = -1, int w = -1, int s = -1) : _i(i), _w(w), _s(s) {}
    ~TreeNode() {}

    void PrintTreeNode() const
    {
        cout << "i: " << _i << " w: " << _w << " s: " << _s << endl;
    }
};

struct Path
{
    vector<TreeNode> _testPath;

    Path(int n, const vector<int> &w, const vector<int> &s)
    {
        if (w.size() != n + 1 || s.size() != n + 1)
        {
            throw "Vectors for construction is wrong";
        }
        _testPath.resize(n + 1); // use a redundancy space at the front of each array
        for (int ii = 1; ii <= n; ii++)
        {
            _testPath[ii]._i = ii; // assign serial number form 1 to n
            _testPath[ii]._w = w[ii];
            _testPath[ii]._s = s[ii];
        }
    }
    ~Path() {}
    void AddNode(TreeNode node)
    {
        _testPath.push_back(node);
    }
    void PrintPath() const
    {
        for (int ii = 1; ii < _testPath.size(); ii++)
        {
            _testPath[ii].PrintTreeNode();
        }
    }

    vector<int> CaluF(const vector<TreeNode> &path, int W)
    {

        if (path.size() == 0 || path.size() == 1)
        {
            throw "Empty, please cheak your path";
        }

        // initialize F array
        vector<int> F(path.size(), 0);    // 1 -> n
        F[1] = IntervalFuncS(path, 1, 1); // index: 1

        // construct F array
        for (int ii = 2; ii < path.size(); ii++)
        { // index: 2 -> n
            // search the min value accroding to the equation of transfer below
            // i.e. F[i]=min{F[j]+S(j+1)|j<i,W(j+1,i)<=W},
            // using method with O(n2) complexity

            cout << "ii: " << ii << " | ";
            int minVal = INT32_MAX;
            for (int jj = 0; jj < ii; jj++)
            {
                int intervalW = IntervalSumW(path, jj + 1, ii);
                if (intervalW <= W)
                {
                    minVal = min(minVal, F[jj] + IntervalFuncS(path, jj + 1, ii));
                    cout << "*"; // symbol "*" represent the variable "minVal" will be renewed in this block
                }
                cout << "W(" << jj + 1 << "," << ii << "):" << intervalW << " "; // print the interval sum of w
            }
            cout << "| F[" << ii << "]: " << minVal << endl;
            F[ii] = minVal;
        }

        return F;
    }

    int IntervalSumW(const vector<TreeNode> &path, int startInd, int endInd)
    {
        //* i.e. W(a,b)

        if (startInd > path.size() || endInd > path.size())
        {
            throw "Index out of range";
        }
        if (startInd > endInd)
        {
            throw "Range value error";
        }

        int sumW = 0;
        for (int ii = startInd; ii <= endInd; ii++)
        {
            sumW += path[ii]._w;
        }
        return sumW;
    }

    int IntervalFuncS(const vector<TreeNode> &path, int startInd, int endInd)
    {
        //* i.e. S(a,b)

        if (startInd > path.size() || endInd > path.size())
        {
            throw "Index out of range";
        }
        if (startInd > endInd)
        {
            throw "Range value error";
        }

        int maxS = INT32_MIN;
        for (int ii = startInd; ii <= endInd; ii++)
        {
            maxS = max(maxS, path[ii]._s);
        }
        return maxS;
    }

    pair<int, int> FuncS(const vector<TreeNode> &group)
    {
        // search in a group represented by a vector, i.e. C(k), return a pair to the caller
        if (group.size() == 0)
        {
            throw "Group empty";
        }
        int maxS = INT32_MIN, maxI = -1;
        for (int ii = 0; ii < group.size(); ii++)
        {
            if (maxS < group[ii]._s)
            {
                maxS = max(maxS, group[ii]._s);
                maxI = group[ii]._i;
            }
        }
        return {maxS, maxI};
    }
};

struct TestModule
{
    TestModule() {}
    ~TestModule() {}
    int testFunc(int W, int n, const vector<int> &w, const vector<int> &s)
    {
        try
        {
            Path path(n, w, s);
            path.PrintPath();
            vector<int> F = path.CaluF(path._testPath, W);
            for (int ii = 1; ii < F.size(); ii++)
            {
                cout << "F[" << ii << "]:" << F[ii] << " ";
            }
            cout << endl;
            return 0; // normal
        }
        catch (const char *exception)
        {
            // exception information will be represented as a character string
            cout << "Exception: " << exception << endl;
            return 1; // exception
        }
    }
};

int main()
{

    // test case 1
    int W1 = 10;
    int n1 = 7;
    vector<int> w1 = {-1, 1, 4, 5, 6, 3, 2, 7};
    vector<int> s1 = {-1, 2, 8, 9, 1, 4, 2, 6};

    // test case 2
    int W2 = 150;
    int n2 = 12;
    vector<int> w2 = {-1, 10, 14, 52, 62, 39, 28, 41, 89, 13, 34, 56, 32};
    vector<int> s2 = {-1, 21, 82, 93, 12, 54, 72, 63, 44, 67, 23, 12, 90};

    TestModule T;
    T.testFunc(W1, n1, w1, s1);
    T.testFunc(W2, n2, w2, s2);

    return 0;
}

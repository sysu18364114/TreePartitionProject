
// 2022/01/03

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <queue>
#include <stack>

using std::cout;
using std::endl;
using std::max;
using std::min;
using std::greater;
using std::pair;
using std::string;
using std::vector;
using std::deque;
using std::priority_queue;
using std::stack;

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

    vector<int> CaluFOn2(const vector<TreeNode> &path, int W)
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

    vector<int> CaluFOn(const vector<TreeNode> &path, int W)
    {

        if (path.size() == 0 || path.size() == 1)
        {
            throw "Empty, please cheak your path";
        }

        for(int ii=1;ii<path.size();ii++){ //TODO preprecessing to confirm the situation of each "j"
            
        }

        // initialize F array
        vector<int> F(path.size(), 0);    // 1 -> n
        F[1] = IntervalFuncS(path, 1, 1); // index: 1
        priority_queue<int,vector<int> ,greater<int> > cost;
        deque<int> J;
        J.push_front(0);
        for(int ii=2; ii < path.size(); ii++){ // F[1]->F[2]->F[3]->...->F[n]
            if(!J.empty()){
                while(!J.empty()&&IntervalSumW(path,J.front()+1,ii)>W){
                    J.pop_front();
                }
                while(!J.empty()&&path[J.back()]._s<=path[ii]._s){
                    J.pop_back();
                }
            }
            if(path[ii-1]._s>path[ii]._s){ // accord with the definition of s-maximal option
                J.push_back(ii-1);
                cost.push(F[ii-1]+path[ii]._s);
            }

        }

    }

    void Renew(){

    }

    void CaluMin(){

    }

    int SMaximalOption(const vector<TreeNode>& path,int j,int i){

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
            vector<int> F = path.CaluFOn2(path._testPath, W);
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

vector<int> AllNearestSmallerValues_SequentialAlg(const vector<int>& seq){
    if(seq.size()==0){
        throw "Empty vector";
    }

    vector<int> preVal(seq.size(),0);
    stack<int> S; // use stack to maintain the "nearest" value
    for(int ii=0;ii<seq.size();ii++){
        while(!S.empty()&&S.top()>=seq[ii]){ // last-in, first-out
            S.pop();
        }

        if(S.empty()){
            preVal[ii]=-1;
        }else{
            preVal[ii]=S.top();
        }

        S.push(seq[ii]);
    }

    return preVal;
}

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

    vector<int> seq={0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
    vector<int> res=AllNearestSmallerValues_SequentialAlg(seq);
    for(auto e:res){
        cout<<e<<" ";
    }cout<<endl;

    return 0;
}

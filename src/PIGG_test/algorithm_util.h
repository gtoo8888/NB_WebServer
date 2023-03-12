#ifndef _ALGORITHM_UTIL_
#define _ALGORITHM_UTIL_

// #include "C:\\Users\\Yan\\Desktop\\languguetest\\Cplusplustest\algorithm_util.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <numeric> // iota() 批量递增
//#include <iomanip>//<< setw(5)
using namespace std;

// 最大质因数
int gcd(int a,int b) { return b?gcd(b,a%b):a;}

// 最小公倍数
int lcm(int m,int g)  {
     return m/gcd(m,g)*g;
}

class PrintVector{
public:
	void operator ()(int val) {
		cout << val << " ";
	}
};
//	for_each(temp.begin(),temp.end(),printVector());

// 最早的方法
// void PrintVector(vector<int> v){
// 	for(auto it = v.begin();it != v.end();it++)
// 		cout << *it << " ";
// 	cout << endl; 
// }

// 常用的方法
// void PrintVector(vector<int> v){
// 	for(auto it : v)
// 		cout << it << " ";
// 	cout << endl;
// }

// 使用模板来简化
template<typename T>
void PrintVector(vector<T> v){
	for(auto it : v)
		cout << it << " ";
	cout << endl;
}
// 模板偏特化
// 对于string有特殊处理
// template<typename T>
void PrintVector(vector<string> v){
	for(auto it : v)
		cout << it << endl;
}

// void PrintSet(set<int> s) {
// 	for(set<int>::iterator it = s.begin();it != s.end();it++)
// 		printf("%d ",*it);
// 	cout << endl;	
// }

// void PrintSet(set<int> s) {
// 	for(auto it : s)
// 		cout << it << endl;
// 	cout << endl;	
// }

template<typename T>
void PrintSet(set<T> s) {
	for(auto it : s)
		cout << it << endl;
	cout << endl;	
}

// void PrintMap(map<int,int> m) {
// // 	for(auto it = m.begin();it != m.end();it++)
// // 		cout << (*it).first << " " << it->second << endl;
// 	for(auto it : m)
// 		printf("%d %d\n",it.first,it.second);
// 	cout << endl;	
// }

template<typename T,typename N>
void PrintMap(map<T,T> m) {
	for(auto it = m.begin();it != m.end();it++)
		cout << (*it).first << " " << it->second << endl;
	cout << endl;	
}

template<typename T>
void PrintUnorder_Set(unordered_set<T> m) {
	for(auto it : m)
		cout << it << " ";
	cout << endl;	
}

// void PrintUnorder_Map(unordered_map<char,int> m) {
// 	for(auto it : m)
// 		cout << it.first << " " << it.second << endl;
// 		// printf("%d %d\n",it.first,it.second);
// 	cout << endl;	
// }

template<typename T,typename N>
void PrintUnorder_Map(unordered_map<T,N> m) {
	for(auto it : m)
		cout << it.first << " " << it.second << endl;
	cout << endl;	
}


vector<int> NumSeparate(int n) {//每个位拆开
	vector<int> ans;
	int temp = n;
	while(temp != 0){
		ans.push_back(temp%10);
		temp = temp / 10;			
	}
	return ans;
}

template<typename T>
void PrintStack(stack<T> st){
	while(!st.empty()){
		cout << st.top() << " ";
		st.pop();
	}
	cout << endl; 
}

template<typename T>
void PrintQueue(queue<T> q){
	while(!q.empty()){
		cout << q.front() << endl;
		q.pop();
	}
	cout << endl; 
}
	
void PrintBool(bool val){
	if(val == true)		cout << "true" << endl;
	else				cout << "false" << endl;
	cout << endl;
}

// 最原始的方法
// void PrintVVector(vector<vector<int>>& v){
// 	for(auto iter = v.begin();iter != v.end();iter++){
// 		auto it_temp = *iter;
// 		for(auto it = it_temp.begin();it != it_temp.end();it++)
// 			cout << *it << " ";
// 		cout << endl;
// 	}
// 	cout << endl;
// }

// 常用的方法
// void PrintVVector(vector<vector<int>>& v){
// 	for(auto iter : v){
// 		auto it_tmp = iter;
// 		for(auto it : it_tmp)
// 			cout << it << " ";
// 		cout << endl;
// 	}
// 	cout << endl;
// }

// 使用模板简化函数
template<typename T>
void PrintVVector(vector<vector<T>> v){
	for(auto iter : v){
		auto it_tmp = iter;
		for(auto it : it_tmp)
			cout << it << " ";
		cout << endl;
	}
	cout << endl;
}


void PrintVVVector(vector<vector<vector<int>>> grid){
	int n = grid.size();
	int m = grid[0].size();
	int l = grid[0][0].size();
	int k = 0;
	for(int k = 0;k < l;k++){	// 把每一面的数组作为一个切片
		for(int i = 0;i < n;i++){
			for(int j = 0;j < m;j++){
				cout << grid[i][j][k] << " ";		
			}
			cout << endl;
		}	
		cout << endl;
	}
	cout << endl;
}

void PrintPriQueue(priority_queue<long,vector<int>,greater<long>> pri){
	while(!pri.empty()){
		cout << pri.top() << endl;
		pri.pop();
	}
}

struct ListNode{
	int val;
	ListNode *next;
	ListNode() : val(0), next(nullptr) {}
	ListNode(int x) : val(x), next(nullptr) {}
	ListNode(int x,ListNode *next) : val(x), next(next){}
};

//释放new开辟的空间，不进行会导致内存泄漏
void releaseNode(ListNode* node)
{
    ListNode* temp = NULL;
    if (node == NULL)
        cout << "链表节点内存为空\n";
    else
	    while (node != NULL){
	        temp = node;
	        node = node->next;
	        delete temp;
	        cout << "节点内存清除成功\n";
	    }
}

// 创建列表节点
ListNode* CreateListNode(vector<int> value){
	int len = value.size();
	ListNode* head = new ListNode(value[len -1]);
	for(int i = 1;i < len;i++)
		head = new ListNode(value[len-1-i],head);
	return head;
}

//输出链表 
void PrintListNode(ListNode* head){
	while(head != nullptr){
		cout << head -> val << " ";
		head = head -> next;
	}
	cout << endl;
}

//n & 1 == 1判断这一位是不是1
//n | 0 == 0判断这一位是不是0
//二进制输出
// 一定要把输入的数字强制转化成uint32_t的
// 如果是int类型,为负数的时候，会进入死循环，无法输出
void PrintBit(uint32_t n) {	// uint32_t保证了输入为负数时，不会报错
	string str;
	while(n != 0) {
		if(n & 1 == 1)  
			str.push_back('1');
		else   
			str.push_back('0');
		n >>= 1;
	}
	reverse(str.begin(),str.end());
	cout << str << endl;
}

int str_to_num(string str){
	reverse(str.begin(),str.end());
	// cout << str << endl;
	int num = 0;
	for(int i = 0;i < str.size();i++){	
		// 返回浮点数，如果不向上取整就有精度问题,9*100.00会变成999
		num += (str[i]-'0')*ceil(pow(10,i));
		// printf("%d %d %f\n",num,i,ceil(pow(10,i));
	}
	return num;
}

string num_to_str(int num){
	string str;
	while(num != 0){
		int tmp = num%10;
		str.push_back('0'+tmp);
		num /= 10;
	}
	reverse(str.begin(),str.end());
	return str;
}
		
void turnVVector(string str1){
	for(int i = 0;i < str1.size();i++){
		if(str1[i] == '[')
			str1[i] = '{';
		if(str1[i] == ']')
			str1[i] = '}';
		if(str1[i] == '\"')
			str1[i] = '\'';
	}
	cout << str1 << endl;
}



#endif /* _ALGORITHM_UTIL_ */


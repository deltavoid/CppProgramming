#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<stack>
#include<string>
#include<utility>
#include<set>
#include<map>
using namespace std;




//vector

vector<int> v;


void v.clear();

bool v.empty();

int  v.size();

vector<int>::iterator v.begin();

vector<int>::iterator v.end();


void v.push_back(int x);

void v.pop_back();

   & v.front();
   
   & v.back();


for (vector<int>::iterator it=v.begin(); it!=v.end(); it++)  {  int t=*it;}

     sort(v.begin(), v.end());
     


//queue

queue<int> q;


bool q.empty();

int  q.size();

void q.push(int x);

void q.pop();

   & q.front();
   
   & q.back();




//stack

stack<int> s;


bool s.empty();

int  s.size();

void s.push(int a);

void s.pop();

   & s.top();
   
   

//deque


//priority_queue

priority_queue<int> pq;
priority_queue< int, vecotr<int>, greter<int> > pq;
//����<�ţ�<�űȽ����ȼ��Ĵ�С�����ȼ��������ǰ�棬С�����ں��� 

bool pq.empty();

int  pq.size();

void pq.push(int x);

void pq.pop();

   & pq.top();




//list




//string

string str;


string str("Li Ming");
string str=string("Li Ming");
string str="Li Ming";

char ch[20];
string str(ch);
string str=string(ch);
string str=ch;


void str.clear();

bool str.empty();

int  str.length();
     = str.size();

string::iterator str.begin();

string::iterator str.end();


//void str.erase(string::iterator loc);//loc

//void str.erase(string::iterator start, stirng::iterator end);//[start, end)


//string::iterator str.find(string str1, string::iteraotr index);


//void str.insert(string::iterator index, char ch);

//void str.insert(string::iterator index, string str1);

//void str.push_back(char ch);




//pair

pair<int,int> p;

  p = make_pair(int a, int b);

     pair<int,int>(int a,int b);

   & p.first;

   & p.second;



//set

set<int> s;// setԪ�� �������� 


bool s.empty();

int  s.size();

void s.clear();

set<int>::iterator s.begin();

set<int>::iterator s.end();


pair<set<int>::iterator, bool> s.insert(int x);

     
int  s.erase(int x);

int  s.erase(set<int>::iterator it);


set<int>::iterator s.find(int x);

set<int>::iterator s.lower_bound(int x);

set<int>::iterator s.upper_bound(int x);

pair<set<int>::iterator,set<int>::iteraotr> s.equal_range(int x);
                                          = pair<set<int>::iterator,set<int>::iterator>(s.lower_bound(int x),s.upper_bound(int x));

int  s.count(int x);


for (set<int>::iterator it=s.begin(); it!=s.end(); it++)  {  int t=*it;}






//map

map<int,int> m;// key -> value 


void m.clear(); 

bool m.empty();

int  m.size();

map<int,int>::iterator m.begin();

map<int,int>::iterator m.end();


pair<map<int,int>::iterator,bool> m.insert(pair<int,int> px);


int  m.erase(int key);

int  m.erase(map<int,int>::iterator it);


map<int,int>::iterator m.find(int key);

map<int,int>::iterator m.lower_bound(int key);

map<int,int>::iterator m.upper_bound(int key);

pair<map<int,int>::iterator,map<int,int>::iterator> m.equal_range(int key); // [start, end)  
                                                  = pair<map<int,int>::iterator,map<int,int>::iterator>(m.lower_bound(int key), m.upper_bound(int key));

int  m.count(int key);


   & m[int key];


for (map<int,int>::iterator it=m.begin(); it!=m.end(); it++)  {  pair<int,int> t=*it;}







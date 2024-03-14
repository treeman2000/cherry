#include<iostream>
#include<string>
using namespace std;

class A{
public:
    int& get_b(){return b;}
    int b = 0;
};

int main(){
    int& x = A().get_b();
    x= 6;
    cout<<x<<endl;
    return 0;  
}
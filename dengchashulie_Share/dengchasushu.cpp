#include<iostream>
#include<vector>
using namespace std;

bool isSushu(int s);

int main() {
	int k;
	int n=0;
	int g=1;
	vector<int> su;
	vector<int> lie(k);
	cout<<"请输入等差素数列的长度:"<<endl;
	cin>>k;
	for(int i=2;i<k;i++) {
		if(isSushu(i)) {
			su.push_back(i);
		}
	}
	// 以下求出一个最小的能被所有小于序列长度k的素数整除的公差
	while(true) {
		int n=0;
		for(auto sus: su) {
			if(g%sus==0) n++;
		}
		if(n>=su.size()) {
			cout<<g<<endl;
			break;
		}
		else {
			n=0;
			g++;
		}
	}
	/*----以下是输出满足的序列----*/
	int ii;
	int i=2;
	int n1=0;
	ii=i;
	while(n1<k) {
		if(isSushu(ii)) {
			lie.push_back(ii);
			n1++;
			ii+=g;
		}
		else {
			lie.clear();
			n1=0;
			i=i+1;
			ii=i;
		}
	}
	for(auto i1:lie) cout<<i1<<' ';
	cout<<endl;
	return 0;
}

bool isSushu(int s) {
	int u;
	for(u=2;u<=s-1;u++) {
		if(s!=u && s%u==0) {
			return false;
		}
	}
	//printf("%d ",s);
	return true;
}

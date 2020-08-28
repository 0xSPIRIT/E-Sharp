#include <iostream>
using namespace std;

#define ull unsigned long long

bool is_prime(ull i);

int main() {
    ull i = 0;
    while (true) {
        if (is_prime(i)) {
            cout << i << endl;
	}
	i++;
    }
}

bool is_prime(ull num) {
    if (num == 0 || num == 1) return false;
    
    bool isprime = true;
    for (ull i = 2; i <= num/2; i++) {
        if (num % i == 0) {
	    isprime = false;
	    break;
	}
    }

    return isprime;
}

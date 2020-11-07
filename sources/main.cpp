/**
 * Temporary main. Includes console for debugging.
 * 
*/

#include <iostream>
#include <string>
#include "math/long/Long.h"
#include "math/test.h"

using namespace std;

int main(){
    {
        R a={new Long{-1920}};
        R b={new Long{-1080}};

        cout<<a<<"\n";
        cout<<b<<"\n";

        cout<<"gcd = "<<gcd(a,b)<<"\n";

        cout<<a<<"\n";
        cout<<b<<"\n";
    }

    {
        R a={new Long{1920}};
        R b={new Long{1080}};

        cout<<a<<"\n";
        cout<<b<<"\n";

        cout<<"gcd = "<<gcd(a,b)<<"\n";

        cout<<a<<"\n";
        cout<<b<<"\n";
    }

    {
        R a={new Long{1920}};
        R b={new Long{-1080}};

        cout<<a<<"\n";
        cout<<b<<"\n";

        cout<<"gcd = "<<gcd(a,b)<<"\n";

        cout<<a<<"\n";
        cout<<b<<"\n";
    }

    {
        R a={new Long{-1920}};
        R b={new Long{1080}};

        cout<<a<<"\n";
        cout<<b<<"\n";

        cout<<"gcd = "<<gcd(a,b)<<"\n";

        cout<<a<<"\n";
        cout<<b<<"\n";
    }

    {
        R a={new Long{1080}};
        R b={new Long{1920}};

        cout<<a<<"\n";
        cout<<b<<"\n";

        cout<<"gcd = "<<gcd(a,b)<<"\n";

        cout<<a<<"\n";
        cout<<b<<"\n";
    }

    {
        R a={new Long{-1080}};
        R b={new Long{-1920}};

        cout<<a<<"\n";
        cout<<b<<"\n";

        cout<<"gcd = "<<gcd(a,b)<<"\n";

        cout<<a<<"\n";
        cout<<b<<"\n";
    }

    {
        R a={new Long{-1080}};
        R b={new Long{1920}};

        cout<<a<<"\n";
        cout<<b<<"\n";

        cout<<"gcd = "<<gcd(a,b)<<"\n";

        cout<<a<<"\n";
        cout<<b<<"\n";
    }

    {
        R a={new Long{1080}};
        R b={new Long{-1920}};

        cout<<a<<"\n";
        cout<<b<<"\n";

        cout<<"gcd = "<<gcd(a,b)<<"\n";

        cout<<a<<"\n";
        cout<<b<<"\n";
    }

    int i;
    cin>>i;
    return 0;
}

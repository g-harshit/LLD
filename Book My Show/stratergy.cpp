#include <bits/stdc++.h>
using namespace std;

class DriveInterface{
    public :
    virtual void Drive() = 0;
};

class DriveNormal : public DriveInterface{
    public:
    void Drive(){
        cout<<"Normal Drive \n";
    }
};

class DriveFast : public DriveInterface{
    public:
    void Drive(){
        cout<<"Fast Drive \n";
    }
};

class Vehichle{
    
    DriveInterface *drive;
    
    public:
    Vehichle(DriveInterface *driveObj){
        drive = driveObj;
    }

    void Drive(){
        drive->Drive();
    }
};

class SportsCar : public Vehichle{
        
    public: 
    SportsCar():Vehichle(new DriveFast()){ }

};


class NormalCar : public Vehichle{
    
    public: 
    NormalCar():Vehichle(new DriveFast()){ }

};

int main(){

    #ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    NormalCar car1;
    car1.Drive();
    SportsCar car2;
    car2.Drive();

    return 0;
}

/*
Stratergy Design Pattern
Let suppose we have a parent class A and 3 child class B1, B2, B3. Now we have a func1 in A
B1 and B2 overide the func1 with same functionality but B3 overide the func1 in different
way. So now code is repaeted in B1 and B2 so for this we create a abstract class and a 
virtual function int that we intialze 2 calss having 2 diff functioality of func1. That can
be used in B1, B2, and B3    
*/
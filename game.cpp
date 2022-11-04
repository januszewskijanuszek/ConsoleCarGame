#include <iostream>
#include <windows.h>
#include <vector>
#include <random>
#include <functional>
#include <map>
#include <string>

using namespace std;

#define NATIVE 0x7
#define RED 0x4
#define GREEN 0x2
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

random_device rd;
mt19937 mtGenerator(rd());
class Car{
private:
    string name;
    int money;
    class Wheel{
    private:
        int index;
        int tier;
        int durability;
        int maxDurability;
        vector<int> rangeOfUsage;
    public:
        Wheel(){}
        Wheel(int index, int maxDurability, vector<int> range){
            Wheel::index = index;
            Wheel::maxDurability = maxDurability;
            Wheel::durability = maxDurability;
            Wheel::rangeOfUsage = range;
            Wheel::tier = 1;
        }
        int getIndex() {return index;}
        int getTier() {return tier;}
        int getDurability() {return durability;}
        int getMaxDurability() {return maxDurability;}
        int getDurabilityPercentage() {return (((double) durability) / maxDurability) * 100;}
        bool addTier(){
            if(Wheel::tier < rangeOfUsage.at(1)){
                Wheel::tier++;
                Wheel::rangeOfUsage[1] = 11 - Wheel::tier;
                Wheel::durability = Wheel::maxDurability;
                return true;
            } else return false;
        }
        void useWheel(){
            uniform_int_distribution<int> distro(rangeOfUsage[0], rangeOfUsage[1]);
            Wheel::durability -= distro(mtGenerator);
        }
        void repairWheel() { Wheel::durability = Wheel::maxDurability;}
        vector<int> returnStats(){return {tier, durability, maxDurability};}
        const vector<int> &getRangeOfUsage() const {return rangeOfUsage;}
        void setDurability(int dur) {Wheel::durability = dur;}
    };
    class Return{
        string baar(int percent){
            string finalString;
            for(int i = 0 ; i < percent ; i++) finalString += char(219);
            for(int i = 0 ; i < (100 - percent) ; i++) finalString += ' ';
            return "[" + finalString + ']';
        }
        vector<Wheel> wheels;
        int money;
        string name;
    public:
        Return(vector<Wheel> wheels,int money, string name){
            Return::wheels = wheels;
            Return::money = money;
            Return::name = name;
        }
        void inGameStats(int fuel){
            cout << "Money: " << Return::money << "$" << " -  Car: " << name << endl;
            cout << "Fuel: " << baar(fuel) << endl << endl;
            for(Wheel element : wheels){
                cout << element.getIndex() << " - " << baar(element.getDurabilityPercentage()) << endl;
            }
        }
        void wheelStatus(){
            for(Wheel element : wheels){
                cout << element.getIndex() << " - Price - "<< (100 - element.getDurabilityPercentage()) / 2 << "$ : " << baar(element.getDurabilityPercentage()) << "%" << endl;
            }
        }
        void wheelTierStats(){
            for(Wheel element : wheels){
                cout << element.getIndex() << " - Tier: " << element.getTier() <<" | Cost for next: "<< 20 * element.getTier() <<"$ | Tier stats: [" << element.getRangeOfUsage().at(0) << " - " << element.getRangeOfUsage().at(1) << "]" <<endl;
            }
        }
    };
    vector<Wheel> wheels;
public:
    Car(string name){
        Car::name = name;
        Car::money = 10000;
        wheels = vector<Wheel>(4);
        for(int i = 0 ; i < wheels.size() ; i++){
            wheels[i] = Wheel(i + 1 , 100, {1, 10});
        }
    }
    bool isForMistire(){
        for(Wheel element : Car::wheels){
            if(element.getDurability() <=0){
                return true;
            }
        }return false;
    }
    void useWheelForEveryWheel(){
        for(int i = 0 ; i < 4 ; i++){
            Car::wheels[i].useWheel();
        }
    }
    void substractMoney(int value){Car::money -= value;}
    void addMoney(int value){Car::money += value;}
    int getMoney() const {return money;}
    vector<Wheel> &getWheels() {return Car::wheels;}
    Return print(){return Return(Car::wheels, Car::money, Car::name);}
    static string baarWithDots(Wheel wheel, int percentAmount){
        string finalString;
        for(int i = 0 ; i < wheel.getDurabilityPercentage() ; i++) finalString += char(219);
        for(int i = 0 ; i < percentAmount - wheel.getDurabilityPercentage() ; i++) finalString += char(176);
        for(int i = 0 ; i < ((100 - wheel.getDurabilityPercentage()) - percentAmount) ; i++) finalString += ' ';
        return "[" + finalString + ']';
    }
};
class Pannel{
private:
    Car car;
    int score;
    map<string, function<void()>> switchMap;
public:
    Pannel(Car &car) : car(car) {
        Pannel::score = 0;
        switchMap["1"] = [&](){
            bool isMistire = false;
            system("cls");
            Pannel::countDown();
            Sleep(1000);
            int lastFuel = 0;
            for(int i = 100 ; i > 0 ; i--){
                lastFuel = i;
                system("cls");
                if(car.isForMistire()){ isMistire = true; break;}
                car.print().inGameStats(i);
                car.addMoney(1);
                car.useWheelForEveryWheel();
                Sleep(500);
                Pannel::score++;
            }
            if(isMistire){
                car.print().inGameStats(lastFuel);
                Pannel::color("Wheel damaged!", RED);
            }
            else Pannel::color("End of fuel!", RED);
            Sleep(4000);
        };
        switchMap["2"] = [&](){
            bool exit = false;
            map<string, function<void()>> shopMap;
            shopMap["1"] = [&](){
                bool exit = false;
                bool flag = true;
                system("cls");
                while(!exit){
                    if(flag) system("cls");
                    flag = true;
                    cout << "Money = "; Pannel::color(to_string(car.getMoney()) + "$", GREEN);
                    car.print().wheelStatus();
                    cout << "0 - To exit from this section" << endl;
                    cout << "Chose number of wheel to repair: ";
                    string temp; cin >> temp;
                    exit = stoi(temp) == 0;
                    try{
                        int price = (100 - car.getWheels().at(stoi(temp) - 1).getDurabilityPercentage()) / 2;
                        if(car.getWheels().at(stoi(temp) - 1).getDurabilityPercentage() >= 100){
                            system("cls");Pannel::color("Wheel fully upgraded!", GREEN); flag = false;}
                        else {
                            bool flag2 = true;
                            bool exit2 = false;
                            while(!exit2){
                                if(flag2) system("cls");
                                flag2 = true;
                                cout << "Money = "; Pannel::color(to_string(car.getMoney()) + "$", GREEN);
                                car.print().wheelStatus();
                                cout << "0 - Exit repair mode" << endl;
                                cout << "Set desired durability amount: ";
                                string temp2; cin >> temp2;
                                if(stoi(temp2) == 0) exit2 = true;
                                if(stoi(temp2) > 100 || stoi(temp2) < 0){system("cls");Pannel::color("Allowed range is 0 - 100", RED); flag2 = false;}
                                else if(car.getWheels().at(stoi(temp) - 1).getDurabilityPercentage() >= stoi(temp2)){
                                    system("cls");Pannel::color("Can not damage whe wheel!", RED); flag2 = false;}
                                else if((int)((double) price * (stod(temp2) / 100)) > car.getMoney()){system("cls"); Pannel::color("No enough money!", RED); flag2 = false;}
                                else{
                                    cout << Car::baarWithDots(car.getWheels().at(stoi(temp) - 1), stoi(temp2)) << endl;
                                    cout << "Cost: " << (int)((double) price * (stod(temp2) / 100)) << "$" << endl;
                                    if(Pannel::sureExit()){
                                        car.substractMoney((int)((double) price * (stod(temp2) / 100)));
                                        car.getWheels().at(stoi(temp) - 1).setDurability(stoi(temp2));
                                        system("cls");
                                        Pannel::color("Repair succesfull!", GREEN);
                                    }
                                    exit2 = true;
                                }
                            }
                        }
                    } catch (out_of_range e){system("cls"); Pannel::errorMessage(); flag = false;}

                }
            };
            shopMap["2"] = [&](){
                bool exit = false;
                bool flag = true;
                system("cls");
                while(!exit){
                    if(flag) system("cls");
                    flag = true;
                    cout << "Money = "; Pannel::color(to_string(car.getMoney()) + "$", GREEN);
                    car.print().wheelTierStats();
                    cout << "0 - To exit from this section" << endl;
                    cout << "Chose number of wheel to upgrade: ";
                    string temp; cin >> temp;
                    exit = stoi(temp) == 0;
                    try{
                        int price = car.getWheels().at(stoi(temp) - 1).getTier() * 20;
                        if(price < car.getMoney()){
                            car.substractMoney(price);
                            if(!car.getWheels().at(stoi(temp) - 1).addTier()){
                                system("cls"); Pannel::color("Wheel fully upgraded!", GREEN); flag = false;
                            }
                        } else{system("cls"); Pannel::color("No enough money!", RED); flag = false;}
                    } catch (out_of_range e){system("cls"); Pannel::errorMessage(); flag = false;}
                }
            };
            shopMap["3"] = [&](){if(Pannel::sureExit()) exit = true;};
            bool flag = true;
            while(!exit){
                if(flag) system("cls");
                flag = true;
                Pannel::printShopPanel();
                string temp; cin >> temp;
                try{shopMap.at(temp)();}
                catch (out_of_range e){system("cls"); Pannel::errorMessage();flag = false;}
            }
        };
        switchMap["3"] = [&](){
            if(Pannel::sureExit()){
                Pannel::color("Thanks for playing!", GREEN);
                cout << "Your score = " << score << endl;
                system("pause>NUL");
                exit(0);
            }
        };
    }
    static void countDown(){
        for(int i = 3 ; i > 0 ; i--){
            Pannel::color(to_string(i), GREEN);
            Sleep(1000);
        } Pannel::color("START", GREEN);
    }
    static void color(string value, int hex){
        SetConsoleTextAttribute(hConsole,hex);
        cout << value << endl;
        SetConsoleTextAttribute(hConsole, NATIVE);
    }
    const map<string, function<void()>> &getSwitchMap() const {return switchMap;}
    static void welcome(){
        cout << "Hello in car game! " << endl;
        cout << "Name your vehicle: ";
    }
    static void printMenu(){
        cout << "1 - Go on ride" << endl;
        cout << "2 - Repair car" << endl;
        cout << "3 - Give up" << endl;
    }
    static void printShopPanel(){
        cout << "1 - Repair wheel" << endl;
        cout << "2 - Upgrade Wheel" << endl;
        cout << "3 - Leave shop" << endl;
    }
    static void errorMessage(){Pannel::color("Invalid value", RED);}
    static bool sureExit(){
        map<string, bool> sure;
        sure["y"] = true;
        sure["n"] = false;
        cout << "Really? y/n" << endl;
        string temp; cin >> temp;
        try{return sure.at(temp);
        } catch (out_of_range e){system("cls"); Pannel::errorMessage(); return false;}
    }
};
int main(){
    system("cls");
    SetConsoleTextAttribute(hConsole, NATIVE);
    Pannel::welcome();
    string name; cin >> name;
    Car mainCar = Car(name);
    Pannel panel = Pannel(mainCar);
    bool flag = true;
    while(mainCar.getMoney() > 0){
        if(flag) system("cls");
        flag = true;
        Pannel::printMenu();
        string temp; cin >> temp;
        try{panel.getSwitchMap().at(temp)();}
        catch (out_of_range e){system("cls");Pannel::errorMessage(); flag = false;}
    }
    return 0;
}
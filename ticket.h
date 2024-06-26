/* ***************************************************************
* Objectives/Description:
* Input: Student or guest check-in. Student Id for students and card details for outsiders.
* Output: Status of the account for students. Confirmation for purchase or entrance, or denial of entrance.
* ***************************************************************
*/


#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

using namespace std;

const int breakfast_price = 8;
const int lunch_price = 12;
const int dinner_price = 15;

int charge_meal();


/* *************************************************
* Class Name: Date
* Description: Atrributes to return date, time, day, etc.
* Function Parameters: None, number of seconds, or any string.
* What is returned, if anything: either date, time, day.
* *************************************************
*/
class Date 
{
    private:
        time_t current_time;
        tm* local_time;

    public:
        Date(){
            current_time = 0;
            local_time = localtime(&current_time);
        };

        Date(int seconds){
            current_time = seconds;
            local_time = localtime(&current_time);
        }

        Date(string now) {
            current_time = time(0);
            local_time = localtime(&current_time);
        }

        string getDate() {
            char date_buffer[11];
            strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", local_time);
            return date_buffer;
        }

        string getTime() {
            char time_buffer[9];
            strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", local_time);
            return time_buffer;
        }

        string getHour(){
            char time_buffer[4];
            strftime(time_buffer, sizeof(time_buffer), "%H", local_time);
            return time_buffer;
        }

        string getDayOfWeek() {
            int day_of_the_week = local_time->tm_wday;
            string weekdays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
            return weekdays[day_of_the_week];
        }

        int getSecondsPassed() {
            return current_time;
    }


};


class Student
{
    private:
        string wid;
        string name;
        string plan;

        int weekly_available;
        int daily_available;
        int guest_swipes;
        int dining_dollars;

        Date last_swipe;
        Date current_swipe;

    public:
        Student();
        Student(string wid,
                string name,
                string plan,
                string weekly_available,
                string daily_available,
                string guest_swipes,
                string dining_dollars,
                string last_swipe);

        void swipe();

        int get_daily_available();

        void use_daily_available();
        void use_guest_swipes();
        void use_dining_dollars();

        void set_daily_swipes();
        void set_weekly_swipes();

        vector<string> vectorize();

        void get_status(){
            cout << endl;
            cout << "-----------------Status-----------------" << endl;
            cout << "Name:" << name << endl;
            cout << "Plan:" << plan << endl;
            cout << "Available this week:" << weekly_available << endl;
            cout << "Available Today:" << daily_available << endl;
            cout << "Guest Swipes Available:"<< guest_swipes << endl;
            cout << "Dining dollars Available:" << dining_dollars << endl;
            cout << "--------------Have a Good Day-----------" << endl << endl;
        }
};


//Default Constructor
Student::Student(){
    wid = "w0000001";
    name = "Seymour";
    plan = "unlimited";
    dining_dollars = 300;

    weekly_available = 99;
    daily_available = 1;
    guest_swipes = 10;

    current_swipe = Date("now");
    last_swipe = Date();
}

//Constructor for creating student from csv.
Student::Student(string wid,
                 string name,
                 string plan,
                 string weekly_available,
                 string daily_available,
                 string guest_swipes,
                 string dining_dollars,
                 string last_swipe){

            this->wid = wid;
            this->name = name;
            this->plan = plan;
            this->weekly_available = stoi(weekly_available);
            this->daily_available = stoi(daily_available);
            this->guest_swipes = stoi(guest_swipes);
            this->dining_dollars = stoi(dining_dollars);

            this->last_swipe = stoi(last_swipe);

            current_swipe = Date("now");
            int current_secs = current_swipe.getSecondsPassed();
};


/* *************************************************
* Function Name: get_daily_available
* Function Description: returns the number of swipes available for the day
* Function Parameters: NA
* What is returned, if anything: int: number of swipes left.
* *************************************************
*/
int Student::get_daily_available(){
    return daily_available;
}

/* *************************************************
* Function Name: use_daily_available
* Function Description: reduces the weekly and daily swipe limit and sets the time of last swiped with current swipe.
* Function Parameters: NA
* What is returned, if anything: NA
* *************************************************
*/
void Student::use_daily_available(){
    this->weekly_available--;
    this->daily_available--;
    this->last_swipe = current_swipe;
}

/* *************************************************
* Function Name: use_guest_swipes
* Function Description: reduces the number of guest swipes available
* Function Parameters: NA
* What is returned, if anything: NA
* *************************************************
*/
void Student::use_guest_swipes(){
    guest_swipes--;
}

/* *************************************************
* Function Name: use_dining_dollars
* Function Description: charges certain amount to the student using dining dollars available
* Function Parameters:NA
* What is returned, if anything: NA
* *************************************************
*/
void Student::use_dining_dollars(){
    int charge;
    charge = charge_meal();

    dining_dollars -= charge;
    cout << "Purchase of $" << charge << " was successfull." << endl;
}

/* *************************************************
* Function Name: set_weekly_swipes
* Function Description: resets the available weekly swipes depending on the plan on each Monday.
* Function Parameters: NA
* What is returned, if anything: NA
* *************************************************
*/
void Student::set_weekly_swipes(){
    string day = current_swipe.getDayOfWeek();

    if (day == "Monday"){
        if (plan == "unlimited"){
            this->weekly_available = 99;
        }
        else if (plan == "limited"){
            this->weekly_available = 10;
        }
    }
}


/* *************************************************
* Function Name: set_daily_swipes
* Function Description: sets the daily available swipe limit to one
* Function Parameters: NA
* What is returned, if anything: NA
* *************************************************
*/
void Student::set_daily_swipes(){
    set_weekly_swipes();
    this->daily_available = 1;
}

/* *************************************************
* Function Name: swipe
* Function Description: gives students to swipe their card(id number) and either allow or deny depending on their status.
* Function Parameters: NA
* Paramter type, name and use: NA
* What is returned, if anything: NA
* *************************************************
*/
void Student::swipe(){
    
    int last_swipe_hr = last_swipe.getSecondsPassed();
    int current_swipe_hr = current_swipe.getSecondsPassed();

    int difference = current_swipe_hr - last_swipe_hr;
    //cout << "Difference in Time: " << difference << endl;

    //cout << "Currently swiped at: " <<current_swipe_hr << endl;
    //cout << "Last swiped at: " << last_swipe_hr << endl;

    if (difference >=60){
        set_daily_swipes();
        //cout << "Difference more than 60.. increasing available swipes.." << endl;
    }

    if ((get_daily_available()>0) && (weekly_available>0)){
        use_daily_available();
        cout << "Thank You. You are good to enter." << endl;

    }
    else{
        cout << "You do not have any available swipes at the moment. Do you want to you Guest Swipe? (yes/no): ";
        string response;
        cin >> response;

        if ((response == "yes") && (guest_swipes >0)){

            use_guest_swipes();
        }
        else{
            cout << "Sorry, you will have to wait until next swipe time to re-enter the Fresh." << endl;
            cout << "You can use your dining dollars to enter the Fresh. Do you want to use it? (yes/no):";
            cin >> response;

            if ((response == "yes") && (dining_dollars >= charge_meal())){
                use_dining_dollars();
            }
            else{
                cout << "Sorry, you cannot enter the Fresh." << endl;
            }
        }
    }
}

/* *************************************************
* Function Name: vectorize
* Function Description: vectorizes the details for each Student.
* Function Parameters: details of students
* Paramter type, name and use: string: swid, sname, splan, sweekly_available, sdaily_available, sguest_swipes, sdining_dollars, slast_swipe
* What is returned, if anything: vector
* ************************************************
*/
vector<string> Student::vectorize(){
    string swid, sname, splan, sweekly_available, sdaily_available, sguest_swipes, sdining_dollars, slast_swipe;
    vector <string> row;
    swid = this->wid;
    sname = this->name;
    splan = this->plan;
    sweekly_available = to_string(this->weekly_available);
    sdaily_available = to_string(this->daily_available);
    sguest_swipes = to_string(this->guest_swipes);
    sdining_dollars = to_string(this->dining_dollars);
    slast_swipe = to_string(this->last_swipe.getSecondsPassed());

    row = {swid, sname, splan, sweekly_available, sdaily_available, sguest_swipes, sdining_dollars, slast_swipe};
    return row;
};


/* *************************************************
* Class Name: Outsider
* Description: Creates an Outsider (non-student) and allows them to enter the Fresh using their Payment Cards (just a demo)
* Functions Parameters: card number, name
* What is returned, if anything: NA
* *************************************************
*/
class Outsider
{
    private:
        long cardnumber;
        string uname;
        Date curr_time;

    public:
        Outsider(long number, string name){
            cardnumber = number;
            uname = name;
        };
        void make_purchase(){
            int charged_amount = charge_meal();
            cout << "Purchase made of $" <<charged_amount << endl;
        };
};


/* *************************************************
* Function Name: charge_meal
* Function Description: Returns the price depending on time for breakfast, lunch, and dinner.
* Function Parameters: NA
* What is returned, if anything: int: price
* *************************************************
*/
int charge_meal(){
    Date current_time = Date("now");
    string hours = current_time.getHour();
    int thour = stoi(hours);

    if (thour<11){
        return breakfast_price;
    }
    else if (thour >=11 && thour <16){
        return lunch_price;
    }
    else{
        return dinner_price;
    }
}
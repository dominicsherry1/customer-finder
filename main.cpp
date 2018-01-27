#include "libs.hpp"

using json = nlohmann::json;
using namespace std;

const double OFFICE_LAT = 53.339428;
const double OFFICE_LONG = -6.257664;
const double PI = 3.1415926536;
const double MIN_DISTANCE = 100.0;
const string FILENAME = "files/customers.json";
const int EARTH_RADIUS = 6371; //mean radius in km

bool compare_user_ids (pos::customer &a, pos::customer &b) {
    return a.user_id < b.user_id;
}

bool contains_number(string s) {
    return find_if(s.begin(), s.end(), ::isdigit) != s.end();
}

double get_distance(pos::customer a, pos::customer b) {
    double central_angle, distance;
    //cout << a.latitude << a.longitude << b.latitude << b.longitude << endl;
    central_angle=(acos(sin(a.latitude)*sin(b.latitude)+cos(a.latitude)*cos(b.latitude)*cos(b.longitude-a.longitude)));
    if (central_angle > PI) {
        central_angle = (2*PI-central_angle); //This ensures we always choose the smaller central angle to the customer
    }
    return distance = central_angle*EARTH_RADIUS;
}

pos::customer select_customer(string customer_json, pos::customer office, double min_distance) {
    try {
        json j = json::parse(customer_json);
        try {
            if (j.at("user_id").is_number() == false) {
                cerr << "A customer user ID is of an incorrect type!" << endl;
                exit(-1);
            }
            else if (j.at("name").is_string() == false) {
                cerr << "A customer name is of an incorrect type!" << endl;
                exit(-1);
            }
            else if ((contains_number(j.at("latitude")) && contains_number(j.at("longitude"))) == false) {
                cerr << "A customer's position has not been correctly provided!" << endl;
                exit(-1);
            }
        }
        catch (json::out_of_range &e) {
            cerr << "A customer is missing a field in \"" << FILENAME << "\"!" << endl;
            exit(-1);
        }
        pos::customer c = j; //Convert from json to customer struct
        double distance = get_distance(office, c);
        if (distance > min_distance) {
            c.user_id = -1; //Set user ID of customers outside the minimum distance to -1
        }
        return c;
    }
    catch (json::parse_error &e) {
        cerr << "The input file \"" << FILENAME << "\" is not valid json!" << endl << "Error code " << e.id << endl;
        exit(-1);
    }
}

int main() {
    fstream file;
    string record;
    pos::customer office {0, "Office", OFFICE_LAT*DEG_TO_RAD, OFFICE_LONG*DEG_TO_RAD}, c;
    vector<pos::customer> customers;
    int i;
    file.open(FILENAME);
    if (file.fail()) {
        cerr << "There was a problem reading from \"" << FILENAME << "\"" << endl;
        return -1;
    }
    while (getline(file, record)) {
    c = select_customer(record, office, MIN_DISTANCE);
        if (c.user_id >= 0) { //If customer is within the minimum distance of the office
            customers.push_back(c); //Add this customer to the vector of customers
        }
    }
    file.close();

    sort(customers.begin(), customers.end(), compare_user_ids);

    cout << "Customers within " << MIN_DISTANCE << "km of office:" << endl;
    for (i = 0; i < customers.size(); i++) {
        cout << "ID: " << customers.at(i).user_id << ",\tname: " << customers.at(i).name << endl;
    }

    return 0;
}

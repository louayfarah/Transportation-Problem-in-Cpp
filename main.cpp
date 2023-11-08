#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long int
#define pb push_back
#define mp make_pair
#define fi first
#define se second


int sources; // number of sources
int destinations; // number of destinations

vector<int> S; // Vector of coefficients of supply of size sources
vector<int> D; // Vector of coefficients of demand of size destinations
vector<vector<int>> C; // Matrix of coefficients of costs of size [sources x destinations]


bool transportation_problem_is_balanced()
{
    int sum_sources_coefs = 0, sum_destinations_coefs = 0;
    for(auto elt: S)
        sum_sources_coefs += elt;
    for(auto elt: D)
        sum_destinations_coefs += elt;

    return (sum_sources_coefs == sum_destinations_coefs);
}

void construct_input_table()
{

}

void print_supply_and_demand(int a, int b)
{
    cout << "Supply: " << a << " - " << min(a, b) << " = " << a - min(a, b) << endl;
    cout << "Demand: " << b << " - " << min(a, b) << " = " << b - min(a, b) << endl;  
}

bool north_west_is_applicable()
{
    return true;
}

void run_north_west(vector<int> supply, vector<int> demand)
{
    int north_west_x = 0, north_west_y = 0; //Initial northwest corner coordinates
    int res = 0;
    
    while(north_west_x != sources-1 || north_west_y != destinations-1)
    {
        cout << "Current north-west corner: (" << north_west_x+1 << ", " << north_west_y+1 << ')' << endl;

        cout << "We add " << C[north_west_x][north_west_y] << " * min(" << supply[north_west_x] << ", " << demand[north_west_y] << ')' << endl;
        res += C[north_west_x][north_west_y] * min(supply[north_west_x], demand[north_west_y]);

        if(supply[north_west_x] < demand[north_west_y])
        {
            print_supply_and_demand(supply[north_west_x], demand[north_west_y]);
            demand[north_west_y] -= supply[north_west_x];
            supply[north_west_x] = 0;
            north_west_x++;
        }
        else if(supply[north_west_x] > demand[north_west_y])
        {
            print_supply_and_demand(supply[north_west_x], demand[north_west_y]);
            supply[north_west_x] -= demand[north_west_y];
            demand[north_west_y] = 0;
            north_west_y++;
        }
        else
        {
            print_supply_and_demand(supply[north_west_x], demand[north_west_y]);
            supply[north_west_x] = 0;
            demand[north_west_y] = 0;
            north_west_x++;
            north_west_y++;
        }

        cout << endl;
    }

    cout << "Final north-west corner: (" << north_west_x+1 << ", " << north_west_y+1 << ')' << endl;
    cout << "We add " << C[north_west_x][north_west_y] << " * min(" << supply[north_west_x] << ", " << demand[north_west_y] << ')' << endl;
    res += C[north_west_x][north_west_y] * min(supply[north_west_x], demand[north_west_y]);

    cout << endl;
    cout << "Initial basic feasable solution: " << res << endl;
    cout << endl;
}

bool vogel_is_applicable()
{
    return true;
}

void run_vogel(vector<int> supply, vector<int> demand)
{
    
}

bool russel_is_applicable()
{
    return true;
}

void run_russel(vector<int> supply, vector<int> demand)
{

}

int main()
{
    // As mentioned in the task statement
    // The number of sources is 3
    // The number of destinations is 4

    sources = 3;
    destinations = 4;

    cout << "Number of sources: " << sources << endl;
    cout << "Number of destinations: " << destinations << endl;
    cout << endl;

    // Initialization of the coefficients
    S.assign(sources, 0);
    D.assign(destinations, 0);
    C.assign(sources, vector<int>(destinations, 0));

    // Getting the coefficeints from stdin
    cout << "Type " << sources << " coefficients which constitute the vector of supply:" << endl;
    for(int i = 0; i<sources; i++)
    {
        cin >> S[i];
    }

    cout << "Type " << destinations << " coefficients which constitute the vector of demand" << endl;
    for(int j = 0; j<destinations; j++)
    {
        cin >> D[j];
    }

    cout << "In each of the " << sources << " lines, type " << destinations << " coefficients corresponding to the matrix of costs:" << endl;
    for(int i = 0; i<sources; i++)
    {
        for(int j = 0; j<destinations; j++)
        {
            cin >> C[i][j];
        }
    }


    cout << "=========================================================================================================================" << endl;
    cout << "Checking whether the transportation problem is balanced..." << endl;
    
    if(!transportation_problem_is_balanced())
    {
        cout << "The problem is not balanced!" << endl;
        return 0;
    }

    cout << "The problem is balanced. here is the input parameters table:" << endl;
    construct_input_table();
    cout << endl << endl;

    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "North-West corner method..." << endl << endl;
    if(!north_west_is_applicable())
    {
        cout << "The method is is not applicable!" << endl;
        cout << "Explication: There are significant differences in the costs associated with different transportation routes." << endl;
        cout << "Warning: The solution obtained using this method may not be optimal." << endl;
    }

    run_north_west(S, D);


    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Vogel's Approximation method..." << endl << endl;
    if(!vogel_is_applicable())
    {
        cout << "The method is is not applicable!" << endl;
        cout << "Explication: There are multiple routes with the same lowest cost." << endl;
        cout << "Warning: The solution obtained using this method may not be optimal." << endl;
    }

    run_vogel(S, D);


    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Russel's Approximation method..." << endl << endl;
    if(!russel_is_applicable())
    {
        cout << "The method is is not applicable!" << endl;
        cout << "Explication: The transportation costs are not significantly different between routes." << endl;
        cout << "Warning: The solution obtained using this method may not be optimal." << endl;
    }

    run_russel(S, D);

    return 0;
}
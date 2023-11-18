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

string tabbed(string s)
{
    string res = s;
    while(int(res.size()) < 10)
        res.pb(' ');

    return res;
}

void construct_input_table()
{
    cout << tabbed("") << ' ' << tabbed("D1") << ' ' << tabbed("D2") << ' ' << tabbed("D3") << ' ' << tabbed("D4") << tabbed("Supply") << endl;
    for(int i = 0; i<sources; i++)
    {
        string current_source = "S";
        current_source.pb(char(i+1+'0'));
        cout << tabbed(current_source) << ' ';

        for(int j = 0; j<destinations; j++)
        {
            cout << tabbed(to_string(C[i][j])) << ' ';
        }

        cout << tabbed(to_string(S[i])) << endl;
    }

    cout << tabbed("Demand") << ' ';
    for(int j = 0; j<destinations; j++)
        cout << tabbed(to_string(D[j])) << ' ';
    cout << endl;
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

vector<int> delete_element_from_vector(vector<int> v, int x)
{
    vector<int> res;
    for(int i = 0; i<int(v.size()); i++)
    {
        if(i == x)
            continue;
        res.pb(v[i]);
    }

    return res;
}

vector<vector<int>> delete_row_from_matrix(vector<vector<int>> mat, int n)
{
    vector<vector<int>> res;
    for(int i = 0; i<int(mat.size()); i++)
    {
        if(i == n)
            continue;

        vector<int> temp;
        for(int j = 0; j<int(mat[0].size()); j++)
            temp.pb(mat[i][j]);
        
        res.pb(temp);
    }

    return res;
}


vector<vector<int>> delete_column_from_matrix(vector<vector<int>> mat, int m)
{
    vector<vector<int>> res;
    for(int i = 0; i<int(mat.size()); i++)
    {
        vector<int> temp;
        
        for(int j = 0; j<int(mat[0].size()); j++)
        {
            if(j == m)
                continue;

            temp.pb(mat[i][j]);
        }

        res.pb(temp);
    }

    return res;
}

void run_vogel(vector<int> supply, vector<int> demand, vector<vector<int>> coefs)
{
    int res = 0;

    while(!(int(coefs.size()) <= 1 && int(coefs[0].size()) <= 1))
    {
        int n = int(coefs.size());
        int m = int(coefs[0].size());
        cout << "The current cost coefficients are:" << endl;
        for(int i = 0; i<n; i++)
        {
            for(int j = 0; j<m; j++)
            {
                cout << coefs[i][j] << ' ';
            }
            cout << endl;
        }

        cout << "The current supply:" << endl;
        for(int i = 0; i<n; i++)
            cout << supply[i] << ' ';
        cout << endl;
        cout << "The current demand:" << endl;
        for(int j = 0; j<m; j++)
            cout << demand[j] << ' ';
        cout << endl;
        

        vector<int> supply_penalty(n, 0);
        vector<int> demand_penalty(m, 0);

        for(int i = 0; i<n; i++)
        {

            int first_minimum = 1e9, second_minimum = 1e9+1;

            for(int j = 0; j<m; j++)
            {
                if(coefs[i][j] < first_minimum)
                    first_minimum = coefs[i][j];
            }

            for(int j = 0; j<m; j++)
            {
                if(coefs[i][j] < second_minimum && coefs[i][j] > first_minimum)
                    second_minimum = coefs[i][j];
            }

            supply_penalty[i] = abs(first_minimum-second_minimum);
        }

        for(int j = 0; j<m; j++)
        {
            int first_minimum = 1e9, second_minimum = 1e9+1;

            for(int i = 0; i<n; i++)
            {
                if(coefs[i][j] < first_minimum)
                    first_minimum = coefs[i][j];
            }

            for(int i = 0; i<n; i++)
            {
                if(coefs[i][j] < second_minimum && coefs[i][j] > first_minimum)
                    second_minimum = coefs[i][j];
            }

            demand_penalty[j] = abs(first_minimum-second_minimum);
        }

        cout << "The supply penalties are:" << endl;
        for(int i = 0; i<n; i++)
            cout << supply_penalty[i] << ' ';
        cout << endl;

        cout << "The demand penalties are:" << endl;
        for(int j = 0; j<m; j++)
            cout << demand_penalty[j] << ' ';
        cout << endl;
        cout << endl;

        int maximum_supply_penalty = 0; // the index in the vector supply_penalty
        int maximum_demand_penalty = 0; // the index in the vector demand_penalty

        for(int i = 1; i<n; i++)
        {
            if(supply_penalty[i] > supply_penalty[maximum_supply_penalty])
                maximum_supply_penalty = i;
        }

        for(int j = 1; j<m; j++)
        {
            if(demand_penalty[j] > demand_penalty[maximum_demand_penalty])
                maximum_demand_penalty = j;
        }

        cout << "Maximum row penalty is " << supply_penalty[maximum_supply_penalty] << " at index " << maximum_supply_penalty+1 << endl;
        cout << "Maximum column penalty is " << demand_penalty[maximum_demand_penalty] << " at index " << maximum_demand_penalty+1 << endl;

        int mini_x, mini_y;
        if (supply_penalty[maximum_supply_penalty] >= demand_penalty[maximum_demand_penalty]) // the maximum penalty corresponds to the supplies (rows)
        {
            mini_x = maximum_supply_penalty;
            mini_y = 0;
            for(int j = 0; j<m; j++)
            {
                if(coefs[mini_x][j] < coefs[mini_x][mini_y])
                    mini_y = j;
            }
        }
        else // the maximum penalty corresponds to the demands (columns)
        {
            mini_y = maximum_demand_penalty;
            mini_x = 0;
            for(int i = 0; i<n; i++)
            {
                if(coefs[i][mini_y] < coefs[mini_x][mini_y])
                    mini_x = i;
            }
        }

        cout << "We add " << coefs[mini_x][mini_y] << " * min(" << supply[mini_x] << ", " << demand[mini_y] << ')' << endl;
        res += coefs[mini_x][mini_y] * min(supply[mini_x], demand[mini_y]); 

        if(supply[mini_x] < demand[mini_y])
        {
            print_supply_and_demand(supply[mini_x], demand[mini_y]);
            demand[mini_y] -= supply[mini_x];
            supply[mini_x] = 0;
            
            supply = delete_element_from_vector(supply, mini_x);
            coefs = delete_row_from_matrix(coefs, mini_x);
        }
        else if(supply[mini_x] > demand[mini_y])
        {
            print_supply_and_demand(supply[mini_x], demand[mini_y]);
            supply[mini_x] -= demand[mini_y];
            demand[mini_y] = 0;
            
            demand = delete_element_from_vector(demand, mini_y);
            coefs = delete_column_from_matrix(coefs, mini_y);
        }
        else
        {
            print_supply_and_demand(supply[mini_x], demand[mini_y]);
            supply[mini_x] = 0;
            demand[mini_y] = 0;
            
            supply = delete_element_from_vector(supply, mini_x);
            coefs = delete_row_from_matrix(coefs, mini_x);
            demand = delete_element_from_vector(demand, mini_y);
            coefs = delete_column_from_matrix(coefs, mini_y);
        }

        cout << endl;
    }

    cout << "Final addition:" << endl;
    cout << "We add " << coefs[0][0] << " * min(" << supply[0] << ", " << demand[0] << ')' << endl;
    res += coefs[0][0] * min(supply[0], demand[0]);

    cout << endl;
    cout << "Initial basic feasable solution: " << res << endl;
    cout << endl;
}

bool russel_is_applicable()
{
    return true;
}

void run_russel(vector<int> supply, vector<int> demand, vector<vector<int>> coefs)
{
    int res = 0;

    while(!(int(coefs.size()) <= 1 && int(coefs[0].size()) <= 1))
    {
        int n = int(coefs.size());
        int m = int(coefs[0].size());
        cout << "The current cost coefficients are:" << endl;
        for(int i = 0; i<n; i++)
        {
            for(int j = 0; j<m; j++)
            {
                cout << coefs[i][j] << ' ';
            }
            cout << endl;
        }

        cout << "The current supply:" << endl;
        for(int i = 0; i<n; i++)
            cout << supply[i] << ' ';
        cout << endl;
        cout << "The current demand:" << endl;
        for(int j = 0; j<m; j++)
            cout << demand[j] << ' ';
        cout << endl;

        vector<int> U(n, -1); // Ui is the largest cost in supply (row) i
        vector<int> V(m, -1); // Vj is the largest cost in demand (column) j

        for(int i = 0; i<n; i++)
        {
            for(int j = 0; j<m; j++)
            {
                U[i] = max(U[i], coefs[i][j]);
            }
        }

        for(int j = 0; j<m; j++)
        {
            for(int i = 0; i<n; i++)
            {
                V[j] = max(V[j], coefs[i][j]);
            }
        }

        cout << "Current vector U:" << endl;
        for(int i = 0; i<n; i++)
            cout << U[i] << ' ';
        cout << endl;

        cout << "Current vector V:" << endl;
        for(int j = 0; j<m; j++)
            cout << V[j] << ' ';
        cout << endl;
        cout << endl;

        cout << "Calculating the values of delta_ij for each i and j using the formula: delta_ij = c_ij - (U_i + V_j)..."  << endl;
        vector<vector<int>> delta(n, vector<int>(m, 0));
        for(int i = 0; i<n; i++)  
        {
            for(int j = 0; j<m; j++)
            {
                delta[i][j] = coefs[i][j] - (U[i] + V[j]);
            }
        }

        cout << "Searching for the variable having the most negative delta value..." << endl;
        int mini_x = 0, mini_y = 0;
        for(int i = 0; i<n; i++)
        {
            for(int j = 0; j<m; j++)
            {
                if(delta[i][j] < 0 && delta[i][j] < delta[mini_x][mini_y])
                {
                    mini_x = i;
                    mini_y = j;
                }
            }
        }
        
        cout << "The most negative delta value is " << delta[mini_x][mini_y] << " lying in position (" << mini_x+1 << ", " << mini_y+1 << ')' << endl;
        cout << endl;

        cout << "We add " << coefs[mini_x][mini_y] << " * min(" << supply[mini_x] << ", " << demand[mini_y] << ')' << endl;
        res += coefs[mini_x][mini_y] * min(supply[mini_x], demand[mini_y]); 

        if(supply[mini_x] < demand[mini_y])
        {
            print_supply_and_demand(supply[mini_x], demand[mini_y]);
            demand[mini_y] -= supply[mini_x];
            supply[mini_x] = 0;
            
            supply = delete_element_from_vector(supply, mini_x);
            coefs = delete_row_from_matrix(coefs, mini_x);
        }
        else if(supply[mini_x] > demand[mini_y])
        {
            print_supply_and_demand(supply[mini_x], demand[mini_y]);
            supply[mini_x] -= demand[mini_y];
            demand[mini_y] = 0;
            
            demand = delete_element_from_vector(demand, mini_y);
            coefs = delete_column_from_matrix(coefs, mini_y);
        }
        else
        {
            print_supply_and_demand(supply[mini_x], demand[mini_y]);
            supply[mini_x] = 0;
            demand[mini_y] = 0;
            
            supply = delete_element_from_vector(supply, mini_x);
            coefs = delete_row_from_matrix(coefs, mini_x);
            demand = delete_element_from_vector(demand, mini_y);
            coefs = delete_column_from_matrix(coefs, mini_y);
        }

        cout << endl;
    }

    cout << "Final addition:" << endl;
    cout << "We add " << coefs[0][0] << " * min(" << supply[0] << ", " << demand[0] << ')' << endl;
    res += coefs[0][0] * min(supply[0], demand[0]);

    cout << endl;
    cout << "Initial basic feasable solution: " << res << endl;
    cout << endl;
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

    cout << "The problem is balanced." << endl << "Here is the input parameters table:" << endl << endl;
    construct_input_table();
    cout << endl;
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

    run_vogel(S, D, C);


    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Russel's Approximation method..." << endl << endl;
    if(!russel_is_applicable())
    {
        cout << "The method is is not applicable!" << endl;
        cout << "Explication: The transportation costs are not significantly different between routes." << endl;
        cout << "Warning: The solution obtained using this method may not be optimal." << endl;
    }

    run_russel(S, D, C);

    return 0;
}
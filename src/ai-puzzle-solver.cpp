#include<bits/stdc++.h>
#include <iostream>
#include <fstream>
using namespace std;
#define MV 5 // maximum times same state may be visited in hill climbing
#define RD 100 // No. of times random restart should be done 
#define RDM 20 // No. of times Random restart should be done with simulated annealing
#define MAX_TIMES 1000.00 // No of instances of N-puzzle we generate and solve
struct state
{   
        int st[3][3]={0};
        state() // constructor to intialize a state to goal state
        {
            int c=0;
            for(int i=0;i<3;i++)
                {
                    for(int j=0;j<3;j++)
                        {
                            st[i][j]=c%9;
                            c++;
                        }
                }
        }
        int getInvCount(vector<int> arr)  // function to get inversion count to check if state is solvable
        { 
            int inv_count = 0; 
            for (int i = 0; i < 9 - 1; i++) 
                for (int j = i+1; j < 9; j++) 
                    // Value 0 is used for empty space 
                    if (arr[j] && arr[i] &&  arr[i] > arr[j]) 
                        inv_count++; 
            return (inv_count%2); 
        } 
        state(int x) // to generate a random initial state of N-Puzzle problem
        {
            vector <int> v;
                for(int i = 0; i <= 8; i += 1)
                {
                    v.push_back(i);
                }

                random_shuffle(v.begin(),v.end());
                while(getInvCount(v)) // to check if our random initial is solvable if not regenerate
                    random_shuffle(v.begin(),v.end());
                int c=0;
                for(int i=0;i<3;i++)
                {
                    for(int j=0;j<3;j++)
                        {
                            st[i][j]=v[c];
                            c++;
                        }
                }

        }
        bool operator == (const state &a) const { // operator to check if two states are equal
            for(int i=0;i<3;i++)
                {
                    for(int j=0;j<3;j++)
                        if(a.st[i][j]!=st[i][j])
                            return false;
                }
                return true;
        }

        int get_heuristic()// Manhattan Distance of each tile from the goal tile
        {
            int c=0;
            for(int i=0;i<3;i++)
                {
                    for(int j=0;j<3;j++)
                        {
                            c+= abs(i-(st[i][j]/3));
                            c+= abs(j-(st[i][j]%3));
                        }
            }
            return c;
        }
        pair<int,int> posof_blank_tile()// to get the position of blank tile
        {
            for(int i=0;i<3;i++)
                {
                    for(int j=0;j<3;j++)
                        if(st[i][j]==0)
                            return make_pair(i,j);
                }
        }
        int get_hash() // get hash value for our state
        {
            int hash_value=0;
            for(int i=0;i<3;i++)
                {
                    for(int j=0;j<3;j++)
                        {
                            hash_value= hash_value*10 + st[i][j];
                        }
                } 
            return hash_value;
        }
        
    
};
vector<char> get_possible_moves(int i,int j) // to get possible moves from tile i,j
    {
        vector<char> moves;
        if(i!=0)
            moves.push_back('U');
        if(i!=2)
            moves.push_back('D');
        if(j!=0)
            moves.push_back('L');
        if(j!=2)
            moves.push_back('R');
        return moves;
    }
double get_probability()
    {
        double i= rand()%32768;
        return i/32767.00;
    }
state move(char a,int i,int j,state temp) // Function to make a move LEFT, RIGHT, UP , DOWN and return the new state
{
    if(a=='U') // move up
        {
            swap(temp.st[i][j],temp.st[i-1][j]);
            return temp;
        }
    if(a=='R') // move right
        {
            swap(temp.st[i][j],temp.st[i][j+1]);
            return temp;
        }
    if(a=='D') // move down
        {
            swap(temp.st[i][j],temp.st[i+1][j]);
            return temp;
        }
    if(a=='L') // move left
        {
            swap(temp.st[i][j],temp.st[i][j-1]);
            return temp;
        }
}
struct Compare{ // to compare f_score for priority of states
      bool operator()( pair<int,state> x,  pair<int,state> y) 
      { 
            return  x.first> y.first;
      } 
};
struct Node { 
	int value = std::numeric_limits<int>::max(); // To initiate values of map to INT_MAX
};
pair <int,int> A_star (state start) // optimal solution because we use admissible heuristic
{
	map <int, int> marked; // marked values indicate the state is inserted in openSet
    priority_queue <pair <int, state>, vector<pair<int, state>>, Compare> openSet; //Priority queue
    unordered_map<int,Node> gScore; // Known path length
	gScore[start.get_hash()].value = 0; //initialize starting to 0
    unordered_map<int,int> f_score; // to store f_score of states
	openSet.push(make_pair(start.get_heuristic(),start)); 
	marked[start.get_hash()] = 1;
	int search_cost = 0;
	while(!openSet.empty()) // while our queue is not empty
    {
		pair<int,state> p = openSet.top(); //get the state to be visited i.e with least f(n)
        state pos=p.second;
        search_cost++;
		if (pos.get_heuristic() == 0) // if we already reached goal state
			return make_pair(search_cost,1);
		openSet.pop();
		marked[pos.get_hash()] = 0;
		pair <int, int> tile_index = pos.posof_blank_tile(); // get position of blank tile
		vector <char> possible_moves = get_possible_moves(tile_index.first, tile_index.second); // get all moves
		for(auto u :possible_moves)
        {
			state x = move(u,tile_index.first,tile_index.second,pos);  // get the corresponding to the move
			int tentative_score = gScore[pos.get_hash()].value+1; 
			if (tentative_score < gScore[x.get_hash()].value)
                { //if tentative score better than g(n)
				    gScore[x.get_hash()].value = tentative_score;
                    f_score[x.get_hash()]= gScore[x.get_hash()].value +x.get_heuristic(); // updating f(n)
				    openSet.push(make_pair(f_score[x.get_hash()],x));  // push the state with new f(n)
				    marked[x.get_hash()] = 1;		
			    }
		}
	}
    f_score.clear(); // clearing memory
    gScore.clear(); 
	return make_pair(search_cost,0);
}
pair<int,int> hill_climbing(state current) // hill Climbing function returns search cost and whether the goal state was acheived or not
{
    int search_cost=0,flag=0; // search_cost calculates no of visited states, flag checks if we reach goal or not
    state  next_state,temp;
    unordered_map<int,int> visited_state; // to store already visited states 
    while(current.get_heuristic()!=0)
        {   
            search_cost++;
            vector<char> ans;
            int mi= current.get_heuristic(); // heuristic of current state
            temp=current;next_state=current;
            pair<int,int> p= current.posof_blank_tile(); // position of blank tile
            int i=p.first,j=p.second;
            vector<char> moves=get_possible_moves(i,j); // get all possible moves from current state
            for(auto u:moves)
                {
                    state temp=move(u,i,j,current);
                    int  h=temp.get_heuristic();
                    if(h < mi && visited_state[temp.get_hash()]< MV) // if move is better than current best move found
                        {
                            ans.clear();mi=h;
                            ans.push_back(u);
                        }
                    if(h == mi && visited_state[temp.get_hash()]<MV) // if move is as good as the best move found till now
                        {
                            ans.push_back(u);
                        }
                }
            if(ans.size()==0) // No better states so terminate
                break;
            random_shuffle(ans.begin(),ans.end()); // choose a random next state from the best possible states
            if(ans.size()>=1)
            {
                next_state= move(ans[0],i,j,temp);
            }
            visited_state[current.get_hash()]++;
            current=next_state;
        }
    if(current.get_heuristic()==0) // Checking whether we reach goal state
        flag=1;
    return make_pair(search_cost,flag);
}
pair<int,int> hill_climbing_random_restart(state start)
{
    int search_cost=0; // search_cost calculates no of visited states, flag checks if we reach goal or not
    int t=RD;
    while(t--) // Random restart
    {
        pair<int,int> p= hill_climbing(start); // Calling hill climbing function t times 
        search_cost+= p.first;
        if(p.second==1) // if goal state is found we terminate
            return make_pair(search_cost,1);
    }
    return make_pair(search_cost,0);
}
pair<int,int> hill_climbing_simulated_annealing(state start)
{
    int search_cost=0,flag=0; // search_cost calculates no of visited states, flag checks if we reach goal or not
    state current=start,next_state,temp;
    double temperature=28; // initial value of temperature;
    double step = 0.001; // change in temperature with each move
    while(current.get_heuristic()!=0 && temperature >0)
        {
            temp=current; search_cost++; 
            pair<int,int> p= current.posof_blank_tile();
            int i=p.first,j=p.second;
            vector<char> moves=get_possible_moves(i,j);
            random_shuffle(moves.begin(),moves.end()); // to check moves in random order
            for(auto u:moves)
                {
                    temp= move(u,i,j,current);
                    double deltaE = current.get_heuristic()- temp.get_heuristic(); 
                    if(deltaE>0) // if move is better than current state make that move
                        {
                            next_state= temp;
                            break;
                        }
                    else
                        {   
                            double probability= exp(deltaE/temperature); // calculate probability is move is not better
                            if(get_probability()<=probability) // determine if we should select or not select based on probability
                                {
                                    next_state= temp;
                                    break;
                                }
                        }
                }
            current=next_state;
            temperature-=step; // change temperature with time
        }
    if(current.get_heuristic()==0) // if we reach goal state we can terminate 
        return make_pair(search_cost,1);
    
    return make_pair(search_cost,0);
}
pair<int,int> hill_climbing_randomrestart_simmulated_annealing(state start)
{
    int search_cost=0; // search_cost calculates no of visited states, flag checks if we reach goal or not
    int t=RDM;
    while(t--) //Random Restart
    {
        pair<int,int> p= hill_climbing_simulated_annealing(start); // calling simulated annealing funciton t times
        search_cost+= p.first;
        if(p.second==1) // if goal state is reached we terminate
            return make_pair(search_cost,1);
    }
    return make_pair(search_cost,0);
}
int main()
{
    srand(time(NULL)); //use computer's internal clock to control the choice of the seed for rand() and rand_shuffle
    double count[5]={0.0},sum[5]={0.0};
    for(int i=1;i<=MAX_TIMES;i++)
    {
        state n= state(1); 
        //cout<<n.convert_state_to_string()<<endl;
        pair<int,int> p[4];
        p[0]= hill_climbing(n);
        p[1]= hill_climbing_random_restart(n);
        p[2]= hill_climbing_simulated_annealing(n);
        p[3]= hill_climbing_randomrestart_simmulated_annealing(n);
        p[4]= A_star(n); // This is the optimal solution because A* always finds the path to goal with shortest path for admissible heuristic
        for(int j=0;j<5;j++)
        {
            if(p[j].second==1) count[j]++;
            sum[j]+=p[j].first;
        }      
    }
    cout<<"HILL Climbing\n"<<"SUCCESS_RATE: "<<count[0]/MAX_TIMES<<"   SEARCH_COST: "<<sum[0]/MAX_TIMES<<endl;
    cout<<"HILL Climbing Random Restart\n"<<"SUCCESS_RATE: "<<count[1]/MAX_TIMES<<"   SEARCH_COST: "<<sum[1]/MAX_TIMES<<endl;
    cout<<"HILL Climbing Simulated Annealing\n"<<"SUCCESS_RATE: "<<count[2]/MAX_TIMES<<"   SEARCH_COST: "<<sum[2]/MAX_TIMES<<endl;
    cout<<"HILL Climbing Random Restart and Simulated Annealing\n"<<"SUCCESS_RATE: "<<count[3]/MAX_TIMES<<"   SEARCH_COST: "<<sum[3]/MAX_TIMES<<endl;
    cout<<"A* Optimal Solution\n"<<"SUCCESS_RATE: "<<count[4]/MAX_TIMES<<"   SEARCH_COST: "<<sum[4]/MAX_TIMES<<endl;

    ofstream results ("results.csv");
    results.is_open();
    for(int i=0; i<5; i++)
        results<<count[i]/MAX_TIMES<<","<<sum[i]/MAX_TIMES<<"\n";
    results.close();
   
    return 0;
}
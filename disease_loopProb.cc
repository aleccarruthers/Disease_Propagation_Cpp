#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <unistd.h>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::sort;

class Person
{
private:
	int state;
	string status;
	//vector <Person> interactions;
	int person_number;
public:
	Person()
	{
		state=-1; // Healthy
		status="Susceptible";
	}
	void infect(int w)
	{
		if (state==-1){
			set_state(w);
		}
	}
	string status_string()
	{
		if (state==-1){
			status="susceptible";
			return status;
		}
		if (state>0){
			status="sick ("+std::to_string(state)+" to go)";
			return status;
		}
		if (state==0){
			status="recovered";
			return status;
		}
		if (state==-2){
			status="immune";
			return status;
		}
	}
	void update()
	{
		if (state>0){
			state=get_state()-1;
		}
	}
	bool is_stable()
	{
		if (state>0 || state==-1){
			return false;
		}
		else
			return true;
	}

	int get_state(){ // Get Current State
		return state;
	}
	void set_state(int n){ // Set State
		state=n;
	}
	
	string get_status(){return status;} // Get string status


	int get_person_number(){
		return person_number;
	}
	
	void set_person_number(int num){
		person_number=num;
	}


};

class Population
{
private:
	vector<Person> P;
	int n_people;
	int n_interactions;
	float transfer_rate;
	int immune_people;
public:
	Population() // Default Constructor
	{
	};
	Population(int num_people, int num_interactions,float infect_rate,float percent_immune)
	{
		n_interactions=num_interactions;
		immune_people=static_cast<int>(ceil(percent_immune*num_people));
		n_people=num_people;
		transfer_rate=infect_rate;
		for (int i=0;i<num_people;i++) // Initialize every person as susceptible. "random_infect()" will infect one person.
		{
			Person a;
			a.set_person_number(i);
			P.push_back(a);
		}
		srand(time(NULL)); // Randomly Disperse immune people within the population by changing state from susceptible to immune.
		if (percent_immune>0.0){
			int q = 0;
			while (q<immune_people)
			{
				int immune_index = rand()%(n_people);
				if (P.at(immune_index).get_state()!=-2){
					P.at(immune_index).set_state(-2);
					q=q+1;
				}
				else {continue;} 
			}		
		}
	}

	int get_ninteractions(){return n_interactions;}

	int get_npeople(){return n_people;}	

	int get_immune_people(){return immune_people;}
	
	void sickness_transfer()
	{
		for (auto h : P) // Iterating through the population
		{
			if (h.get_state()>0){ // Determines if the people that interact with the sick people get sick. Rand() < infect_rate ===> Sick
				for (int j=0;j<n_interactions;j++){
					float prob=(float) rand()/(float)RAND_MAX;
					int random_person=rand()%n_people;
					if (prob<transfer_rate && P.at(random_person).get_state()==-1){
						P.at(random_person).infect(10);
					}
					else {continue;}
				}
			}
		}
	}

	void printPeople()
	{
		for (auto person : P)
		{
			cout << "Person " << person.get_person_number() << " has state "  << person.get_state()<< " and is " << person.status_string() << endl;
		}
	}
	
	vector<Person> getPeopleV()
	{
		return P;
	}
	
	void random_infect()
	{
		int infect_check=0;
		while (infect_check==0){
			int infect_ind = rand()%n_people;
			if (P.at(infect_ind).get_state()!=-2){
				P.at(infect_ind).infect(10);
				infect_check=1;
			}
			else
				continue;
		}
	}
	
	int count_infected()
	{
		int sick_count=0;
		int num_sick_people=0;
		for (int m=0;m<get_npeople();m++){
			if (P.at(m).get_state()>=1){
				num_sick_people = num_sick_people+1;
			}
		}
		return num_sick_people;	
	}

	void update_population()
	{
		for (int p=0;p<n_people;p++)
		{
			P[p].update();
		}
	}

	void update_num_interactions(int v){
		cout << "Old Number of interactions: " << get_ninteractions() << endl;
		n_interactions=n_interactions/2;
		cout << "New Number of interactions: " << get_ninteractions() << endl;
	}
	
	int herd_count(){
		int herd_tot=0;
		for (int f=0;f<get_npeople();f++){
			if (P.at(f).get_state()==-1){
				herd_tot=herd_tot+1;
			}
		}
		return herd_tot;
	}

};

int main()
{
//for (int aa=0;aa<20;aa++){
//sleep(2);
int population_size=50000; int people_interactions=20; // The city I live in has a population of ~ 50,000 people
float infection_prob=0.05; float fraction_immune=0.25;
int interaction_check=0;
Population myPop(population_size,people_interactions,infection_prob,fraction_immune); // Population, Interactions, Infect Rate, Fraction Vaccinated
vector<int> daily_sick_count; // Vector to hold daily sick total
myPop.random_infect(); // Randomly infect one person in the population
int g=1;
int herd_immune=0;
for ( ; ;g++)
{
	if (g>1){
		myPop.sickness_transfer();
		myPop.update_population();
	}
	daily_sick_count.push_back(myPop.count_infected());
	if (myPop.count_infected()==0){
		break;
	}

	//if (myPop.count_infected()>population_size/4 && interaction_check<1){
	//	myPop.update_num_interactions(people_interactions/2); // Cut initial interactions in half.
	//	interaction_check=2;
	//}
	
	//if (interaction_check==2){
	//	cout << "Social Distancing interactions: " << myPop.get_ninteractions() << endl;
	//}
}
//cout << "Daily Sick Total: " << endl;
for (int w =0; w<g; w++){
	cout << daily_sick_count[w] << endl;
	}
herd_immune=myPop.herd_count();
cout << ' ' << endl;
//cout << "Number of people who never got sick: " << herd_immune << endl;
//}
}

#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
using namespace std;

// Define the three states of algorithm
enum State { HEALTHY = 0, COLD = 1, REMOVED = 2 };

class ColdAlgorithm {
private:
    int population_size;
    vector<State> population;
    vector<vector<int>> network; // Adjacency list for social connections
    double transmission_rate;
    double removal_rate;
    mt19937 rng;

public:
    ColdAlgorithm(int size, double t_rate, double r_rate, double connection_prob = 0.05)
        : population_size(size), transmission_rate(t_rate), removal_rate(r_rate) {

        population.resize(population_size, HEALTHY);
        network.resize(population_size);

        random_device rd;
        rng = mt19937(rd());
        uniform_real_distribution<double> dist(0.0, 1.0);

          
        // Build the network graph
        for (int i = 0; i < population_size; ++i) {
            for (int j = i + 1; j < population_size; ++j) {
                if (dist(rng) < connection_prob) {
                    network[i].push_back(j);
                    network[j].push_back(i);
                }
            }
        }
    }





    void seed_cold_agents(int initial_count) {
        uniform_int_distribution<int> dist(0, population_size - 1);
        int seeded = 0;
        while (seeded < initial_count) {
            int target = dist(rng);
            if (population[target] == HEALTHY) {
                population[target] = COLD;
                seeded++;
            }
        }
    }






    void simulate_step() {
        vector<State> next_state = population; // Synchronous updating
        uniform_real_distribution<double> dist(0.0, 1.0);



      
        for (int i = 0; i < population_size; ++i) {
            if (population[i] == COLD) {
              
              
                for (int neighbor : network[i])
                {
                    if (population[neighbor] == HEALTHY && dist(rng) < transmission_rate) {
                        next_state[neighbor] = COLD;
                      
                    }

                  
                }
              
                if (dist(rng) < removal_rate) {
                    next_state[i] = REMOVED;
                }

              
            }



          
        }
        population = next_state;


      
    }

    double get_prevalence_rate() {
        int cold_count = 0;
        for (State s : population) {
            if (s == COLD) cold_count++;
        }
        return (double)cold_count / population_size;
    }

    void run_simulation(int steps) {
        cout << "Starting Cold Algorithm Simulation...\n";
        for (int step = 0; step <= steps; ++step) {
            double prevalence = get_prevalence_rate();
            cout << "Step " << setw(3) << step
                << " | Prevalence Rate: " << fixed << setprecision(2)
                << (prevalence * 100.0) << "%\n";

            if (prevalence == 0.0) {
                cout << "The 'Cold' entities have been fully eradicated.\n";
                break;
            }
          
            simulate_step();
        }



      
    }
};

int main() {

      ColdAlgorithm sim(1000, 0.15, 0.05, 0.02);

    sim.seed_cold_agents(3); // Start with 3 "Cold" entities
    sim.run_simulation(30);

    return 0;



  
}
